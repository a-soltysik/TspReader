#pragma once

#include "Tags.h"
#include <functional>

namespace tsplib
{

template<typename DF>
concept DistanceFunction =
requires
{
    std::same_as<std::invoke_result_t<DF, Node2d, Node2d>, int32_t> ||
    std::same_as<std::invoke_result_t<DF, Node3d, Node3d>, int32_t>;
};

namespace detail
{

auto euclideanDistance(const Node2d& node1, const Node2d& node2) -> int32_t;

auto euclideanDistance(const Node3d& node1, const Node3d& node2) -> int32_t;

auto manhattanDistance(const Node2d& node1, const Node2d& node2) -> int32_t;

auto manhattanDistance(const Node3d& node1, const Node3d& node2) -> int32_t;

auto maxDistance(const Node2d& node1, const Node2d& node2) -> int32_t;

auto maxDistance(const Node3d& node1, const Node3d& node2) -> int32_t;

auto ceilDistance(const Node2d& node1, const Node2d& node2) -> int32_t;

auto ceilDistance(const Node3d& node1, const Node3d& node2) -> int32_t;

auto geographicalDistance(const Node2d& node1, const Node2d& node2) -> int32_t;

auto geographicalDistance(const Node3d& node1, const Node3d& node2) -> int32_t;

}

inline auto euclideanDistance = []<typename T>(T&& node1, T&& node2) -> int32_t {
    return detail::euclideanDistance(std::forward<T>(node1), std::forward<T>(node2));
};

inline auto manhattanDistance = []<typename T>(T&& node1, T&& node2) -> int32_t {
    return detail::manhattanDistance(std::forward<T>(node1), std::forward<T>(node2));
};

inline auto maxDistance = []<typename T>(T&& node1, T&& node2) -> int32_t {
    return detail::maxDistance(std::forward<T>(node1), std::forward<T>(node2));
};

inline auto ceilDistance = []<typename T>(T&& node1, T&& node2) -> int32_t {
    return detail::ceilDistance(std::forward<T>(node1), std::forward<T>(node2));
};

inline auto geographicalDistance = []<typename T>(T&& node1, T&& node2) -> int32_t {
    return detail::geographicalDistance(std::forward<T>(node1), std::forward<T>(node2));
};

inline auto getDistanceFunction(EdgeWeightType type) -> DistanceFunction auto
{
    return [type]<typename T>(T&& node1, T&& node2) -> int32_t {
        switch (type)
        {
        case EdgeWeightType::EUC:
            return euclideanDistance(std::forward<T>(node1), std::forward<T>(node2));
        case EdgeWeightType::MAN:
            return manhattanDistance(std::forward<T>(node1), std::forward<T>(node2));
        case EdgeWeightType::MAX:
            return maxDistance(std::forward<T>(node1), std::forward<T>(node2));
        case EdgeWeightType::CEIL:
            return ceilDistance(std::forward<T>(node1), std::forward<T>(node2));
        case EdgeWeightType::GEO:
            return geographicalDistance(std::forward<T>(node1), std::forward<T>(node2));
        default:
            return [](T&&, T&&) { return 0; }(std::forward<T>(node1), std::forward<T>(node2));
        }
    };
}

}