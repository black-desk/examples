#include <QCoreApplication>

#include "DBusGetCredentialsExample/ServerInterface.h"
#include "DBusGetCredentialsExample/configure.hpp"
#include "DBusGetCredentialsExample/init.hpp"

using namespace DBusGetCredentialsExample;

auto main(int argc, char **argv) -> int
{
        QCoreApplication app(argc, argv);
        initQCoreApplication();

        auto ret = QMetaObject::invokeMethod(
                QCoreApplication::instance(),
                []() {
                        auto server =
                                new org::example::DBusGetCredentialsExampleServer(
                                        QString::fromStdString(
                                                serverDBusServiceName.data()),
                                        QString::fromStdString(
                                                serverDBusObjectPath.data()),
                                        QDBusConnection::sessionBus(),
                                        QCoreApplication::instance());

                        if (!server->isValid()) {
                                qCritical() << server->lastError();
                                QCoreApplication::exit(-1);
                                return;
                        }

                        auto reply = server->Ping();
                        reply.waitForFinished();
                        if (reply.isError()) {
                                qCritical() << "TestMethod error.";
                                QCoreApplication::exit(-1);
                                return;
                        }
                        QCoreApplication::exit(0);
                        return;
                },
                Qt::QueuedConnection);

        Q_ASSERT(ret);

        return QCoreApplication::exec();
}
