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

class Graph{
    public:
        int n_nodes;
        int n_edges;
        std::vector<std::vector<int>> matrix;
        Graph(): n_nodes(0), n_edges(0) {}

        void display(){
            std::cout << "Graph with " << n_nodes << " nodes and " << n_edges << " edges." << std::endl;
            for(int i=0; i<n_nodes; i++){
                for(int j=0; j<n_nodes; j++){
                    std::cout << matrix[i][j] << " ";
                }
                std::cout << std::endl;
            }
        }
};

class Path{
    public:
        int starting_node;
        int ending_node;
        std::vector<int> nodes;
        int total_weight;
        Path(): total_weight(0) {}
        void display(){
            std::cout << "Path from node " << starting_node << " to node " << ending_node << " with total weight " << total_weight << ": ";
            for(auto node: nodes){
                std::cout << node << " ";
            }
            std::cout << std::endl;
        }
};

void load_input_file(const std::string& filename, Graph& graph, int& starting_node, int& ending_node) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        throw std::runtime_error("File not found");
    }

    // First line with the number of element
    file >> graph.n_nodes >> graph.n_edges >> starting_node >> ending_node;
    starting_node--;
    ending_node--;
    graph.matrix.resize(graph.n_nodes, std::vector<int>(graph.n_nodes, 0));

    for(int i=0; i<graph.n_edges; i++){
        int u = 0;
        int v = 0;
        int weight = 0;
        file >> u >> v >> weight;
        graph.matrix[u-1][v-1] = weight;
        graph.matrix[v-1][u-1] = weight;
    } 
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


void find_shortest_path(Graph& graph, int starting_node, int ending_node, Path& path) {

    std::vector<int> unvisited;
    std::vector<int> distances;
    std::vector<int> best_path;


    for(int i=0; i<graph.n_nodes; i++){
        unvisited.push_back(i);
    }

    for (int i=0; i<graph.n_nodes; i++){
        distances.push_back(1000);
    }
    
    distances[starting_node] = 0;
    int current_node;

    int flag = 1;
    // loop
    while (flag){   

        // checking stopping conditions
        if (unvisited.size()==0){
            flag = 0;
            continue;
        }
        int index = 0;
        while (unvisited[index] == 1000 && index < unvisited.size()) {
            index++;
        }
        if (index == unvisited.size()) {
            flag = 0;
            continue;
        }


        //update current_node
        int current_node = unvisited[0];
        for (int i = 1; i < unvisited.size(); ++i) {
            if (distances[unvisited[i]] < distances[current_node]) {
                current_node = unvisited[i];
            }
        }

        if (current_node == ending_node){
            flag = 0;
            continue;
        }
        
        std::cout << "Current node: " << current_node << std::endl;

        std::vector<int> neighbors;
        for (int i =0; i < graph.n_nodes; i++){
            if (graph.matrix[current_node][i] != 0){
                neighbors.push_back(i);
            }
        }

        for (auto neighbor: neighbors){
            int new_path_dist = distances[current_node] + graph.matrix[current_node][neighbor];
            if (new_path_dist <= distances[neighbor]){
                distances[neighbor] = new_path_dist;
            }
        }

        std::cout << "Unvisited nodes: ";
        for (auto node: unvisited){
            std::cout << node << " ";
        }
        std::cout<<std::endl;
        // erase the current node from the unvisited list
        
        for (int i=0; i<unvisited.size(); i++){
            if (unvisited[i] == current_node){
                unvisited.erase(unvisited.begin()+i);
            }
        }
    }
    
    path.starting_node = starting_node;
    path.ending_node = ending_node;

    path.total_weight = distances[ending_node];

}


int main() {
    int starting_node;
    int ending_node;
    Graph graph;
    load_input_file("../inpdijgraph.txt", graph, starting_node, ending_node);
    Path path;
    find_shortest_path(graph, starting_node, ending_node, path);



    path.display();

    
    // // Displaying loaded data
    // std::cout<<"Loaded data"<<std::endl;
    // std::cout << "List size: " << array_size << std::endl;
    // std::cout << "Data: ";
    // for (int val : data_vector) {
    //     std::cout << val << " ";
    // }
    // std::cout<<std::endl;

    // std::array<int, MAX_SEQ_LEN> A;
    // for (int i = 0; i < array_size; i++) {
    //     A[i] = data_vector[i];
    // } 

    // int subsequence_size = 0;
    // std::vector<int> subsequence;
    // std::vector<int> indices;
    // find_longest_subsequence(A, array_size, subsequence_size, subsequence, indices);

    // write_output_file("../outmonoseq.txt", subsequence_size, subsequence, indices);

    // // Displaying results
    // std::cout<<"Results"<<std::endl;
    // std::cout << "Subsequence size: " << subsequence_size << std::endl
    //             << "Subsequence: ";
    // for (int val : subsequence) {
    //     std::cout << val << " ";
    // }
    // std::cout<<std::endl;
}
