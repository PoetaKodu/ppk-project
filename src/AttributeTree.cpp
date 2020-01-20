#include "AttributeTree.h"

#include <utility>

////////////////////////////////////////////////////////////////////
AttributeTree::Node::Node(std::string name_, double value_ )
	:
	name( std::move(name_) ),
	value( value_ )
{
}

////////////////////////////////////////////////////////////////////
AttributeTree::Node::Node(Node && rhs_)
	:
	name( std::move(rhs_.name) ),
	value( rhs_.value ),
	left(rhs_.left),
	right(rhs_.right)
{
	rhs_.left = rhs_.right = nullptr;
}

////////////////////////////////////////////////////////////////////
AttributeTree::Node::~Node()
{
	if (left)
		delete left;
	if (right)
		delete right;
}

////////////////////////////////////////////////////////////////////
AttributeTree::AttributeTree(AttributeTree && rhs_)
	:
	root(rhs_.root)
{
	rhs_.root = nullptr;
}

////////////////////////////////////////////////////////////////////
AttributeTree::~AttributeTree()
{
	if (root) delete root;
}

////////////////////////////////////////////////////////////////////
void AttributeTree::set(std::string name_, double value_)
{
	Node** processedNode = &root;
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
	*processedNode = new Node{ std::move(name_), value_ };
}

////////////////////////////////////////////////////////////////////
double AttributeTree::get(std::string const & name_) const
{
	Node* n = root;
	while (n)
	{
		auto comp = name_.compare(n->name);
		if (comp == 0)
			return n->value;
		else if (comp < 0)
			n = n->left;
		else
			n = n->right;
	}
	throw std::runtime_error("attribute does not exist");
}
