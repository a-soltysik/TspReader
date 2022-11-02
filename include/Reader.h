#pragma once

#include "Content.h"

#ifdef NDEBUG

namespace tsplib
{

[[nodiscard]]
auto getTspContent(std::string_view input) -> std::optional<Content>;

}

#endif
