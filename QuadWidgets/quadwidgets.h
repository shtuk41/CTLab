#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_quadwidgets.h"
#include <glviewquadaxial.h>
#include <glviewquadcoronal.h>
#include <glviewquadsagittal.h>
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

    GLViewQuadAxial* glViewQuadAxial;
    GLViewQuadCoronal* glViewQuadCoronal;
    GLViewQuadSagittal* glViewQuadSagittal;
    GLViewQuad3D* glViewQuad3d;

};

