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

    int sum = 0;
    for (const int& val : subsequence) {
        sum += val;
    }
    file << "Sum = " << sum << std::endl;
    file << std::endl;
    file.close();
}


void find_longest_subsequence_divisible_by_k(std::array<int, MAX_SEQ_LEN>& A, int array_size, int k, int& res_subsequence_size, std::vector<int>& res_subsequence, std::vector<int>& res_indices) {
    std::vector<int> lengths(k, -1);
    std::vector<std::vector<int>> indices(k,std::vector<int>());
    lengths[0] = 0;

    for (int i=0; i<array_size; i++) {
        std::vector<int> next_lengths = lengths; // on part de l’état actuel
        std::vector<std::vector<int>> next_indices = indices;
        for (int r = 0; r < k; r++) {
            if (lengths[r] != -1) {
                int nr = (r + A[i]) % k;
                if (next_lengths[nr]<lengths[r]+1){
                    next_lengths[nr] = lengths[r]+1;
                    indices[r].push_back(i);
                    next_indices[nr] = indices[r];
                }
        
            }
        }
        lengths = next_lengths;
        indices = next_indices;
    }

    std::vector<int> subsequence;
    for (const int& idx : indices[0]) {
        subsequence.push_back( A[idx] );
    }

    res_subsequence_size = lengths[0];
    res_subsequence = subsequence;
    res_indices = indices[0];



}


int main() {
    int array_size = 0;
    std::vector<int> data_vector;
    load_input_file("../inpdivseq.txt", array_size, data_vector);

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
    int k = 5;

    find_longest_subsequence_divisible_by_k(A, array_size, k, subsequence_size, subsequence, indices);

    write_output_file("../outdivseq.txt", subsequence_size, subsequence, indices);

    // Displaying results
    std::cout<<"Results"<<std::endl;
    std::cout << "Subsequence size: " << subsequence_size << std::endl
                << "Subsequence: ";
    for (int val : subsequence) {
        std::cout << val << " ";
    }
    std::cout<<std::endl;

}