#include "DecisionTree.h"

#include "TextHelper.h"

#include <string>
#include <iostream>


// TODO: document this.
static void readNode(DecisionTree::Node &node_, char const* beg_, char const* end_);
// TODO: document this.
static char const* readNodeCondition(DecisionTree::Node::Condition &cond_, char const* beg_, char const* end_);
// TODO: document this.
static char const* readNodeAnchor(DecisionTree::Node::Anchor &anchor_, char const* beg_, char const* end_);

///////////////////////////////////////////////////////////////////
DecisionTree::Node::Node(Node && rhs_)
{
	index = rhs_.index;
	std::swap(rhs_.cond, cond);
	std::swap(rhs_.succeededAnchor, succeededAnchor);
	std::swap(rhs_.failedAnchor, failedAnchor);
	std::swap(rhs_.succeeded, succeeded);
	std::swap(rhs_.failed, failed);
}

///////////////////////////////////////////////////////////////////
DecisionTree::Node::~Node()
{
	if (failed)
		delete failed;
	if (succeeded)
		delete succeeded;
}

///////////////////////////////////////////////////////////////////
DecisionTree::Node::Anchor::Anchor(std::string label_)
	:
	label( std::move(label_) ),
	isLabel( true )
{
}

///////////////////////////////////////////////////////////////////
DecisionTree::Node::Anchor::Anchor(std::uint32_t nodeIndex_)
	:
	nodeIndex( std::move(nodeIndex_) ),
	isLabel( false )
{

}

///////////////////////////////////////////////////////////////////
DecisionTree::DecisionTree(DecisionTree && rhs_)
	:
	root(rhs_.root)
{
	rhs_.root = nullptr;
}

///////////////////////////////////////////////////////////////////
DecisionTree::~DecisionTree()
{
	if (root) delete root;
}

///////////////////////////////////////////////////////////////////
DecisionTree& DecisionTree::operator=(DecisionTree && rhs_)
{
	std::swap(root, rhs_.root);
	return *this;
}

///////////////////////////////////////////////////////////////////
DecisionTree readDecisionTree(char const* beg_, char const* end_)
{
	constexpr char newLineCharacter = '\n';

	DecisionTree tree;

	do
	{
		auto endOfLine = std::find(beg_, end_, newLineCharacter);

		DecisionTree::Node node;
		readNode(node, beg_, endOfLine);

		if (!tree.root)
		{
			tree.root = new DecisionTree::Node( std::move(node) );
		}
		else
		{
			tree.forEachUntil(
				[&](DecisionTree::Node & n)
				{
					if (!n.failedAnchor.isLabel && n.failedAnchor.nodeIndex == node.index)
						n.failed = new DecisionTree::Node{ std::move(node) };
					else if (!n.succeededAnchor.isLabel && n.succeededAnchor.nodeIndex == node.index)
						n.succeeded = new DecisionTree::Node{ std::move(node) };
				},
				[&](DecisionTree::Node & n)
				{
					return (!n.failedAnchor.isLabel && n.failedAnchor.nodeIndex == node.index) ||
						(!n.succeededAnchor.isLabel && n.succeededAnchor.nodeIndex == node.index);
				}
			);
		}

		beg_ = endOfLine + 1;
	}
	while(beg_ <= end_);

	return tree;
}

///////////////////////////////////////////////////////////////////
void readNode(DecisionTree::Node &node_, char const* beg_, char const* end_)
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

	DecisionTree::Node::Condition cond;
	DecisionTree::Node::Anchor failed, succeeded;

	it = findFirstNonSpace( readNodeCondition(node_.cond, it, end_), end_ );
	it = findFirstNonSpace( readNodeAnchor(node_.failedAnchor, it, end_), end_ );
	readNodeAnchor(node_.succeededAnchor, it, end_);
}

///////////////////////////////////////////////////////////////////
char const* readNodeCondition(DecisionTree::Node::Condition &cond_, char const* beg_, char const* end_)
{
	using Cond = DecisionTree::Node::Condition;
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
char const* readNodeAnchor(DecisionTree::Node::Anchor &anchor_, char const* beg_, char const* end_)
{
	using Anchor = DecisionTree::Node::Anchor;

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
