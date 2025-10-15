#include "Binary_Heap_Min.hpp"
#include <stdexcept>

size_t Binary_Heap_Min::parent(size_t i){
    return (i - 1) / 2;
}

size_t Binary_Heap_Min::left_child(size_t i){
    return 2 * i + 1;
}

size_t Binary_Heap_Min::right_child(size_t i){
    return 2 * i + 2;
}
void Binary_Heap_Min::move_up(size_t i){
    if(i == 0) return;
    if (i >= buffer.size()) return;

    size_t p = parent(i);
    if(buffer[i] < buffer[p]){
        std::swap(buffer[i], buffer[p]);
        move_up(p);
    }
}

void Binary_Heap_Min::move_down(size_t i){
    size_t left = left_child(i);
    size_t right = right_child(i);
    size_t smallest = i;

    if(left < buffer.size() && buffer[left] < buffer[smallest]){
        smallest = left;
    }
    if(right < buffer.size() && buffer[right] < buffer[smallest]){
        smallest = right;
    }
    if(smallest != i){
        std::swap(buffer[i], buffer[smallest]);
        move_down(smallest);
    }
}

void Binary_Heap_Min::push(int value){
    buffer.push_back(value);
    move_up(buffer.size() - 1);
}

int Binary_Heap_Min::pop(){
    if(buffer.empty()){
        throw std::out_of_range("Heap is empty");
    }
    int root = buffer[0];
    buffer[0] = buffer.back();
    buffer.pop_back();
    move_down(0);
    return root;
}