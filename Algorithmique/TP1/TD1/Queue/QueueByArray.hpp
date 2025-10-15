#ifndef QUEUEBYARRAY_HPP
#define QUEUEBYARRAY_HPP

#include <iostream>
#include <array>

class QueueByArray {
	static constexpr std::size_t max = 1000;
	std::array<int,max> queue;
	int front, rear;

public:
	QueueByArray();
	void push(int value);
	int pop();
};

#endif //QUEUEBYARRAY_HPP
