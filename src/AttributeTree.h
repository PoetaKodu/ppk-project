/** @file Drzewo atrybutów dla rekordów. */

#pragma once

#include "BasicTree.h"

#include <string>
#include <stdexcept>


/** Węzeł drzewa atrybutów.
 * Węzły same nie przechowują nazw atrybutów, tylko wskaźniki na nie!
 * Jest to zabieg optymalizacyjny wykorzystanie pamięci.
*/
class AttributeTreeNode
{
public:
	/** Inicjalizuje instancje klasy @ref AttributeTreeNode. */
	AttributeTreeNode() = default;

	/** Inicjalizuje instancje klasy @ref AttributeTreeNode przenosząc wartość z rhs_.
	 * @param rhs_ opis
	*/
	AttributeTreeNode(AttributeTreeNode && rhs_);

	/** Inicjalizuje instancje klasy @ref AttributeTreeNode za pomocą nazwy i wartości.
	 * @param name_ wskaźnik na nazwę atrybutu
	 * @param value_ wartość atrybutu
	*/
	AttributeTreeNode(std::string const* name_, double value_ = 0);

	/** Niszczy instancje klasy @ref AttributeTreeNode.
	 * Niszczy również lewe i prawe poddrzewo.
	*/
	~AttributeTreeNode();

	std::string const* name = nullptr; 	///< wskaźnik na nazwę atrybutu 
	double value = 0; 					///< wartość atrybutu

	AttributeTreeNode* left = nullptr; 	///< wskaźnik na korzeń lewego poddrzewa
	AttributeTreeNode* right = nullptr; ///< wskaźnik na korzeń prawego poddrzewa
};

/** Drzewo decyzyjne */
class AttributeTree
	: public BasicTree<AttributeTreeNode>
{
public:
	/// Używaj konstruktorów klasy bazowej drzewa
	using BasicTree::BasicTree;

	using Node = AttributeTreeNode; ///< typ węzła

	/** Ustawia wartość atrybutowi o danej nazwie.
	 * @param name_ wskaźnik na nazwę atrybutu
	 * @param value_ wartość atrybutu
	*/
	void set(std::string const* name_, double value_);

	/** Zwraca wartość atrybutu o danej nazwie.
	 * Rzuca wyjątek, jeśli atrybutu o danej nazwie nie znaleziono.
	 * @param name_ nazwa atrybutu
	 * @return wartość atrybutu
	*/
	double get(std::string const& name_) const;
};
