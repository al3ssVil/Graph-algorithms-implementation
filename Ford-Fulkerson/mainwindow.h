#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include "graph.h"
#include <QRadioButton>
#include <QVBoxLayout>
#include <QPushButton>

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void mouseReleaseEvent(QMouseEvent* m) override;
    void paintEvent(QPaintEvent* ev) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    bool bfs(const std::vector<std::vector<int> > &residualGraph, int source, int sink, std::vector<int> &parent);
    void calculateMaxFlow();
    void findMinCut(std::vector<std::vector<int> > &residualGraph, int source);
    int fordFulkerson(int source, int sink, std::vector<std::vector<int> > &residualGraph);
private:
    Ui::MainWindow *ui;
    Graph m_graph;
    Node* m_selectedNode=nullptr;
    Node* selectedNode = nullptr;
    bool isFirstNodeSelected=false;
    void onButonOrientatClicked();
    void onButonNeorientatClicked();
    QRadioButton* m_ButonOrientat=nullptr;
    QRadioButton* m_ButonNeorientat=nullptr;
    bool draggingNode = false;
    std::vector<std::pair<int, int>> minCutEdges;
    std::vector<std::vector<int>> residualGraph;
};
#endif // MAINWINDOW_H
