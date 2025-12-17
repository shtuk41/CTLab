#pragma once

#include <QObject>
#include <context.h>

class ContextWrapper : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString volumePath READ volumePath CONSTANT)

public:
    explicit ContextWrapper(const QString& path, QObject* parent = nullptr)
        : QObject(parent), m_context(std::make_shared<Context>(path.toStdString())) {}

    Q_INVOKABLE void loadVolume(const QString& path)
    {
        qDebug() << "Loading volume from file:" << path;
        //getContext()->setVolume(path.toStdString());
    }

    QString volumePath() const { return path; }

    std::shared_ptr<Context> getContext() const { return m_context; }

private:
    std::shared_ptr<Context> m_context;
    QString path;
};

