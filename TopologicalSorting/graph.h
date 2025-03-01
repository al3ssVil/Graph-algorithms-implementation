#ifndef GRAPH_H
#define GRAPH_H

#include "edge.h"
#include <vector>
#include<unordered_map>

class Graph
{
public:
    Graph();
    ~Graph();
    void addNode(QPoint pos);
    void addEdge(Node* first, Node* second);
    std::vector<Node*>& getNodes() { return m_nodes; }
    std::vector<Edge*>& getEdges() { return m_edges; }
    bool areNodesOverlapping(Node* node1, Node* node2);
    void matriceDeAdiacenta();
    void saveTopologicalSortToFile(const std::vector<int> &sortedNodes);
    void eliminaArceDupaTopologica(const std::vector<int> &sortedNodes);
    std::vector<int> topologicalSort();
    std::vector<Node *> getNeighbors(Node *node);
    std::unordered_map<Node *, int> computeInDegree();
private:
    std::vector<Node*> m_nodes;
    std::vector<Edge*> m_edges;
    std::unordered_map<int, std::vector<int>> adjList; // Lista de adiacență
};

#endif // GRAPH_H
