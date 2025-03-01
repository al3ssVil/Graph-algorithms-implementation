#ifndef GRAPH_H
#define GRAPH_H

#include "edge.h"
#include <vector>
#include <stack>
#include<unordered_set>

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
    void dfsOriginal(Node *node, std::unordered_set<int> &visited, std::stack<Node *> &finishStack);
    void dfsReversed(Node *node, std::unordered_set<int> &visited, std::unordered_set<int> &component);
    std::vector<std::unordered_set<int> > findStronglyConnectedComponents();
private:
    std::vector<Node*> m_nodes;
    std::vector<Edge*> m_edges;
};

#endif // GRAPH_H
