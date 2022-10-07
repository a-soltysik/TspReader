#pragma once

#include "Parser.h"
#include "Tags.h"

namespace tsp
{

[[nodiscard]]
auto name(std::string_view input) -> fp::Result<Name>;

[[nodiscard]]
auto type(std::string_view input) -> fp::Result<AlgorithmType>;

[[nodiscard]]
auto comment(std::string_view input) -> fp::Result<Comment>;

[[nodiscard]]
auto dimension(std::string_view input) -> fp::Result<Dimension>;

[[nodiscard]]
auto capacity(std::string_view input) -> fp::Result<Capacity>;

[[nodiscard]]
auto edgeWeightType(std::string_view input) -> fp::Result<EdgeWeightType>;

[[nodiscard]]
auto edgeWeightFormat(std::string_view input) -> fp::Result<EdgeWeightFormat>;

[[nodiscard]]
auto edgeDataFormat(std::string_view input) -> fp::Result<EdgeDataFormat>;

[[nodiscard]]
auto node2d(std::string_view input) -> fp::Result<Node2d>;

[[nodiscard]]
auto nodes2d(std::string_view input) -> fp::Result<NodeCoordSection>;

[[nodiscard]]
auto node3d(std::string_view input) -> fp::Result<Node3d>;

[[nodiscard]]
auto nodes3d(std::string_view input) -> fp::Result<NodeCoordSection>;

[[nodiscard]]
auto nodeCoordSection(std::string_view input) -> fp::Result<NodeCoordSection>;

[[nodiscard]]
auto edgeDataSection(std::string_view input) -> fp::Result<EdgeDataSection>;

[[nodiscard]]
auto edgeWeightSection(std::string_view input) -> fp::Result<EdgeWeightSection>;

[[nodiscard]]
auto tspData(std::string_view input) -> fp::Result<Config>;

}