#include "Graph.h"
#include "utils/utils.h"

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

auto Graph::getWeightUnchecked(Graph::Edge edge) const -> Weight
{
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

auto Graph::toString() const -> std::string
{
    std::string result = "\n";
    const auto columnWidth = getColumnWidth();
    const auto separator = rowSeparator(getSize() + 1, columnWidth);

    for (uint32_t i = 0; i <= getOrder(); i++)
    {
        for (uint32_t j = 0; j <= getSize(); j++)
        {
            if (i == 0)
            {
                if (j == 0)
                {
                    result += std::string(columnWidth, ' ') + utils::DOUBLE_VERTICAL_BAR;
                }
                else
                {
                    const auto number = utils::numberToString(j - 1);
                    result += utils::putInStringCenter(number, columnWidth) + utils::VERTICAL_BAR;
                }
            }
            else
            {
                if (j == 0)
                {
                    const auto number = utils::numberToString(i - 1);
                    result += utils::putInStringCenter(number, columnWidth) + utils::DOUBLE_VERTICAL_BAR;
                }
                else
                {
                    auto number = std::string{};
                    if (graph[i - 1][j - 1] == INFINITY_WEIGHT)
                    {
                        number = "inf";
                    }
                    else
                    {
                        number = utils::numberToString(graph[i - 1][j - 1]);
                    }
                    result += utils::putInStringCenter(number, columnWidth) + utils::VERTICAL_BAR;
                }
            }
        }
        if (i == 0)
        {
            result += "\n" + openingSeparator(getSize() + 1, columnWidth) + "\n";
        }
        else if (i != getOrder())
        {
            result += "\n" + separator + "\n";
        }
        else
        {
            result += "\n" + closingSeparator(getSize() + 1, columnWidth);
        }
    }
    return result;
}

auto Graph::getColumnWidth() const -> size_t
{
    auto columnWidth = size_t{};
    for (uint32_t i = 0; i <= getSize(); i++)
    {
        if (i == 0)
        {
            const auto length = utils::numberToString(getOrder() - 1).length();
            if (length > columnWidth)
            {
                columnWidth = length;
            }
        }
        else
        {
            for (uint32_t j = 0; j <= getOrder(); j++)
            {
                auto weightLength = size_t{};
                if (j == 0)
                {
                    weightLength = utils::numberToString(i - 1).length();
                }
                else if (graph[j - 1][i - 1] == INFINITY_WEIGHT)
                {
                    weightLength = 3;
                }
                else
                {
                    weightLength = utils::numberToString(graph[j - 1][i - 1]).length();
                }

                if (weightLength > columnWidth)
                {
                    columnWidth = weightLength;
                }
            }
        }
    }

    return columnWidth;
}

auto Graph::rowSeparator(size_t columns, size_t columnWidth) -> std::string
{
    auto result = std::string{};
    for (uint32_t i = 0; i < columns; i++)
    {
        for (uint32_t j = 0; j < columnWidth; j++)
        {
            result += utils::HORIZONTAL_BAR;
        }
        if (i == 0)
        {
            result += utils::DOUBLE_VERTICAL_BAR_HORIZONTAL_BAR;
        }
        else if (i != columns - 1)
        {
            result += utils::CROSS;
        }
        else
        {
            result += utils::VERTICAL_BAR_LEFT;
        }
    }
    return result;
}

auto Graph::closingSeparator(size_t columns, size_t columnWidth) -> std::string
{
    auto result = std::string{};
    for (uint32_t i = 0; i < columns; i++)
    {
        for (uint32_t j = 0; j < columnWidth; j++)
        {
            result += utils::HORIZONTAL_BAR;
        }
        if (i == 0)
        {
            result += utils::HORIZONTAL_BAR_DOUBLE_UP;
        }
        else if (i != columns - 1)
        {
            result += utils::HORIZONTAL_BAR_UP;
        }
        else
        {
            result += utils::UP_LEFT;
        }
    }
    return result;
}

auto Graph::openingSeparator(size_t columns, size_t columnWidth) -> std::string
{
    auto result = std::string{};
    for (uint32_t i = 0; i < columns; i++)
    {
        for (uint32_t j = 0; j < columnWidth; j++)
        {
            result += utils::DOUBLE_HORIZONTAL_BAR;
        }
        if (i == 0)
        {
            result += utils::DOUBLE_CROSS;
        }
        else if (i != columns - 1)
        {
            result += utils::DOUBLE_HORIZONTAL_BAR_VERTICAL_BAR;
        }
        else
        {
            result += utils::VERTICAL_BAR_DOUBLE_LEFT;
        }
    }
    return result;
}

auto Graph::isEmpty() const -> bool
{
    return getOrder() == 0;
}

auto Graph::isComplete() const -> bool
{
    return size == getOrder() * (getOrder() - 1) && !isEmpty() && !isEdgeless();
}

auto Graph::isEdgeless() const -> bool
{
    return size == 0;
}

}