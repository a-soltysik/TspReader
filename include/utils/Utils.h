#pragma once

#include "Numbers.h"

namespace tsplib::utils
{

constexpr const char* HORIZONTAL_BAR                     = "\xE2\x94\x80";
constexpr const char* VERTICAL_BAR                       = "\xE2\x94\x82";
constexpr const char* VERTICAL_BAR_RIGHT                 = "\xE2\x94\x9C";
constexpr const char* VERTICAL_BAR_LEFT                  = "\xE2\x94\xA4";
constexpr const char* UP_RIGHT                           = "\xE2\x94\x94";
constexpr const char* UP_LEFT                            = "\xE2\x94\x98";
constexpr const char* CROSS                              = "\xE2\x94\xBC";
constexpr const char* DOUBLE_CROSS                       = "\xE2\x95\xAC";
constexpr const char* HORIZONTAL_BAR_UP                  = "\xE2\x94\xB4";
constexpr const char* DOUBLE_HORIZONTAL_BAR              = "\xE2\x95\x90";
constexpr const char* DOUBLE_VERTICAL_BAR                = "\xE2\x95\x91";
constexpr const char* VERTICAL_BAR_DOUBLE_LEFT           = "\xE2\x95\xA1";
constexpr const char* DOUBLE_VERTICAL_BAR_HORIZONTAL_BAR = "\xE2\x95\xAB";
constexpr const char* HORIZONTAL_BAR_DOUBLE_UP           = "\xE2\x95\xA8";
constexpr const char* DOUBLE_HORIZONTAL_BAR_VERTICAL_BAR = "\xE2\x95\xAA";

template<typename T>
[[nodiscard]]
auto toString(const T& value) -> std::string
{
    if constexpr(std::is_arithmetic_v<T>)
    {
        return numberToString(value);
    }
    else if constexpr(std::same_as<char*, std::decay_t<T>> ||
                      std::same_as<const char*, std::decay_t<T>> ||
                      std::same_as<std::string, std::decay_t<T>>)
    {
        return value;
    }
    else
    {
        return value.toString();
    }
}

[[nodiscard]] auto putInStringCenter(const auto& object, size_t width, char space = ' ') -> std::string
{
    auto objectStr = toString(object);
    const auto objectStrLength = objectStr.length();

    if (width <= objectStrLength )
    {
        return objectStr;
    }

    const auto halfSpaces = (width - objectStrLength ) / 2;

    return std::string(halfSpaces, space) + std::move(objectStr) + std::string(width - halfSpaces - objectStrLength, space);
}

}
