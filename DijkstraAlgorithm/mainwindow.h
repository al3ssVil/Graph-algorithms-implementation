#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <QPointF>
#include <queue>
#include "Graph.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    QVector<int> dijkstra(int startId, int endId);

private:
    Ui::MainWindow *ui;
    Graph graph;

    int selectedNode1 = -1;
    int selectedNode2 = -1;

    QPointF scaleCoordinates(long long latitude, long long longitude);
};
#endif // MAINWINDOW_H
