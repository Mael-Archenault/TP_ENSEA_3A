#include "StackByLinkedList.hpp"

StackByLinkedList::StackByLinkedList() : top(nullptr) {}

void StackByLinkedList::push(int value) {
	TNode *P = new TNode;
	P->value = value;
	P->next = top;
	top = P;
}

int StackByLinkedList::pop() {
	TNode *P = new TNode;
	if (top == nullptr) {
		std::cout << "Stack is empty" << std::endl;
	}
	else {
		P = top->next;
		int value = top->value;
		top = P;
		return value;
	}
}

void StackByLinkedList::displayStack() {

	std::cout<<"--Stack contents--"<<std::endl;
	TNode *P = top;
	while (P != nullptr) {
		std::cout << P->value << std::endl;
		P = P->next;
	}
	std::cout<<"--------------"<<std::endl;
}