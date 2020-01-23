#pragma once

#include "BasicTree.h"

#include <string>

/** Węzeł drzewa decyzyjnego */
class DecisionTreeNode
{
public:
	/** Inicjalizuje instancje klasy @ref DecisionTreeNode. */
	DecisionTreeNode() = default;

	/** Inicjalizuje instancje klasy @ref DecisionTreeNode przenosząc wartość z rhs_.
	 * @param rhs_ inny obiekt, z którego wartość zostanie przeniesiona
	*/
	DecisionTreeNode(DecisionTreeNode && rhs_);

	/** Niszczy instancje klasy @ref DecisionTreeNode. */
	~DecisionTreeNode();

	/** Warunek drzewa decyzyjnego */
	struct Condition
	{
		/** Rodzaj operatora */
		enum Op {
			LowerThan,	///< operator "<"
			GreaterThan ///< operator ">"
		};

		std::string attributeName; 	///< nazwa porównywanego atrybutu
		Op op; 						///< operator porównania
		double value; 				///< wartość z którą atrybut jest porównywany
	};

	/** Połączenie w drzewie decyzyjnym (albo id węzła albo etykieta) */
	struct Anchor
	{
		/** Inicjalizuje instancje klasy @ref Anchor. */
		Anchor() = default;

		/** Inicjalizuje instancje klasy @ref Anchor etykietą.
		 * @param label_ etykieta
		*/
		Anchor(std::string label_);

		/** Inicjalizuje instancje klasy @ref Anchor identyfikatorem węzła.
		 * @param nodeIndex_ identyfikator węzła
		*/
		Anchor(std::uint32_t nodeIndex_);

		std::string label; 				///< etykieta
		std::uint32_t nodeIndex = 0; 	///< identyfikator węzła

		bool isLabel = false;			///< czy połączenie jest etykietą?
	};


	std::uint32_t index = 0; 				///< indeks węzła

	Condition cond;							///< warunek

	Anchor failedAnchor; 					///< połączenie dla niespełnienia warunku
	Anchor succeededAnchor; 				///< połączenie dla spełnienia warunku

	DecisionTreeNode* failed = nullptr; 	///< wskaźnik na węzeł niespełnionego warunku (ważny tylko gdy failedAnchor.isLabel == false)
	DecisionTreeNode* succeeded = nullptr; 	///< wskaźnik na węzeł spełnionego warunku (ważny tylko gdy succeededAnchor.isLabel == false)
};

/** Drzewo decyzyjne */
class DecisionTree
	: public BasicTree<DecisionTreeNode>
{
public:
	/// Używaj konstruktorów klasy bazowej drzewa
	using BasicTree::BasicTree;

	using Node = DecisionTreeNode; ///< typ węzła
};

/** Wczytuje drzewo decyzyjne z tablicy znaków.
 * @param beg_ początek tablicy znaków
 * @param end_ koniec tablicy znaków (ostatni + 1 element)
 * @return wczytane drzewo decyzyjne
*/
DecisionTree readDecisionTree(char const* beg_, char const* end_);

/** Wczytuje pojedynczy węzeł drzewa decyzyjnego z tablicy znaków.
 * @param[out] node_ węzeł drzewa do wypełnienia
 * @param beg_ początek tablicy znaków
 * @param end_ koniec tablicy znaków (ostatni + 1 element)
*/
void readNode(DecisionTreeNode &node_, char const* beg_, char const* end_);

/** Wczytuje warunek węzła drzewa decyzyjnego z tablicy znaków.
 * @param[out] cond_ warunek do wypełnienia
 * @param beg_ początek tablicy znaków
 * @param end_ koniec tablicy znaków (ostatni + 1 element)
 * @return wskaźnik na znak, na którym skończono parsowanie tablicy wejściowej
*/
char const* readNodeCondition(DecisionTreeNode::Condition &cond_, char const* beg_, char const* end_);

/** Wczytuje połączenie węzła drzewa decyzyjnego z tablicy znaków.
 * @param[out] anchor_ połączenie do wypełnienia
 * @param beg_ początek tablicy znaków
 * @param end_ koniec tablicy znaków (ostatni + 1 element)
 * @return wskaźnik na znak, na którym skończono parsowanie tablicy wejściowej
*/
char const* readNodeAnchor(DecisionTreeNode::Anchor &anchor_, char const* beg_, char const* end_);