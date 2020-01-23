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
std::string trimAndRemComment(std::string line_)
{
	auto commentPos = line_.find('%');
	if (commentPos != std::string::npos)
		line_ = line_.substr(0, commentPos);

	auto notSpacePos = line_.find_first_not_of(" \t\v");
	if (notSpacePos == std::string::npos)
		return {};
	else
		line_ = line_.substr(notSpacePos);
	return line_;
}