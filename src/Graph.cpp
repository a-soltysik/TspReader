#include "Graph.h"

#include <algorithm>
#include <numeric>

namespace tsplib
{

Graph::Graph(size_t order)
    : graph(order, std::vector<Weight>(order, Graph::INFINITY_WEIGHT))
{

}

bool Graph::EdgeData::operator==(const EdgeData& rhs) const noexcept
{
    return vertices == rhs.vertices && weight == rhs.weight;
}

bool Graph::EdgeData::operator!=(const EdgeData& rhs) const noexcept
{
    return !(*this == rhs);
}

auto Graph::addVertex() -> Vertex
{
    for (auto& row : graph)
    {
        row.push_back(Graph::INFINITY_WEIGHT);
    }

    const auto newVertex = getOrder() - 1;

    graph.emplace_back(getOrder() + 1, Graph::INFINITY_WEIGHT);

    return newVertex;
}

auto Graph::setOrder(size_t order) -> void
{
    for (auto& row : graph)
    {
        row.resize(order, Graph::INFINITY_WEIGHT);
    }

    graph.resize(order, std::vector<Weight>(order, Graph::INFINITY_WEIGHT));
}

auto Graph::addEdge(const EdgeData& edge) -> bool
{
    if (edge.vertices.first == edge.vertices.second)
    {
        return false;
    }

    if (!doesExist(edge.vertices.first) || !doesExist(edge.vertices.second))
    {
        return false;
    }

    if (doesExist(edge.vertices))
    {
        return false;
    }

    graph[edge.vertices.first][edge.vertices.second] = edge.weight;

    size++;

    return true;
}

auto Graph::removeEdge(Edge edge) -> bool
{
    if (!doesExist(edge))
    {
        return false;
    }

    graph[edge.first][edge.second] = INFINITY_WEIGHT;

    size--;

    return true;
}

auto Graph::getWeight(Edge edge) const -> std::optional<Weight>
{
    if (!doesExist(edge))
    {
        return {};
    }
    return graph[edge.first][edge.second];
}

auto Graph::setWeight(Edge edge, Weight weight) -> bool
{
    if (!doesExist(edge))
    {
        return false;
    }

    graph[edge.first][edge.second] = weight;

    return true;
}

auto Graph::getOrder() const -> size_t
{
    return graph.size();
}

auto Graph::getSize() const -> size_t
{
    return size;
}

auto Graph::getDensity() const -> double
{
    return static_cast<float>(getSize()) / static_cast<float>((getOrder() * (getOrder() - 1)));
}

auto Graph::getNumberOfNeighboursOf(Vertex vertex) const -> size_t
{
    if (!doesExist(vertex))
    {
        return 0;
    }

    return std::ranges::count_if(graph[vertex], [](auto weight) { return weight != INFINITY_WEIGHT; });
}

auto Graph::doesExist(Vertex vertex) const -> bool
{
    return vertex < getOrder();
}

auto Graph::doesExist(Edge edge) const -> bool
{
    if (!doesExist(edge.first) || !doesExist(edge.second))
    {
        return false;
    }
    return graph[edge.first][edge.second] != INFINITY_WEIGHT;
}

auto Graph::getNeighboursOf(Vertex vertex) const -> std::optional<std::vector<Neighbour>>
{
    if (!doesExist(vertex))
    {
        return {};
    }

    auto result = std::vector<Neighbour> {};
    result.reserve(getNumberOfNeighboursOf(vertex));

    for (auto i = Vertex {}; i < getOrder(); i++)
    {
        if (graph[vertex][i] != INFINITY_WEIGHT)
        {
            result.push_back({i, graph[vertex][i]});
        }
    }

    return result;
}

auto Graph::getVertices() const -> std::vector<Vertex>
{
    auto result = std::vector<Vertex>(getOrder());

    std::iota(result.begin(), result.end(), 0);

    return result;
}

auto Graph::getEdges() const -> std::vector<EdgeData>
{
    auto result = std::vector<EdgeData> {};
    result.reserve(getSize());

    for (auto i = Vertex {}; i < getOrder(); i++)
    {
        for (auto j = Vertex {}; j < getOrder(); j++)
        {
            if (graph[i][j] != INFINITY_WEIGHT)
            {
                result.push_back({{i, j}, graph[i][j]});
            }
        }
    }

    return result;
}

auto Graph::forEachNeighbourOf(Vertex vertex, const NeighbourPredicate& predicate) const -> void
{
    if (!doesExist(vertex))
    {
        return;
    }

    for (auto i = Vertex {}; i < getOrder(); i++)
    {
        if (graph[vertex][i] != INFINITY_WEIGHT)
        {
            predicate({i, graph[vertex][i]});
        }
    }
}

auto Graph::forEachVertex(const VertexPredicate& predicate) const -> void
{
    for (auto i = Vertex {}; i < getOrder(); i++)
    {
        predicate(i);
    }
}

auto Graph::forEachEdge(const EdgePredicate& predicate) const -> void
{
    for (auto i = Vertex {}; i < getOrder(); i++)
    {
        for (auto j = Vertex {}; j < getOrder(); j++)
        {
            if (graph[i][j] != Graph::INFINITY_WEIGHT)
            {
                predicate({{i, j}, graph[i][j]});
            }
        }
    }
}

}