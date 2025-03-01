#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QInputDialog>
#include <set>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_ButonOrientat = new QRadioButton("Orientat", this);
    m_ButonNeorientat = new QRadioButton("Neorientat", this);
    m_Kruskal = new QPushButton("Kruskal Algoritm", this);
    m_ButonNeorientat->setChecked(true);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(m_ButonOrientat);
    layout->addWidget(m_ButonNeorientat);
    layout->addWidget(m_Kruskal);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
    connect(m_ButonOrientat, &QRadioButton::clicked, this, &MainWindow::onButonOrientatClicked);
    connect(m_ButonNeorientat, &QRadioButton::clicked, this, &MainWindow::onButonNeorientatClicked);
    connect(m_Kruskal, &QPushButton::clicked, this, &MainWindow::onButtonKruskalClicked);
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
    std::vector<Edge*> a;
    std::set<Edge*> setA;

    if(Kruskal)
    {
        a=kruskalAlgorithm();
        setA=std::set<Edge*>(a.begin(),a.end());
    }
    if(eOrientat==false)
        for(auto&e:edges)
        {
            QPoint start = e->getFirst()->getPos();
            QPoint end = e->getSecond()->getPos();

            bool isEdgeInMST = false;
            for (auto m : setA) {
                if (*m == *e) {
                    isEdgeInMST = true;
                    break;
                }
            }

            if (isEdgeInMST)
                    p.setPen(Qt::red);
             else
               p.setPen(Qt::white);


            p.drawLine(start, end);


            QPoint middle((start.x() + end.x()) / 2, (start.y() + end.y()) / 2);
            QPoint offset(10, 10);
            QPoint textPos = middle + offset;


            p.setPen(Qt::blue);
            QString costText = QString::number(e->getCost());
            p.drawText(textPos, costText);
        }
    else
        for (auto& e : edges)
        {
            QPoint start = e->getFirst()->getPos();
            QPoint end = e->getSecond()->getPos();
            p.drawLine(start, end);
            double angle = std::atan2(end.y() - start.y(), end.x() - start.x());
            int arrowSize = 20;
            QPoint arrowP1 = end - QPoint(arrowSize * std::cos(angle - M_PI / 6), arrowSize * std::sin(angle - M_PI / 6));
            QPoint arrowP2 = end - QPoint(arrowSize * std::cos(angle + M_PI / 6), arrowSize * std::sin(angle + M_PI / 6));
            QPolygon arrowHead;
            arrowHead << end << arrowP1 << arrowP2;
            p.drawPolygon(arrowHead);
        }
    Kruskal=false;
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

void MainWindow::onButtonKruskalClicked()
{
    Kruskal=true;
    update();
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

int MainWindow::find(int u, std::vector<int>& parent) {
    if (u != parent[u]) {
        parent[u] = find(parent[u], parent);
    }
    return parent[u];
}

void MainWindow::unionSets(int u, int v, std::vector<int>& parent, std::vector<int>& rank) {
    u = find(u, parent);
    v = find(v, parent);
    if (u != v) {
        if (rank[u] < rank[v]) {
            parent[u] = v;
        } else if (rank[u] > rank[v]) {
            parent[v] = u;
        } else {
            parent[v] = u;
            rank[u]++;
        }
    }
}

std::vector<Edge*> MainWindow::kruskalAlgorithm() {
    std::vector<Edge*> mst;
    std::vector<Edge*> edges = m_graph.getEdges();
    int n = m_graph.getNodes().size();

    std::sort(edges.begin(), edges.end(), [](Edge* e1, Edge* e2) {
        return e1->getCost() < e2->getCost();
    });

    std::vector<int> parent(n);
    std::vector<int> rank(n, 0);
    for (int i = 0; i < n; ++i) {
        parent[i] = i;
    }

    for (Edge* e : edges) {
        int u = e->getFirst()->getValue();
        int v = e->getSecond()->getValue();

        if (find(u, parent) != find(v, parent)) {
            mst.push_back(e);
            unionSets(u, v, parent, rank);
        }

        if (mst.size() == n - 1) {
            break;
        }
    }

    return mst;
}
