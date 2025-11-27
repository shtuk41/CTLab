#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_quadwidgets.h"
#include <glviewquadxy.h>
#include <glviewquadyz.h>
#include <glviewquadxz.h>
#include <glviewquad3d.h>


class QuadWidgets : public QMainWindow
{
    Q_OBJECT

public:
    QuadWidgets(QWidget *parent = nullptr);
    ~QuadWidgets();

private:
    Ui::QuadWidgetsClass ui;

    void populateTree();

    GLViewQuadXY* glViewQuadXY;
    GLViewQuadYZ* glViewQuadYZ;
    GLViewQuadXZ* glViewQuadXZ;
    GLViewQuad3D* glViewQuad3d;

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;

};

