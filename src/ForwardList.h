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

	void push(T value_)
	{
		end->next = new Node{ std::move(value_) };
		end = end->next;
	}

	Node* head = nullptr;
	Node* end = nullptr;
};