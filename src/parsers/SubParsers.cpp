#include "SubParsers.h"

namespace Tsp
{

auto name(std::string_view input) -> fp::Result<Name>
{
    return fp::sequence(
        [](const auto&, auto, const auto& name) { return Name {name}; },
        fp::str("NAME"),
        fp::token(fp::symbol(':')),
        fp::many(fp::satisfy(std::not_fn(fp::isEol)))
    )(input);
}

auto comment(std::string_view input) -> fp::Result<Comment>
{
    return fp::sequence(
        [](const auto&, auto, const auto& comment) { return Comment {comment}; },
        fp::str("COMMENT"),
        fp::token(fp::symbol(':')),
        fp::many(fp::satisfy(std::not_fn(fp::isEol)))
    )(input);
}

auto type(std::string_view input) -> fp::Result<AlgorithmType>
{
    using
    enum AlgorithmType;

    return fp::sequence(
        [](const auto&, auto, auto type) { return type; },
        fp::str("TYPE"),
        fp::token(fp::symbol(':')),
        fp::choice(
            fp::sequence([](const auto&) { return TSP; }, fp::str("TSP")),
            fp::sequence([](const auto&) { return ATSP; }, fp::str("ATSP")),
            fp::sequence([](const auto&) { return SOP; }, fp::str("SOP")),
            fp::sequence([](const auto&) { return HCP; }, fp::str("HCP")),
            fp::sequence([](const auto&) { return CRVP; }, fp::str("CRVP")),
            fp::sequence([](const auto&) { return TOUR; }, fp::str("TOUR"))
        )
    )(input);
}

[[nodiscard]]
auto dimension(std::string_view input) -> fp::Result<Dimension>;

[[nodiscard]]
auto capacity(std::string_view input) -> fp::Result<Capacity>;

auto edgeWeightType(std::string_view input) -> fp::Result<EdgeWeightType>
{
    using
    enum EdgeWeightType;

    return fp::sequence(
        [](const auto&, auto, auto type) { return type; },
        fp::str("EDGE_WEIGHT_TYPE"),
        fp::token(fp::symbol(':')),
        choice(
            fp::sequence([](const auto&) { return EXPLICIT; }, fp::str("EXPLICIT")),
            fp::sequence([](const auto&) { return EUC; }, fp::str("EUC_2D")),
            fp::sequence([](const auto&) { return EUC; }, fp::str("EUC_3D")),
            fp::sequence([](const auto&) { return MAX; }, fp::str("MAX_2D")),
            fp::sequence([](const auto&) { return MAX; }, fp::str("MAX_3D")),
            fp::sequence([](const auto&) { return MAN; }, fp::str("MAN_2D")),
            fp::sequence([](const auto&) { return MAN; }, fp::str("MAN_3D")),
            fp::sequence([](const auto&) { return CEIL; }, fp::str("CEIL_2D")),
            fp::sequence([](const auto&) { return GEO; }, fp::str("GEO"))
        )
    )(input);
}

auto edgeWeightFormat(std::string_view input) -> fp::Result<EdgeWeightFormat>
{
    using
    enum EdgeWeightFormat;

    return fp::sequence(
        [](const auto&, auto, auto format) { return format; },
        fp::str("EDGE_WEIGHT_FORMAT"),
        fp::token(fp::symbol(':')),
        choice(
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
        )
    )(input);
}

auto edgeDataFormat(std::string_view input) -> fp::Result<EdgeDataFormat>
{
    using
    enum EdgeDataFormat;

    return fp::sequence(
        [](const auto&, auto, auto format) { return format; },
        fp::str("EDGE_DATA_FORMAT"),
        fp::token(fp::symbol(':')),
        choice(
            fp::sequence([](const auto&) { return EDGE_LIST; }, fp::str("EDGE_LIST")),
            fp::sequence([](const auto&) { return ADJ_LIST; }, fp::str("ADJ_LIST"))
        )
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
        fp::real<float>,
        fp::some(fp::whitespaceNotEol),
        fp::tokenRight(fp::real<float>, fp::whitespacesNotEol),
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
        fp::real<float>,
        fp::some(fp::whitespaceNotEol),
        fp::real<float>,
        fp::some(fp::whitespaceNotEol),
        fp::tokenRight(fp::real<float>, fp::whitespacesNotEol),
        fp::eol
    )(input);
}

auto nodes3d(std::string_view input) -> fp::Result<NodeCoordSection>
{
    return fp::sequence([](const auto& nodes) { return NodeCoordSection {Nodes3d {nodes}}; },
                        fp::some(fp::tokenLeft(node3d))
    )(input);
}

auto nodeCoordSection(std::string_view input) -> fp::Result<NodeCoordSection>
{
    return fp::sequence([](const auto&, const auto&, const auto& nodes) { return NodeCoordSection {nodes}; },
                        fp::str("NODE_COORD_SECTION"),
                        fp::token(fp::symbol(':')),
                        (fp::choice(
                            nodes2d,
                            nodes3d))
    )(input);
}

auto edgeDataSection(std::string_view input) -> fp::Result<EdgeDataSection>
{
    return fp::sequence([](const auto&, auto, const auto& data) { return EdgeDataSection {data}; },
                        fp::str("EDGE_DATA_SECTION"),
                        fp::token(fp::symbol(':')),
                        fp::some(fp::tokenLeft(fp::integer<int32_t>))
    )(input);
}

auto edgeWeightSection(std::string_view input) -> fp::Result<EdgeWeightSection>
{
    return fp::sequence([](const auto&, auto, const auto& weights) { return EdgeWeightSection {weights}; },
                        fp::str("EDGE_WEIGHT_SECTION"),
                        fp::token(fp::symbol(':')),
                        fp::some(fp::tokenLeft(fp::integer<int32_t>))
    )(input);
}

auto tspData(std::string_view input) -> fp::Result<Config>
{
    return fp::sequence([](const auto& data) { return TspData {data}.filtered(); },
                        fp::many(fp::tokenLeft(fp::choice(
                            fp::sequence([](const auto& item) -> TspData::Item { return item; }, name),
                            fp::sequence([](const auto& item) -> TspData::Item { return item; }, type),
                            fp::sequence([](const auto& item) -> TspData::Item { return item; }, comment),
                            fp::sequence([](const auto& item) -> TspData::Item { return item; }, edgeWeightType),
                            fp::sequence([](const auto& item) -> TspData::Item { return item; }, edgeWeightFormat),
                            fp::sequence([](const auto& item) -> TspData::Item { return item; }, edgeDataFormat),
                            fp::sequence([](const auto& item) -> TspData::Item { return item; }, nodeCoordSection),
                            fp::sequence([](const auto& item) -> TspData::Item { return item; }, edgeDataSection),
                            fp::sequence([](const auto& item) -> TspData::Item { return item; }, edgeWeightSection),
                            fp::sequence([](const auto&) -> TspData::Item { return {}; }, fp::line)
                        )))
    )(input);
}

}