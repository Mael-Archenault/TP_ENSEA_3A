#include "QueueByArray.hpp"

QueueByArray::QueueByArray() : front(1), rear(0) {}

void QueueByArray::push(int value) {
	if (rear >= max) {
		std::cout << "Overflow" << std::endl;
	}
	else {
		rear++;
		queue[rear] = value;	}
}

int QueueByArray::pop() {
	if (front > rear) {
		std::cout << "Queue is empty" << std::endl;
	}
	else {
		int popped_value = queue[front];
		front++;
		return popped_value;
	}
}