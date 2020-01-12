#include "DecisionTree.h"

#include "TextHelper.h"

#include <string>

///////////////////////////////////////////////////////////////////
DecisionTreeNode::~DecisionTreeNode()
{
	if (failed)
		delete failed;
	if (succeeded)
		delete succeeded;
}

///////////////////////////////////////////////////////////////////
DecisionTreeNode::Anchor::Anchor(std::string label_)
	:
	label( std::move(label_) ),
	isLabel( true )
{
}

///////////////////////////////////////////////////////////////////
DecisionTreeNode::Anchor::Anchor(std::uint32_t nodeIndex_)
	:
	nodeIndex( std::move(nodeIndex_) ),
	isLabel( false )
{

}

///////////////////////////////////////////////////////////////////
void readNode(DecisionTreeNode &node_, char const* beg_, char const* end_)
{
	constexpr char lineCommentCharacter = '%';

	// Trim whitespaces at the beginning of the string
	beg_ = findFirstNonSpace(beg_, end_);

	// Trim line comment
	end_ = std::find(beg_, end_, lineCommentCharacter);

	auto it = beg_;

	// Read node index:
	{
		auto idxEnd = findFirstSpace(it, end_);
		node_.index = std::stoul(std::string{it, idxEnd});

		it = findFirstNonSpace(idxEnd, end_);
	}

	DecisionTreeNode::Condition cond;
	DecisionTreeNode::Anchor failed, succeeded;

	it = findFirstNonSpace( readNodeCondition(node_.cond, it, end_), end_ );
	it = findFirstNonSpace( readNodeAnchor(node_.failedAnchor, it, end_), end_ );
	readNodeAnchor(node_.succeededAnchor, it, end_);
}

///////////////////////////////////////////////////////////////////
char const* readNodeCondition(DecisionTreeNode::Condition &cond_, char const* beg_, char const* end_)
{
	using Cond = DecisionTreeNode::Condition;
	constexpr char ltChar = '<';
	constexpr char gtChar = '>';

	auto it = beg_;

	// Read condition attribute name:
	{
		auto attEnd = std::find_if(it, end_,
			[](char c) {
				return c == ltChar || c == gtChar || isWhitespace(c);
			});

		cond_.attributeName.assign(it, attEnd);

		it = findFirstNonSpace(attEnd, end_);
	}
	
	// Read condition operator:
	if (*it == gtChar)
		cond_.op = Cond::GreaterThan;
	else if (*it == ltChar)
		cond_.op = Cond::LowerThan;
	else
		throw std::runtime_error("invalid condition operator");

	it = findFirstNonSpace(it + 1, end_);
	
	// Read condition value
	{
		auto valueEnd = findFirstSpace(it, end_);
		cond_.value = std::stod(std::string{it, valueEnd});
		it = valueEnd;
	}
	return it;
}

///////////////////////////////////////////////////////////////////
char const* readNodeAnchor(DecisionTreeNode::Anchor &anchor_, char const* beg_, char const* end_)
{
	using Anchor = DecisionTreeNode::Anchor;

	auto anchorEnd = findFirstSpace(beg_, end_);

	// Try to read index:
	try {
		anchor_ = Anchor{ std::stoul( std::string{ beg_, anchorEnd} ) };
	}
	catch(...) {
		// If failed, treat it as a label
		anchor_ = Anchor{ std::string{beg_, anchorEnd } };
	}

	return anchorEnd;
}
