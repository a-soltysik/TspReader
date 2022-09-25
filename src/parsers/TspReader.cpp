#include "TspReader.h"
#include "Parser.h"
#include "Content.h"

namespace Tsp
{

struct WeightConfig
{
    enum class EdgeWeightType
    {
        EXPLICIT,
        EUC_2D,
        EUC_3D,
        MAX_2D,
        MAX_3D,
        MAN_2D,
        MAN_3D,
        CEIL_2D,
        GEO
    };

    EdgeWeightType edgeWeightType;
    uint8_t dimensionsNumber;
};

struct Node2d
{
    uint32_t id;
    float x;
    float y;
};

struct Node3d
{
    uint32_t id;
    float x;
    float y;
    float z;
};

auto nameParser(std::string_view input) -> fp::Result<std::string>
{
    return fp::sequence(
        [](auto, auto, const auto& name) { return name; },
        fp::token(fp::str("NAME")),
        fp::token(fp::symbol(':')),
        fp::many(fp::letter)
    )(input);
}

auto commentParser(std::string_view input) -> fp::Result<std::string>
{
    return fp::sequence(
        [](auto, auto, const auto& comment) { return comment; },
        fp::token(fp::str("COMMENT")),
        fp::token(fp::symbol(':')),
        fp::many(fp::letter)
    )(input);
}

auto typeParser(std::string_view input) -> fp::Result<Type>
{
    return fp::sequence(
        [](auto, auto, auto type) { return type; },
        fp::token(fp::str("TYPE")),
        fp::token(fp::symbol(':')),
        choice(
            fp::sequence([](auto) { return Type::TSP;  }, fp::token(fp::str("TSP"))),
            fp::sequence([](auto) { return Type::ATSP; }, fp::token(fp::str("ATSP"))),
            fp::sequence([](auto) { return Type::SOP;  }, fp::token(fp::str("SOP"))),
            fp::sequence([](auto) { return Type::HCP;  }, fp::token(fp::str("HCP"))),
            fp::sequence([](auto) { return Type::CRVP; }, fp::token(fp::str("CRVP"))),
            fp::sequence([](auto) { return Type::TOUR; }, fp::token(fp::str("TOUR")))
        )
    )(input);
}

auto edgeWeightTypeParser(std::string_view input) -> fp::Result<WeightConfig>
{
    using enum WeightConfig::EdgeWeightType;

    return fp::sequence(
        [](auto, auto, auto type) { return type; },
        fp::token(fp::str("EDGE_WEIGHT_TYPE")),
        fp::token(fp::symbol(':')),
        choice(
            fp::sequence([](auto) { return WeightConfig{EXPLICIT, 1}; }, fp::token(fp::str("EXPLICIT"))),
            fp::sequence([](auto) { return WeightConfig{EUC_2D,   2}; }, fp::token(fp::str("EUC_2D"))),
            fp::sequence([](auto) { return WeightConfig{EUC_3D,   3}; }, fp::token(fp::str("EUC_3D"))),
            fp::sequence([](auto) { return WeightConfig{MAX_2D,   2}; }, fp::token(fp::str("MAX_2D"))),
            fp::sequence([](auto) { return WeightConfig{MAX_3D,   2}; }, fp::token(fp::str("MAX_3D"))),
            fp::sequence([](auto) { return WeightConfig{MAN_2D,   2}; }, fp::token(fp::str("MAN_2D"))),
            fp::sequence([](auto) { return WeightConfig{MAN_3D,   3}; }, fp::token(fp::str("MAN_3D"))),
            fp::sequence([](auto) { return WeightConfig{CEIL_2D,  2}; }, fp::token(fp::str("CEIL_2D"))),
            fp::sequence([](auto) { return WeightConfig{GEO,      2}; }, fp::token(fp::str("GEO")))
        )
    )(input);
}

auto node2dParser(std::string_view input) -> fp::Result<Node2d>
{
    return fp::sequence(
        [](auto id, auto x, auto y) {return Node2d{id, x, y};},
        fp::token(fp::natural<uint32_t>),
        fp::token(fp::real<float>),
        fp::token(fp::real<float>)
        )(input);
}

auto node3dParser(std::string_view input) -> fp::Result<Node3d>
{
    return fp::sequence(
        [](auto id, auto x, auto y, auto z) {return Node3d{id, x, y, z};},
        fp::token(fp::natural<uint32_t>),
        fp::token(fp::real<float>),
        fp::token(fp::real<float>),
        fp::token(fp::real<float>)
    )(input);
}

auto metaDataParser(std::string_view input) -> fp::Result<MetaData>
{
    return fp::sequence(
        [](const auto& name, const auto& comment, auto type) { return MetaData {name, comment, type}; },
        nameParser,
        commentParser,
        typeParser
    )(input);
}

}