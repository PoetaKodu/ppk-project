#pragma once

#include <string>
#include <stdexcept>

class AttributeTreeNode
{
public:
	AttributeTreeNode() = default;
	AttributeTreeNode(AttributeTreeNode && rhs_);
	AttributeTreeNode(std::string name_, double value_ = 0);

	~AttributeTreeNode();

	std::string name;
	double value = 0;

	// Children
	AttributeTreeNode* left = nullptr;
	AttributeTreeNode* right = nullptr;
};

void setAttribute(AttributeTreeNode*& treeRoot_, std::string name_, double value_);

double getAttributeValue(AttributeTreeNode const* treeRoot_, std::string const & name_);
