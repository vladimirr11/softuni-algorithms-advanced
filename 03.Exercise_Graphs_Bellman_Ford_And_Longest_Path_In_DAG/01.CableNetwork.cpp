#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <queue>
#include <functional>
#include <vector>
#include <tuple>

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

    bool operator>(const Edge& other) {
        return weight > other.weight;
    }
};

int getNumberFromStream() {
    int n;

    std::string line, trash;
    std::getline(std::cin >> std::ws, line);

    std::istringstream istr(line);

    istr >> trash >> n;

    return n;
}

std::tuple<GraphMap, PriorityQueue, std::vector<bool>, int> readInput() {
    int budgest = getNumberFromStream();
    int numNodes = getNumberFromStream();
    int numEdges = getNumberFromStream();

    GraphMap graph;
    PriorityQueue pq([](const Edge& e1, const Edge& e2) {
        return e1.weight > e2.weight;
    });

    std::vector<bool> used(numNodes, false);

    for (int i = 0; i < numEdges; i++) {
        std::string line;
        std::getline(std::cin >> std::ws, line);
        std::istringstream istr(line);
        
        std::string currToken;
        
        int counter{};
        bool connected = false;
        std::vector<int> edgeTokens;
        while (istr >> currToken) {
            counter++;
            if (counter == 4) {
                connected = true;
                break;
            }
            edgeTokens.push_back(stoi(currToken));
        }

        int from = edgeTokens[0];
        int to = edgeTokens[1];
        int weight = edgeTokens[2];

        if (connected) {
            used[from] = used[to] = true;
        }

        Edge edge(from, to, weight);

        if (graph.find(from) == graph.end()) {
            graph[from].push_back(edge);
        }

        pq.push(edge);
    }

    return std::make_tuple(graph, pq, used, budgest);
}

void findMSForesCosttUsingPrim(GraphMap& graph, PriorityQueue& pqEdges, std::vector<bool>& used, int budget, int& cost) {

    while (!pqEdges.empty()) {
        Edge minEdge = pqEdges.top();
        pqEdges.pop();

        int removedValue = -1;

        if (used[minEdge.from] && !used[minEdge.to]) {
            used[minEdge.to] = true;
            removedValue = minEdge.weight;
        } else if (!used[minEdge.from] && used[minEdge.to]) {
            used[minEdge.from] = true;  
            removedValue = minEdge.weight;
        }

        if (removedValue != -1 && budget - removedValue > 0) {
            budget -= removedValue;
            cost += removedValue;
        } else if (budget - removedValue < 0) {
            return;
        }
    }

}

int main() {
    auto [graphMap, pq, used, budget] = readInput();

    int cost{};
    findMSForesCosttUsingPrim(graphMap, pq, used, budget, cost);

    std::cout << "Used budget: " << cost << std::endl;

    return 0;
}
