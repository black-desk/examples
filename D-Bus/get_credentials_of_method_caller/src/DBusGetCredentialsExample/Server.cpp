#include "DBusGetCredentialsExample/Server.hpp"

#include <QDBusArgument>
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusUnixFileDescriptor>
#include <QDebug>
#include <QFile>
#include <QJsonObject>

namespace DBusGetCredentialsExample
{
namespace
{
void printCallerUIDAndPID(const QDBusContext &context) noexcept
{
        // NOTE:
        // If you just want UID and PID,
        // you can use QDBusConnectionInterface::serviceUid
        // and QDBusConnectionInterface::servicePid like this.

        auto uid = QDBusConnection::sessionBus().interface()->serviceUid(
                context.message().service());
        qInfo() << "UID of caller:" << uid;

        // WARNING:
        // PID get here is not a reliable way to identify a process,
        // as linux allows PID reuse.
        // If you want to identify a process reliably,
        // you should let caller pass its `starttime` to your service
        // and your service must check it.
        // The `starttime` of a process can get from /proc filesystem.
        // Go check `man proc_pid_stat.5` for more information.

        auto pid = QDBusConnection::sessionBus().interface()->servicePid(
                context.message().service());
        qInfo() << "PID of caller:" << pid;
}

void printCallerUnixCredentials(const QDBusContext &context) noexcept
{
        // NOTE:
        // If you want more detailed information,
        // like GIDs or linux security labels,
        // you have to call GetConnectionCredentials method manually.

        auto credentialsDBusMessageArguments =
                QDBusConnection::sessionBus()
                        .interface()
                        ->call("GetConnectionCredentials",
                               context.message().service())
                        .arguments();
        Q_ASSERT(credentialsDBusMessageArguments.size() == 1);

        Q_ASSERT(credentialsDBusMessageArguments.constBegin()
                         ->canConvert<QDBusArgument>());

        auto credentialsQDBusArgument =
                credentialsDBusMessageArguments.constBegin()
                        ->value<QDBusArgument>();

        QMap<QString, QVariant> credentials;

        credentialsQDBusArgument >> credentials;

        qInfo() << "Unix credentials of caller received:";

        // https://dbus.freedesktop.org/doc/dbus-specification.html#bus-messages-get-connection-credentials

        if (credentials.contains("UnixUserID")) {
                Q_ASSERT(credentials.find("UnixUserID")->type() ==
                         QVariant::UInt);
                bool ok = false;
                qInfo() << "\tUnixUserID:"
                        << credentials["UnixUserID"].toUInt(&ok);
                Q_ASSERT(ok);
        }

        if (credentials.contains("UnixGroupIDs")) {
                Q_ASSERT(credentials.find("UnixGroupIDs")
                                 ->canConvert<QDBusArgument>());

                auto unixGroupIDsQDBusArgument =
                        credentials.find("UnixGroupIDs")->value<QDBusArgument>();
                QList<quint32> unixGroupIDs;
                unixGroupIDsQDBusArgument >> unixGroupIDs;
                qInfo() << "\tUnixGroupIDs:" << unixGroupIDs;
        }

        if (credentials.contains("ProcessFD")) {
                Q_ASSERT(credentials.find("ProcessFD")
                                 ->canConvert<QDBusArgument>());

                auto unixProcessFDQDBusArgument =
                        credentials.find("ProcessFD")->value<QDBusArgument>();
                QDBusUnixFileDescriptor unixProcessFD;
                unixProcessFDQDBusArgument >> unixProcessFD;
                qInfo() << "\tProcessFD:" << unixProcessFD.fileDescriptor();
                auto fdinfoFile =
                        QFile(QString("/proc/self/fdinfo/%1")
                                      .arg(unixProcessFD.fileDescriptor()));
                bool ok = fdinfoFile.open(QIODevice::ReadOnly);
                Q_ASSERT(ok);

                auto fdinfo = fdinfoFile.readAll();

                qInfo() << "\tProcessFD info:" << Qt::endl;
                qInfo() << QString::fromUtf8(fdinfo);
        }

        if (credentials.contains("ProcessID")) {
                // WARNING:
                // Check the warning in printCallerUIDAndPID function.

                Q_ASSERT(credentials.find("ProcessID")->type() ==
                         QVariant::UInt);

                bool ok = false;
                qInfo() << "\tProcessID:"
                        << credentials["ProcessID"].toUInt(&ok);
                Q_ASSERT(ok);
        }

        if (credentials.contains("LinuxSecurityLabel")) {
                Q_ASSERT(credentials.find("LinuxSecurityLabel")->type() ==
                         QVariant::ByteArray);

                qInfo() << "\tLinuxSecurityLabel:"
                        << credentials["LinuxSecurityLabel"].toByteArray();
        }
}
}

void Server::Ping()
{
        printCallerUIDAndPID(*this);
        printCallerUnixCredentials(*this);
        return;
}
}
