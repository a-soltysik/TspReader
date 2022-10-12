#include "SubParsers.h"

#include <iostream>

namespace tsp
{

template<typename ItemT>
auto tokenizedLine(std::string_view input) -> fp::Result<TspData::Item>
{
    return fp::sequence(
        [](const auto& name) { return ItemT {name}; },
        fp::tokenLeft(fp::line)
    )(input);
}

template<std::unsigned_integral T, typename ItemT>
auto tokenizedNatural(std::string_view input) -> fp::Result<TspData::Item>
{
    return fp::sequence([](auto dimension) { return ItemT {dimension}; },
        fp::tokenLeft(fp::natural<T>)
    )(input);
}

template<std::integral T, typename ItemT>
auto tokenizedIntegers(std::string_view input) -> fp::Result<TspData::Item>
{
    return fp::sequence([](const auto& integers) { return ItemT {integers}; },
                        fp::some(fp::tokenLeft(fp::integer<T>))
    )(input);
}

auto name(std::string_view input) -> fp::Result<TspData::Item>
{
    return tokenizedLine<Name>(input);
}

auto comment(std::string_view input) -> fp::Result<TspData::Item>
{
    return tokenizedLine<Comment>(input);
}

auto type(std::string_view input) -> fp::Result<TspData::Item>
{
    using
    enum AlgorithmType;

    return fp::sequence(
        [](auto type) { return type; },
        fp::tokenLeft(fp::choice(
            fp::sequence([](const auto&) { return TSP; }, fp::str("TSP")),
            fp::sequence([](const auto&) { return ATSP; }, fp::str("ATSP")),
            fp::sequence([](const auto&) { return SOP; }, fp::str("SOP")),
            fp::sequence([](const auto&) { return HCP; }, fp::str("HCP")),
            fp::sequence([](const auto&) { return CRVP; }, fp::str("CRVP")),
            fp::sequence([](const auto&) { return TOUR; }, fp::str("TOUR"))
        ))
    )(input);
}

auto dimension(std::string_view input) -> fp::Result<TspData::Item>
{
    return tokenizedNatural<uint32_t, Dimension>(input);
}

auto capacity(std::string_view input) -> fp::Result<TspData::Item>
{
    return tokenizedNatural<uint32_t, Capacity>(input);
}

auto edgeWeightType(std::string_view input) -> fp::Result<TspData::Item>
{
    using
    enum EdgeWeightType;

    return fp::sequence(
        [](auto type) { return type; },
        fp::tokenLeft(fp::choice(
            fp::sequence([](const auto&) { return EXPLICIT; }, fp::str("EXPLICIT")),
            fp::sequence([](const auto&) { return EUC; }, fp::str("EUC_2D")),
            fp::sequence([](const auto&) { return EUC; }, fp::str("EUC_3D")),
            fp::sequence([](const auto&) { return MAX; }, fp::str("MAX_2D")),
            fp::sequence([](const auto&) { return MAX; }, fp::str("MAX_3D")),
            fp::sequence([](const auto&) { return MAN; }, fp::str("MAN_2D")),
            fp::sequence([](const auto&) { return MAN; }, fp::str("MAN_3D")),
            fp::sequence([](const auto&) { return CEIL; }, fp::str("CEIL_2D")),
            fp::sequence([](const auto&) { return GEO; }, fp::str("GEO"))
        ))
    )(input);
}

auto edgeWeightFormat(std::string_view input) -> fp::Result<TspData::Item>
{
    using
    enum EdgeWeightFormat;

    return fp::sequence(
        [](auto format) { return format; },
        fp::tokenLeft(fp::choice(
            fp::sequence([](const auto&) { return FUNCTION; }, fp::str("FUNCTION")),
            fp::sequence([](const auto&) { return FULL_MATRIX; }, fp::str("FULL_MATRIX")),
            fp::sequence([](const auto&) { return UPPER_ROW; }, fp::str("UPPER_ROW")),
            fp::sequence([](const auto&) { return LOWER_ROW; }, fp::str("LOWER_ROW")),
            fp::sequence([](const auto&) { return UPPER_DIAG_ROW; }, fp::str("UPPER_DIAG_ROW")),
            fp::sequence([](const auto&) { return LOWER_DIAG_ROW; }, fp::str("LOWER_DIAG_ROW")),
            fp::sequence([](const auto&) { return UPPER_COL; }, fp::str("UPPER_COL")),
            fp::sequence([](const auto&) { return LOWER_COL; }, fp::str("LOWER_COL")),
            fp::sequence([](const auto&) { return UPPER_DIAG_COL; }, fp::str("UPPER_DIAG_COL")),
            fp::sequence([](const auto&) { return LOWER_DIAG_COL; }, fp::str("LOWER_DIAG_COL"))
        ))
    )(input);
}

auto edgeDataFormat(std::string_view input) -> fp::Result<TspData::Item>
{
    using
    enum EdgeDataFormat;

    return fp::sequence(
        [](auto format) { return format; },
        fp::tokenLeft(fp::choice(
            fp::sequence([](const auto&) { return EDGE_LIST; }, fp::str("EDGE_LIST")),
            fp::sequence([](const auto&) { return ADJ_LIST; }, fp::str("ADJ_LIST"))
        ))
    )(input);
}

auto node2d(std::string_view input) -> fp::Result<Node2d>
{
    return fp::sequence(
        [](auto id, const auto&,
           auto x, const auto&,
           auto y, auto) { return Node2d {id, x, y}; },
        fp::natural<uint32_t>,
        fp::some(fp::whitespaceNotEol),
        fp::real<double>,
        fp::some(fp::whitespaceNotEol),
        fp::tokenRight(fp::real<double>, fp::whitespacesNotEol),
        fp::eol
    )(input);
}

auto nodes2d(std::string_view input) -> fp::Result<NodeCoordSection>
{
    return fp::sequence(
        [](const auto& nodes) { return NodeCoordSection {Nodes2d {nodes}}; },
        fp::some(fp::tokenLeft(node2d))
    )(input);
}

auto node3d(std::string_view input) -> fp::Result<Node3d>
{
    return fp::sequence(
        [](auto id, const auto&,
           auto x, const auto&,
           auto y, const auto&,
           auto z, auto) {
            return Node3d {id, x, y, z};
        },
        fp::natural<uint32_t>,
        fp::some(fp::whitespaceNotEol),
        fp::real<double>,
        fp::some(fp::whitespaceNotEol),
        fp::real<double>,
        fp::some(fp::whitespaceNotEol),
        fp::tokenRight(fp::real<double>, fp::whitespacesNotEol),
        fp::eol
    )(input);
}

auto nodes3d(std::string_view input) -> fp::Result<NodeCoordSection>
{
    return fp::sequence([](const auto& nodes) { return NodeCoordSection {Nodes3d {nodes}}; },
                        fp::some(fp::tokenLeft(node3d))
    )(input);
}

auto nodeCoordSection(std::string_view input) -> fp::Result<TspData::Item>
{
    return fp::sequence([](const auto& nodes) { return NodeCoordSection {nodes}; },
                        fp::choice(
                            nodes2d,
                            nodes3d)
    )(input);
}

auto edgeDataSection(std::string_view input) -> fp::Result<TspData::Item>
{
    return tokenizedIntegers<int32_t, EdgeDataSection>(input);
}

auto edgeWeightSection(std::string_view input) -> fp::Result<TspData::Item>
{
    return tokenizedIntegers<int32_t, EdgeWeightSection>(input);
}

auto tag(std::string_view input) -> fp::Result<std::string>
{
    return fp::sequence([](const auto& tag, auto) { return tag; },
                        fp::many(fp::satisfy([](auto symbol) { return symbol != ':' && !::isspace(symbol); })),
                        fp::maybe(fp::tokenLeft(fp::symbol(':')))
                        )(input);
}

auto getParser(std::string_view tag) -> fp::Parser auto
{
    using ParserT = std::function<fp::Result<TspData::Item>(std::string_view)>;
    static const auto tagMap = std::unordered_map<std::string_view, ParserT> {
        {NAME,                name},
        {TYPE,                type},
        {COMMENT,             comment},
        {DIMENSION,           dimension},
        {CAPACITY,            capacity},
        {EDGE_WEIGHT_TYPE,    edgeWeightType},
        {EDGE_WEIGHT_FORMAT,  edgeWeightFormat},
        {EDGE_DATA_FORMAT,    edgeDataFormat},
        {NODE_COORD_SECTION,  nodeCoordSection},
        {EDGE_DATA_SECTION,   edgeDataSection},
        {EDGE_WEIGHT_SECTION, edgeWeightSection}
    };

    const auto it = tagMap.find(tag);

    if (it == tagMap.cend())
    {
        return ParserT {fp::sequence([](const auto&) -> TspData::Item { return {}; }, fp::line)};
    }
    else
    {
        return it->second;
    }
}

auto tspData(std::string_view input) -> fp::Result<Config>
{
    const auto parser = fp::chain(fp::tokenLeft(tag), [](const auto& tagName) {
        return getParser(tagName);
    });
    return fp::sequence([](const auto& data) { return TspData {data}.filtered(); },
                        fp::many(
                            fp::sequence([](const auto& item) -> TspData::Item { return item; }, parser)
                        )
    )(input);
}

}