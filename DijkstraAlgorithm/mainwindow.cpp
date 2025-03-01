#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(900,750);
    graph.readXML("Harta_Luxemburg.xml", graph);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Desenăm fiecare nod (nodurile rămân albe/roșii în funcție de selecție)
    for (const Node &node : graph.nodes) {
        QPointF point = scaleCoordinates(node.latitude, node.longitude);

        if (node.id == selectedNode1 || node.id == selectedNode2) {
            painter.setBrush(Qt::red); // Noduri selectate -> Roșu
        } else {
            painter.setBrush(Qt::white); // Noduri normale -> Alb
        }
        painter.drawEllipse(point, 2, 2); // Cercuri pentru noduri (raza 5px)
    }

    // Calculăm cel mai scurt drum doar dacă avem două noduri selectate
    QVector<int> shortestPath;
    if (selectedNode1 != -1 && selectedNode2 != -1) {
        shortestPath = dijkstra(selectedNode1, selectedNode2);
    }

    // Desenăm toate muchiile din graf
    for (auto it = graph.adjList.constBegin(); it != graph.adjList.constEnd(); ++it) {
        int nodeId = it.key();
        const QVector<QPair<int, int>> &neighbors = it.value();

        for (const auto &neighbor : neighbors) {
            int neighborId = neighbor.first;

            // Nodurile conectate prin muchie
            const Node *node1 = graph.getNodeById(nodeId);
            const Node *node2 = graph.getNodeById(neighborId);

            if (node1 && node2) {
                QPointF point1 = scaleCoordinates(node1->latitude, node1->longitude);
                QPointF point2 = scaleCoordinates(node2->latitude, node2->longitude);

                // Verificăm dacă muchia este parte din cel mai scurt drum
                if (!shortestPath.isEmpty()) {
                    int idx1 = shortestPath.indexOf(nodeId);
                    int idx2 = shortestPath.indexOf(neighborId);

                    if (idx1 != -1 && idx2 != -1 && std::abs(idx1 - idx2) == 1) {
                        // Dacă nodurile sunt consecutive în shortestPath
                        painter.setPen(QPen(Qt::red, 2)); // Muchie parte din cel mai scurt drum
                    } else {
                        painter.setPen(QPen(Qt::blue, 1)); // Muchie normală
                    }
                } else {
                    painter.setPen(QPen(Qt::blue, 1)); // Muchie normală
                }

                // Desenăm linia reprezentând muchia
                painter.drawLine(point1, point2);
            }
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPointF clickPoint = event->pos(); // Coordonatele punctului unde s-a dat click

    double minDistance = std::numeric_limits<double>::max();
    int closestNodeId = -1;

    // Parcurgem nodurile pentru a găsi cel mai apropiat de clickPoint
    for (const Node &node : graph.nodes) {
        QPointF nodePoint = scaleCoordinates(node.latitude, node.longitude);
        double distance = std::hypot(nodePoint.x() - clickPoint.x(), nodePoint.y() - clickPoint.y());

        if (distance < minDistance && distance < 10) { // Threshold pentru selecție (10px)
            minDistance = distance;
            closestNodeId = node.id;
        }
    }

    if (closestNodeId != -1) { // Dacă am găsit un nod în apropiere
        if (selectedNode1 == -1) {
            selectedNode1 = closestNodeId; // Setăm primul nod selectat
        } else if (selectedNode2 == -1) {
            selectedNode2 = closestNodeId; // Setăm al doilea nod selectat
        } else {
            // Resetăm selecția dacă sunt deja două noduri selectate
            selectedNode1 = closestNodeId;
            selectedNode2 = -1;
        }
        update(); // Re-desenăm pentru a reflecta selecția
    }
}

QVector<int> MainWindow::dijkstra(int startId, int endId)
{
    // Distantele minime de la nodul de start
    QMap<int, int> dist;
    QMap<int, int> prev; // Pentru a reconstrui drumul
    QSet<int> visited;

    // Initializare
    for (const Node &node : graph.nodes) {
        dist[node.id] = std::numeric_limits<int>::max();
    }
    dist[startId] = 0;

    // Coada de priorități
    auto compare = [&dist](int n1, int n2) { return dist[n1] > dist[n2]; };
    std::priority_queue<int, QVector<int>, decltype(compare)> pq(compare);
    pq.push(startId);

    // Algoritmul Dijkstra
    while (!pq.empty()) {
        int currentNode = pq.top();
        pq.pop();

        if (visited.contains(currentNode)) {
            continue;
        }
        visited.insert(currentNode);

        // Parcurgem vecinii
        for (const auto &neighbor : graph.adjList[currentNode]) {
            int neighborId = neighbor.first;
            int weight = neighbor.second;

            if (dist[currentNode] + weight < dist[neighborId]) {
                dist[neighborId] = dist[currentNode] + weight;
                prev[neighborId] = currentNode;
                pq.push(neighborId);
            }
        }
    }

    // Reconstruim drumul de la endId la startId
    QVector<int> path;
    for (int at = endId; at != 0; at = prev.value(at, 0)) {
        path.prepend(at);
    }

    // Dacă nodul final nu este accesibil
    if (path.isEmpty() || path.first() != startId) {
        return {};
    }
    return path;
}

QPointF MainWindow::scaleCoordinates(long long latitude, long long longitude)
{

        // Conversia latitudinii și longitudinii în coordonate decimale
    double newLat = static_cast<double>(latitude) / 100000.0;
    double newLon = static_cast<double>(longitude) / 100000.0;

    //qDebug()<<newLat<<' '<<newLon;

    // Definirea limitelor coordonatelor pentru scalare
    double minLat = 49.44;
    double maxLat = 50.17;
    double minLon = 5.82;
    double maxLon = 6.53;

    // Obținem dimensiunile ferestrei
    double width = 700;   // Lățimea ferestrei
    double height = 750;  // Înălțimea ferestrei

    // Scalăm coordonatele pentru a le adapta în fereastră
    double x = (newLon - minLon) / (maxLon - minLon) * height+100;
    double y = (maxLat - newLat) / (maxLat - minLat) * width+50;  // Inversăm latitudinea pentru a o plasa corect pe fereastră

    //qDebug() << "Coordonate scalate: (" << x << "," << y << ")";

    // Returnăm coordonatele scalate
    return QPointF(x, y);
}
