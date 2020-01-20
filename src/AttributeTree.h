
#pragma once

#include "BasicTree.h"

#include <string>
#include <stdexcept>


class AttributeTreeNode
{
public:
	AttributeTreeNode() = default;
	AttributeTreeNode(AttributeTreeNode && rhs_);
	AttributeTreeNode(std::string const* name_, double value_ = 0);

	~AttributeTreeNode();

	std::string const* name;
	double value = 0;

	// Children
	AttributeTreeNode* left = nullptr;
	AttributeTreeNode* right = nullptr;
};

class AttributeTree
	: public BasicTree<AttributeTreeNode>
{
public:
	using BasicTree::BasicTree;
	using Node = AttributeTreeNode;

	void set(std::string const* name_, double value_);
	double get(std::string const& name_) const;
};
