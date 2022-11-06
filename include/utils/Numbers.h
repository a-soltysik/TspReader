#pragma once

#include <stdexcept>
#include <charconv>
#include <optional>
#include <sstream>
#include <iomanip>
#include <array>

namespace tsplib::utils
{

template<std::integral T>
constexpr auto maxLengthOfType() -> uint32_t
{
    return std::numeric_limits<T>::digits10 + 2;
}

template<std::floating_point T>
constexpr auto maxLengthOfType() -> uint32_t
{
    return std::numeric_limits<T>::max_exponent10 +
           std::numeric_limits<T>::max_digits10 + 2;
}

template<std::integral T>
[[nodiscard]]
auto stringToNumber(const std::string& number) -> std::optional<T>
{
    T result;
    const auto [ptr, code] {std::from_chars(number.data(), number.data() + number.size(), result)};

    if (code == std::errc::invalid_argument || code == std::errc::result_out_of_range)
    {
        return {};
    }

    return result;
}

/**
 * GCC and clang does not support std::from_chars for floating point types (although it is standardized)
 */
#if defined(_MSC_VER)

template<std::floating_point T>
[[nodiscard]]
auto stringToNumber(const std::string& number) -> std::optional<T>
{
    T result;
    auto const[ptr, code] {std::from_chars(number.data(), number.data() + number.size(), result)};

    if (code == std::errc::invalid_argument || code == std::errc::result_out_of_range)
    {
        return {};
    }

    return result;
}

#else

template<std::floating_point T>
[[nodiscard]]
auto stringToNumber(const std::string& number) -> std::optional<T>
{
    try
    {
        if constexpr (std::same_as<T, float>)
        {
            return std::stof(number);
        }
        else if constexpr (std::same_as<T, double>)
        {
            return std::stod(number);
        }
        else
        {
            return std::stold(number);
        }
    }
    catch (const std::out_of_range&)
    {
        return {};
    }
    catch (const std::invalid_argument&)
    {
        return {};
    }
}

#endif

template<std::integral T>
[[nodiscard]]
auto numberToString(T number) -> std::string
{
    static constexpr auto size = maxLengthOfType<T>();
    auto buffer = std::array<char, size>{};
    std::to_chars(buffer.data(), buffer.data() + size, number);
    auto result = std::string{buffer.data()};
    result.erase(std::find(result.begin(), result.end(), '\0'), result.end());
    return result;
}

template<std::floating_point T>
[[nodiscard]]
auto numberToString(T number) -> std::string
{
    auto stream = std::stringstream {};
    stream << number;
    return stream.str();
}

template<std::floating_point T>
[[nodiscard]]
auto numberToString(T number, int32_t precision) -> std::string
{
    auto stream = std::stringstream {};
    stream << std::fixed << std::setprecision(precision) << number;
    return stream.str();
}

}
