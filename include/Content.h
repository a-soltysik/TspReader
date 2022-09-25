#pragma once

#include "Graph.h"

#include <optional>
#include <string>

namespace Tsp
{

enum class Type
{
    TSP,
    ATSP,
    SOP,
    HCP,
    CRVP,
    TOUR
};

struct MetaData
{
    std::optional<std::string> name;
    std::optional<std::string> comment;
    std::optional<Type> type;
};

struct Content
{
    MetaData metaData;
    Graph graph;
};

auto addName(const MetaData& metaData, const std::optional<std::string>& name) -> MetaData;
auto addComment(const MetaData& metaData, const std::optional<std::string>& comment) -> MetaData;
auto addType(const MetaData& metaData, const std::optional<Type>& type) -> MetaData;

}
