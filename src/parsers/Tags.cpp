#include "Tags.h"

namespace tsplib
{

auto TspData::filtered() -> Config
{
    Config config;

    for (const auto& item : data)
    {
        std::visit(match {
            [&config](const Name& name) { config.specification.name = name.name; },
            [&config](AlgorithmType type) { config.specification.type = type; },
            [&config](const Comment& comment) { config.specification.comment = comment.comment; },
            [&config](Dimension dimension) { config.specification.dimension = dimension.dimension; },
            [&config](Capacity capacity) { config.specification.capacity = capacity.capacity; },
            [&config](EdgeWeightType edgeWeightType) { config.specification.edgeWeightType = edgeWeightType; },
            [&config](EdgeWeightFormat edgeWeightFormat) { config.specification.edgeWeightFormat = edgeWeightFormat; },
            [&config](EdgeDataFormat edgeDataFormat) { config.specification.edgeDataFormat = edgeDataFormat; },
            [&config](NodeCoordType nodeCoordType) { config.specification.nodeCoordType = nodeCoordType; },
            [&config](
                const NodeCoordSection& nodeCoordSection) { config.data.nodeCoordSection = nodeCoordSection.nodesCoord; },
            [&config](
                const EdgeDataSection& edgeDataSection) { config.data.edgeDataSection = edgeDataSection.edgeData; },
            [&config](
                const EdgeWeightSection& edgeWeightSection) { config.data.edgeWeightSection = edgeWeightSection.weights; },
            [](auto) { }
        }, item);
    }

    return config;
}

}