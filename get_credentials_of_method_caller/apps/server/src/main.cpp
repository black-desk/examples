#include <QCoreApplication>
#include <QDBusServer>
#include <QStringView>

#include "QDBusGetCredentialsExample/Server.hpp"
#include "QDBusGetCredentialsExample/ServerAdaptor.h"
#include "QDBusGetCredentialsExample/init.hpp"

using namespace QDBusGetCredentialsExample;

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
                                "/io/github/blackdesk/QDBusGetCredentialsExample/Server",
                                server);
                        QDBusConnection::sessionBus().registerService(
                                "io.github.blackdesk.QDBusGetCredentialsExample.Server");
                },
                Qt::QueuedConnection);

        Q_ASSERT(ret);

        return QCoreApplication::exec();
}
