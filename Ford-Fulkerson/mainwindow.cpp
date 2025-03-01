#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QInputDialog>
#include <queue>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_ButonOrientat = new QRadioButton("Orientat", this);
    m_ButonNeorientat = new QRadioButton("Neorientat", this);
    m_ButonOrientat->setChecked(true);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(m_ButonOrientat);
    layout->addWidget(m_ButonNeorientat);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
    connect(m_ButonOrientat, &QRadioButton::clicked, this, &MainWindow::onButonOrientatClicked);
    connect(m_ButonNeorientat, &QRadioButton::clicked, this, &MainWindow::onButonNeorientatClicked);

    QPushButton* calculateFlowButton = new QPushButton("Calculează flux maxim", this);
    layout->addWidget(calculateFlowButton);
    connect(calculateFlowButton, &QPushButton::clicked, this, &MainWindow::calculateMaxFlow);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mouseReleaseEvent(QMouseEvent *m)
{

    if (m->button() == Qt::RightButton)
    {
        m_graph.addNode(m->pos());
        update();
    }
    else
        if (m->button() == Qt::LeftButton)
        {
            if (draggingNode)
            {
                draggingNode = false;
                selectedNode = nullptr;
                return;
            }
            std::vector<Node*> nodes = m_graph.getNodes();
            for (auto n:nodes)
            {
                if ((n->getPos().x() - m->pos().x())* (n->getPos().x() - m->pos().x()) +
                        (n->getPos().y() - m->pos().y())* (n->getPos().y() - m->pos().y()) < 100)
                {
                    if (isFirstNodeSelected&& m_selectedNode!=n)
                    {
                        bool ok;
                        int cost = QInputDialog::getInt(this, "Cost Muchie",
                                                        "Introduceți costul muchiei:",
                                                        1, 1, 100, 1, &ok);
                        if (ok)
                        {
                            m_graph.addEdge(m_selectedNode, n, cost);
                        }
                        isFirstNodeSelected = false;
                    }
                    else
                    {
                        isFirstNodeSelected = true;
                        m_selectedNode = n;
                    }
                    update();
                    break;
                }
            }
        }
    if (draggingNode)
    {
        draggingNode = false;
        selectedNode = nullptr;
    }
}

void MainWindow::paintEvent(QPaintEvent *ev)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    std::vector<Node*> nodes = m_graph.getNodes();
    for (auto& n:nodes)
    {
        QRect r(n->getPos().x()-10, n->getPos().y()-10, 20 , 20);
        p.drawEllipse(r);
        QString str = QString::number(n->getValue()+1);
        p.drawText(r, Qt::AlignCenter, str);
    }
    std::vector<Edge*> edges = m_graph.getEdges();
    bool eOrientat = m_ButonOrientat->isChecked();

    if(eOrientat==false)
        for(auto&e:edges)
        {
            QPoint start = e->getFirst()->getPos();
            QPoint end = e->getSecond()->getPos();

            p.setPen(Qt::white);

            p.drawLine(start, end);
        }
    else
        for (auto& e : edges)
        {
            QPoint start = e->getFirst()->getPos();
            QPoint end = e->getSecond()->getPos();

            bool isMinCutEdge = false;
            for (auto& minCutEdge : minCutEdges) {
                if (minCutEdge.first == e->getFirst()->getValue() && minCutEdge.second == e->getSecond()->getValue()) {
                    isMinCutEdge = true;
                    break;
                }
            }
            if (isMinCutEdge) {
                p.setPen(Qt::red);
            } else {
                p.setPen(Qt::white);
            }

            p.drawLine(start, end);

            double angle = std::atan2(end.y() - start.y(), end.x() - start.x());
            int arrowSize = 20;
            QPoint arrowP1 = end - QPoint(arrowSize * std::cos(angle - M_PI / 6), arrowSize * std::sin(angle - M_PI / 6));
            QPoint arrowP2 = end - QPoint(arrowSize * std::cos(angle + M_PI / 6), arrowSize * std::sin(angle + M_PI / 6));
            QPolygon arrowHead;
            arrowHead << end << arrowP1 << arrowP2;
            p.drawPolygon(arrowHead);

            QPoint middle((start.x() + end.x()) / 2, (start.y() + end.y()) / 2);
            QPoint offset(10, 10);
            QPoint textPos = middle + offset;
            p.setPen(Qt::blue);
            QString costText = QString::number(e->getCost());
            p.drawText(textPos, costText);
        }
}

void MainWindow::onButonOrientatClicked()
{
    if (m_ButonOrientat->isChecked())
    {
        qDebug() << "Graful este orientat.";
    }
}

