#include "graph.h"
#include <QDebug>
#include <fstream>
#include <cmath>
#include <stack>
#include <iostream>
#include <vector>
#include <utility>
#include <queue>

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


std::vector<Node*> Graph::getNeighbors(Node* node)
{
    std::vector<Node*> neighbors;

    for (auto edge : m_edges)
    {
        if (edge->getFirst() == node) {
            neighbors.push_back(edge->getSecond());
        }
        else if (edge->getSecond() == node) {
            neighbors.push_back(edge->getFirst());
        }
    }

    return neighbors;
}

std::unordered_map<Node*, int> Graph:: computeInDegree() {
    std::unordered_map<Node*, int> inDegree;


    for (auto& node : m_nodes) {
        inDegree[node] = 0;
    }

    for (auto& edge : m_edges) {
        inDegree[edge->getSecond()]++;
    }

    return inDegree;
}

std::vector<int> Graph:: topologicalSort() {
    std::unordered_map<Node*, int> inDegree = computeInDegree();
    std::stack<Node*> q;
    std::vector<int> result;

    for (auto& node : m_nodes) {
        if (inDegree[node] == 0) {
            q.push(node);
        }
    }

    while (!q.empty()) {
        Node* current = q.top();
        q.pop();
        result.push_back(current->getValue());

        for (auto& edge : m_edges) {
            if (edge->getFirst() == current) {
                Node* neighbor = edge->getSecond();
                inDegree[neighbor]--;
                if (inDegree[neighbor] == 0) {
                    q.push(neighbor);
                }
            }
        }
    }
    if (result.size() != m_nodes.size()) {
        std::cout << "Graful conține un ciclu!" << std::endl;
        return {};
    }

    return result;
}

void Graph::saveTopologicalSortToFile(const std::vector<int>& sortedNodes)
{
    std::ofstream file("topological_sort.txt");
    if (!file.is_open()) {
        std::cout << "Eroare la deschiderea fișierului pentru salvare." << std::endl;
        return;
    }

    file << "Sortare topologică: ";
    for (int value : sortedNodes)
    {
        file << value + 1 << " ";
    }
    file << std::endl;
    file.close();
}

void Graph::eliminaArceDupaTopologica(const std::vector<int>& sortedNodes)
{
    std::vector<Edge*> validEdges;

    for (size_t i = 0; i < sortedNodes.size() - 1; ++i) {
        int currentNodeValue = sortedNodes[i];
        int nextNodeValue = sortedNodes[i + 1];

        Node* currentNode = m_nodes[currentNodeValue];
        Node* nextNode = m_nodes[nextNodeValue];

        bool edgeExists = false;
        for (auto& edge : m_edges) {
            if (edge->getFirst() == currentNode && edge->getSecond() == nextNode) {
                edgeExists = true;
                break;
            }
        }

        if (edgeExists) {
            for (auto& edge : m_edges) {
                if (edge->getFirst() == currentNode && edge->getSecond() == nextNode)
                {
                    validEdges.push_back(edge);
                    break;
                }
            }
        }
    }

    m_edges = validEdges;

}
