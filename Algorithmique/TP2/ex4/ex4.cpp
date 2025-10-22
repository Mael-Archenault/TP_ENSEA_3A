#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

void connected_graph_matrix(std::string filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filepath << std::endl;
        return;
    }

    int nb_of_vertices, nb_of_edges;
    if (file >> nb_of_vertices >> nb_of_edges) {
        std::cout << "Number of vertices: " << nb_of_vertices << std::endl;
        std::cout << "Number of edges: " << nb_of_edges << std::endl;
    }
    else {
        std::cerr << "Error reading number of vertices and edges." << std::endl;
        return;
    }

    std::vector<std::vector<int>> adjacency_matrix(nb_of_vertices, std::vector<int>(nb_of_vertices, 0));
    for (int i = 0; i < nb_of_edges; i++) {
        int vertex1, vertex2;
        if (!(file >> vertex1 >> vertex2)) {
            std::cerr << "Error reading edge data." << std::endl;
            return;
        }
        else {
            adjacency_matrix[vertex1-1][vertex2-1]++;
            adjacency_matrix[vertex2-1][vertex1-1]++;
        }
    }
    file.close();

    std::vector<bool> hasBeenVisited(nb_of_vertices, false);
    std::vector<std::vector<int>> sub_graphs_list;
    // visits every vertex once to find all sub-graphs
    for (int i = 0; i < nb_of_vertices; i++) {
        if (!hasBeenVisited[i]) {
            hasBeenVisited[i] = true;
            std::vector<int> to_visit;
            std::vector<int> sub_graph;
            int index = 0;
            to_visit.push_back(i); 
            // when an unvisited vertex is found, explore all reachable vertices (one sub-graph)
            while (index < to_visit.size()) {
                int current_vertex = to_visit[index];
                sub_graph.push_back(current_vertex + 1);
                for (int j = 0; j < nb_of_vertices; j++) {
                    if (adjacency_matrix[current_vertex][j] > 0 && !hasBeenVisited[j]) {
                        hasBeenVisited[j] = true;
                        to_visit.push_back(j);
                    }
                }
                index++;
            }
            sub_graphs_list.push_back(sub_graph);
        }
    }

    std::ofstream output_file("../outcongraph.txt");
    if (!output_file.is_open()) {
        std::cerr << "Error opening output file." << std::endl;
        return;
    }
    int nb_of_subgraphs = sub_graphs_list.size();
    output_file << "Graphs contains " << nb_of_subgraphs << " connected subgraphs" << std::endl << std::endl;
    for (int i = 0; i < nb_of_subgraphs; i++) {
        std::vector<int> sub_graph = sub_graphs_list[i];
        output_file << "Connected subgraph " << i+1 << ": " << std::endl;
        for (int vertex : sub_graph) {
            output_file << vertex << " ";
        }
        output_file << std::endl << std::endl;
    }
    output_file.close();
}


void connected_graph_list(std::string filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filepath << std::endl;
        return;
    }

    int nb_of_vertices, nb_of_edges;
    if (file >> nb_of_vertices >> nb_of_edges) {
        std::cout << "Number of vertices: " << nb_of_vertices << std::endl;
        std::cout << "Number of edges: " << nb_of_edges << std::endl;
    }
    else {
        std::cerr << "Error reading number of vertices and edges." << std::endl;
        return;
    }

    std::vector<std::vector<int>> adjacency_list(nb_of_vertices);
    for (int i = 0; i < nb_of_edges; i++) {
        int vertex1, vertex2;
        if (!(file >> vertex1 >> vertex2)) {
            std::cerr << "Error reading edge data." << std::endl;
            return;
        }
        else {
            adjacency_list[vertex1-1].push_back(vertex2-1);
            adjacency_list[vertex2-1].push_back(vertex1-1);
        }
    }
    file.close();

    std::vector<bool> hasBeenVisited(nb_of_vertices, false);
    std::vector<std::vector<int>> sub_graphs_list;
    // visits every vertex once to find all sub-graphs
    for (int i = 0; i < nb_of_vertices; i++) {
        if (!hasBeenVisited[i]) {
            std::vector<int> to_visit;
            std::vector<int> sub_graph;
            to_visit.push_back(i); 
            while (!to_visit.empty()) {
                int current_vertex = to_visit.back();
                to_visit.pop_back();
                hasBeenVisited[current_vertex] = true;
                sub_graph.push_back(current_vertex + 1);
                for (int neighbour : adjacency_list[current_vertex]) {
                    if (!hasBeenVisited[neighbour]) {    
                        hasBeenVisited[neighbour] = true;
                        to_visit.push_back(neighbour);
                    }
                }
            }
            sub_graphs_list.push_back(sub_graph);
        }
    }

    std::ofstream output_file("../outcongraph.txt");
    if (!output_file.is_open()) {
        std::cerr << "Error opening output file." << std::endl;
        return;
    }
    int nb_of_subgraphs = sub_graphs_list.size();
    output_file << "Graphs contains " << nb_of_subgraphs << " connected subgraphs" << std::endl << std::endl;
    for (int i = 0; i < nb_of_subgraphs; i++) {
        std::vector<int> sub_graph = sub_graphs_list[i];
        output_file << "Connected subgraph " << i+1 << ": " << std::endl;
        for (int vertex : sub_graph) {
            output_file << vertex << " ";
        }
        output_file << std::endl << std::endl;
    }
    output_file.close();
}


int main() {
    connected_graph_list("../inpcongraph.txt");
    //connected_graph_matrix("../inpcongraph.txt");
    return 0;
}