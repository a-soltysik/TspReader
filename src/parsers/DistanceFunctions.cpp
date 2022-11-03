#include "Tags.h"
#include "DistanceFunctions.h"

#include <algorithm>
#include <cmath>
#include <numbers>

namespace tsplib::detail
{

auto euclideanDistance(const Node2d& node1, const Node2d& node2) -> int32_t
{
    const auto xDistance = node1.x - node2.x;
    const auto yDistance = node1.y - node2.y;

    return static_cast<int32_t>(std::lround(
        std::sqrt(xDistance * xDistance + yDistance * yDistance)
    ));
}

auto euclideanDistance(const Node3d& node1, const Node3d& node2) -> int32_t
{
    const auto xDistance = node1.x - node2.x;
    const auto yDistance = node1.y - node2.y;
    const auto zDistance = node1.z - node2.z;

    return static_cast<int32_t>(std::lround(
        std::sqrt(xDistance * xDistance + yDistance * yDistance + zDistance * zDistance)
    ));
}

auto manhattanDistance(const Node2d& node1, const Node2d& node2) -> int32_t
{
    const auto xDistance = std::abs(node1.x - node2.x);
    const auto yDistance = std::abs(node1.y - node2.y);

    return static_cast<int32_t>(std::lround(
        xDistance + yDistance
    ));
}

auto manhattanDistance(const Node3d& node1, const Node3d& node2) -> int32_t
{
    const auto xDistance = std::abs(node1.x - node2.x);
    const auto yDistance = std::abs(node1.y - node2.y);
    const auto zDistance = std::abs(node1.z - node2.z);

    return static_cast<int32_t>(std::lround(
        xDistance + yDistance + zDistance
    ));
}

auto maxDistance(const Node2d& node1, const Node2d& node2) -> int32_t
{
    const auto xDistance = std::abs(node1.x - node2.x);
    const auto yDistance = std::abs(node1.y - node2.y);

    return static_cast<int32_t>(std::max(std::lround(xDistance), std::lround(yDistance)));
}

auto maxDistance(const Node3d& node1, const Node3d& node2) -> int32_t
{
    const auto xDistance = std::abs(node1.x - node2.x);
    const auto yDistance = std::abs(node1.y - node2.y);
    const auto zDistance = std::abs(node1.z - node2.z);

    return static_cast<int32_t>(std::ranges::max(
        {std::lround(xDistance), std::lround(yDistance), std::lround(zDistance)}));
}

auto geographicalDistance(const Node2d& node1, const Node2d& node2) -> int32_t
{
    const auto degx1     = std::lround(node1.x);
    const auto minx1     = node1.x - degx1;
    const auto latitude1 = std::numbers::pi * (degx1 + 5.0 * minx1 / 3.0) / 180.0;

    const auto degy1      = std::lround(node1.y);
    const auto miny1      = node1.y - degy1;
    const auto longitude1 = std::numbers::pi * (degy1 + 5.0 * miny1 / 3.0) / 180.0;


    const auto degx2     = std::lround(node2.x);
    const auto minx2     = node2.x - degx2;
    const auto latitude2 = std::numbers::pi * (degx2 + 5.0 * minx2 / 3.0) / 180.0;

    const auto degy2      = std::lround(node2.y);
    const auto miny2      = node2.y - degy2;
    const auto longitude2 = std::numbers::pi * (degy2 + 5.0 * miny2 / 3.0) / 180.0;

    static constexpr auto R = 6378.388;

    const auto q1 = std::cos(longitude1 - longitude2);
    const auto q2 = std::cos(latitude1 - latitude2);
    const auto q3 = std::cos(latitude1 + latitude2);

    return static_cast<int32_t>(
        R * std::acos(0.5 * ((1.0 + q1) * q2 - (1.0 - q1) * q3)) + 1.0
    );
}

auto geographicalDistance(const Node3d& node1, const Node3d& node2) -> int32_t
{
    return geographicalDistance(Node2d{node1.id, node1.x, node1.y}, Node2d{node2.id, node2.x, node2.y});
}

auto ceilDistance(const Node2d& node1, const Node2d& node2) -> int32_t
{
    const auto xDistance = node1.x - node2.x;
    const auto yDistance = node1.y - node2.y;

    return static_cast<int32_t>(std::ceil(
        std::sqrt(xDistance * xDistance + yDistance * yDistance)
    ));
}

auto ceilDistance(const Node3d& node1, const Node3d& node2) -> int32_t
{
    const auto xDistance = node1.x - node2.x;
    const auto yDistance = node1.y - node2.y;
    const auto zDistance = node1.z - node2.z;

    return static_cast<int32_t>(std::ceil(
            std::sqrt(xDistance * xDistance + yDistance * yDistance + zDistance * zDistance)
        )
    );
}

}