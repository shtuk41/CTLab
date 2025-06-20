#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_quadwidgets.h"

class QuadWidgets : public QMainWindow
{
    Q_OBJECT

public:
    QuadWidgets(QWidget *parent = nullptr);
    ~QuadWidgets();

private:
    Ui::QuadWidgetsClass ui;

    void populateTree();
};

