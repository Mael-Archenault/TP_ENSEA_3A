#include "StackByArray.hpp"

StackByArray::StackByArray() : top(0){}

void StackByArray::push(int value) {
	if (top >= max) {
		std::cout << "Stack is full" << std::endl;
	}
	else {
		top++;
		stack[top-1] = value;
	}
}

int StackByArray::pop() {
	if (top <= 0) {
		std::cout << "Stack is empty" << std::endl;
	}
	else {
		int value = stack[top-1];
		top--;
		return value;
	}
}

void StackByArray::displayStack() {
	std::cout<<"--Stack contents--"<<std::endl;
	for (int i = 0; i < top; i++) {
		std::cout << stack[i] << std::endl;
	}
	std::cout<<"--------------"<<std::endl;
}

