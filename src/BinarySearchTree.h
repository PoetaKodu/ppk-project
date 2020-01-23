#pragma once

#include "BasicTree.h"

#include <functional>

/** Węzeł drzewa poszukiwania binarnego */
template <typename TKeyType, typename TValueType>
class BSTNode
{
public:

	/** Inicjalizuje instancje klasy @ref BSTNode. */
	BSTNode() = default;

	/** Inicjalizuje instancje klasy @ref BSTNode za pomocą klucza i wartości.
	 * @param key_ klucz
	 * @param value_ wartość
	*/
	BSTNode(TKeyType key_, TValueType value_)
		:
		key(std::move(key_)),
		value(std::move(value_))
	{	
	}

	/** Usunięty konstruktor kopiujący klasy @ref BSTNode.
	 * @param rhs_ drugi obiekt
	*/
	BSTNode(BSTNode const&& rhs_) = delete;

	/** Inicjalizuje instancje klasy @ref BSTNode przenosząc wartość z rhs_.
	 * @param rhs_ drugi obiekt
	*/
	BSTNode(BSTNode&& rhs_)
		:
		left(rhs_.left),
		right(rhs_.right),
		key(std::move(rhs_.key)),
		value(std::move(rhs_.value))
	{
		rhs_.left = rhs_.right = nullptr;
		rhs_.key = {};
		rhs_.value = {};
	}

	/** Niszczy instancje klasy @ref BSTNode.
	 * Niszczy również lewe i prawe poddrzewo.
	*/
	~BSTNode()
	{
		if (left)
			delete left;
		if (right)
			delete right;
	}

	TKeyType key;		///< klucz węzła
	TValueType value;	///< wartość węzła

	BSTNode* left = nullptr;	///< wskaźnik na korzeń lewego poddrzewa
	BSTNode* right = nullptr;	///< wskaźnik na korzeń prawego poddrzewa
};

/** Drzewo poszukiwania binarnego */
template <typename TKeyType, typename TValueType, typename TCompareKeys = std::less<TKeyType const&> >
class BinarySearchTree
	: public BasicTree< BSTNode<TKeyType, TValueType> >
{
public:
	using BasicTree< BSTNode<TKeyType, TValueType> >::BasicTree;
	using KeyType 		= TKeyType;
	using ValueType 	= TValueType;
	using CompareKeys 	= TCompareKeys;
	using Node 			= BSTNode<TKeyType, TValueType>;

	/** Wstawia wartość do drzewa pod danym kluczem
	 * lub ustawia wartość już istniejącemu węzłowi.
	 * @param key_ klucz
	 * @param value_ wartość
	 * @param comp_ funkcja porównująca dwa klucze (lhs < rhs)
	 * @return referencja na wstawioną wartość
	*/
	ValueType& set(KeyType key_, ValueType value_, CompareKeys comp_ = CompareKeys())
	{
		Node** pn = &this->root;
		while(*pn != nullptr)
		{
			if (comp_(key_, (*pn)->key))
				pn = &(*pn)->left;
			else if (comp_((*pn)->key, key_))
				pn = &(*pn)->right;
			else
			{
				(*pn)->value = std::move(value_);
				return (*pn)->value;
			}
		}
		*pn = new Node{ std::move(key_), std::move(value_) };
		return (*pn)->value;
	}

	/** Zwraca referencję na wartość węzła o danym kluczu.
	 * Rzuca wyjątek, jeśli węzła o danym kluczu nie znaleziono.
	 * @param key_ klucz
	 * @param comp_ funkcja porównująca dwa klucze (lhs < rhs)
	 * @return wartość
	*/
	ValueType& get(KeyType const& key_, CompareKeys comp_ = CompareKeys()) const
	{
		Node* n = this->root;
		while (n)
		{
			if (comp_(key_, n->key))
				n = n->left;
			else if(comp_(n->key, key_))
				n = n->right;
			else
				return n->value;
		}
		throw std::runtime_error("key does not exist");
	}

	/** Zwraca wskaźnik na wartość węzła o danym kluczu.
	 * Zwraca nullptr, jeśli węzła o danym kluczu nie znaleziono.
	 * @param key_ klucz
	 * @param comp_ funkcja porównująca dwa klucze (lhs < rhs)
	 * @return wartość
	*/
	ValueType* tryGet(KeyType const& key_, CompareKeys comp_ = CompareKeys()) const
	{
		Node* n = this->root;
		while (n)
		{
			if (comp_(key_, n->key))
				n = n->left;
			else if(comp_(n->key, key_))
				n = n->right;
			else
				return &n->value;
		}
		return nullptr;
	}

	/** Wykonuje whatToDo_ dla każdego elementu drzewa.
	 * @tparam TWhatToDo typ funkcji whatToDo_ o sygnaturze typ(KeyType const&, ValueType const&)
	 * @param whatToDo_ funkcja operująca na kluczu i wartości węzła
	*/
	template <typename TWhatToDo>
	void forEach(TWhatToDo whatToDo_)
	{	
		const_cast<BinarySearchTree*>(this)->forEach(whatToDo_);
	}

	/** Wykonuje niemodyfikującą funkcję whatToDo_ dla każdego elementu drzewa.
	 * @tparam TWhatToDo typ funkcji whatToDo_ o sygnaturze typ(KeyType const&, ValueType const&)
	 * @param whatToDo_ funkcja operująca na kluczu i wartości węzła
	*/
	template <typename TWhatToDo>
	void forEach(TWhatToDo whatToDo_) const
	{	
		if (this->root)
			this->forEach(*this->root, whatToDo_);
	}

private:

	/** Wykonuje whatToDo_ dla każdego podwęzła elementu node_ w drzewie.
	 * @tparam TWhatToDo typ funkcji whatToDo_ o sygnaturze typ(KeyType const&, ValueType const&)
	 * @param node_ element drzewa
	 * @param whatToDo_ funkcja operująca na kluczu i wartości węzła
	*/
	template <typename TWhatToDo>
	void forEach(Node& node_, TWhatToDo whatToDo_) const
	{	
		whatToDo_(node_.key, node_.value);

		if (node_.left)
			forEach(*node_.left, whatToDo_);
		if (node_.right)
			forEach(*node_.right, whatToDo_);
	}
};
