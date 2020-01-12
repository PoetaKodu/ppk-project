#pragma once

#include "Record.h"

#include <string>

class DecisionTreeNode
{
public:
	~DecisionTreeNode();

	struct Condition
	{
		std::string attributeName;
		enum {
			LowerThan,
			GreaterThan
		} op;
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

void readNode(DecisionTreeNode &node_, char const* beg_, char const* end_);
char const* readNodeCondition(DecisionTreeNode::Condition &cond_, char const* beg_, char const* end_);
char const* readNodeAnchor(DecisionTreeNode::Anchor &anchor_, char const* beg_, char const* end_);