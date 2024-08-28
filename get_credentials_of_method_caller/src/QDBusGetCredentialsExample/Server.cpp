#include "QDBusGetCredentialsExample/Server.hpp"

#include <QDBusArgument>
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDebug>
#include <QJsonObject>

namespace QDBusGetCredentialsExample
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

        Q_ASSERT(credentials.contains("LinuxSecurityLabel"));
        Q_ASSERT(credentials.find("LinuxSecurityLabel")->type() ==
                 QVariant::ByteArray);

        qInfo() << "\tLinuxSecurityLabel:"
                << credentials["LinuxSecurityLabel"].toByteArray();

        // WARNING:
        // Check the warning in printCallerUIDAndPID function.

        Q_ASSERT(credentials.contains("ProcessID"));
        Q_ASSERT(credentials.find("ProcessID")->type() == QVariant::UInt);

        bool ok = false;
        qInfo() << "\tProcessID:" << credentials["ProcessID"].toUInt(&ok);

        Q_ASSERT(ok);

        Q_ASSERT(credentials.contains("UnixGroupIDs"));
        Q_ASSERT(credentials.find("UnixGroupIDs")->canConvert<QDBusArgument>());

        auto unixGroupIDsQDBusArgument =
                credentials.find("UnixGroupIDs")->value<QDBusArgument>();
        QList<quint64> unixGroupIDs;
        unixGroupIDsQDBusArgument >> unixGroupIDs;
        qInfo() << "\tUnixGroupIDs:" << unixGroupIDs;

        Q_ASSERT(credentials.contains("UnixUserID"));
        Q_ASSERT(credentials.find("UnixUserID")->type() == QVariant::UInt);

        qInfo() << "\tUnixUserID:" << credentials["UnixUserID"].toUInt(&ok);
        Q_ASSERT(ok);
}
}

void Server::TestMethod()
{
        printCallerUIDAndPID(*this);
        printCallerUnixCredentials(*this);
        return;
}
}
