#pragma once

#include <cctype>

inline bool isWhitespace(char c_)
{
	return std::isspace( static_cast<unsigned char>(c_) );
}