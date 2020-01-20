#pragma once

#include <utility>

template <typename T>
class BasicTree
{
public:
	BasicTree() = default;
	BasicTree(BasicTree const& rhs_) = delete;
	BasicTree(BasicTree && rhs_)
		:
		root(rhs_.root)
	{
		rhs_.root = nullptr;
	}

	BasicTree& operator=(BasicTree const& rhs_) = delete;
	BasicTree& operator=(BasicTree && rhs_)
	{
		std::swap(root, rhs_.root);
		return *this;
	}

	~BasicTree()
	{
		if (root) delete root;
	}

	T* root = nullptr;
};