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
			if (accept_(t->value))
				return t;
			t = t->next;
		}
		return t;
	}

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

	Node* head = nullptr;
	Node* tail = nullptr;
};