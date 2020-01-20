#pragma once

#include "Record.h"

#include <string>

class DecisionTree
{
public:
	class Node
	{
	public:
		Node() = default;
		Node(Node && rhs_);
		~Node();

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

		Node* failed = nullptr;
		Node* succeeded = nullptr;
	};

	DecisionTree() = default;
	~DecisionTree();
	
	DecisionTree(DecisionTree const& rhs_) = delete;
	DecisionTree(DecisionTree && rhs_);

	DecisionTree& operator=(DecisionTree const& rhs_) = delete;
	DecisionTree& operator=(DecisionTree && rhs_);


	template <typename TWhatToDo, typename TShouldStop>
	Node* forEachUntil(TWhatToDo whatToDo_, TShouldStop shouldStop_)
	{	
		if (root)
			return this->forEachUntil(*root, whatToDo_, shouldStop_);

		return nullptr;
	}

	Node* root = nullptr;

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