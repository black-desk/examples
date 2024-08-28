#include <QCoreApplication>

#include "QDBusGetCredentialsExample/init.hpp"
#include "QDBusGetCredentialsExample/io_github_blackdesk_QDBusGetCredentialsExample_Server.h"

using namespace QDBusGetCredentialsExample;

auto main(int argc, char **argv) -> int
{
        QCoreApplication app(argc, argv);
        initQCoreApplication();

        auto ret = QMetaObject::invokeMethod(
                QCoreApplication::instance(),
                []() {
                        auto server = new io::github::blackdesk::
                                QDBusGetCredentialsExample::Server(
                                        "io.github.blackdesk.QDBusGetCredentialsExample.Server",
                                        "/io/github/blackdesk/QDBusGetCredentialsExample/Server",
                                        QDBusConnection::sessionBus(),
                                        QCoreApplication::instance());

                        if (!server->isValid()) {
                                qCritical() << server->lastError();
                                QCoreApplication::exit(-1);
                                return;
                        }

                        auto reply = server->TestMethod();
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
