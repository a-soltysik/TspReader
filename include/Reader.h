#pragma once

#include "Content.h"

namespace tsplib
{

[[nodiscard]]
auto getTspContent(std::string_view input) -> std::optional<Content>;

}
