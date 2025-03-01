#include "graph.h"
#include <QDebug>
#include <fstream>
#include <cmath>
#include <stack>
#include <iostream>
#include <vector>
#include <unordered_set>

Graph::Graph() {}

Graph::~Graph()
{
    for (auto node : m_nodes) {
        delete node;
    }
    for (auto edge : m_edges) {
        delete edge;
    }
}

void Graph::addNode(QPoint pos)
{
    Node* node = new Node(m_nodes.size(), pos);
    for (auto n : m_nodes)
    {
        if (areNodesOverlapping(n, node))
        {
            qDebug() << "Nodurile" << pos << "și" << n->getPos() << "se suprapun.";
            delete node;
            return;
        }
    }
    m_nodes.push_back(node);
    matriceDeAdiacenta();
}

void Graph::addEdge(Node* first, Node* second)
{
    if (first == second)
    {
        qDebug() << "Nu se poate crea o muchie între același nod!";
        return;
    }
    for (auto edge : m_edges)
    {
        if (edge->getFirst() == first && edge->getSecond() == second)
        {
            qDebug() << "Muchia dintre nodurile" << first->getPos() << "și" << second->getPos() << "există deja.";
            return;
        }
    }

    Edge* edge = new Edge(first, second);
    m_edges.push_back(edge);

    matriceDeAdiacenta();
}

bool Graph::areNodesOverlapping(Node* node1, Node* node2)
{
    QPoint pos1 = node1->getPos();
    QPoint pos2 = node2->getPos();
    double distance = std::sqrt(std::pow(pos2.x() - pos1.x(), 2) + std::pow(pos2.y() - pos1.y(), 2));
    const int radius = 10;
    return distance < 2 * radius;
}

void Graph::matriceDeAdiacenta()
{
    std::ofstream file("matrice.out");
    if (!file.is_open()) {
        qDebug() << "Eroare la deschiderea fișierului pentru scriere.\n";
        return;
    }

    int size = m_nodes.size();

    std::vector<std::vector<int>> matriceAdiacenta(size, std::vector<int>(size, 0));
    for (auto edge : m_edges)
    {
        int primul = edge->getFirst()->getValue();
        int alDoilea = edge->getSecond()->getValue();
        matriceAdiacenta[primul][alDoilea] = 1;
        matriceAdiacenta[alDoilea][primul] = 1;
    }

    file << "Matricea pentru graf neorientat: " << size << std::endl;
    for (const auto& row : matriceAdiacenta)
    {
        for (int j : row)
        {
            file << j << ' ';
        }
        file << std::endl;
    }

    std::vector<std::vector<int>> matriceAdiacenta2(size, std::vector<int>(size, 0));
    for (auto edge : m_edges)
    {
        int primul = edge->getFirst()->getValue();
        int alDoilea = edge->getSecond()->getValue();
        matriceAdiacenta2[primul][alDoilea] = 1;
    }

    file << "Matricea pentru graf orientat: " << size << std::endl;
    for (const auto& row : matriceAdiacenta2)
    {
        for (int j : row)
        {
            file << j << ' ';
        }
        file << std::endl;
    }

    file.close();
}

void Graph::dfsOriginal(Node* node, std::unordered_set<int>& visited, std::stack<Node*>& finishStack)
{
    std::stack<Node*> stiva;
    stiva.push(node);
    visited.insert(node->getValue());

    while (!stiva.empty()) {
        Node* current = stiva.top();
        stiva.pop();

        bool hasUnvisitedNeighbor = false;

        for (Edge* edge : m_edges) {
            Node* neighbor = nullptr;
            if (edge->getFirst() == current) {
                neighbor = edge->getSecond();
            }

            if (neighbor && visited.find(neighbor->getValue()) == visited.end()) {
                stiva.push(neighbor);
                visited.insert(neighbor->getValue());
                hasUnvisitedNeighbor = true;
            }
        }

        if (!hasUnvisitedNeighbor) {
            finishStack.push(current);
        }
    }
}

void Graph::dfsReversed(Node* node, std::unordered_set<int>& visited, std::unordered_set<int>& component)
{
    std::stack<Node*> stiva;
    stiva.push(node);
    visited.insert(node->getValue());
    component.insert(node->getValue());

    while (!stiva.empty()) {
        Node* current = stiva.top();
        stiva.pop();

        for (Edge* edge : m_edges) {
            Node* neighbor = nullptr;
            if (edge->getSecond() == current)
            {
                neighbor = edge->getFirst();
            }

            if (neighbor && visited.find(neighbor->getValue()) == visited.end()) {
                stiva.push(neighbor);
                visited.insert(neighbor->getValue());
                component.insert(neighbor->getValue());
            }
        }
    }
}

std::vector<std::unordered_set<int>> Graph::findStronglyConnectedComponents()
{
    std::unordered_set<int> visited;
    std::stack<Node*> finishStack;
    std::vector<std::unordered_set<int>> components;


    for (Node* node : m_nodes)
    {
        if (visited.find(node->getValue()) == visited.end())
        {
            dfsOriginal(node, visited, finishStack);
        }
    }


    visited.clear();
    while (!finishStack.empty())
    {
        Node* node = finishStack.top();
        finishStack.pop();

        if (visited.find(node->getValue()) == visited.end())
        {
            std::unordered_set<int> component;
            dfsReversed(node, visited, component);
            components.push_back(component);
        }
    }

    return components;
}
