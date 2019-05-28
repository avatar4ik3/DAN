#pragma once
#define TOKEN_QUEUE_H
#include "token.h"
class token_queue
{
private:
	class node {
	public:
		token value;
		node* next;
		node(const token, node * const);
	};
	node* tail;
	size_t size;
public:
	token_queue();
	~token_queue();
	const bool empty();
	void push(const token &);
	const token front();
	void pop();
};

