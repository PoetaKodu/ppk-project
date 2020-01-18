#pragma once

template <typename T>
class ForwardList
{
public:
	class Node
	{
	public:
		Node(T v)
			: value(std::move(v))
		{
		}
		T value;
		Node* next = nullptr;
	};

	~ForwardList()
	{
		this->clear();
	}

	void clear()
	{
		while(head)
		{
			Node* temp = head->next;
			delete head;
			head = temp;
		}
	}

	template <typename TAcceptFunc>
	Node* findIf(TAcceptFunc accept_) const
	{
		Node* t = head;
		while(t)
		{
			if (accept(t->value))
				return t;
			t = t->next;
		}
		return t;
	}

	Node& push(T value_)
	{
		end->next = new Node{ std::move(value_) };
		end = end->next;
		return end->value;
	}

	Node* head = nullptr;
	Node* end = nullptr;
};