#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <numeric>
#include <tuple>
#include <queue>
#include <algorithm>

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

void sortDAGTopologicallyUsingDfs(int startNode, std::deque<int>& nodesToSort, 
                                  const Graph& graph, std::vector<bool>& visisted) {
    
    if (visisted[startNode]) {
        return;
    }

    visisted[startNode] = true;

    for (int i = 0; i < graph[startNode].size(); i++) {
        if (graph[startNode][i] != 0) {
            sortDAGTopologicallyUsingDfs(i, nodesToSort, graph, visisted);
        }
    }

    nodesToSort.push_back(startNode);
} 

void findLonegstPathInDAG(const Graph& graph, int startNode, int dest, 
                          std::vector<int>& distances, std::vector<int>& prevs) {
    
    distances[startNode] = 0;

    std::deque<int> tSortedNodes;
    std::vector<bool> visited(graph.size(), false);

    for (int i = 0; i < graph.size(); i++) {
        sortDAGTopologicallyUsingDfs(i, tSortedNodes, graph, visited);
    }

    while (!tSortedNodes.empty()) {
        int node = tSortedNodes.back();
        tSortedNodes.pop_back();

        for (int i = 0; i < graph[node].size(); i++) {
            int weight = graph[node][i];
            if (weight != 0 && distances[i] < distances[node] + weight) {
                distances[i] = distances[node] + weight;
                prevs[i] = node;
            }
        }
    }
}

std::vector<int> constructPathFromPrevs(int dest, std::vector<int>& prevs) {
    std::vector<int> path;
    
    path.push_back(dest);
    
    int node = prevs[dest];
    while (node != -1) {
        path.push_back(node);
        node = prevs[node];
    }

    return path;
}

void printPath(std::vector<int>& path) {
    for (const auto node : path) {
        std::cout << node << " ";
    }
    std::cout << std::endl;
}

int main() {
    const auto& [graph, startNode, destNode] = readInput();

    std::vector<int> distances(graph.size(), std::numeric_limits<int>::max() * -1);
    std::vector<int> prevs(graph.size(), -1);

    findLonegstPathInDAG(graph, startNode, destNode, distances, prevs);

    std::vector<int> path = constructPathFromPrevs(destNode, prevs);

    std::reverse(path.begin(), path.end());

    printPath(path);

    std::cout << distances[destNode] << std::endl;

    return 0;
}
