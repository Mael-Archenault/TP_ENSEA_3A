#include <iostream>
#include <fstream>
#include <vector>


void spanning_tree_matrix(std::string filepath) {
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

    // BFS search for spanning tree
    std::vector<bool> hasBeenVisited(nb_of_vertices, false); // already visited vertices will not be revisited
    std::vector<std::pair<int, int>> spanning_tree_edges; // stores edges of spanning tree for future display
    std::vector<int> to_visit; // vertices to visit next

    hasBeenVisited[0] = true;
    to_visit.push_back(0);

    int index = 0;
    while (index < to_visit.size()) {
        int current_vertex = to_visit[index];
        for (int j = 0; j < nb_of_vertices; j++) {
            if (adjacency_matrix[current_vertex][j] > 0 && !hasBeenVisited[j]) {
                // when a new edge is found, mark vertex as visited, add edge to spanning tree and add vertex to visit list
                hasBeenVisited[j] = true;
                spanning_tree_edges.push_back({current_vertex + 1, j + 1});
                to_visit.push_back(j);
            }
        }
        index++;
    }
    std::ofstream output_file("../outarbgraph.txt");
    if (!output_file.is_open()) {
        std::cerr << "Error opening output file." << std::endl;
        return;
    }
    for (std::pair<int,int> edge : spanning_tree_edges) {
        output_file << edge.first << " " << edge.second << std::endl;
    }
    output_file.close();
}


void spanning_tree_list(std::string filepath) {
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

    // DFS search for spanning tree
    std::vector<bool> hasBeenVisited(nb_of_vertices, false); // already visited vertices will not be revisited
    std::vector<std::pair<int, int>> spanning_tree_edges; // stores edges of spanning tree for future display
    std::vector<int> to_visit; // vertices to visit next

    hasBeenVisited[0] = true;
    to_visit.push_back(0);

    while (!to_visit.empty()) {
        int current_vertex = to_visit.back();
        to_visit.pop_back();
        hasBeenVisited[current_vertex] = true;
        
        for (int neighbour : adjacency_list[current_vertex]) {
            if (!hasBeenVisited[neighbour]) {
                // when a new edge is found, mark vertex as visited, add edge to spanning tree and add vertex to visit list
                hasBeenVisited[neighbour] = true;
                spanning_tree_edges.push_back({current_vertex + 1, neighbour + 1});
                to_visit.push_back(neighbour);
            }
        }
    }

    std::ofstream output_file("../outarbgraph.txt");
    if (!output_file.is_open()) {
        std::cerr << "Error opening output file." << std::endl;
        return;
    }
    for (std::pair<int,int> edge : spanning_tree_edges) {
        output_file << edge.first << " " << edge.second << std::endl;
    }
    output_file.close();
}


int main() {
    spanning_tree_list("../inparbgraph.txt");
    //spanning_tree_matrix("../inparbgraph.txt");
    return 0;
}