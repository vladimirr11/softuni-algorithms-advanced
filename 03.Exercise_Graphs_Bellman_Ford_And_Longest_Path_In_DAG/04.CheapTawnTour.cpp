#include <iostream>
#include <string>
#include <sstream>
#include <numeric>
#include <queue>
#include <functional>
#include <vector>
#include <tuple>

struct Edge;

using PriorityQueue = std::priority_queue<Edge, std::vector<Edge>, std::function<bool(const Edge& e1, const Edge& e2)>>;

struct Edge {
    int from{};
    int to{};
    int weight{};

    Edge() = default;

    Edge(int src, int dest, int inWeight) : 
        from(src), to(dest), weight(inWeight) {}
};

std::tuple<PriorityQueue, int> readInput() {
    int numNodes{}, numEdges{};

    PriorityQueue pq([](const Edge& e1, const Edge& e2) {
        return e1.weight > e2.weight;
    });

    std::cin >> numNodes >> numEdges;

    for (int i = 0; i < numEdges; i++) {
        std::string line;
        std::getline(std::cin >> std::ws, line);
        std::istringstream istr(line);
        
        char d1, d2;
        int from{}, to{}, weight{};
        istr >> from >> d1 >> to >> d2 >> weight;
 
        Edge edge(from, to, weight);

        pq.push(edge);
    }

    return std::make_tuple(pq, numNodes);
}

int findRoot(int node, std::vector<int>& parents) {
    int root = parents[node];

    while (root != parents[node]) {
        root = parents[root];
    }

    return root;
}

void findCheapestMSForestUsingKruskal(PriorityQueue& pq, std::vector<int>& parents, int& forestWeight) {
    
    while (!pq.empty()) {
        Edge minEdge = pq.top();
        pq.pop();

    	int srcRoot = findRoot(minEdge.from, parents);
        int destRoot = findRoot(minEdge.to, parents);

        if (srcRoot != destRoot) {
            forestWeight += minEdge.weight;
            parents[destRoot] = srcRoot;

            for (int i = 0; i < parents.size(); i++) {
                if (parents[i] == destRoot) {
                    parents[i] = srcRoot;
                }
            }
        }
    }
}

int main() {
    auto [pq, numNodes] = readInput();

    std::vector<int> parents(numNodes, -1);
    std::iota(parents.begin(), parents.end(), 0);

    int forestWeight{};

    findCheapestMSForestUsingKruskal(pq, parents, forestWeight);

    std::cout << forestWeight << std::endl;

    return 0;
}
