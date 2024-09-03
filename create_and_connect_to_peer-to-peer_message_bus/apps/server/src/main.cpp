#include <QCoreApplication>
#include <QDBusServer>
#include <QStringView>

#include "DBusP2PExample/Server.hpp"
#include "DBusP2PExample/ServerAdaptor.h"
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
                        auto server = // NOLINT
                                new Server(QCoreApplication::instance());
                        auto adaptor = new ServerAdaptor(server); // NOLINT
                        Q_UNUSED(adaptor);

                        auto address = getPeerToPeerSocketAddress();
                        auto dirname = QDir::root().relativeFilePath(
                                QFileInfo(address).absolutePath());

                        if (!QDir::root().exists(dirname) &&
                            !QDir::root().mkpath(dirname)) {
                                qCritical().noquote()
                                        << QString("mkpath \"%1\" failed.")
                                                   .arg("/" + dirname);
                                QCoreApplication::exit(-1);
                                return;
                        }

                        auto qDBusServer = new QDBusServer( // NOLINT
                                "unix:path=" + address,
                                QCoreApplication::instance());
                        if (!qDBusServer->isConnected()) {
                                qCritical() << "QDBusServer not connected:"
                                            << qDBusServer->lastError();
                                QCoreApplication::exit(-1);
                        }

                        // FIXME:
                        // If your client is not waiting
                        // until authentication is finished,
                        // you can enable anonymous authentication here.
                        // Check https://codereview.qt-project.org/c/qt/qtbase/+/308735
                        // for more information.

                        // qDBusServer->setAnonymousAuthenticationAllowed(true);

                        QObject::connect(
                                QCoreApplication::instance(),
                                &QCoreApplication::aboutToQuit, [address]() {
                                        if (QDir::root().remove(
                                                    QDir::root()
                                                            .relativeFilePath(
                                                                    address))) {
                                                return;
                                        }

                                        qCritical().noquote()
                                                << QString("remove \"%1\" failed.")
                                                           .arg(address);
                                });

                        QList<QDBusConnection> connections;
                        QObject::connect(
                                qDBusServer, &QDBusServer::newConnection,
                                [server](const QDBusConnection &conn) noexcept {
                                        auto res = registerDBusObject(
                                                conn,
                                                serverDBusObjectPath.data(),
                                                server);
                                        if (!res.has_value()) {
                                                qCritical()
                                                        << res.error().what();
                                                return;
                                        }
                                        QObject::connect(
                                                QCoreApplication::instance(),
                                                &QCoreApplication::aboutToQuit,
                                                [conn]() {
                                                        unregisterDBusObject(
                                                                conn,
                                                                serverDBusObjectPath
                                                                        .data());
                                                });
                                });
                },
                Qt::QueuedConnection);

        Q_ASSERT(ret);

        return QCoreApplication::exec();
}
