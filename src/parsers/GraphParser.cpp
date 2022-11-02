#include "GraphParser.h"

#include <cmath>
#include <iostream>

namespace tsplib
{

auto makeGraphFromFullMatrix(const std::vector<int32_t>& weights) -> std::optional<Graph>;

auto makeGraphFromWeights(const std::vector<int32_t>& weights, EdgeWeightFormat format) -> std::optional<Graph>
{
    using enum EdgeWeightFormat;
    switch (format)
    {
    case FULL_MATRIX:
        return makeGraphFromFullMatrix(weights);
        //TODO other formats
    default:
        return {};
    }
}

auto makeGraphFromFullMatrix(const std::vector<int32_t>& weights) -> std::optional<Graph>
{
    const auto size = static_cast<size_t>(std::llround(std::sqrt(weights.size())));
    if (size * size != weights.size())
    {
        return {};
    }

    auto result = Graph{size};

    auto i = size_t {};
    auto j = size_t {};
    for (auto weight : weights)
    {
        result.addEdge({{j, i}, weight});

        if (++i == size)
        {
            i = 0;
            j++;
        }
    }

    return result;
}

namespace detail
{

auto makeEdgeList(const std::vector<int32_t>& edgeData) -> std::optional<std::vector<Graph::Edge>>
{
    if ((edgeData.size() - 1) % 2 != 0 || edgeData.back() != -1)
    {
        return {};
    }

    if (edgeData.empty())
    {
        return {{}};

    }
    auto result = std::vector<Graph::Edge> {};
    result.reserve((edgeData.size() - 1) / 2);

    for (auto i = size_t {}; i < edgeData.size() - 1; i += 2)
    {
        result.emplace_back(edgeData[i], edgeData[i + 1]);
    }

    return result;
}

auto makeAdjacencyList(const std::vector<int32_t>& edgeData) -> std::optional<std::vector<std::vector<Graph::Vertex>>>
{
    if (edgeData.empty())
    {
        return {{}};
    }

    const auto [minNodeNumber, maxNodeNumber] = std::ranges::minmax_element(edgeData);

    if (*maxNodeNumber < 0 || *minNodeNumber < -1)
    {
        return {};
    }

    auto result = std::vector<std::vector<Graph::Vertex>>();
    result.reserve(static_cast<size_t>(*maxNodeNumber) + 1);

    for (const auto adjacency : edgeData | std::ranges::views::split(-1))
    {
        const auto unsignedAdjacency = adjacency
                                       | std::ranges::views::transform(
            [](auto vertex) { return static_cast<Graph::Vertex>(vertex); })
                                       | std::ranges::views::common;

        auto adjacencyVec = std::vector(std::next(unsignedAdjacency.begin()), unsignedAdjacency.end());
        result.push_back(std::move(adjacencyVec));
    }
    return result;
}

}

}
