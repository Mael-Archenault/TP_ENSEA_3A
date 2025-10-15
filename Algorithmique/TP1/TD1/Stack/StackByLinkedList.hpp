#ifndef STACKBYLINKEDLIST_HPP
#define STACKBYLINKEDLIST_HPP

#include <iostream>

class StackByLinkedList {
	struct TNode {
		int value;
		TNode *next;
	};
	TNode *top;

public:
	StackByLinkedList();
	void push(int value);
	int pop();
	void displayStack();
};

#endif //STACKBYLINKEDLIST_HPP
