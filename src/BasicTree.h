/** @file Szablon klasy bazowej dla drzew binarnych. */
#pragma once

#include <utility>

/** Szablon klasy bazowej dla drzew binarnych.
 * @tparam T typ węzła
*/
template <typename T>
class BasicTree
{
public:

	/** Inicjalizuje instancje klasy @see BasicTree. */
	BasicTree() = default;

	/** Usunięty konstruktor kopiujący klasy @see BasicTree.
	 * @param rhs_ drugi obiekt.
	*/
	BasicTree(BasicTree const& rhs_) = delete;

	/** Inicjalizuje instancje klasy @see Basic Tree przenosząc wartość z rhs_.
	 * @param rhs_ inny obiekt, z którego drzewo zostanie przeniesione
	*/
	BasicTree(BasicTree && rhs_)
		:
		root(rhs_.root)
	{
		rhs_.root = nullptr;
	}

	/** Niszczy instancje klasy @see BasicTree. */
	~BasicTree()
	{
		if (root) delete root;
	}

	/** Usunięty kopiujący operator przypisania klasy @see BasicTree.
	 * @param rhs_ inny obiekt
	 * @return referencja na siebie
	*/
	BasicTree& operator=(BasicTree const& rhs_) = delete;

	/** Przenosi wartość z rhs_ do siebie.
	 * @param rhs_ inny obiekt, z którego drzewo zostanie przeniesione
	 * @return referencja na siebie
	*/
	BasicTree& operator=(BasicTree && rhs_)
	{
		std::swap(root, rhs_.root);
		return *this;
	}

	T* root = nullptr; //< korzeń drzewa
};