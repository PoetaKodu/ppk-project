/** @file Zawiera drzewo decyzyjne. */

#pragma once

#include "BasicTree.h"

#include <string>

/** Węzeł drzewa decyzyjnego */
class DecisionTreeNode
{
public:
	/** Inicjalizuje instancje klasy @see DecisionTreeNode. */
	DecisionTreeNode() = default;

	/** Inicjalizuje instancje klasy @see DecisionTreeNode przenosząc wartość z rhs_.
	 * @param rhs_ inny obiekt, z którego wartość zostanie przeniesiona
	*/
	DecisionTreeNode(DecisionTreeNode && rhs_);

	/** Niszczy instancje klasy @see DecisionTreeNode. */
	~DecisionTreeNode();

	/** Warunek drzewa decyzyjnego */
	struct Condition
	{
		std::string attributeName; 			///< nazwa porównywanego atrybutu
		enum { LowerThan, GreaterThan } op; ///< operator porównania
		double value; 						///< wartość z którą atrybut jest porównywany
	};

	/** Połączenie w drzewie decyzyjnym (albo id węzła albo etykieta) */
	struct Anchor
	{
		/** Inicjalizuje instancje klasy @see Anchor. */
		Anchor() = default;

		/** Inicjalizuje instancje klasy @see Anchor etykietą.
		 * @param label_ etykieta
		*/
		Anchor(std::string label_);

		/** Inicjalizuje instancje klasy @see Anchor identyfikatorem węzła.
		 * @param label_ identyfikator węzła
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

	/** Wykonuje whatToDo_ dla każdego elementu drzewa, przestaje gdy shouldStop_ zwróci true.
	 * Wykonanie shouldStop_ ma miejsce po wykonaniu whatToDo_.
	 * @tparam TWhatToDo typ funkcji whatToDo_ o sygnaturze <typ>(Node&)
	 * @tparam TShouldStop typ funkcji shouldStop_ o sygnaturze bool(Node const&)
	 * @param whatToDo_ funkcja operująca na węźle drzewa
	 * @param shouldStop_ funkcja sprawdzająca, czy dla danego węzła zakończyć wykonywanie
	 * @return wskaźnik na węzeł, na którym zakończono wykonywanie whatToDo_
	*/
	template <typename TWhatToDo, typename TShouldStop>
	Node* forEachUntil(TWhatToDo whatToDo_, TShouldStop shouldStop_)
	{	
		if (root)
			return this->forEachUntil(*root, whatToDo_, shouldStop_);

		return nullptr;
	}

private:

	/** Wykonuje whatToDo_ dla każdego podwęzła elementu node_ w drzewie, przestaje gdy shouldStop_ zwróci true.
	 * Wykonanie shouldStop_ ma miejsce po wykonaniu whatToDo_.
	 * @tparam TWhatToDo typ funkcji whatToDo_ o sygnaturze <typ>(Node&)
	 * @tparam TShouldStop typ funkcji shouldStop_ o sygnaturze bool(Node const&)
	 * @param node_ element drzewa
	 * @param whatToDo_ funkcja operująca na węźle drzewa
	 * @param shouldStop_ funkcja sprawdzająca, czy dla danego węzła zakończyć wykonywanie
	 * @return wskaźnik na węzeł, na którym zakończono wykonywanie whatToDo_
	*/
	template <typename TWhatToDo, typename TShouldStop>
	Node* forEachUntil(Node& node_, TWhatToDo whatToDo_, TShouldStop shouldStop_)
	{	
		whatToDo_(node_);

		if (shouldStop_(node_))
			return &node_;

		Node* toReturn = nullptr;
		if (node_.failed)
			toReturn = forEachUntil(*node_.failed, whatToDo_, shouldStop_);
		if (node_.succeeded && !toReturn)
			toReturn = forEachUntil(*node_.succeeded, whatToDo_, shouldStop_);

		return toReturn;
	}
};

/** Wczytuje drzewo decyzyjne z tablicy znaków.
 * @param beg_ początek tablicy znaków
 * @param end_ koniec tablicy znaków (ostatni + 1 element)
 * @return wczytane drzewo decyzyjne
*/
DecisionTree readDecisionTree(char const* beg_, char const* end_);