#pragma once

#include <QDBusContext>
#include <qobjectdefs.h>

namespace QDBusGetCredentialsExample
{

class Server : public QObject, public QDBusContext {
        Q_OBJECT
        using QObject::QObject;

    public:
        virtual void TestMethod();
};
}
