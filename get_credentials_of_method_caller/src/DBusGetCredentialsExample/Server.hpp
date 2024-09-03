#pragma once

#include <QDBusContext>

namespace DBusGetCredentialsExample
{

class Server : public QObject, public QDBusContext {
        Q_OBJECT
        using QObject::QObject;

    public:
        virtual void Ping();
};
}
