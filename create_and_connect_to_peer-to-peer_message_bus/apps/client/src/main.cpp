#include <QCoreApplication>

#include "DBusP2PExample/ServerInterface.h"
#include "DBusP2PExample/configure.hpp"
#include "DBusP2PExample/dbus.hpp"
#include "DBusP2PExample/init.hpp"

using namespace DBusP2PExample;

auto main(int argc, char **argv) -> int
{
        QCoreApplication app(argc, argv);
        initQCoreApplication();

        auto ret = QMetaObject::invokeMethod(
                QCoreApplication::instance(),
                []() {
                        auto conn = QDBusConnection::connectToPeer(
                                "unix:path=" + getPeerToPeerSocketAddress(),
                                "p2p DBus connection to server");

                        // FIXME:
                        // Wait for connection authentication finished.
                        // Otherwise,
                        // you have to enable annonymous authentication
                        // in your service by calling
                        // QDBusServer::setAnonymousAuthenticationAllowed(true)
                        // Check https://codereview.qt-project.org/c/qt/qtbase/+/308735
                        // for more information.
                        QThread::sleep(1);

                        if (!conn.isConnected()) {
                                qCritical()
                                        << conn.name() << "is not connected:"
                                        << conn.lastError();
                                QCoreApplication::exit(-1);
                                return;
                        }

                        auto server = new org::example::QDBusP2PExampleServer(
                                "", serverDBusObjectPath.data(), conn,
                                QCoreApplication::instance());

                        if (!server->isValid()) {
                                qCritical() << server->lastError();
                                QCoreApplication::exit(-1);
                                return;
                        }

                        QObject::connect(
                                server,
                                &org::example::QDBusP2PExampleServer::Pong,
                                []() {
                                        qInfo() << "Signal Pong arrived";
                                        QCoreApplication::exit(0);
                                });

                        auto reply = server->Ping();
                        reply.waitForFinished();
                        if (reply.isError()) {
                                qCritical() << "Ping error:" << reply.error();
                                QCoreApplication::exit(-1);
                                return;
                        }
                        qInfo() << "Method call of Ping return";
                        return;
                },
                Qt::QueuedConnection);

        Q_ASSERT(ret);

        return QCoreApplication::exec();
}
