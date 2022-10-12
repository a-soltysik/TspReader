#pragma once

#include <optional>
#include <vector>
#include <cstdint>
#include <istream>
#include <functional>
#include <limits>

namespace tsplib
{

class Graph
{
public:
    struct Neighbour;
    struct EdgeData;

    using Vertex = size_t;
    using Weight = int32_t;
    using Edge = std::pair<Vertex, Vertex>;

    using VertexPredicate = std::function<void(Vertex)>;
    using NeighbourPredicate = std::function<void(Neighbour)>;
    using EdgePredicate = std::function<void(const EdgeData&)>;

    struct Neighbour
    {
        Vertex vertex;
        Weight weight;
    };

    struct EdgeData
    {
        Edge vertices;
        Weight weight;

        [[nodiscard]]
        auto operator==(const EdgeData& rhs) const noexcept -> bool;
        [[nodiscard]]
        auto operator!=(const EdgeData& rhs) const noexcept -> bool;
    };

    static constexpr Weight INFINITY_WEIGHT = std::numeric_limits<Weight>::max();

    Graph() = default;
    explicit Graph(size_t order);

    auto addVertex() -> Vertex;
    auto setOrder(size_t order) -> void;

    auto addEdge(const EdgeData& edge) -> bool;
    auto removeEdge(Edge edge) -> bool;

    [[nodiscard]]
    auto getOrder() const -> size_t;
    [[nodiscard]]
    auto getSize() const -> size_t;
    [[nodiscard]]
    auto getDensity() const -> double;
    [[nodiscard]]
    auto getNumberOfNeighboursOf(Vertex vertex) const -> size_t;

    [[nodiscard]]
    auto getWeight(Edge edge) const -> std::optional<Weight>;
    auto setWeight(Edge edge, Weight weight) -> bool;

    [[nodiscard]]
    auto doesExist(Vertex vertex) const -> bool;
    [[nodiscard]]
    auto doesExist(Edge edge) const -> bool;

    [[nodiscard]]
    auto getNeighboursOf(Vertex vertex) const -> std::optional<std::vector<Neighbour>>;
    [[nodiscard]]
    auto getVertices() const -> std::vector<Vertex>;
    [[nodiscard]]
    auto getEdges() const -> std::vector<EdgeData>;

    auto forEachNeighbourOf(Vertex vertex, const NeighbourPredicate& predicate) const -> void;
    auto forEachVertex(const VertexPredicate& predicate) const -> void;
    auto forEachEdge(const EdgePredicate& predicate) const -> void;

    [[nodiscard]]
    auto toString() const -> std::string;

private:
    std::vector<std::vector<Weight>> graph;
    size_t size = 0;
};

}
