/** Drzewo atrybutów dla rekordów. */

#pragma once

#include "BinarySearchTree.h"

#include <string>
#include <stdexcept>

struct LessAttributeName
{
	bool operator()(std::string const* lhs_, std::string const* rhs_)
	{
		return (*lhs_) < (*rhs_);
	}
};

using AttributeTree = BinarySearchTree<std::string const*, double, LessAttributeName>;