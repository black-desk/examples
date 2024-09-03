#pragma once

#include <QDBusContext>
#include <qobjectdefs.h>

namespace DBusGetCredentialsExample
{

class Server : public QObject, public QDBusContext {
        Q_OBJECT
        using QObject::QObject;

    public:
        virtual void Ping();
};
}
