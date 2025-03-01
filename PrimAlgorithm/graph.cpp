#include "graph.h"
#include <QDebug>
#include <fstream>
#include <cmath>
#include <iostream>
#include <vector>

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

void Graph::addEdge(Node* first, Node* second, int cost)
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
        else
            if(first==second)
            {
                qDebug() << "Muchia dintre nodurile" << first->getPos() << "și" << second->getPos() << "nu se poate crea";
                return;
            }
    }

    Edge* edge = new Edge(first, second,cost);
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
        matriceAdiacenta[primul][alDoilea] = edge->getCost();
        matriceAdiacenta[alDoilea][primul] = edge->getCost();
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
        matriceAdiacenta2[primul][alDoilea] =  edge->getCost();
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
    m_matriceAdiacenta=matriceAdiacenta;
    file.close();
}

std::vector<std::vector<int>>& Graph::getMatriceAdiacenta() {
    return m_matriceAdiacenta;
}
