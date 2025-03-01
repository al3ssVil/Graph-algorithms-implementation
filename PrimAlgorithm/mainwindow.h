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
    std::vector<Edge *> primAlgorithm();
private:
    Ui::MainWindow *ui;
    Graph m_graph;
    Node* m_selectedNode=nullptr;
    bool isFirstNodeSelected=false;
    void onButonOrientatClicked();
    void onButonNeorientatClicked();
    void onButonPrimClicked();
    void onButtonKruskalClicked();
    QRadioButton* m_ButonOrientat=nullptr;
    QRadioButton* m_ButonNeorientat=nullptr;
    QPushButton* m_Prim=nullptr;
    QPushButton* m_Kruskal=nullptr;
    bool draggingNode = false;
    Node* selectedNode = nullptr;
    bool Prim=false;
};
#endif // MAINWINDOW_H
