#pragma once

#include "Record.h"
#include "BasicTree.h"

#include <string>


class DecisionTreeNode
{
public:
	DecisionTreeNode() = default;
	DecisionTreeNode(DecisionTreeNode && rhs_);
	~DecisionTreeNode();

	struct Condition
	{
		std::string attributeName;
		enum { LowerThan, GreaterThan } op;
		double value;
	};

	struct Anchor
	{
		Anchor() = default;
		Anchor(std::string label_);
		Anchor(std::uint32_t nodeIndex_);

		std::string label;
		std::uint32_t nodeIndex = 0;

		bool isLabel = false;
	};

	std::uint32_t index = 0;

	Condition cond;

	Anchor failedAnchor;
	Anchor succeededAnchor;

	DecisionTreeNode* failed = nullptr;
	DecisionTreeNode* succeeded = nullptr;
};

class DecisionTree
	: public BasicTree<DecisionTreeNode>
{
public:
	using BasicTree::BasicTree;
	using Node = DecisionTreeNode;

	template <typename TWhatToDo, typename TShouldStop>
	Node* forEachUntil(TWhatToDo whatToDo_, TShouldStop shouldStop_)
	{	
		if (root)
			return this->forEachUntil(*root, whatToDo_, shouldStop_);

		return nullptr;
	}

private:
	template <typename TWhatToDo, typename TShouldStop>
	Node* forEachUntil(Node& node_, TWhatToDo whatToDo_, TShouldStop shouldStop_)
	{	
		whatToDo_(node_);

		if (shouldStop_(node_))
			return &node_;

		Node* toReturn = nullptr;
		if (node_.failed)
			toReturn = forEachUntil(*node_.failed, whatToDo_, shouldStop_);
		if (node_.succeeded && !toReturn)
			toReturn = forEachUntil(*node_.succeeded, whatToDo_, shouldStop_);

		return toReturn;
	}
};

DecisionTree readDecisionTree(char const* beg_, char const* end_);