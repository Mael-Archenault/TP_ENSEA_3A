cmake_minimum_required(VERSION 3.28)
project(TD1_algo)

set(CMAKE_CXX_STANDARD 20)

add_executable(TD1_algo main.cpp
        StackByArray.cpp
        StackByArray.hpp
        StackByLinkedList.cpp
        StackByLinkedList.hpp
        QueueByArray.cpp
        QueueByArray.hpp
        QueueByCircularList.cpp
        QueueByCircularList.hpp
        QueueByLinkedList.cpp
        QueueByLinkedList.hpp)
