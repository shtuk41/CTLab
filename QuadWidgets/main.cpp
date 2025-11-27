#include "quadwidgets.h"
#include <QtWidgets/QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QSurfaceFormat fmt;
    fmt.setRenderableType(QSurfaceFormat::OpenGL);
    fmt.setVersion(3, 3);                
    fmt.setProfile(QSurfaceFormat::CoreProfile);  
    QSurfaceFormat::setDefaultFormat(fmt);


    QApplication app(argc, argv);
    app.setWindowIcon(QIcon("icon.png"));

    QuadWidgets window;
    window.show();
    return app.exec();
}
