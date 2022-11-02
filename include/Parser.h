#pragma once

#include "utils/Numbers.h"

#include <functional>

/**
 * C++20 functional parser created by Petter Holmberg
 * https://github.com/petter-holmberg/eop-parser
 *
 * With subtle modifications
 */

namespace fp
{


template<typename T>
using Result = std::optional<std::pair<T, std::string_view>>;

template<typename P>
concept Parser =
std::regular_invocable<P, std::string_view> &&
requires(std::invoke_result_t<P, std::string_view> result)
{
    std::same_as<
        decltype(result),
        Result<typename decltype(result)::value_type::first_type>>;
};

template<Parser P>
using ParserResult = std::invoke_result_t<P, std::string_view>;

template<Parser P>
using ParserValue = typename ParserResult<P>::value_type::first_type;

template<typename F, typename... Args>
concept ParserCombinator =
std::regular_invocable<F, Args...> &&
Parser<std::invoke_result_t<F, Args...>>;

template<typename F, typename... Args>
    requires ParserCombinator<F, Args...>
using ParserCombinatorValue = std::invoke_result_t<F, Args...>;

inline constexpr auto partialApply = []<typename F, typename... Args>(F&& f, Args&& ... args) {
    if constexpr (std::invocable<F, Args...>)
    {
        return std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
    }
    else
    {
        return std::bind_front(std::forward<F>(f), std::forward<Args>(args)...);
    }
};


// Operators

template<Parser P, Parser Q>
requires std::convertible_to<ParserValue<P>, ParserValue<Q>>
constexpr Parser auto operator|(P p, Q q)
{
    return [p, q](std::string_view input) -> ParserResult<Q> {
        if (const auto& result = std::invoke(p, input))
        {
            return result;
        }
        else
        {
            return std::invoke(q, input);
        }
    };
}

template<Parser P, ParserCombinator<ParserValue<P>> F>
constexpr Parser auto operator&(P parser, F func)
{
    using ParserT = ParserCombinatorValue<F, ParserValue<P>>;
    return [parser, func](std::string_view input) -> ParserResult<ParserT> {
        if (const auto& result = std::invoke(parser, input))
        {
            return std::invoke(std::invoke(func, result->first), result->second);
        }
        else
        {
            return {};
        }
    };
}

template<Parser P, Parser Q>
constexpr Parser auto operator^(P p, Q q)
{
    using ResultT = std::invoke_result_t<decltype(partialApply), ParserValue<P>, ParserValue<Q>>;
    return [p, q](std::string_view input) -> Result<ResultT> {
        if (const auto& pr = std::invoke(p, input))
        {
            if (const auto& qr = std::invoke(q, pr->second))
            {
                return {{partialApply(pr->first, qr->first), qr->second}};
            }
        }
        return {};
    };
}

// Helpers for operators

constexpr Parser auto choice(Parser auto parser, Parser auto... parsers)
{
    if constexpr (std::is_pointer_v<decltype(parser)>)
    {
        return ([parser](auto input) {
            return std::invoke(parser, input);
        } | ... | parsers);
    }
    else
    {
        return (parser | ... | parsers);
    }
}

template<typename T>
constexpr Parser auto unit(const T& thing)
{
    return [thing](std::string_view input) -> Result<T> {
        return {{thing, input}};
    };
}

template<Parser P>
constexpr Parser auto maybe(P parser)
{
    return [parser](std::string_view input) -> Result<std::optional<ParserValue<P>>> {
        return choice(
            chain(parser, [](auto const& thing) { return unit(std::optional {thing}); }),
            unit(std::optional<ParserValue<P>> {})
        )(input);
    };
}

constexpr Parser auto chain(Parser auto parser, auto... funcs)
{
    if constexpr (std::is_pointer_v<decltype(parser)>)
    {
        return ([parser](auto input) {
            return std::invoke(parser, input);
        } & ... & funcs);
    }
    else
    {
        return (parser & ... & funcs);
    }
}

template<typename F, Parser... Ps>
requires std::regular_invocable<F, ParserValue<Ps>...>
constexpr Parser auto sequence(F func, Ps... parsers)
{
    return (unit(func) ^ ... ^ parsers);
}

// Essential parsers

template<typename T>
inline constexpr auto empty = [](const std::string_view&) -> Result<T> {
    return {};
};

inline constexpr auto item = [](std::string_view input) -> Result<char> {
    if (input.empty())
    {
        return {};
    }
    else
    {
        return {{input[0], input.substr(1)}};
    }
};

// Basic parser generators

constexpr Parser auto str(std::string_view match)
{
    return [match](std::string_view input) -> Result<std::string> {
        if (input.starts_with(match))
        {
            return {{
                        std::string {match}, {input.begin() + match.size(), input.end()}
                    }};
        }
        else
        {
            return {};
        }
    };
}

constexpr Parser auto skip(Parser auto p, Parser auto q)
{
    return choice(chain(p, [q](const auto&) { return q; }), q);
}

template<typename Pr, Parser P = decltype(item)>
requires std::predicate<Pr, ParserValue<P>>
constexpr Parser auto satisfy(Pr pred, P parser = item)
{
    return chain(parser,
                 [pred](const auto& thing) -> Parser auto {
                     return [pred, thing](std::string_view input) -> Result<ParserValue<P>> {
                         if (std::invoke(pred, thing))
                         {
                             return {{thing, input}};
                         }
                         else
                         {
                             return {};
                         }
                     };
                 });
}

constexpr Parser auto symbol(char x)
{
    return satisfy([x](char y) {
        return x == y;
    });
}

// Iteration helpers

template<typename T, Parser P, std::regular_invocable<T, ParserValue<P>> F>
requires std::convertible_to<std::invoke_result_t<F, T, ParserValue<P>>, T>
class ReduceMany
{
public:
    constexpr ReduceMany(const T& thing, const P& p, const F& fn)
        : init {thing}
        , parser {p}
        , func {fn} { }

