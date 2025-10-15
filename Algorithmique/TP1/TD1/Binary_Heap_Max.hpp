#pragma once
#include <iostream>
#include <vector>

class Binary_Heap_Max{
    private:
        std::vector<int> buffer;

        size_t parent(size_t i);
        size_t left_child(size_t i);
        size_t right_child(size_t i);

        void move_up(size_t i);
        void move_down(size_t i);

    public:
        void push(int value);
        int pop();
};