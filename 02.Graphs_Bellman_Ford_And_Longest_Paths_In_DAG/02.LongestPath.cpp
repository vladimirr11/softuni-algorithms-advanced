#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <numeric>
#include <tuple>
#include <queue>

using Graph = std::vector<std::vector<int>>;

std::tuple<Graph, int, int> readInput() {

    int numNodes{}, numEdges{};
    std::cin >> numNodes >> numEdges;

    Graph graph(numNodes + 1, std::vector<int>(numNodes + 1, 0));

    for (int i = 0; i < numEdges; i++) {
        std::string line;
        std::getline(std::cin >> std::ws, line);
        std::istringstream istr(line);

        int src{}, dest{}, weight{};
        istr >> src >> dest >> weight;

        graph[src][dest] = weight;
    }

    int startNode{}, destNode{};
    std::cin >> startNode >> destNode;

    return std::make_tuple(graph, startNode, destNode);
}

void topologicalSortOfDAGNodes(int startNode, std::deque<int>& sortedNodes, const Graph& graph, 
                               std::vector<bool>& visited) {

    if (visited[startNode]) {
        return;
    }

    visited[startNode] = true;

    for (int i = 1; i < graph[startNode].size(); i++) {
        if (graph[startNode][i] != 0) {
            topologicalSortOfDAGNodes(i, sortedNodes, graph, visited);
        }
    }

    sortedNodes.push_back(startNode);
}

void findLongestPathInDAG(const Graph& graph, int startNode, int dest, 
                          std::vector<int>& distances, std::vector<int>& prevs) {
    
    distances[startNode] = 0;

    std::deque<int> sortedNodes;

    std::vector<bool> visited(graph.size(), false);

    for (int i = 1; i < graph.size(); i++) {
        topologicalSortOfDAGNodes(i, sortedNodes, graph, visited);
    }

    while (!sortedNodes.empty()) {
        int node = sortedNodes.back();
        sortedNodes.pop_back();

        for (int i = 1; i < graph[node].size(); i++) {
            // relaxation step
            int weight = graph[node][i];
            if (weight != 0 && distances[i] < distances[node] + weight) {
                distances[i] = distances[node] + weight;
                prevs[i] = node;
            }
        }
    }

}

int main() {
    const auto& [graph, startNode, destNode] = readInput();

    std::vector<int> distances(graph.size(), std::numeric_limits<int>::max() * -1);
    std::vector<int> prevs(graph.size(), -1);

    findLongestPathInDAG(graph, startNode, destNode, distances, prevs);

    std::cout << "Longest path weight = " << distances[destNode] << std::endl;
}
