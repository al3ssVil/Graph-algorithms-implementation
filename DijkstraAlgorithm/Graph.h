#ifndef GRAPH_H
#define GRAPH_H

#include <QXmlStreamReader>
#include <QFile>
#include <QMap>
#include <QVector>
#include <QDebug>

struct Node{
    int id;
    long long latitude;
    long long longitude;
};

class Graph
{
public:
    Graph();
    QVector<Node> nodes;
    QMap<int, QVector<QPair<int, int>>> adjList;

    void addNode(int id, long long latitude, long long longitude);
    void addEdge(int id1, int id2, int value);
    void readXML(const QString &fileName, Graph &graph);
    const Node* getNodeById(int id) const;
};

#endif // GRAPH_H
