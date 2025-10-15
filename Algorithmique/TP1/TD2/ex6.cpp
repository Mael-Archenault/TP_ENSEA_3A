#include <iostream>
#include <array>
#include <random>

#define ARRAY_SIZE 100
#define MIN_INT_VAL -10000
#define MAX_INT_VAL 10000

int find_longest_subsequence(const std::array<int, ARRAY_SIZE>& A) {
    std::array
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        std::cout << A[i] << " ";
    }
}

int main() {
    // génère des entiers aléatoires pour le tableau A
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(MIN_INT_VAL, MAX_INT_VAL);
    
    std::array<int, ARRAY_SIZE> A;
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        A[i] = dist(gen);
    }


}