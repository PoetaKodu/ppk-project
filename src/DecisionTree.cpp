#include "DecisionTree.h"

#include "BinarySearchTree.h"
#include "TextHelper.h"

#include <string>
#include <iostream>

///////////////////////////////////////////////////////////////////
DecisionTreeNode::DecisionTreeNode(DecisionTreeNode && rhs_)
{
	index = rhs_.index;
	std::swap(rhs_.cond, cond);
	std::swap(rhs_.succeededAnchor, succeededAnchor);
	std::swap(rhs_.failedAnchor, failedAnchor);
	std::swap(rhs_.succeeded, succeeded);
	std::swap(rhs_.failed, failed);
}

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
DecisionTree readDecisionTree(char const* beg_, char const* end_)
{
	constexpr char newLineCharacter = '\n';

	DecisionTree tree;

	BinarySearchTree<std::uint32_t, DecisionTree::Node*> parentsOfFailed, parentsOfSucceeded;

	do
	{
		auto endOfLine = std::find(beg_, end_, newLineCharacter);

		DecisionTreeNode node;
		readNode(node, beg_, endOfLine);

		DecisionTreeNode* insertedNode = nullptr;
		if (!tree.root)
		{
			tree.root = insertedNode = new DecisionTreeNode( std::move(node) );
		}
		else
		{
			DecisionTreeNode* parent = tree.root;
			bool useFailedAnchor = true;

			if(parent = parentsOfFailed.get(node.index))
				parent->failed = insertedNode = new DecisionTreeNode{ std::move(node) };
			else if (parent = parentsOfSucceeded.get(node.index))
				parent->succeeded = insertedNode = new DecisionTreeNode{ std::move(node) };
			else
				throw std::runtime_error("tree structure file is broken (invalid link)");
		}

		if (!insertedNode->failedAnchor.isLabel)
			parentsOfFailed.set(insertedNode->failedAnchor.nodeIndex, insertedNode);
		if (!insertedNode->succeededAnchor.isLabel)
			parentsOfSucceeded.set(insertedNode->succeededAnchor.nodeIndex, insertedNode);

		beg_ = endOfLine + 1;
	}
	while(beg_ <= end_);

	return tree;
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
	std::string b = std::string{ beg_, anchorEnd};
	// Try to read index:
	try {
		anchor_ = Anchor{ std::stoul( b ) };
	}
	catch(...) {
		// If failed, treat it as a label
		anchor_ = Anchor{ b };
	}

	return anchorEnd;
}
