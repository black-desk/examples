#include "QDBusP2PExample/Server.hpp"

#include <QDebug>
#include <QJsonObject>
#include <qdbusargument.h>

namespace QDBusP2PExample
{

void Server::TestMethod()
{
        qInfo() << "method called";
        emit this->TestSignal();
        return;
}

}
