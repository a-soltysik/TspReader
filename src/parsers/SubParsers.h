#pragma once

#include "Parser.h"
#include "Tags.h"

namespace tsplib
{

template<typename T>
concept TspParser = fp::Parser<TspData::Item>;

[[nodiscard]]
auto name(std::string_view input) -> fp::Result<TspData::Item>;

[[nodiscard]]
auto type(std::string_view input) -> fp::Result<TspData::Item>;

[[nodiscard]]
auto comment(std::string_view input) -> fp::Result<TspData::Item>;

[[nodiscard]]
auto dimension(std::string_view input) -> fp::Result<TspData::Item>;

[[nodiscard]]
auto capacity(std::string_view input) -> fp::Result<TspData::Item>;

[[nodiscard]]
auto edgeWeightType(std::string_view input) -> fp::Result<TspData::Item>;

[[nodiscard]]
auto edgeWeightFormat(std::string_view input) -> fp::Result<TspData::Item>;

[[nodiscard]]
auto edgeDataFormat(std::string_view input) -> fp::Result<TspData::Item>;

[[nodiscard]]
auto node2d(std::string_view input) -> fp::Result<Node2d>;

[[nodiscard]]
auto nodes2d(std::string_view input) -> fp::Result<NodeCoordSection>;

[[nodiscard]]
auto node3d(std::string_view input) -> fp::Result<Node3d>;

[[nodiscard]]
auto nodes3d(std::string_view input) -> fp::Result<NodeCoordSection>;

[[nodiscard]]
auto nodeCoordSection(std::string_view input) -> fp::Result<TspData::Item>;

[[nodiscard]]
auto edgeDataSection(std::string_view input) -> fp::Result<TspData::Item>;

[[nodiscard]]
auto edgeWeightSection(std::string_view input) -> fp::Result<TspData::Item>;

[[nodiscard]]
auto tspData(std::string_view input) -> fp::Result<Config>;

}