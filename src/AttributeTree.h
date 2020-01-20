
#pragma once
#include <string>
#include <stdexcept>

class AttributeTree
{
public:
	class Node
	{
	public:
		Node() = default;
		Node(Node && rhs_);
		Node(std::string name_, double value_ = 0);

		~Node();

		std::string name;
		double value = 0;

		// Children
		Node* left = nullptr;
		Node* right = nullptr;
	};

	AttributeTree() = default;
	AttributeTree(AttributeTree && rhs_);
	AttributeTree(AttributeTree const& rhs_) = delete;
	~AttributeTree();

	void set(std::string name_, double value_);
	double get(std::string const & name_) const;

	Node *root = nullptr;
};
