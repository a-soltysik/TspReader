#include "Reader.h"
#include "SubParsers.h"
#include "GraphParser.h"

namespace tsp
{

auto getGraphFromConfig(const Config& config) -> std::optional<Graph>;
auto getTypeFromConfig(const Config& config) -> std::optional<Type>;

auto canGraphBeMadeFromEdgeData(const Config& config) -> bool;
auto canGraphBeMadeFromWeights(const Config& config) -> bool;

auto getTspContent(std::string_view input) -> std::optional<Content>
{
    const auto data = tspData(input);

    if (!data)
    {
        return {};
    }

    const auto config = data->first;
    const auto graph = getGraphFromConfig(config);

    if (!graph.has_value())
    {
        return {};
    }

    auto content = Content{
        .metaData = {
            .name = config.specification.name,
            .comment = config.specification.comment,
            .type = getTypeFromConfig(config)
        },
        .graph = graph.value()
    };

    return content;
}

auto getGraphFromConfig(const Config& config) -> std::optional<Graph>
{
    if (canGraphBeMadeFromEdgeData(config))
    {
        return std::visit(match{
            [&config](const Nodes2d& nodes2d) {
                return makeGraphFromEdgeData(config.data.edgeDataSection.value(),
                                              config.specification.edgeDataFormat.value(),
                                              nodes2d.nodes2d,
                                              getDistanceFunction(config.specification.edgeWeightType.value()));

            },
            [&config](const Nodes3d& nodes3d) {
                return makeGraphFromEdgeData(config.data.edgeDataSection.value(),
                                              config.specification.edgeDataFormat.value(),
                                              nodes3d.nodes3d,
                                              getDistanceFunction(config.specification.edgeWeightType.value()));
            },
        }, config.data.nodeCoordSection.value());
    }
    else if (canGraphBeMadeFromWeights(config))
    {
        return makeGraphFromWeights(config.data.edgeWeightSection.value(), config.specification.edgeWeightFormat.value());
    }
    else
    {
        return {};
    }
}

auto getTypeFromConfig(const Config& config) -> std::optional<Type>
{
    if (!config.specification.type)
    {
        return {};
    }

    switch (config.specification.type.value())
    {
    case AlgorithmType::TSP:
        return Type::TSP;
    case AlgorithmType::ATSP:
        return Type::ATSP;
    case AlgorithmType::SOP:
        return Type::SOP;
    case AlgorithmType::HCP:
        return Type::HCP;
    case AlgorithmType::CRVP:
        return Type::CRVP;
    case AlgorithmType::TOUR:
        return Type::TOUR;
    default:
        return Type::TSP;
    }
}

auto canGraphBeMadeFromEdgeData(const Config& config) -> bool
{
    return config.data.edgeDataSection.has_value() &&
           config.specification.edgeDataFormat.has_value() &&
           config.data.nodeCoordSection.has_value() &&
           config.specification.edgeWeightType.has_value() &&
           config.specification.edgeWeightType.value() != EdgeWeightType::EXPLICIT;
}

auto canGraphBeMadeFromWeights(const Config& config) -> bool
{
    return config.data.edgeWeightSection.has_value() && config.specification.edgeWeightFormat.has_value();
}

}
