#pragma once

#include <cctype>
#include <algorithm>

////////////////////////////////
inline bool isWhitespace(char c_)
{
	return std::isspace( static_cast<unsigned char>(c_) );
}

////////////////////////////////
inline char const* findFirstNonSpace(char const* beg_, char const* end_)
{
	return std::find_if(beg_, end_, [](char c) { return !isWhitespace(c); });
}

////////////////////////////////
inline char const* findFirstSpace(char const* beg_, char const* end_)
{
	return std::find_if(beg_, end_, [](char c) { return isWhitespace(c); });
}