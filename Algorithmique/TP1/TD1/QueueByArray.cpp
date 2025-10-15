#include "QueueByArray.hpp"

QueueByArray::QueueByArray() : front(1), rear(0) {}

void QueueByArray::push(int value) {
	if (rear >= max) {
		std::cout << "Overflow" << std::endl;
	}
	else {
		rear++;
	}
}