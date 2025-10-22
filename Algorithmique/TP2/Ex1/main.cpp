#include <iostream>
#include <array>
#include <random>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>

#define MIN_INT_VAL -10000
#define MAX_INT_VAL 10000

#define MAX_SEQ_LEN 5000

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

void write_output_file(const std::string& filename,const int& subsequence_size, const std::vector<int>& subsequence, const std::vector<int>& indices) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        throw std::runtime_error("File not found");
    }
    file << subsequence_size << std::endl;

    for (int i=0; i<subsequence.size(); i++) {
        file <<"a["<< indices[i] << "] = "<< subsequence[i] << std::endl;
    }
    file << std::endl;
    file.close();
}


void find_longest_subsequence(std::array<int, MAX_SEQ_LEN>& A, int array_size, int& res_subsequence_size, std::vector<int>& res_subsequence, std::vector<int>& res_indices) {
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
    std::vector<int> indices;
    for (int i = lastIndex; i != -1; i = previous[i]) {
        subsequence.push_back(A[i]);
        indices.push_back(i);
    }
    std::reverse(subsequence.begin(), subsequence.end());
    std::reverse(indices.begin(), indices.end());

    res_subsequence_size = maxLen;
    res_subsequence = subsequence;
    res_indices = indices;



}


int main() {
    int array_size = 0;
    std::vector<int> data_vector;
    load_input_file("../inpmonoseq.txt", array_size, data_vector);

    // Displaying loaded data
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
    std::vector<int> indices;
    find_longest_subsequence(A, array_size, subsequence_size, subsequence, indices);

    write_output_file("../outmonoseq.txt", subsequence_size, subsequence, indices);

    // Displaying results
    std::cout<<"Results"<<std::endl;
    std::cout << "Subsequence size: " << subsequence_size << std::endl
                << "Subsequence: ";
    for (int val : subsequence) {
        std::cout << val << " ";
    }
    std::cout<<std::endl;
}
