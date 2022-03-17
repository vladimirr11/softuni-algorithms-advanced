#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <queue>
#include <functional>
#include <vector>
#include <tuple>
#include <stack>
#include <numeric>

struct Edge;

using Graph = std::vector<std::vector<int>>;
using PriorityQueue = std::priority_queue<int, std::vector<int>, std::function<bool(double, double)>>;


int getNumberFromStream() {
    int n;

    std::string line, trash;
    std::getline(std::cin >> std::ws, line);

    std::istringstream istr(line);

    istr >> trash >> n;

    return n;
}

std::pair<int, int> getStartAndEndNodes() {
    int startNode{}, endNode{};

    std::string line;
    std::getline(std::cin >> std::ws, line);
    std::istringstream istr(line);

    std::string path, src, dash, dest;
    istr >> path >> src >> dash >> dest;

    startNode = stoi(src);
    endNode = stoi(dest);

    return std::make_pair(startNode, endNode);
}

std::tuple<Graph, int, int, int> readInput() {
    int numNodes = getNumberFromStream();
    auto [startNode, endNode] = getStartAndEndNodes();
    int numEdges = getNumberFromStream();

    Graph graph(numNodes, std::vector<int>(numNodes, 0));

    for (int i = 0; i < numEdges; i++) {
        std::string line;
        std::getline(std::cin >> std::ws, line);
        std::istringstream istr(line);

        int from{}, to{}, weight{};
        istr >> from >> to >> weight;

        graph[from][to] = weight;
        graph[to][from] = weight;
    }

    return std::make_tuple(graph, startNode, endNode, numNodes);
}

std::vector<int> findMostRiablePathInGraphUsingDijkstra(const Graph& graph, int startNode, int endNode, int numNodes) {
    std::vector<double> distances(numNodes, 0.0);
    std::vector<bool> visited(numNodes, false);
    std::vector<int> prevs(numNodes, -1);

    distances[startNode] = 100.00;

    PriorityQueue pq([&distances](double first, double second) {
        return distances[first] < distances[second];
    });

    pq.push(startNode);

    while (!pq.empty()) {
        int minNode = pq.top();
        pq.pop();

        visited[minNode] = true;

        for (int i = 0; i < graph[minNode].size(); i++) {
            int weight = graph[minNode][i];
            if (weight != 0 && !visited[i]) {
                double newDistance = distances[minNode] * weight / 100.00;

                if (newDistance > distances[i]) {
                    distances[i] = newDistance;
                    prevs[i] = minNode;
                }

                pq.push(i);
            }
        }
    }

    std::cout << "Most reliable path reliability: " << std::fixed 
        << std::setprecision(2) << distances[endNode] << "%" << std::endl;

    return prevs;
}

std::stack<int> constructPathFromPrevs(std::vector<int>& prevs, int endNode) {
    std::stack<int> path;
    path.push(endNode);

    int node = prevs[endNode];
    
    while (node != -1) {
        path.push(node);
        node = prevs[node];
    }

    return path;
}

void printPath(std::stack<int>& path) {
    while (!path.empty()) {
        int node = path.top();
        path.pop();
        std::cout << node;
        if (path.size() > 0) {
            std::cout << " -> ";
        }
    }
}

int main() {
    const auto& [graph, startNode, endNode, numNodes] = readInput();

    std::vector<int> prevs = findMostRiablePathInGraphUsingDijkstra(graph, startNode, endNode, numNodes);

    std::stack<int> path = constructPathFromPrevs(prevs, endNode);

    printPath(path);

    return 0;
}
