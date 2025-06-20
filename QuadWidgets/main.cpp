#include "quadwidgets.h"
#include <QtWidgets/QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QSurfaceFormat fmt;
    fmt.setRenderableType(QSurfaceFormat::OpenGL);
    fmt.setVersion(3, 3);                // or lower if needed
    fmt.setProfile(QSurfaceFormat::CoreProfile);  // or CompatibilityProfile
    QSurfaceFormat::setDefaultFormat(fmt);


    QApplication app(argc, argv);
    QuadWidgets window;
    window.show();
    return app.exec();
}
