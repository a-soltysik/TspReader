#pragma once

#include "Graph.h"
#include "DistanceFunctions.h"
#include <algorithm>
#include <ranges>

namespace tsplib
{
[[nodiscard]]
auto makeGraphFromEdgeData(const std::vector<int32_t>& edgeData,
                           EdgeDataFormat format,
                           std::ranges::range auto&& nodes,
                           DistanceFunction auto distanceFunction) -> std::optional<Graph>;

[[nodiscard]]
auto makeGraphFromWeights(const std::vector<int32_t>& weights, EdgeWeightFormat format) -> std::optional<Graph>;

namespace detail
{

[[nodiscard]]
auto makeGraphFromEdgeList(const std::vector<Graph::Edge>& edgeList,
                           std::ranges::range auto&& nodes,
                           DistanceFunction auto distanceFunction) -> Graph
{
    const auto vertexCount = std::ranges::max_element(edgeList, [](auto edge1, auto edge2) {
        return std::max(edge1.first, edge1.second) > std::max(edge2.first, edge2.second);
    });

    auto result = Graph {std::max(vertexCount->first, vertexCount->second)};

    for (auto edge : edgeList)
    {
        const auto begin = std::ranges::find(nodes, edge.first, [](const auto& node) { return node.id; });
        const auto end   = std::ranges::find(nodes, edge.second, [](const auto& node) { return node.id; });

        if (begin != nodes.end() && end != nodes.end())
        {
            result.addEdge({edge, distanceFunction(*begin, *end)});
        }
    }

    return result;
}

[[nodiscard]]
auto makeGraphFromAdjacencyList(const std::vector<std::vector<Graph::Vertex>>& adjacencyList,
                                std::ranges::range auto&& nodes,
                                DistanceFunction auto distanceFunction) -> Graph
{
    const auto vertexCount = adjacencyList.size();

    auto result = Graph {vertexCount};

    for (auto i = size_t {}; i < adjacencyList.size(); i++)
    {
        const auto begin = std::ranges::find(nodes, i, [](const auto& node) { return node.id; });

        for (auto vertex : adjacencyList[i])
        {
            const auto end = std::ranges::find(nodes, vertex, [](const auto& node) { return node.id; });

            if (begin != nodes.end() && end != nodes.end())
            {
                result.addEdge({{i, vertex}, distanceFunction(*begin, *end)});
            }
        }
    }

    return result;
}

auto makeEdgeList(const std::vector<int32_t>& edgeData) -> std::optional<std::vector<Graph::Edge>>;

auto makeAdjacencyList(const std::vector<int32_t>& edgeData) -> std::optional<std::vector<std::vector<Graph::Vertex>>>;

}

auto makeGraphFromEdgeData(const std::vector<int32_t>& edgeData,
                           EdgeDataFormat format,
                           std::ranges::range auto&& nodes,
                           DistanceFunction auto distanceFunction) -> std::optional<Graph>
{
    switch (format)
    {
    case EdgeDataFormat::EDGE_LIST:
    {
        auto edgeList = detail::makeEdgeList(edgeData);
        if (edgeList)
        {
            return detail::makeGraphFromEdgeList(edgeList.value(),
                                                 std::forward<decltype(nodes)>(nodes),
                                                 distanceFunction);
        }
        else
        {
            return {};
        }
    }
    case EdgeDataFormat::ADJ_LIST:
    {
        auto adjacencyList = detail::makeAdjacencyList(edgeData);
        if (adjacencyList)
        {
            return detail::makeGraphFromAdjacencyList(adjacencyList.value(),
                                                      std::forward<decltype(nodes)>(nodes),
                                                      distanceFunction);
        }
        else
        {
            return {};
        }
    }
    default:
        return {};
    }
}

}
