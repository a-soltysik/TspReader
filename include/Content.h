#pragma once

#include "Graph.h"

#include <optional>
#include <string>

namespace tsplib
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

}
