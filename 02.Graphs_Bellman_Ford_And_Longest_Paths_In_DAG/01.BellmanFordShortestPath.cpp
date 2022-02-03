#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <numeric>
#include <tuple>
#include <algorithm>

class Edge;

using Graph = std::vector<Edge>;

class Edge {
public:
    Edge() = default;

    Edge(int src, int dest, int weight) : _src(src), _dest(dest), _weight(weight) {}

    int getSource() const { return _src; }
    int getDest() const { return _dest; }
    int getWeight() const { return _weight; }

    void setSource(const int src) { _src = src; }
    void setDest(const int dest) { _dest = dest; }
    void setWeight(const int weight) { _weight = weight; }

private:
    int _src{}, _dest{}, _weight{};
};

std::tuple<int, Graph, int, int> readInput() {
    Graph graph;

    int numNodes{}, numEdges{};
    std::cin >> numNodes >> numEdges;

    for (int i = 0; i < numEdges; i++) {
        std::string line;
        std::getline(std::cin >> std::ws, line);
        std::istringstream istr(line);

        int src{}, dest{}, weight{};
        istr >> src >> dest >> weight;

        Edge currEdge(src, dest, weight);
        graph.push_back(currEdge);
    }

    int startNode{}, destNode{};
    std::cin >> startNode >> destNode;

    return std::make_tuple(numNodes, graph, startNode, destNode);
}

void runBellmanFordForGraphWithNegativeEdges(int numNodes, const Graph& graph, int startNode, int dest, 
                                             std::vector<int>& distances, std::vector<int>& prevs) {
    
    distances[startNode] = 0;

    for (int i = 0; i < numNodes - 1; i++) {
        for (const Edge& edge : graph) {
            int from = edge.getSource();
            int to = edge.getDest();
            int weight = edge.getWeight();

            // relaxation step
            if (distances[from] != std::numeric_limits<int>::max() &&
                distances[from] + weight < distances[to]) {
                
                distances[to] = distances[from] + weight;
                prevs[to] = from;
            }
        }
    }

    // detect negative cycles
    for (const Edge& edge : graph) {
        int from = edge.getSource();
        int to = edge.getDest();
        int weight = edge.getWeight();

        if (distances[from] != std::numeric_limits<int>::max() &&
            distances[from] + weight < distances[to]) {
            
            std::cerr << "Negative cycle detected!" << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
}

std::vector<int> constructPathFromPrevIndices(std::vector<int>& prevs, int dest) {
    std::vector<int> path;
    
    path.push_back(dest);

    int parent = prevs[dest];
    while (parent != -1) {
        path.push_back(parent);
        parent = prevs[parent];
    }

    std::reverse(path.begin(), path.end());

    return path;
}

int findWeightOfShortestPath(const Graph& graph, std::vector<int>& path) {
    int weight{};
    for (int i = 0; i < path.size() - 1; i++) {
        int src = path[i], dest = path[i + 1];
        for (const Edge& edge : graph) {
            if (edge.getSource() == src && edge.getDest() == dest) {
                weight += edge.getWeight();
            }
        }
    }

    return weight;
}

void print(std::vector<int>& path) {
    for (const int node : path) {
        std::cout << node << " ";
    }
    std::cout << std::endl;
}

int main() {
    const auto& [nodes, graph, startNode, destNode] = readInput();
    
    std::vector<int> distances(nodes + 1, std::numeric_limits<int>::max());
    std::vector<int> prevs(nodes + 1, -1);

    runBellmanFordForGraphWithNegativeEdges(nodes, graph, startNode, destNode, distances, prevs);

    std::vector<int> shortestPath = constructPathFromPrevIndices(prevs, destNode);

    print(shortestPath);

    int pathWeight = findWeightOfShortestPath(graph, shortestPath);

    std::cout << pathWeight << std::endl;

    return 0;
}
