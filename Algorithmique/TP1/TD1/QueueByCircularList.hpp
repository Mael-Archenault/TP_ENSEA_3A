#ifndef QUEUEBYCIRCULARLIST_HPP
#define QUEUEBYCIRCULARLIST_HPP

#include <iostream>
#include <array>

class QueueByCircularList {
    static constexpr std::size_t max = 1000;
    std::array<int,max-1> queue;
    int     n, front, rear;

public:
    QueueByCircularList();
    void push(int value);
    int pop();
};

#endif // QUEUEBYCIRCULARLIST_HPP