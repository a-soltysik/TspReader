#pragma once

#include "Content.h"

namespace tsplib
{

[[nodiscard]]
auto getTspContent([[maybe_unused]] std::string_view input) -> std::optional<Content>;

}