#pragma once

#include "Content.h"

namespace tsp
{

[[nodiscard]]
auto getTspContent(std::string_view input) -> std::optional<Content>;

}
