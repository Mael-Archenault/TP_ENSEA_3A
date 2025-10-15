#include <iostream>
#include "Stack/StackByArray.hpp"
#include "Stack/StackByLinkedList.hpp"



int main(){
    std::cout<<"Testing Stack with Array"<<std::endl<<std::endl;
    StackByArray stack_by_array;
	std::cout<<"Pushing "<<69420<<std::endl;
	stack_by_array.push(69420);
	std::cout<<"Pushing "<<42069<<std::endl;
	stack_by_array.push(42069);
	std::cout<<"Pushing "<<12345<<std::endl;
	stack_by_array.push(12345);
	std::cout<<"Pushing "<<988<<std::endl;
	stack_by_array.push(988);
	stack_by_array.displayStack();
	std::cout << "Popped value: " << stack_by_array.pop() << std::endl;
	stack_by_array.displayStack();

	std::cout<<std::endl;
    std::cout<<"Testing Stack with Linked List"<<std::endl<<std::endl;
	StackByLinkedList stack_by_linked_list;
	std::cout<<"Pushing "<<69420<<std::endl;
	stack_by_linked_list.push(69420);
	std::cout<<"Pushing "<<42069<<std::endl;
	stack_by_linked_list.push(42069);
	std::cout<<"Pushing "<<12345<<std::endl;
	stack_by_linked_list.push(12345);
	std::cout<<"Pushing "<<988<<std::endl;
	stack_by_linked_list.push(988);
	stack_by_linked_list.displayStack();
	std::cout << "Popped value: " << stack_by_linked_list.pop() << std::endl;
	stack_by_linked_list.displayStack();
    return 0;
}