void MainWindow::onButonNeorientatClicked()
{
    if (m_ButonNeorientat->isChecked())
    {
        qDebug() << "Graful este neorientat.";
    }

}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint clickPos = event->pos();
    for (auto& node : m_graph.getNodes())
    {
        if (QRect(node->getPos().x() - 10, node->getPos().y() - 10, 20, 20).contains(clickPos))
        {
            selectedNode = node;
            break;
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if ( selectedNode != nullptr)
    {
        selectedNode->setPos(event->pos());
        update();
        draggingNode=true;
    }
}

int MainWindow::fordFulkerson(int source, int sink, std::vector<std::vector<int>>& residualGraph) {
    int size = m_graph.getNodes().size();
    std::vector<int> parent(size, -1);
    int maxFlow = 0;

    while (bfs(residualGraph, source, sink, parent)) {
        int pathFlow = std::numeric_limits<int>::max();
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            pathFlow = std::min(pathFlow, residualGraph[u][v]);
        }

        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            residualGraph[u][v] -= pathFlow;
            residualGraph[v][u] += pathFlow;
        }

        maxFlow += pathFlow;
    }

    return maxFlow;
}

bool MainWindow::bfs(const std::vector<std::vector<int>>& residualGraph, int source, int sink, std::vector<int>& parent) {
    int size = m_graph.getNodes().size();
    std::vector<bool> visited(size, false);
    std::queue<int> q;
    q.push(source);
    visited[source] = true;
    parent[source] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < size; ++v) {
            if (!visited[v] && residualGraph[u][v] > 0) {
                q.push(v);
                parent[v] = u;
                visited[v] = true;
                if (v == sink) {
                    return true;
                }
            }
        }
    }

    return false;
}

void MainWindow::calculateMaxFlow() {
    bool ok;
    int source = QInputDialog::getInt(this, "Introducere nod sursă",
                                      "Introduceți ID-ul nodului sursă (1-based):",
                                      1, 1, m_graph.getNodes().size(), 1, &ok);
    if (!ok) return;

    int sink = QInputDialog::getInt(this, "Introducere nod destinație",
                                    "Introduceți ID-ul nodului destinație (1-based):",
                                    1, 1, m_graph.getNodes().size(), 1, &ok);
    if (!ok) return;

    source -= 1;
    sink -= 1;

    if (source < 0 || source >= m_graph.getNodes().size() || sink < 0 || sink >= m_graph.getNodes().size()) {
        QMessageBox::warning(this, "Eroare", "ID-urile nodurilor sunt invalide.");
        return;
    }
    int size = m_graph.getNodes().size();
    std::vector<std::vector<int>> residualGraph(size, std::vector<int>(size, 0));

    for (auto edge : m_graph.getEdges()) {
        int u = edge->getFirst()->getValue();
        int v = edge->getSecond()->getValue();
        residualGraph[u][v] = edge->getCost();
        if (!m_ButonOrientat->isChecked()) {
            residualGraph[v][u] = edge->getCost();
        }
    }
    int maxFlow = fordFulkerson(source, sink, residualGraph);

    QMessageBox::information(this, "Flux maxim", QString("Fluxul maxim între nodurile %1 și %2 este: %3")
                                                     .arg(source + 1).arg(sink + 1).arg(maxFlow));
    for (int u = 0; u < size; ++u) {
        for (int v = 0; v < size; ++v) {
            qDebug() << "Residual graph[" << u << "][" << v << "]: " << residualGraph[u][v];
        }
    }
    findMinCut(residualGraph, source);
    update();
}

void MainWindow::findMinCut(std::vector<std::vector<int>>& residualGraph, int source) {
    int size = m_graph.getNodes().size();
    std::vector<bool> visited(size, false);

    std::queue<int> q;
    q.push(source);
    visited[source] = true;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        qDebug() << "Nod accesibil: " << u + 1;

        for (int v = 0; v < size; ++v) {
            if (!visited[v] && residualGraph[u][v] > 0) {
                visited[v] = true;
                q.push(v);
            }
        }
    }

    minCutEdges.clear();
    for (int u = 0; u < size; ++u) {
        if (visited[u]) {
            for (int v = 0; v < size; ++v) {
                if (!visited[v] && residualGraph[v][u] > 0) {
                    minCutEdges.push_back({u, v});
                    qDebug() << "Tăietura minimă: (" << u + 1 << ", " << v + 1 << ")";
                }
            }
        }
    }
    qDebug() << "Numărul de muchii din tăietura minimă: " << minCutEdges.size();
}
