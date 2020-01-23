#include "TextHelper.h"

////////////////////////////////
bool isWhitespace(char c_)
{
	return std::isspace( static_cast<unsigned char>(c_) );
}

////////////////////////////////
char const* findFirstNonSpace(char const* beg_, char const* end_)
{
	return std::find_if(beg_, end_, [](char c) { return !isWhitespace(c); });
}

////////////////////////////////
char const* findFirstSpace(char const* beg_, char const* end_)
{
	return std::find_if(beg_, end_, [](char c) { return isWhitespace(c); });
}

////////////////////////////////
std::string trimAndRemComment(std::string const& line_)
{
	constexpr char const* inlineWhitespaces = " \t\v";
	constexpr char commentCharacter = '%';

	auto notSpacePos = line_.find_first_not_of(inlineWhitespaces);
	if (notSpacePos == std::string::npos)
		return {};

	auto commentPos = line_.find(commentCharacter, notSpacePos);
	if (commentPos != std::string::npos)
		return line_.substr(notSpacePos, commentPos - notSpacePos);

	return line_.substr(notSpacePos);
}