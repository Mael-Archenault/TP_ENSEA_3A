#include <iostream>

#include "QueueByArray.hpp"
#include "StackByArray.hpp"
#include "StackByLinkedList.hpp"

int main() {
	std::cout << "Hello and welcome to Stack" << std::endl;
	StackByArray stack;
	for (int i = 1; i < 10; i++) {
		stack.push(2*i);
	}
	std::cout << "Stack size: " << stack.getTopIndex() << std::endl;
	std::cout << "Stack top: " << stack.pop() << std::endl;
	stack.push(69420);
	stack.displayStack();

	StackByLinkedList ll;
	ll.push(69420);
	ll.push(42069);
	ll.push(12345);
	ll.push(988);
	ll.displayStack();
	int pop = ll.pop();
	std::cout << "Popped value: " << pop << std::endl;
	ll.displayStack();

	QueueByArray queue;
	return 0;
}
