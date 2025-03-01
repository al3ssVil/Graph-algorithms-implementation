#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <vector>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QBrush>
#include <QColor>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QVBoxLayout>

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

private:
    Ui::MainWindow *ui;

    void readMazeFromFile(const QString &fileName);
    void createGraph();
    void displayMaze();
    void displayGraph();
    void solveMaze();
    void drawPath(const std::vector<int>& path);

    std::vector<std::vector<int>> maze;
    std::vector<std::vector<int>> graph;
    int numRows, numCols;

    QGraphicsScene *sceneMaze;
    QGraphicsScene *sceneGraph;

    QGraphicsView *viewMaze;
    QGraphicsView *viewGraph;
};
#endif // MAINWINDOW_H
