#include "QueueByCircularList.hpp"

QueueByCircularList::QueueByCircularList() : n(0), front(0), rear(max-1) {}

void QueueByCircularList::push(int value) {
    if (n >= max) {
        std::cout << "Queue is full" << std::endl;
    }   
    else {
        rear = (rear + 1) % max;
        queue[rear] = value;
        n++;
    }
}

int QueueByCircularList::pop() {
    if (n <= 0) {
        std::cout << "Queue is empty" << std::endl;
    }
    else {
        int popped_value = queue[front];
        front = (front + 1) % max;
        n--;
        return popped_value;
    }
}