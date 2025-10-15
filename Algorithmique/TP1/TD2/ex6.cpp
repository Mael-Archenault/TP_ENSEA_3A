#include <iostream>
#include <array>
#include <random>
#include <algorithm>

#define ARRAY_SIZE 15
#define MIN_INT_VAL -10000
#define MAX_INT_VAL 10000

void find_longest_subsequence(std::array<int, ARRAY_SIZE>& A) {
    int L[ARRAY_SIZE];
    int previous[ARRAY_SIZE];
    for (int i = 0; i < ARRAY_SIZE; i++) {
        L[i] = 1;
        previous[i] = -1;
    }

    for (int i = 1; i < ARRAY_SIZE; i++) {
        for (int j = 0; j < i; j++) {
            if (A[j] < A[i] && L[i] < L[j] + 1) {
                L[i] = L[j] + 1;
                previous[i] = j;
            }
        }
    }

    int maxLen = L[0];
    int lastIndex = 0;
    for (int i = 1; i < ARRAY_SIZE; i++) {
        if (L[i] > maxLen) {
            maxLen = L[i];
            lastIndex = i;
        }
    }

    std::vector<int> subsequence;
    for (int i = lastIndex; i != -1; i = previous[i]) {
        subsequence.push_back(A[i]);
    }
    std::reverse(subsequence.begin(), subsequence.end());

    // Affichage
    std::cout << "Length of longest subsequence = " << maxLen << std::endl;
    std::cout << "Longest subsequence = ";
    for (int val : subsequence) std::cout << val << " ";
    std::cout << std::endl;
}


int main() {
    // génère des entiers aléatoires pour le tableau A
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(MIN_INT_VAL, MAX_INT_VAL);
    
    std::array<int, ARRAY_SIZE> A;
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        A[i] = dist(gen);
        std::cout << A[i] << " ";
    }
    std::cout << std::endl;

    std::array<int, ARRAY_SIZE> C = {1,2,3,4,9,10,5,6,7};
    find_longest_subsequence(A);
}
