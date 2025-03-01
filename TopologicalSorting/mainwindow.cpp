#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_ButonOrientat = new QRadioButton("Orientat", this);
    m_ButonNeorientat = new QRadioButton("Neorientat", this);
    m_ButonOrientat->setChecked(true);

    m_ButonCalcSortareTopologica = new QPushButton("Calculare topologie", this);
    connect(m_ButonCalcSortareTopologica, &QPushButton::clicked, this, &MainWindow::onButonCalcSortareTopologicaClicked);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(m_ButonOrientat);
    layout->addWidget(m_ButonNeorientat);
    layout->addWidget(m_ButonCalcSortareTopologica);  // Adăugăm noul buton la layout

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
    connect(m_ButonOrientat, &QRadioButton::clicked, this, &MainWindow::onButonOrientatClicked);
    connect(m_ButonNeorientat, &QRadioButton::clicked, this, &MainWindow::onButonNeorientatClicked);
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
                    if (isFirstNodeSelected)
                    {
                        m_graph.addEdge(m_selectedNode, n);
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
            p.drawLine(e->getFirst()->getPos().x(), e->getFirst()->getPos().y(),e->getSecond()->getPos().x(), e->getSecond()->getPos().y());
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

void MainWindow::onButonCalcSortareTopologicaClicked()
{
    bool esteOrientat = m_ButonOrientat->isChecked();

    if (esteOrientat)
    {
        std::vector<int> topSort = m_graph.topologicalSort();

        if (topSort.empty())
        {
            QMessageBox::warning(this, "Eroare", "Graful conține un ciclu și nu poate fi sortat topologic.");
        } else
        {
            QString result = "Sortare topologică: ";
            for (int value : topSort) {
                result += QString::number(value + 1) + " ";
            }
            QMessageBox::information(this, "Sortare Topologică", result);
            m_graph.saveTopologicalSortToFile(topSort);
            m_graph.eliminaArceDupaTopologica(topSort);
            update();
        }
    }
    else
    {
        QMessageBox::warning(this, "Eroare", "Graful nu poate fi sortat topologic pentru că este neorientat.");
    }
}


