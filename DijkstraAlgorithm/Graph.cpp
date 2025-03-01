#include "Graph.h"

Graph::Graph() {}

void Graph::addNode(int id, long long latitude, long long longitude)
{
    Node newNode = {id, latitude, longitude};
    nodes.append(newNode);
}

void Graph::addEdge(int id1, int id2, int value)
{
    adjList[id1].append(qMakePair(id2, value));
    adjList[id2].append(qMakePair(id1, value)); // Dacă graful este neorientat
}

void Graph::readXML(const QString &fileName, Graph &graph)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Nu s-a putut deschide fișierul!";
        return;
    }

    QXmlStreamReader xml(&file);

    while (!xml.atEnd()) {
        xml.readNext();

        // Căutăm nodurile
        if (xml.isStartElement() && xml.name().toString() == "node") {
            int id = xml.attributes().value("id").toInt();
            long long latitude = xml.attributes().value("latitude").toLongLong();
            long long longitude = xml.attributes().value("longitude").toLongLong();

            //qDebug() << "Nod ID:" << id << "Latitudine:" << latitude << "Longitudine:" << longitude;

            graph.addNode(id, latitude, longitude);
        }
        // Căutăm legături între noduri
        if (xml.isStartElement() && xml.name().toString() == "arc") {
            int from = xml.attributes().value("from").toInt();
            int to = xml.attributes().value("to").toInt();
            int length = xml.attributes().value("length").toInt(); // Valoarea muchiei
            //qDebug() << "Nod ID:" << from << "Latitudine:" << to << "Longitudine:" << length;
            // Adăugăm muchia cu valoare
            addEdge(from, to, length);
        }
    }

    if (xml.hasError()) {
        qWarning() << "Eroare la parsarea fișierului XML: " << xml.errorString();
    }
    file.close();
}

const Node *Graph::getNodeById(int id) const
{
    for (const Node &node : nodes) {
        if (node.id == id) {
            return &node;
        }
    }
    return nullptr;
}

