#include "pch.h"
#include "token_queue.h"


token_queue::token_queue() :tail(nullptr), size(0) {};


token_queue::~token_queue()
{
	while (size)pop();
}

const bool token_queue::empty()
{
	return size == 0;
}

void token_queue::push(const token & value)
{
	tail = new node(value, tail);
	size++;
}

const token token_queue::front()
{
	if (tail == nullptr)throw exception("attemt to get unexisting node");
	node *to_get(tail);
	while (to_get->next != nullptr)to_get = to_get->next;
	return to_get->value;
}

void token_queue::pop()
{
	if (tail == nullptr)throw exception("attempt to delete unexisting node");
	if (tail->next != nullptr) {
		node *to_del(tail->next), *next_last(tail);
		while (to_del->next != nullptr) {
			to_del = to_del->next;
			next_last = next_last->next;
		}
		delete to_del;
		next_last->next = nullptr;
	}
	else delete tail;
	size--;
}

token_queue::node::node(const token value, node * const next) :value(value), next(next) {};
