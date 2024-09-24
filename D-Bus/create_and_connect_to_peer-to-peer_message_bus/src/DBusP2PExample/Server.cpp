#include "DBusP2PExample/Server.hpp"

#include <QDebug>
#include <QJsonObject>
#include <qdbusargument.h>

namespace DBusP2PExample
{

void Server::Ping()
{
        qInfo() << "Method Ping called";
        qInfo() << "Emitting Pong signal";
        emit this->Pong();
        qInfo() << "Pong signal emitted";
        qInfo() << "Method Ping return";
        return;
}

}
