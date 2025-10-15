#ifndef STACKBYARRAY_HPP
#define STACKBYARRAY_HPP

#include <array>
#include <iostream>

class StackByArray {
	static constexpr std::size_t max = 1000;
	std::array<int, max> stack;
	int top;

public:
	StackByArray();
	void push(int value);
	int getTopIndex() {return top;}
	int pop();
	void displayStack();
};

#endif //STACKBYARRAY_HPP
