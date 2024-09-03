#include <QCoreApplication>
#include <QDBusServer>
#include <QStringView>

#include "DBusGetCredentialsExample/Server.hpp"
#include "DBusGetCredentialsExample/ServerAdaptor.h"
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
                        auto server = // NOLINT
                                new Server(QCoreApplication::instance());
                        auto adaptor = new ServerAdaptor(server); // NOLINT
                        Q_UNUSED(adaptor);

                        QDBusConnection::sessionBus().registerObject(
                                serverDBusObjectPath.data(), server);
                        QDBusConnection::sessionBus().registerService(
                                serverDBusServiceName.data());
                },
                Qt::QueuedConnection);

        Q_ASSERT(ret);

        return QCoreApplication::exec();
}
