#include "DecisionTree.h"

#include "TextHelper.h"

template <std::size_t TMaxAttributes>
inline void DecisionTree<TMaxAttributes>::loadRecordStructure(char const* beg_, char const* end_)
{
	constexpr char lineCommentCharacter = '%';

	std::size_t idx = 0;
	auto tokenBeg = beg_;
	for(auto it = beg_; it != end_; ++it)
	{
		if (*it == lineCommentCharacter)
			break;

		if (isWhitespace(*it))
		{
			if (tokenBeg != it)
			{
				recordStructure[idx].assign(tokenBeg, it); 
				++idx;
			}
			tokenBeg = it + 1;
		}
	}
	// Append last token
	if (tokenBeg != end_)
	{
		recordStructure[idx].assign(tokenBeg, end_); 
		++idx;
	}

	// Clear all other strings.
	for(; idx < MaxAttributes; ++idx)
		recordStructure[idx].clear();
}