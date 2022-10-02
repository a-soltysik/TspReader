#pragma once

#include <string>
#include <variant>
#include <vector>

namespace Tsp
{

using namespace std::string_view_literals;

inline constexpr auto NAME               = "NAME"sv;
inline constexpr auto TYPE               = "TYPE"sv;
inline constexpr auto COMMENT            = "COMMENT"sv;
inline constexpr auto DIMENSION          = "DIMENSION"sv;
inline constexpr auto CAPACITY           = "CAPACITY"sv;
inline constexpr auto EDGE_WEIGHT_TYPE   = "EDGE_WEIGHT_TYPE"sv;
inline constexpr auto EDGE_WEIGHT_FORMAT = "EDGE_WEIGHT_FORMAT"sv;
inline constexpr auto EDGE_DATA_FORMAT   = "EDGE_DATA_FORMAT"sv;
inline constexpr auto NODE_COORD_TYPE    = "NODE_COORD_TYPE"sv;
inline constexpr auto DISPLAY_DATA_TYPE  = "DISPLAY_DATA_TYPE"sv;
inline constexpr auto END_OF_FILE        = "EOF"sv;

inline constexpr auto NODE_COORD_SECTION   = "NODE_COORD_SECTION"sv;
inline constexpr auto DEPOT_SECTION        = "DEPOT_SECTION"sv;
inline constexpr auto DEMAND_SECTION       = "DEMAND_SECTION"sv;
inline constexpr auto EDGE_DATA_SECTION    = "EDGE_DATA_SECTION"sv;
inline constexpr auto FIXED_EDGES_SECTION  = "FIXED_EDGES_SECTION"sv;
inline constexpr auto DISPLAY_DATA_SECTION = "DISPLAY_DATA_SECTION"sv;
inline constexpr auto TOUR_SECTION         = "TOUR_SECTION"sv;
inline constexpr auto EDGE_WEIGHT_SECTION  = "EDGE_WEIGHT_SECTION"sv;

struct Name
{
    std::string name;
};

enum class AlgorithmType
{
    TSP,
    ATSP,
    SOP,
    HCP,
    CRVP,
    TOUR
};

struct Comment
{
    std::string comment;
};

struct Dimension
{
    uint32_t dimension;
};

struct Capacity
{
    uint32_t capacity;
};

enum class EdgeWeightType{
    EXPLICIT,
    EUC,
    MAX,
    MAN,
    CEIL,
    GEO
};

enum class EdgeWeightFormat{
    FUNCTION,
    FULL_MATRIX,
    UPPER_ROW,
    LOWER_ROW,
    UPPER_DIAG_ROW,
    LOWER_DIAG_ROW,
    UPPER_COL,
    LOWER_COL,
    UPPER_DIAG_COL,
    LOWER_DIAG_COL
};

enum class EdgeDataFormat{
    EDGE_LIST,
    ADJ_LIST
};

enum class NodeCoordType
{
    _2D,
    _3D,
    NO_COORD
};

struct DisplayDataType
{
    //TODO
};

struct Eof
{

};

struct Node2d
{
    uint32_t id;
    float x;
    float y;
};

struct Nodes2d
{
    std::vector<Node2d> nodes2d;
};

struct Node3d
{
    uint32_t id;
    float x;
    float y;
    float z;
};

struct Nodes3d
{
    std::vector<Node3d> nodes3d;
};

struct NodeCoordSection
{
    std::variant<Nodes2d, Nodes3d> nodesCoord;
};

struct DepotSection
{
    //TODO
};

struct DemandSection
{
    //TODO
};

struct EdgeDataSection
{
    std::vector<int32_t> edgeData;
};

struct FixedEdgesSection
{
    //TODO
};

struct DisplayDataSection
{
    //TODO
};

struct TourSection
{
    //TODO
};

struct EdgeWeightSection
{
    std::vector<int32_t> weights;
};

struct Config;

struct TspData
{
    using Item = std::variant<
        std::monostate,
        Name,
        AlgorithmType,
        Comment,
        Dimension,
        Capacity,
        EdgeWeightType,
        EdgeWeightFormat,
        EdgeDataFormat,
        Eof,
        NodeCoordSection,
        EdgeDataSection,
        EdgeWeightSection
    >;
    std::vector<Item> data;

    [[nodiscard]]
    auto filtered() -> Config;
};

struct Specification
{
    std::optional<std::string> name;
    std::optional<AlgorithmType> type;
    std::optional<std::string> comment;
    std::optional<uint32_t> dimension;
    std::optional<uint32_t> capacity;
    std::optional<EdgeWeightType> edgeWeightType;
    std::optional<EdgeWeightFormat> edgeWeightFormat;
    std::optional<EdgeDataFormat> edgeDataFormat;
    std::optional<NodeCoordType> nodeCoordType;
    //std::optional<DisplayDataType> diplayDataType; TODO
};

struct Data
{
    std::variant<Nodes2d, Nodes3d> nodeCoordSection;
    //std::optional<std::vector<uint32_t>> depotSection; TODO
    //std::optional<std::vector<uint32_t>> demandSection; TODO
    std::optional<std::vector<int32_t>> edgeDataSection;
    //std::optional<std::vector<uint32_t>> fixedEdgesSection; TODO
    // displayDataSection TODO
    // tourSection TODO
    std::optional<std::vector<int32_t>> edgeWeightSection;
};

struct Config
{
    Specification specification;
    Data data;
};


}
