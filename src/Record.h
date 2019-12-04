#pragma once

#include <string>
#include <tuple>

template <std::size_t TMaxAttributes>
using RecordStructure = std::array<std::string, TMaxAttributes>;

template <std::size_t TMaxAttributes = 16> 
using AttributeArray = std::array<double, TMaxAttributes>;