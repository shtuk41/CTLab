#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QItemSelectionModel>
#include <QtQml>
#include <QQuickWindow>

#include "treeviewmodel.h"
#include "glitem.h"

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN) && QT_VERSION_CHECK(5, 6, 0) <= QT_VERSION && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
    QGuiApplication app(argc, argv);

    qmlRegisterType<GLItem>("CustomItems", 1, 0, "GLItem");


    TreeViewModel* model = new TreeViewModel();
    QItemSelectionModel* selectionModel = new QItemSelectionModel(model);


    qDebug() << "Row count:" << model->rowCount();


    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("yourTreeModel", model);
    engine.rootContext()->setContextProperty("yourSelectionModel", selectionModel);
    engine.load(QUrl(QStringLiteral("qrc:/qt/qml/quadqml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
