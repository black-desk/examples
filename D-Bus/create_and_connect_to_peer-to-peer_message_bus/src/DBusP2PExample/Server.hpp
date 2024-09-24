#pragma once

#include <QDBusContext>
#include <QObject>

namespace DBusP2PExample
{

class Server : public QObject, public QDBusContext {
        Q_OBJECT
        using QObject::QObject;

    public:
        virtual void Ping();

    Q_SIGNALS:
        void Pong();
};
}
