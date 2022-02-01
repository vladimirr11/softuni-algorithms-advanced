#include <iostream>
#include <vector>
#include <queue>
#include <numeric>
#include <functional>

using AdjMatrix = std::vector<std::vector<int>>;
using PriorityQueue = std::priority_queue<int, std::vector<int>, std::function<bool(int, int)>>;

std::vector<int> constructPathFromPrevIndices(std::vector<int>& prevs, int dest) {
    std::vector<int> path;
    
    path.push_back(dest);

    int parent = prevs[dest];
    while (parent != -1) {
        path.push_back(parent);
        parent = prevs[parent];
    }

    return path;
}

std::vector<int> findShortestPathUsingDijkstra(AdjMatrix& graph, int startNode, int destNode) {
    // helper containers
    std::vector<int> distances(graph.size(), std::numeric_limits<int>::max());
    std::vector<bool> visited(graph.size(), false);
    std::vector<int> prevs(graph.size(), -1);

    distances[startNode] = 0;

    PriorityQueue prQueue([&distances](int node1, int node2) { 
        return distances[node1] > distances[node2]; 
    });

    prQueue.push(startNode);

    while (!prQueue.empty()) {
        int minParentNode = prQueue.top();
        prQueue.pop();

        visited[minParentNode] = true;
        
        auto children = graph[minParentNode];
        for (int child = 0; child < children.size(); child++) {
            if (children[child] != 0 && !visited[child]) {
                prQueue.push(child);

                int newDistance = distances[minParentNode] + graph[minParentNode][child];

                if (newDistance < distances[child]) {
                    distances[child] = newDistance;
                    prevs[child] = minParentNode;
                }
            }
        }
    }

    std::vector<int> shortestPathInReverse = constructPathFromPrevIndices(prevs, destNode);

    std::reverse(shortestPathInReverse.begin(), shortestPathInReverse.end());
    
    return shortestPathInReverse;
}   

void print(std::vector<int>& path) {
    for (const int node : path) {
        std::cout << node << " ";
    }
    std::cout << std::endl;
}

int main() {
    AdjMatrix graph = {
        // 0   1   2   3   4   5   6   7   8   9  10  11
        { 0,  0,  0,  0,  0,  0, 10,  0, 12,  0,  0,  0 }, // 0
        { 0,  0,  0,  0, 20,  0,  0, 26,  0,  5,  0,  6 }, // 1
        { 0,  0,  0,  0,  0,  0,  0, 15, 14,  0,  0,  9 }, // 2
        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  0 }, // 3
        { 0, 20,  0,  0,  0,  5, 17,  0,  0,  0,  0, 11 }, // 4
        { 0,  0,  0,  0,  5,  0,  6,  0,  3,  0,  0, 33 }, // 5
        {10,  0,  0,  0, 17,  6,  0,  0,  0,  0,  0,  0 }, // 6
        { 0, 26, 15,  0,  0,  0,  0,  0,  0,  3,  0, 20 }, // 7
        {12,  0, 14,  0,  0,  3,  0,  0,  0,  0,  0,  0 }, // 8
        { 0,  5,  0,  0,  0,  0,  0,  3,  0,  0,  0,  0 }, // 9
        { 0,  0,  0,  7,  0,  0,  0,  0,  0,  0,  0,  0 }, // 10
        { 0,  6,  9,  0, 11, 33,  0, 20,  0,  0,  0,  0 }, // 11
    };

    int startNode = 0, destNode = 9;

    std::vector<int> path = findShortestPathUsingDijkstra(graph, startNode, destNode);

    print(path);

    return 0;
}
