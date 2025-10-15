#include <iostream>
#include "Binary_Heap_Max.hpp"
#include "Binary_Heap_Min.hpp"



int main(){
    // testing max binary tree
    std::cout << "Testing Binary_Heap_Max" << std::endl<<std::endl;
    Binary_Heap_Max Binary_Heap_Max;
    std::cout<<"Pushing "<<10<<std::endl;
    Binary_Heap_Max.push(10);
    std::cout<<"Pushing "<<20<<std::endl;
    Binary_Heap_Max.push(20);
    std::cout<<"Pushing "<<5<<std::endl;
    Binary_Heap_Max.push(5);
    std::cout<<"Pushing "<<15<<std::endl;
    Binary_Heap_Max.push(15);
    std::cout << "Popped: " << Binary_Heap_Max.pop() << std::endl; // Should print 20
    std::cout << "Popped: " << Binary_Heap_Max.pop() << std::endl; // Should print 15
    std::cout << "Popped: " << Binary_Heap_Max.pop() << std::endl; // Should print 10
    std::cout << "Popped: " << Binary_Heap_Max.pop() << std::endl; // should print 5

    std::cout<<std::endl;

    // testing min binary tree
    std::cout << "Testing Binary_Heap_Min" << std::endl<<std::endl;
    Binary_Heap_Min Binary_Heap_Min;
    std::cout<<"Pushing "<<10<<std::endl;
    Binary_Heap_Min.push(10);
    std::cout<<"Pushing "<<20<<std::endl;
    Binary_Heap_Min.push(20);
    std::cout<<"Pushing "<<5<<std::endl;
    Binary_Heap_Min.push(5);
    std::cout<<"Pushing "<<15<<std::endl;
    Binary_Heap_Min.push(15);
    std::cout << "Popped: " << Binary_Heap_Min.pop() << std::endl; // Should print 5
    std::cout << "Popped: " << Binary_Heap_Min.pop() << std::endl; // Should print 10
    std::cout << "Popped: " << Binary_Heap_Min.pop() << std::endl; // Should print 15
    std::cout << "Popped: " << Binary_Heap_Min.pop() << std::endl; // should print 20
    return 0;
}