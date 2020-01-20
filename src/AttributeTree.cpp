#include "AttributeTree.h"

#include <utility>

////////////////////////////////////////////////////////////////////
AttributeTreeNode::AttributeTreeNode(std::string const* name_, double value_ )
	:
	name( name_ ),
	value( value_ )
{
}

////////////////////////////////////////////////////////////////////
AttributeTreeNode::AttributeTreeNode(AttributeTreeNode && rhs_)
	:
	name( rhs_.name ),
	value( rhs_.value ),
	left(rhs_.left),
	right(rhs_.right)
{
	rhs_.left = rhs_.right = nullptr;
	rhs_.name = nullptr;
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
void AttributeTree::set(std::string const* name_, double value_)
{
	Node** pn = &root;
	while(*pn != nullptr)
	{
		auto comp = name_->compare(*(*pn)->name);
		if (comp < 0)
			pn = &(*pn)->left;
		else if (comp > 0)
			pn = &(*pn)->right;
		else
		{
			(*pn)->value = value_;
			return;
		}
	}
	*pn = new Node{ name_, value_ };
}

////////////////////////////////////////////////////////////////////
double AttributeTree::get(std::string const& name_) const
{
	Node* n = root;
	while (n)
	{
		auto comp = name_.compare(*n->name);
		if (comp == 0)
			return n->value;
		else if (comp < 0)
			n = n->left;
		else
			n = n->right;
	}
	throw std::runtime_error("attribute does not exist");
}
