#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "treeviewmodel.h"
#include <QItemSelectionModel>

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN) && QT_VERSION_CHECK(5, 6, 0) <= QT_VERSION && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);


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
