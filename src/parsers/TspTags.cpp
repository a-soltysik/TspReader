#pragma once

#include "TspTags.h"

namespace Tsp
{

template<typename... Ts> struct match : Ts... { using Ts::operator()...; };
template<typename... Ts> match(Ts...) -> match<Ts...>;

auto TspData::filtered() -> Config
{
    Config config;

    for (const auto& item : data)
    {
        std::visit(match{
            [](auto){}
        }, item);
    }

    return config;
}

}