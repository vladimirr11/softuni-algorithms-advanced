#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <queue>
#include <functional>
#include <vector>
#include <tuple>
#include <numeric>

struct Edge;

using GraphMap = std::unordered_map<int, std::vector<Edge>>;
using PriorityQueue = std::priority_queue<Edge, std::vector<Edge>, std::function<bool(const Edge& e1, const Edge& e2)>>;

struct Edge {
    int from{};
    int to{};
    int weight{};

    Edge() = default;

    Edge(int src, int dest, int inWeight) : 
        from(src), to(dest), weight(inWeight) {}
};

int getNumberFromStream() {
    int n;

    std::string line, trash;
    std::getline(std::cin >> std::ws, line);

    std::istringstream istr(line);

    istr >> trash >> n;

    return n;
}

std::tuple<GraphMap, PriorityQueue, std::vector<bool>> readInput() {
    int numNodes = getNumberFromStream();
    int numEdges = getNumberFromStream();

    GraphMap graph;

    // priority_queue ordered in ascending order
    PriorityQueue pq([](const Edge& e1, const Edge& e2) {
        return e1.weight > e2.weight;
    });

    std::vector<bool> used(numNodes, false);

    for (int i = 0; i < numEdges; i++) {
        std::string line;
        std::getline(std::cin >> std::ws, line);
        std::istringstream istr(line);

        int from{}, to{}, weight{};
        istr >> from >> to >> weight;

        Edge edge(from, to, weight);

        if (graph.find(from) == graph.end()) {
            graph[from].push_back(edge);
        }

        pq.push(edge);
    }

    return std::make_tuple(graph, pq, used);
}

int findRoot(int node, std::vector<int>& parents) {
    int root = parents[node];

    while (parents[node] != root) {
        root = parents[node];
    }

    return root;
}

void findMSForestWeightUsingKruskal(GraphMap& graph, PriorityQueue& pq, std::vector<bool>& used, 
                                    std::vector<int>& parents, std::string& output, int& forestWeight) {
    
    while (!pq.empty()) {
        Edge minEdge = pq.top();
        pq.pop();

        int fromRoot = findRoot(minEdge.from, parents);
        int toRoot = findRoot(minEdge.to, parents);

        if (fromRoot != toRoot) {
            output.append("(").append(std::to_string(minEdge.from)).
            append(" ").append(std::to_string(minEdge.to)).append(") -> ").
            append(std::to_string(minEdge.weight)).append("\n");

            forestWeight += minEdge.weight;
            parents[toRoot] = fromRoot;

            for (int i = 0; i < parents.size(); i++) {
                if (parents[i] == toRoot) {
                    parents[i] = fromRoot;
                }
            }
        }
    }    
}

int main() {
    auto [graph, pq, used] = readInput();

    std::vector<int> parents(used.size(), -1);
    std::iota(parents.begin(), parents.end(), 0);

    std::string output;
    int msForestWeigth{};

    findMSForestWeightUsingKruskal(graph, pq, used, parents, output, msForestWeigth);

    std::cout << "\nMinimum spanning forest weight: " << msForestWeigth << std::endl;

    std::cout << output << std::endl;

    return 0;
}
