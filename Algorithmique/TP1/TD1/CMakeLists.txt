cmake_minimum_required(VERSION 3.28)
project(TD1_algo)

set(CMAKE_CXX_STANDARD 20)


add_executable(test_stack test_stack.cpp 
                Stack/StackByArray.cpp
                Stack/StackByArray.hpp
                Stack/StackByLinkedList.cpp
                Stack/StackByLinkedList.hpp)

add_executable(test_queue test_queue.cpp
                Queue/QueueByArray.cpp
                Queue/QueueByArray.hpp
                Queue/QueueByCircularList.cpp
                Queue/QueueByCircularList.hpp
                Queue/QueueByLinkedList.cpp
                Queue/QueueByLinkedList.hpp)

add_executable(test_heap test_heap.cpp
                Heap/Binary_Heap_Max.cpp
                Heap/Binary_Heap_Max.hpp
                Heap/Binary_Heap_Min.cpp
                Heap/Binary_Heap_Min.hpp)
