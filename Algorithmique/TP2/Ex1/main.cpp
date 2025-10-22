#include <iostream>
#include <array>
#include <random>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>

#define MIN_INT_VAL -10000
#define MAX_INT_VAL 10000

#define MAX_SEQ_LEN 1000

void load_input_file(const std::string& filename, int& list_size, std::vector<int>& data) {
    std::ifstream file(filename);
    std::string s;

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        throw std::runtime_error("File not found");
    }

    // First line with the number of element
    std::getline(file, s);
    list_size = std::stoi(s);

    // Next line with the values separated by a space
    std::getline(file, s);
    std::istringstream iss(s);
    int value;
    while (iss >> value) {
        data.push_back(value);
    }
    file.close();

    
}

void write_output_file(const std::string& filename,const int& subsequence_size, const std::vector<int>& subsequence) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        throw std::runtime_error("File not found");
    }
    file << subsequence_size << std::endl;

    for (const int& val : subsequence) {
        file << val << " ";
    }
    file << std::endl;
    file.close();
}


void find_longest_subsequence(std::array<int, MAX_SEQ_LEN>& A, int array_size, int& res_subsequence_size, std::vector<int>& res_subsequence) {
    int L[array_size];
    int previous[array_size];
    for (int i = 0; i < array_size; i++) {
        L[i] = 1;
        previous[i] = -1;
    }

    for (int i = 1; i < array_size; i++) {
        for (int j = 0; j < i; j++) {
            if (A[j] < A[i] && L[i] < L[j] + 1) {
                L[i] = L[j] + 1;
                previous[i] = j;
            }
        }
    }

    int maxLen = L[0];
    int lastIndex = 0;
    for (int i = 1; i < array_size; i++) {
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

    res_subsequence_size = maxLen;
    res_subsequence = subsequence;
}


int main() {
    int array_size = 0;
    std::vector<int> data_vector;
    load_input_file("../inpmonoseq.txt", array_size, data_vector);

    // Affichage des données chargées
    std::cout<<"Loaded data"<<std::endl;
    std::cout << "List size: " << array_size << std::endl;
    std::cout << "Data: ";
    for (int val : data_vector) {
        std::cout << val << " ";
    }
    std::cout<<std::endl;

    std::array<int, MAX_SEQ_LEN> A;
    for (int i = 0; i < array_size; i++) {
        A[i] = data_vector[i];
    } 

    int subsequence_size = 0;
    std::vector<int> subsequence;
    find_longest_subsequence(A, array_size, subsequence_size, subsequence);

    write_output_file("../outmonoseq.txt", subsequence_size, subsequence);

    
    
}
