#include <iostream>
#include "Binary_Heap_Max.hpp"
#include "Binary_Heap_Min.hpp"



int main(){
    // testing max binary tree
    std::cout << "Testing Binary_Heap_Max" << std::endl;
    Binary_Heap_Max Binary_Heap_Max;
    Binary_Heap_Max.push(10);
    Binary_Heap_Max.push(20);
    Binary_Heap_Max.push(5);
    Binary_Heap_Max.push(15);
    std::cout << "Popped: " << Binary_Heap_Max.pop() << std::endl; // Should print 20
    std::cout << "Popped: " << Binary_Heap_Max.pop() << std::endl; // Should print 15
    std::cout << "Popped: " << Binary_Heap_Max.pop() << std::endl; // Should print 10
    std::cout << "Popped: " << Binary_Heap_Max.pop() << std::endl; // should print 5

    // testing min binary tree
    std::cout << "Testing Binary_Heap_Min" << std::endl;
    Binary_Heap_Min Binary_Heap_Min;
    Binary_Heap_Min.push(10);
    Binary_Heap_Min.push(20);
    Binary_Heap_Min.push(5);
    Binary_Heap_Min.push(15);
    std::cout << "Popped: " << Binary_Heap_Min.pop() << std::endl; // Should print 5
    std::cout << "Popped: " << Binary_Heap_Min.pop() << std::endl; // Should print 10
    std::cout << "Popped: " << Binary_Heap_Min.pop() << std::endl; // Should print 15
    std::cout << "Popped: " << Binary_Heap_Min.pop() << std::endl; // should print 20
    return 0;
}