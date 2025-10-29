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
        std::vector<std::vector<std::pair<int, int>>> list;
        Graph(): n_nodes(0), n_edges(0) {}

        void display(){
            std::cout << "Graph with " << n_nodes << " nodes and " << n_edges << " edges." << std::endl;
            for(int i=0; i<n_nodes; i++){
                std::cout << "Node " << i << ": ";
                for(auto neighbor : list[i]){
                    std::cout << "(" << neighbor.first << ", " << neighbor.second << ") ";
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
    file >> graph.n_nodes >> graph.n_edges >> starting_node >> ending_node;
    starting_node--;
    ending_node--;
    graph.list.resize(graph.n_nodes);

    for(int i=0; i<graph.n_edges; i++){
        int u = 0;
        int v = 0;
        int weight = 0;
        file >> u >> v >> weight;
        graph.list.at(u-1).push_back(std::make_pair(v-1, weight));
        graph.list.at(v-1).push_back(std::make_pair(u-1, weight));
    } 
}

void write_output_file(const std::string& filename, Path& path) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        throw std::runtime_error("File not found");
    }
    file << path.total_weight << std::endl;

    for (int i=0; i<path.nodes.size()-1; i++) {
        file <<path.nodes[i] << "->";
    }
    file << path.nodes.back();
    file << std::endl;
    file.close();
}


void find_shortest_path(Graph& graph, int starting_node, int ending_node, Path& path) {

    std::vector<int> unvisited;
    std::vector<int> distances;
    std::vector<std::vector<int>> best_paths;
    best_paths.resize(graph.n_nodes);


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
        if (unvisited.size()==0){ // unvisited empty
            flag = 0;
            continue;
        }

        std::vector<int> unvisited_distances;
        for (auto node: unvisited){
            unvisited_distances.push_back(distances[node]);
        }

        if (*std::min_element(unvisited_distances.begin(), unvisited_distances.end()) == 1000){ // all remaining unvisited are unreachable
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
        
        
        for (auto neighbor: graph.list.at(current_node)){
            int new_path_dist = distances[current_node] + neighbor.second;
            if (new_path_dist <= distances[neighbor.first]){
                distances[neighbor.first] = new_path_dist;
                best_paths[neighbor.first] = best_paths[current_node];
                best_paths[neighbor.first].push_back(current_node);
            }
        }

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
    path.nodes = best_paths[ending_node];
    path.nodes.push_back(ending_node);

    for (auto& node : path.nodes){
        node += 1; // to have nodes starting from 1
    }


}


int main() {
    int starting_node;
    int ending_node;
    Graph graph;
    load_input_file("../inpdijgraph.txt", graph, starting_node, ending_node);
    Path path;
    find_shortest_path(graph, starting_node, ending_node, path);
    write_output_file("../outdijgraph.txt", path);
}
