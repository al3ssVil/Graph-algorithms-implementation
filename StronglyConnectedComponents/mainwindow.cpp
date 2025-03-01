#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_ButonOrientat = new QRadioButton("Orientat", this);
    m_ButonNeorientat = new QRadioButton("Neorientat", this);
    m_ButonOrientat->setChecked(true);

    m_DrawArea = new QWidget(this);
    m_DrawArea->setFixedSize(400, 100);

    m_ButonCalcComponenteTare = new QPushButton("Calculare componente tare conexe", this);
    connect(m_ButonCalcComponenteTare, &QPushButton::clicked, this, &MainWindow::onButonCalcComponenteTareClicked);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(m_DrawArea);
    layout->addWidget(m_ButonOrientat);
    layout->addWidget(m_ButonNeorientat);
    layout->addWidget(m_ButonCalcComponenteTare);



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

    const int radius = 40;
    const int offsetX = 100;
    const int offsetY = 10;
    int groupId = 0;
    for (const auto& componenta : componenteTareConexe)
    {
        int centerX = offsetX + (groupId * 150);
        int centerY = offsetY + 100;

        QRectF circle(centerX - radius, centerY - radius, 2 * radius, 2 * radius);
        p.setPen(QPen(Qt::white));
        p.setBrush(Qt::transparent);
        p.drawEllipse(circle);

        QStringList nodeNames;
        for (int nodeIndex : componenta) {
            Node* node = m_graph.getNodes()[nodeIndex];
            nodeNames.append(QString::number(node->getValue() + 1));
        }

        QString allNodeNames = nodeNames.join(" ");

        QFont font = p.font();
        font.setPointSize(14);
        p.setFont(font);
        p.setPen(QColor(0, 0, 255));

        QRect textRect(centerX - radius, centerY - radius, 2 * radius, 2 * radius);
        p.drawText(textRect, Qt::AlignCenter, allNodeNames);

        groupId++;
    }
    if (m_ButonOrientat->isChecked())
    {
        std::vector<Edge*> edges = m_graph.getEdges();
        for (auto& edge : edges) {
            Node* first = edge->getFirst();
            Node* second = edge->getSecond();
            int groupFirst = -1;
            int groupSecond = -1;
            for (int i = 0; i < componenteTareConexe.size(); i++) {
                if (std::find(componenteTareConexe[i].begin(), componenteTareConexe[i].end(), first->getValue()) != componenteTareConexe[i].end()) {
                    groupFirst = i;
                }
                if (std::find(componenteTareConexe[i].begin(), componenteTareConexe[i].end(), second->getValue()) != componenteTareConexe[i].end()) {
                    groupSecond = i;
                }
            }

            if (groupFirst != groupSecond) {
                QPoint start = QPoint(offsetX + (groupFirst * 150), offsetY + 100);
                QPoint end = QPoint(offsetX + (groupSecond * 150), offsetY + 100);
                p.setPen(QPen(Qt::white, 2));
                p.drawLine(start, end);

                // Desenăm săgeata
                double angle = std::atan2(end.y() - start.y(), end.x() - start.x());
                int arrowSize = 20;
                QPoint arrowP1 = end - QPoint(arrowSize * std::cos(angle - M_PI / 6), arrowSize * std::sin(angle - M_PI / 6));
                QPoint arrowP2 = end - QPoint(arrowSize * std::cos(angle + M_PI / 6), arrowSize * std::sin(angle + M_PI / 6));
                QPolygon arrowHead;
                arrowHead << end << arrowP1 << arrowP2;
                p.drawPolygon(arrowHead);
            }
        }
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

void MainWindow::onButonCalcComponenteTareClicked()
{
    if(m_ButonOrientat->isChecked())
    {
        componenteTareConexe = m_graph.findStronglyConnectedComponents();
        if (componenteTareConexe.empty()) {
            qDebug() << "No strongly connected components found!";
        }
        else
        {
            for (const auto& componenta : componenteTareConexe)
            {
                qDebug() << "Componenta conexă:";
                for (const auto& node : componenta)
                {
                    qDebug() << node + 1;
                }
                qDebug() << "sfârșit";
            }
            update();
        }
    }
    else
        qDebug()<<"Nu e graf orientat";
}
