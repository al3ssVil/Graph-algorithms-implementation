#ifndef EDGE_H
#define EDGE_H

#include "node.h"

class Edge
{
public:
    Edge();
    Edge(Node* first, Node* second, int cost):m_first(first), m_second(second), m_cost(cost){}
    Node* getFirst(){return m_first;}
    Node* getSecond(){return m_second;}
    int getCost() const { return m_cost; }
    void setCost(int cost) { m_cost = cost; }
    bool operator==(const Edge& other) const {
        return (m_first->getValue() == other.m_first->getValue() && m_second->getValue() == other.m_second->getValue()) ||
               (m_first->getValue() ==other.m_second->getValue() && m_second->getValue() == other.m_first->getValue());
    }
private:
    Node* m_first,* m_second;
    int m_cost;
};

#endif // EDGE_H
