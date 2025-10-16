#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include "glrenderer.h"
#include "volumeViewControl.h"

int main(int argc, char *argv[])
{

#if defined(Q_OS_WIN) && QT_VERSION_CHECK(5, 6, 0) <= QT_VERSION && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
    QGuiApplication app(argc, argv);

    qmlRegisterType<VolumeViewControl>("VolumeViewControl", 1, 0, "VolumeViewControl");

    QQmlApplicationEngine engine;
    engine.addImportPath("C:/Qt/6.9.1/msvc2022_64/qml");
    engine.addImportPath("C:/Qt/6.9.1/msvc2022_64/qml/QtQuick/Controls.2");
    engine.addImportPath("C:/Qt/6.9.1/msvc2022_64/qml/QtQuick/Layouts");
    engine.load(QUrl(QStringLiteral("qrc:/qt/qml/volumeviewcontrol/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
