#ifndef FTPSERVERCHECKER_H
#define FTPSERVERCHECKER_H

#include "AbstractDeviceChecker.h"

#include <QTcpSocket>
#include <QThread>
#include <queue>
#include <optional>

namespace DeviceChecker {

struct FtpConnectionData : public DeviceConnectionData {
    FtpConnectionData(QString url,
                      QString userName,
                      QString password)
        : DeviceConnectionData(DeviceType::Ftp)
        , m_url(url)
        , m_userName(userName)
        , m_password(password)
    {}
    FtpConnectionData()
        : DeviceConnectionData(DeviceType::Ftp)
        , m_url()
        , m_userName()
        , m_password()
    {}
    FtpConnectionData(const FtpConnectionData &another)
        : DeviceConnectionData(DeviceType::Ftp)
        , m_url(another.m_url)
        , m_userName(another.m_userName)
        , m_password(another.m_password)
    {}

    QString m_url;
    QString m_userName;
    QString m_password;
};

class FtpConnectionWorker : public QObject {
    Q_OBJECT
public:
    explicit FtpConnectionWorker(QObject *parent = nullptr)
        : QObject(parent)
        , m_tcpSocker(this)
    {}
    ~FtpConnectionWorker() {
        closeSocket();
    }

    bool connect(FtpConnectionData ftp) {
        if (!closeSocket())
            return false;

        const quint16 ftpPort {21};

        m_tcpSocker.connectToHost(ftp.m_url, ftpPort);

        if (!m_tcpSocker.waitForConnected(m_timeout)) {
            m_tcpSocker.close();
            emit message("Connection failed. Error "
                         + QString::number((int)m_tcpSocker.error())
                         + ". Check url/ip of server.");
            emit connectionError();
            return false;
        }

        emit message("Connected to " + ftp.m_url);

        getResponse("220");

        m_ftpConnectionData = ftp;

        if (!signIn()) {
            emit message("Sing In error.");
            emit connectionError();
            return false;
        }

        emit message("Signed in! Now you can test your scenario.");
        return true;
    }

    void closeConnect() {
        if (!m_tcpSocker.isOpen())
            return;

        const QPair<QString, QString> commandCode {"QUIT", "221"};

        if (!sendCommand(commandCode.first)) {
            emit message("Failed to quit from FTP server. ");
        }

        getResponse();
    }

    void checkCommand(QPair<QString, QString> command) {
        if (!m_tcpSocker.isOpen()) {
            emit message("There's no connection.");
            emit commandChecked(command.first, false);
            return;
        }

        emit message("\nCheck command: " + command.first + "...");
        if (!sendCommand(command.first)) {
            emit commandChecked(command.first, false);
            return;
        }

        if (!getResponse(command.second)) {
            emit commandChecked(command.first, false);
            return;
        }

        emit message("PASSED. Command: " + command.first + ".");
        emit commandChecked(command.first, true);
    }

    void clearBuffer() {
        m_tcpSocker.readAll();
    }

private:
    bool signIn() {
        emit message("\nTrying to sign in with user name \""
                     + m_ftpConnectionData.m_userName + "\"...");

        bool isAnon = m_ftpConnectionData.m_userName == "anonymous";

        const QPair<QString, QString> userCommand {
            "USER "+ m_ftpConnectionData.m_userName,
                    isAnon ? "230" : "33"
        };

        if (!sendCommand(userCommand.first) ||
                !getResponse(userCommand.second))
            return false;

        if (!isAnon) {
            emit message("Sending the password...");

            const QPair<QString, QString> passCommand {
                "PASS "+ m_ftpConnectionData.m_password, "230"
            };

            if (!sendCommand(passCommand.first) ||
                    !getResponse(passCommand.second))
                return false;
        }

        return true;
    }

    bool sendCommand(const QString &command) {
        m_tcpSocker.write((command + "\r\n").toUtf8().constData());
        if (!m_tcpSocker.waitForBytesWritten(m_timeout)) {
            emit message("Failed send command: " + command);
            return false;
        }

        return true;
    }

    bool getResponse(const QString &expectedResponseCode = "") {
        bool isNumber;
        int code = expectedResponseCode.toInt(&isNumber);
        if (!expectedResponseCode.isEmpty() && isNumber
                && (code < 1 || code > 599)) {
            emit message("Wrong expected response code on command."
                         "You code " + expectedResponseCode
                         + "must be 1 > codeNumber > 599.");
            return false;
        }

        if (!m_tcpSocker.isOpen()) {
            emit message("Socket is not opened");
            return false;
        }


        if (m_tcpSocker.canReadLine() ||
                (m_tcpSocker.waitForReadyRead(m_timeout) && m_tcpSocker.canReadLine())) {
            auto responseData = m_tcpSocker.readLine();
            qDebug() << "From FTP:" << responseData;

            emit responseFromFtp(responseData.mid(0, responseData.size() - 2));

            int codeSize{3};

            if (!expectedResponseCode.isEmpty() && responseData.size() >= expectedResponseCode.size()
                    && responseData.mid(0, expectedResponseCode.size()).compare(expectedResponseCode.toUtf8())) {
                emit message("FAILED. Expected response is \""
                             + expectedResponseCode
                             + "\", but got \"" + responseData.mid(0, expectedResponseCode.size())
                             + "\" in response from FTP server.");
                return false;
            }


            if (responseData.size() > codeSize
                    && (responseData[codeSize] == '-' || responseData[0] == ' '))
                return getResponse();

            return true;
        }
        else {
            emit message("Failed to get a response from the FTP server.");
            return false;
        }

    }

    bool closeSocket() {
        if (!m_tcpSocker.isOpen())
            return true;

        closeConnect();
        m_tcpSocker.close();
//        if (!m_tcpSocker.waitForDisconnected(m_timeout)) {
//            emit message("Disconnection failed. Error "
//                         + QString::number((int)m_tcpSocker.error()));
//            return false;
//        }

        return true;
    }
public:
signals:
    void message(QString msg);
    void responseFromFtp(QString response);
    void commandChecked(QString command, bool success);

    void connectionError();

private:
    QTcpSocket m_tcpSocker;
    FtpConnectionData m_ftpConnectionData;
    int m_timeout {3000};
};

class FtpServerChecker : public AbstractDeviceChecker
{
    Q_OBJECT
public:
    explicit FtpServerChecker(QObject *parent = nullptr);
    ~FtpServerChecker();

    // AbstractDeviceChecker interface
    void connectToDevice(const DeviceConnectionData &deviceData) override;
    void checkCommands(const QVector<QPair<QString, QString>> &commands) override;
    void closeConnection() override;

private:
signals:
    void signalConnectToDevice(DeviceChecker::FtpConnectionData);
    void signalDisconnect();
    void signalCheckCommand(QPair<QString, QString>);
    void clearBuffer();

private:
    void onCommandChecked(QString command, bool success);

private:
    FtpConnectionWorker *m_connectionWorker = nullptr;
    QThread m_workerThread;

    int m_commandSize {0};
    int m_commandCounter{0};
    std::queue<QPair<QString, QString>> m_commandQueue;
};

}

Q_DECLARE_METATYPE(DeviceChecker::FtpConnectionData)

#endif // FTPSERVERCHECKER_H
