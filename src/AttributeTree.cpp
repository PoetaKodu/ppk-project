#include "AttributeTree.h"

#include <utility>

////////////////////////////////////////////////////////////////////
AttributeTreeNode::AttributeTreeNode(std::string name_, double value_ )
	:
	name( std::move(name_) ),
	value( value_ )
{
}

////////////////////////////////////////////////////////////////////
AttributeTreeNode::~AttributeTreeNode()
{
	if (left)
		delete left;
	if (right)
		delete right;
}

////////////////////////////////////////////////////////////////////
void setAttribute(AttributeTreeNode*& treeRoot_, std::string name_, double value_)
{
	AttributeTreeNode** processedNode = &treeRoot_;
	while(*processedNode != nullptr)
	{
		auto comp = name_.compare((*processedNode)->name);
		if (comp < 0)
			processedNode = &(*processedNode)->left;
		else if (comp > 0)
			processedNode = &(*processedNode)->right;
		else
		{
			(*processedNode)->value = value_;
			return;
		}
	}
	*processedNode = new AttributeTreeNode{ std::move(name_), value_ };
}

////////////////////////////////////////////////////////////////////
double getAttributeValue(AttributeTreeNode const* treeRoot_, std::string const & name_)
{
	while (treeRoot_)
	{
		auto comp = name_.compare(treeRoot_->name);
		if (comp == 0)
			return treeRoot_->value;
		else if (comp < 0)
			treeRoot_ = treeRoot_->left;
		else
			treeRoot_ = treeRoot_->right;
	}
	throw std::runtime_error("attribute does not exist");
}
