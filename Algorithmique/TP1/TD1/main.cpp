#include <iostream>

#include "StackByArray.hpp"
#include "StackByLinkedList.hpp"
#include "QueueByArray.hpp"
#include "QueueByCircularList.hpp"
#include "QueueByLinkedList.hpp"

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

	QueueByArray qba;
	qba.push(1);
	qba.push(2);
	qba.push(3);
	std::cout << "Popped from queue: " << qba.pop() << std::endl;
	std::cout << "Popped from queue: " << qba.pop() << std::endl;
	std::cout << "Popped from queue: " << qba.pop() << std::endl;
	qba.push(69420);

	QueueByCircularList qbcl;
	qbcl.push(5);
	qbcl.push(8);
	qbcl.push(14);
	qbcl.push(16);
	qbcl.push(18);
	std::cout << "Popped from circular queue: " << qbcl.pop() << std::endl;
	std::cout << "Popped from circular queue: " << qbcl.pop() << std::endl;
	std::cout << "Popped from circular queue: " << qbcl.pop() << std::endl;
	qbcl.push(69420);

	QueueByLinkedList qbll;
	qbll.push(312);
	qbll.push(92);
	qbll.push(211);
	qbll.push(43);
	std::cout << "Popped from linked list queue: " << qbll.pop() << std::endl;
	std::cout << "Popped from linked list queue: " << qbll.pop() << std::endl;
	std::cout << "Popped from linked list queue: " << qbll.pop() << std::endl;
	qbll.push(69420);
	return 0;
}
