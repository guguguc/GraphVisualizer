#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_visualizer.h"
#include "Graph.h"

class visualizer : public QMainWindow
{
    Q_OBJECT

public:
    visualizer(QWidget *parent = nullptr);
    ~visualizer();

private:
    Ui::visualizerClass ui;
    PGraph graph;
};