    constexpr auto operator()(std::string_view input) const -> Result<T>
    {
        return choice(
            chain(parser, [this](const auto& thing) {
                return ReduceMany {std::invoke(func, init, thing), parser, func};
            }), unit(init)
        )(input);
    }

private:
    T init;
    P parser;
    F func;
};

template<typename T>
auto appendedVector(std::vector<T> x, const T& y) -> std::vector<T>
{
    x.push_back(y);
    return x;
}

template<typename T>
auto appendedVector(std::vector<T> x, const std::vector<T>& vec) -> std::vector<T>
{
    x.insert(x.end(), vec.cbegin(), vec.cend());
    return x;
}

template<Parser P>
requires std::same_as<ParserValue<P>, char>
constexpr Parser auto many(P parser)
{
    return ReduceMany(std::string {},
                      parser,
                      [](const std::string& str, char ch) {
                          return str + ch;
                      });
}

template<Parser P>
constexpr Parser auto many(P parser)
{
    using T = ParserValue<P>;
    using Ts = std::vector<T>;
    return ReduceMany(
        Ts {},
        parser,
        [](Ts const& ts, T const& t) { return appendedVector(ts, t); }
    );
}

template<Parser P>
requires std::same_as<ParserValue<P>, char>
constexpr Parser auto some(P parser)
{
    return sequence(
        [](char ch, const std::string& str) { return std::string(1, ch) + str; },
        parser,
        many(parser)
    );
}

template<Parser P>
constexpr Parser auto some(P parser)
{
    using T = ParserValue<P>;
    using Ts = std::vector<T>;
    return sequence(
        [](const T& item, const Ts& vec) { return appendedVector(Ts {item}, vec); },
        parser,
        many(parser)
    );
}


template<class, template<class...> class>
inline constexpr bool is_specialization = false;

template<template<class...> class T, class... Args>
inline constexpr bool is_specialization<T<Args...>, T> = true;

template<class T>
concept OptionalType = is_specialization<T, std::optional>;

template<Parser P>
requires OptionalType<ParserValue<P>>
constexpr Parser auto flatten(P parser)
{
    using ResultT = Result<typename ParserValue<P>::value_type>;
    return [parser](std::string_view input) -> ResultT {
        const auto result = std::invoke(parser, input);
        if (result && result->first)
        {
            return {{result->first.value(), result->second}};
        }
        return {};
    };
}

// Helpers



inline bool isEol(char symbol)
{
    return symbol == '\r' || symbol == '\n';
}

inline bool isSpaceNotEol(char symbol)
{
    return ::isspace(symbol) && !isEol(symbol);
}

// Basic parsers

inline Parser auto digit             = satisfy(::isdigit);
inline Parser auto lower             = satisfy(::islower);
inline Parser auto upper             = satisfy(::isupper);
inline Parser auto letter            = satisfy(::isalpha);
inline Parser auto notLetter         = satisfy(std::not_fn(::isalpha));
inline Parser auto alphanum          = satisfy(::isalnum);
inline Parser auto whitespace        = satisfy(::isspace);
inline Parser auto whitespaces       = many(satisfy(::isspace));
inline Parser auto whitespaceNotEol  = satisfy(isSpaceNotEol);
inline Parser auto whitespacesNotEol = many(satisfy(isSpaceNotEol));
inline Parser auto eol               = satisfy(isEol);
inline Parser auto eols              = many(satisfy(isEol));
inline Parser auto line              = choice(
    sequence([](const auto& line, const auto&) { return line; }, many(satisfy(std::not_fn(isEol))),
             choice(str("\r\n"), str("\n\r"), str("\n"), str("\r"))),
    sequence([](const auto& line) { return line; }, some(satisfy([](auto) { return true; }))));//End of File case

template<Parser P = decltype(whitespaces)>
constexpr Parser auto token(Parser auto parser, P toSkip = whitespaces)
{
    return sequence(
        [](auto const& thing, const auto&) { return thing; },
        skip(toSkip, parser),
        toSkip
    );
}

template<Parser P = decltype(whitespaces)>
constexpr Parser auto tokenLeft(Parser auto parser, P toSkip = whitespaces)
{
    return sequence(
        [](auto const& thing) { return thing; },
        skip(toSkip, parser)
    );
}

template<Parser P = decltype(whitespaces)>
constexpr Parser auto tokenRight(Parser auto parser, P toSkip = whitespaces)
{
    return sequence(
        [](auto const& thing, const auto&) { return thing; },
        parser,
        toSkip
    );
}

template<std::integral T = int32_t>
Parser auto natural = flatten(sequence(
    [](auto const& str) { return tsplib::utils::stringToNumber<T>(str); },
    some(digit)
));

template<std::integral T = int32_t>
Parser auto negative = flatten(sequence(
    [](const auto ch, const auto& str) { return tsplib::utils::stringToNumber<T>(std::string {ch} + str); },
    symbol('-'),
    some(digit)
));


template<std::signed_integral T = int32_t>
Parser auto integer = choice(
    natural<T>,
    negative<T>
);

template<std::floating_point T = double>
Parser auto real = flatten(sequence([](const auto& str) { return tsplib::utils::stringToNumber<T>(str); },
                                    choice(
                                        sequence([](auto minus, const auto& x, auto, const auto& y) {
                                                     return
                                                         (minus ? std::string {*minus} : std::string {}) +
                                                         x +
                                                         '.' +
                                                         (y ? *y : std::string {});
                                                 },
                                                 maybe(symbol('-')),
                                                 some(digit),
                                                 symbol('.'),
                                                 maybe(some(digit))),
                                        sequence([](auto minus, auto point, const auto& x) {
                                                     return
                                                         (minus ? std::string {*minus} : std::string {}) +
                                                         (point ? std::string {*point} : std::string {}) +
                                                         x;
                                                 },
                                                 maybe(symbol('-')),
                                                 maybe(symbol('.')),
                                                 some(digit)
                                        ))));


}