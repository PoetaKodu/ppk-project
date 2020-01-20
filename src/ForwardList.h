/** @file Szablon listy jednokierunkowej. */

#pragma once

/** Szablon listy jednokierunkowej.
 * @tparam T typ danych przechowywanych w liście
*/
template <typename T>
class ForwardList
{
public:

	/** Węzeł listy. */
	class Node
	{
	public:
		/**
		 * Inicjalizuje instancje klasy @see Node.
		 * @param v wartość do przechowania
		*/
		Node(T v_)
			: value(std::move(v_))
		{
		}

		T value; 				///< przechowywana wartość
		Node* next = nullptr; 	///< wskaźnik na następny węzeł
	};

	/** Niszczy instancje szablonu klasy @see ForwardList.
	*/
	~ForwardList()
	{
		this->clear();
	}

	/** Czyści listę, zwalniając pamięć. */
	void clear()
	{
		while(head)
		{
			Node* temp = head->next;
			delete head;
			head = temp;
		}
	}

	/** Szuka wartości, korzystając z funkcji accept
	 * @tparam TAcceptFunc typ pozwalający wykonać funkcję o sygnaturze bool(T const&)
	 * @param accept_ wartość typu TAcceptFunc zwracająca true dla wartości węzła, który ma być zwrócony 
	 * @return wskaźnik na znaleziony węzeł lub nullptr jeśli nie znaleziono
	*/
	template <typename TAcceptFunc>
	Node* findIf(TAcceptFunc accept_) const
	{
		Node* t = head;
		while(t)
		{
			if (accept_(t->value))
				return t;
			t = t->next;
		}
		return t;
	}

	/** Dodaje wartość na koniec.
	 * @param value_ wartość do dodania
	 * @return referencja na węzeł zawierający wartość (ogon listy)
	*/
	Node& push(T value_)
	{
		auto ptr = new Node{ std::move(value_) };
		if (!head)
			head = tail = ptr;
		else
		{
			tail->next = ptr;
			tail = tail->next;
		}
		return *tail;
	}

	Node* head = nullptr; ///< głowa listy (pierwszy element)
	Node* tail = nullptr; ///< ogon listy (ostatni element)
};