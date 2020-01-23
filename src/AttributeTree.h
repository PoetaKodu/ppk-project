/** Drzewo atrybutów dla rekordów. */

#pragma once

#include "BinarySearchTree.h"

#include <string>
#include <stdexcept>

/** Funktor porównujący dwa stringi za pomocą wskaźników */
struct LessAttributeName
{
	/** Operator porównujący dwa stringi za pomocą wskaźników
	 * @param lhs_ lewy łańcuch
	 * @param rhs_ prawy łańcuch
	 * @return true jeśli lewy łańcuch jest mniejszy od prawego.
	*/
	bool operator()(std::string const* lhs_, std::string const* rhs_)
	{
		return (*lhs_) < (*rhs_);
	}
};

using AttributeTree = BinarySearchTree<std::string const*, double, LessAttributeName>;