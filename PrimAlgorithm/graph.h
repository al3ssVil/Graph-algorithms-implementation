#ifndef GRAPH_H
#define GRAPH_H

#include "edge.h"
#include <vector>

class Graph
{
public:
    Graph();
    ~Graph();
    void addNode(QPoint pos);
    void addEdge(Node* first, Node* second, int cost);
    std::vector<Node*>& getNodes() { return m_nodes; }
    std::vector<Edge*>& getEdges() { return m_edges; }
    bool areNodesOverlapping(Node* node1, Node* node2);
    void matriceDeAdiacenta();
    std::vector<std::vector<int> > &getMatriceAdiacenta();
private:
    std::vector<Node*> m_nodes;
    std::vector<Edge*> m_edges;
    std::vector<std::vector<int>> m_matriceAdiacenta;
};

#endif // GRAPH_H
