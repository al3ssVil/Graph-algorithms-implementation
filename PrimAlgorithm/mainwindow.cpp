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
    m_Prim = new QPushButton("Prim Algoritm", this);
    m_ButonNeorientat->setChecked(true);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(m_ButonOrientat);
    layout->addWidget(m_ButonNeorientat);
    layout->addWidget(m_Prim);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
    connect(m_ButonOrientat, &QRadioButton::clicked, this, &MainWindow::onButonOrientatClicked);
    connect(m_ButonNeorientat, &QRadioButton::clicked, this, &MainWindow::onButonNeorientatClicked);
    connect(m_Prim, &QPushButton::clicked, this, &MainWindow::onButonPrimClicked);
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

    if(Prim)
    {
        a = primAlgorithm();
        setA = std::set<Edge*>(a.begin(), a.end());
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
                p.setPen(Qt::green);
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
    Prim=false;
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

void MainWindow::onButonPrimClicked()
{
    Prim=true;
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

std::vector<Edge*> MainWindow::primAlgorithm()
{
    std::vector<Edge*> A;

    int n = m_graph.getNodes().size();

    if (n == 0) {
        qDebug() << "Graful este gol!";
        return A;
    }

    std::vector<int> v(n, INT_MAX);
    std::vector<Edge*> e(n, nullptr);
    v[0] = 0;

    std::set<int> N1;
    std::set<int> N1_barat;
    std::set<int> N;
    for (int i = 0; i < n; ++i) {
        N1_barat.insert(i);
        N.insert(i);
    }

    while (N1 != N) {
        int y = 0;
        int minValue = INT_MAX;

        for (int k : N1_barat) {
            if (v[k] < minValue) {
                minValue = v[k];
                y = k;
            }
        }

        N1.insert(y);
        N1_barat.erase(y);

        if (y != 0 && e[y] != nullptr) {
            A.push_back(e[y]);
        }

        for (int ybarat : N1_barat) {
            if (y >= 0 && y < n && ybarat >= 0 && ybarat < n) {
                int cost = m_graph.getMatriceAdiacenta()[y][ybarat];
                if (cost != 0 && cost < v[ybarat]) {
                    v[ybarat] = cost;
                    Edge* newEdge = new Edge(m_graph.getNodes()[y], m_graph.getNodes()[ybarat], cost);
                    e[ybarat] = newEdge;
                }
            }
        }
    }
    return A;
}


