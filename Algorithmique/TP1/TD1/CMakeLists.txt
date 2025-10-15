cmake_minimum_required(VERSION 3.28)
project(TD1_algo)

set(CMAKE_CXX_STANDARD 20)


add_executable(test_stack test_stack.cpp StackByArray.cpp StackByArray.hpp StackByLinkedList.cpp StackByLinkedList.hpp)

add_executable(test_queue test_queue.cpp QueueByArray.cpp QueueByArray.hpp QueueByCircularList.cpp QueueByCircularList.hpp QueueByLinkedList.cpp QueueByLinkedList.hpp)

add_executable(test_heap test_heap.cpp Binary_Heap_Max.cpp Binary_Heap_Max.hpp Binary_Heap_Min.cpp Binary_Heap_Min.hpp)
