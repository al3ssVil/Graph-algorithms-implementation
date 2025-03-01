#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <queue>
#include<QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sceneMaze = new QGraphicsScene(this);
    sceneGraph = new QGraphicsScene(this);

    viewMaze = new QGraphicsView(sceneMaze, this);
    viewGraph = new QGraphicsView(sceneGraph, this);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(viewGraph);
    layout->addWidget(viewMaze);
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    readMazeFromFile("labirint.txt");
    createGraph();
    displayGraph();
    displayMaze();
    solveMaze();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readMazeFromFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Nu s-a putut deschide fisierul!";
        return;
    }
    QTextStream in(&file);
    QString line;
    while (!in.atEnd())
    {
        line = in.readLine();
        std::vector<int> row;
        for (int i = 0; i < line.length(); i++)
        {
            row.push_back(line[i].digitValue());
        }
        maze.push_back(row);
    }
    numRows = maze.size();
    numCols = maze[0].size();
    file.close();
}

void MainWindow::createGraph()
{
    graph.resize(numRows * numCols);
    for (int i = 0; i < numRows; ++i)
        for (int j = 0; j < numCols; ++j)
            if (maze[i][j] !=0)
            {
                int node = i * numCols + j;
                if (i > 0 && maze[i - 1][j] !=0)
                    graph[node].push_back((i - 1) * numCols + j);
                if (i < numRows - 1 && maze[i + 1][j] !=0)
                    graph[node].push_back((i + 1) * numCols + j);
                if (j > 0 && maze[i][j-1] !=0)
                    graph[node].push_back(i * numCols + (j - 1));
                if (j < numCols - 1 && maze[i ][j+1] !=0)
                    graph[node].push_back(i * numCols + (j + 1));
            }
}

void MainWindow::displayMaze()
{
    for (int i = 0; i < numRows; ++i)
    {
        for (int j = 0; j < numCols; ++j)
        {
            QColor color;
            if (maze[i][j] == 0)
                color = Qt::black;
            else
                if (maze[i][j] == 1)
                    color = Qt::white;
                else
                    if (maze[i][j] == 2)
                        color = Qt::red;
                    else
                        color = Qt::blue;
            QGraphicsRectItem *cell = new QGraphicsRectItem(j * 30, i * 30, 30, 30);
            cell->setBrush(QBrush(color));
            sceneMaze->addItem(cell);
        }
    }
}

void MainWindow::displayGraph()
{
    int radius = 15;
    int spacing = 50;
    for (int i = 0; i < numRows; ++i)
    {
        for (int j = 0; j < numCols; ++j)
        {
            if (maze[i][j] !=0)
            {
                int node = i * numCols + j;
                int x = j * spacing;
                int y = i * spacing;
                QColor nodeColor;
                if (maze[i][j] == 3)
                    nodeColor = Qt::blue;
                else
                    if (maze[i][j] == 2)
                        nodeColor = Qt::red;
                    else
                        nodeColor = Qt::white;
                QGraphicsEllipseItem *nodeCircle = new QGraphicsEllipseItem(x - radius, y - radius, 2 * radius, 2 * radius);
                nodeCircle->setBrush(QBrush(nodeColor));
                sceneGraph->addItem(nodeCircle);
                for (int neighbor : graph[node])
                {
                    int neighborRow = neighbor / numCols;
                    int neighborCol = neighbor % numCols;
                    int neighborX = neighborCol * spacing;
                    int neighborY = neighborRow * spacing;
                    QGraphicsLineItem *edge = new QGraphicsLineItem(x, y, neighborX, neighborY);
                    sceneGraph->addItem(edge);
                }
            }
        }
    }
}

void MainWindow::solveMaze()
{
    std::vector<int> start;
    int endNode = -1;
    for (int i = 0; i < numRows; ++i)
    {
        for (int j = 0; j < numCols; ++j)
        {
            if (maze[i][j] == 3)
            {
                endNode = i * numCols + j;
            }
            else
                if (maze[i][j] == 2)
                {
                    start.emplace_back(i * numCols + j);
                }
        }
    }

    if (start.size()==0 || endNode == -1) {
        qDebug() << "Nu am găsit punctul de intrare sau ieșire!";
        return;
    }

    std::vector<int> l(numRows * numCols, INT_MAX);
    std::queue<std::vector<int>> pathsQueue;
    std::vector<std::vector<int>> allPaths;

    for(const auto& startNode: start)
    {
        std::fill(l.begin(), l.end(), INT_MAX);
        l[startNode] = 0;
        pathsQueue.push({startNode});
        while (!pathsQueue.empty())
        {
            std::vector<int> currentPath = pathsQueue.front();
            pathsQueue.pop();
            int currentNode = currentPath.back();

            if (currentNode == endNode)
            {
                allPaths.push_back(currentPath);
                continue;
            }

            for (int neighbor : graph[currentNode])
            {
                if (l[neighbor] >= l[currentNode] + 1)
                {
                    l[neighbor] = l[currentNode] + 1;
                    std::vector<int> newPath = currentPath;
                    newPath.push_back(neighbor);
                    pathsQueue.push(newPath);
                }
            }
        }
    }
    QTimer::singleShot(5000, this, [this, allPaths]() {
        // Aici vei colora fiecare drum găsit
        for (auto& path : allPaths) {
            drawPath(path);  // Colorează fiecare drum
        }
    });
    if (allPaths.empty())
    {
        qDebug() << "Nu am găsit niciun drum!";
    }
}

void MainWindow::drawPath(const std::vector<int>& path)
{
    for (int node : path)
    {
        int row = node / numCols;
        int col = node % numCols;
        if (maze[row][col] == 3 || maze[row][col] == 2)
        {

            continue;
        }
        QGraphicsRectItem *cell = new QGraphicsRectItem(col * 30, row * 30, 30, 30);
        cell->setBrush(QBrush(Qt::green));
        sceneMaze->addItem(cell);
    }
}
