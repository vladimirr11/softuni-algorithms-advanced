#include <iostream>
#include <string>
#include <sstream>
#include <numeric>
#include <vector>
#include <stack>
#include <tuple>

using AdjMatrix = std::vector<std::vector<int>>;

struct Edge {
    int from{}, to{};
    Edge() = default;
    Edge(int iFrom, int iTo) : from(iFrom), to(iTo) {};
};

std::tuple<AdjMatrix, std::vector<Edge>, int, int> readInput() {
    int numNodes{}, numEdges{};
    std::cin >> numNodes >> numEdges;

    AdjMatrix adjMatrix(numNodes + 1, std::vector<int>(numNodes + 1, 0));

    std::vector<Edge> edges;

    for (int i = 0; i < numEdges; i++) {
        std::string line;
        std::getline(std::cin >> std::ws, line);
        std::istringstream istr(line);
        
        int from{}, to{}, weight{};
        istr >> from >> to >> weight;
 
        adjMatrix[from][to] = weight;
        adjMatrix[to][from] = weight;

        edges.emplace_back(from, to);
    }

    int startNode{}, endNode{};
    std::cin >> startNode >> endNode;

    return std::make_tuple(adjMatrix, edges, startNode, endNode);
}

int findRoot(int node, std::vector<int>& parents) {
    int root = parents[node];

    while (root != parents[node]) {
        root = parents[root];
    }

    return root;
}

void findShortestPathInGraphUsingBellmanFord(AdjMatrix& graph, std::vector<Edge>& edges, std::vector<int>& distances, 
                                             std::vector<int>& prevs) {
    
    for (int i = 0; i < graph.size() - 1; i++) {
        for (const auto [from, to] : edges) {
            if (distances[from] != std::numeric_limits<int>::max()) {
                int newDistance = distances[from] + graph[from][to];
                if (newDistance < distances[to]) {
                    distances[to] = newDistance;
                    prevs[to] = from;
                }
            }   
        }
    }
}

bool detectNegativeCycle(AdjMatrix& graph, std::vector<Edge>& edges, std::vector<int>& distances) {
    for (const auto& [from, to] : edges) {
        if (distances[from] != std::numeric_limits<int>::max()) {
            int newDistance = distances[from] + graph[from][to];
            if (newDistance < distances[to]) {
                return true;
            }
        }
    }

    return false;
}

std::stack<int> constructPathFromPrevIndices(int dest, std::vector<int>& prevs) {
    std::stack<int> path;
    
    path.push(dest);

    int parent = prevs[dest];
    while (parent != -1) {
        path.push(parent);
        parent = prevs[parent];
    }

    return path;
}

void printPath(std::stack<int>& path) {
    while (!path.empty()) {
        std::cout << path.top() << " ";
        path.pop();
    }

    std::cout << std::endl;
}

int main() {
    auto [graph, edges, startNode, endNode] = readInput();

    std::vector<int> distances(graph.size() + 1, std::numeric_limits<int>::max());
    std::vector<int> parents(graph.size() + 1, -1);

    distances[startNode] = 0;

    findShortestPathInGraphUsingBellmanFord(graph, edges, distances, parents);

    bool hasNegativeCycle = detectNegativeCycle(graph, edges, distances);

    if (hasNegativeCycle) {
        std::cout << "Undefined!" << std::endl;
    } else {
        std::stack<int> path = constructPathFromPrevIndices(endNode, parents);
        printPath(path);
        std::cout << distances[endNode] << std::endl;
    }

    return 0;
}
