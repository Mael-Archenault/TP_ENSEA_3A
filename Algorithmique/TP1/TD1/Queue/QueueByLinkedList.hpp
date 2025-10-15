#ifndef QUEUEBYLINKEDLIST_HPP
#define QUEUEBYLINKEDLIST_HPP

#include <iostream>

class QueueByLinkedList {
	struct TNode {
		int value;
		TNode *next;
	};

    TNode *front, *rear;

public:
    QueueByLinkedList();
    void push(int value);
    int pop();
};

#endif // QUEUEBYLINKEDLIST_HPP