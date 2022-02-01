#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <algorithm>

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

    friend bool operator>(const Edge& first, const Edge& second);

    friend std::ostream& operator<<(std::ostream& out, const Edge& edge);

private:
    int _src{}, _dest{}, _weight{};
};

bool operator>(const Edge& first, const Edge& second) {
    if (first._weight == second._weight) {
        return first._dest > second._dest;
    }
    return first._weight > second._weight;
}

std::ostream& operator<<(std::ostream& out, const Edge& edge) {
    out << "(" << edge.getSource() << ", " << edge.getDest() << ") -> " << edge.getWeight();
    return out;
}

int findRoot(int node, std::vector<int>& parents) {    
    while (node != parents[node]) {
        node = parents[node];
    }

    return node;
}

std::vector<Edge> findMSTUsingKruskal(std::vector<Edge>& edges, int numVertices) {
    // keeps edges ordered by smallest weight
    std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> orderedEdges(edges.begin(), edges.end()); 

    // helper containers
    std::vector<Edge> msForest;
    std::vector<int> parents(numVertices, 0);

    for (int i = 0; i < parents.size(); i++) {
        parents[i] = i;
    }

    while (!orderedEdges.empty()) {
        Edge currentMinEdge = orderedEdges.top();
        orderedEdges.pop();

        int src = currentMinEdge.getSource();
        int dest = currentMinEdge.getDest();

        int srcRoot = findRoot(src, parents);
        int destRoot = findRoot(dest, parents);

        if (srcRoot != destRoot) {
            msForest.push_back(currentMinEdge);
            parents[destRoot] = srcRoot;
        }
    }

    return msForest;
}

void findMSTUsingPrim(int startNode, std::set<int>& graphNodes, std::map<int, std::vector<Edge>>& edgesByNode, 
                      std::set<int>& visited, std::vector<Edge>& mstPrim) {

    visited.insert(startNode);

    std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> 
                        edges(edgesByNode[startNode].begin(), edgesByNode[startNode].end());

    while (!edges.empty()) {
        Edge minEdge = edges.top();
        edges.pop();

        int srcNode = minEdge.getSource();
        int destNode = minEdge.getDest();

        int nonTreeNode = -1;
        if (visited.find(srcNode) != visited.end() && visited.find(destNode) == visited.end()) {
            nonTreeNode = destNode;
        
        }
        if (visited.find(destNode) != visited.end() && visited.find(srcNode) == visited.end()) {
            nonTreeNode = srcNode;
        }
        
        if (nonTreeNode != -1) {
            mstPrim.push_back(minEdge);
            visited.insert(nonTreeNode);
            for (auto it = edgesByNode[nonTreeNode].begin(); it != edgesByNode[nonTreeNode].end(); it++) {
                edges.push(*it);
            }
        }
    }
}

void printEdges(std::vector<Edge>& edges) {
    for (const auto& edge : edges) {
        std::cout << edge << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::vector<Edge> graphEdges;
    
    graphEdges.emplace_back(0, 3, 9);
    graphEdges.emplace_back(0, 5, 4);
    graphEdges.emplace_back(0, 8, 5);
    graphEdges.emplace_back(1, 4, 8);
    graphEdges.emplace_back(1, 7, 7);
    graphEdges.emplace_back(2, 6, 12);
    graphEdges.emplace_back(3, 5, 2);
    graphEdges.emplace_back(3, 6, 8);
    graphEdges.emplace_back(3, 8, 20);
    graphEdges.emplace_back(4, 7, 10);
    graphEdges.emplace_back(6, 8, 7);

    std::vector<Edge> expectedEdges = { 
        graphEdges.at(6),
        graphEdges.at(1),
        graphEdges.at(2),
        graphEdges.at(4),
        graphEdges.at(10),
        graphEdges.at(3),
        graphEdges.at(5)
    };

    // kruskal
    int numVertices = 9;
    std::vector<Edge> mstKruskal = findMSTUsingKruskal(graphEdges, numVertices);

    printEdges(expectedEdges);
    printEdges(mstKruskal);

    // prim
    std::set<int> graphNodes;
    std::map<int, std::vector<Edge>> edgesByNode;

    for (const auto& edge : graphEdges) {
        graphNodes.insert(edge.getSource());
        graphNodes.insert(edge.getDest());

        edgesByNode[edge.getSource()].push_back(edge);
        edgesByNode[edge.getDest()].push_back(edge);
    }

    std::vector<Edge> primForest;
    std::set<int> visited;

    // run prim for each node
    for (auto it = graphNodes.begin(); it != graphNodes.end(); it++) {
        if (visited.find(*it) == visited.end()) {
            findMSTUsingPrim(*it, graphNodes, edgesByNode, visited, primForest);
        }
    }

    // sort forest by weight
    std::sort(primForest.begin(), primForest.end(), [](const Edge& e1, const Edge& e2) {
        return e1.getWeight() < e2.getWeight();
    });

    printEdges(primForest);

    return 0;
}
