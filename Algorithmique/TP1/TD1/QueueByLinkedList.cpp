#include "QueueByLinkedList.hpp"

QueueByLinkedList::QueueByLinkedList() : front(nullptr), rear(nullptr) {}

void QueueByLinkedList::push(int value) {
    TNode *P = new TNode;
    P->value = value;
    P->next = nullptr;
    if (front == nullptr) {
        front = P;
        rear = P;
    }
    else {
        rear->next = P;
        rear = P;
    }
}

int QueueByLinkedList::pop() {
    TNode *P;
    if (front == nullptr) {
        std::cout << "Queue is empty" << std::endl;
    }
    else {
        int popped_value = front->value;
        P = front->next;
        front = P;
        return popped_value;
    }
}