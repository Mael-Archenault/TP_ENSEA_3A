#include <iostream>
#include "QueueByArray.hpp"
#include "QueueByCircularList.hpp"
#include "QueueByLinkedList.hpp"


int main(){
    std::cout<<"Testing Queue with Array"<<std::endl<<std::endl;
    QueueByArray qba;
    std::cout<<"Pushing "<<10<<std::endl;
	qba.push(10);
    std::cout<<"Pushing "<<20<<std::endl;
	qba.push(20);
    std::cout<<"Pushing "<<5<<std::endl;
	qba.push(5);
	std::cout << "Popped from queue: " << qba.pop() << std::endl;
	std::cout << "Popped from queue: " << qba.pop() << std::endl;
	std::cout << "Popped from queue: " << qba.pop() << std::endl;

    std::cout<<std::endl;

    std::cout<<"Testing Queue with Circular List"<<std::endl<<std::endl;
	QueueByCircularList qbcl;
    std::cout<<"Pushing "<<13<<std::endl;
	qbcl.push(13);
    std::cout<<"Pushing "<<21<<std::endl;
	qbcl.push(21);
    std::cout<<"Pushing "<<32<<std::endl;
	qbcl.push(32);
	std::cout << "Popped from circular queue: " << qbcl.pop() << std::endl;
	std::cout << "Popped from circular queue: " << qbcl.pop() << std::endl;
	std::cout << "Popped from circular queue: " << qbcl.pop() << std::endl;

    std::cout<<std::endl;

    std::cout<<"Testing Queue with Linked List"<<std::endl<<std::endl;
	QueueByLinkedList qbll;
    std::cout<<"Pushing "<<4<<std::endl;
	qbll.push(4);
    std::cout<<"Pushing "<<2<<std::endl;
	qbll.push(2);
    std::cout<<"Pushing "<<8<<std::endl;
	qbll.push(8);
	std::cout << "Popped from linked list queue: " << qbll.pop() << std::endl;
	std::cout << "Popped from linked list queue: " << qbll.pop() << std::endl;
	std::cout << "Popped from linked list queue: " << qbll.pop() << std::endl;
	return 0;
}