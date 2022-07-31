#ifndef FTPSERVERCHECKER_H
#define FTPSERVERCHECKER_H

#include "AbstractDeviceChecker.h"

#include <QTcpSocket>
#include <QThread>

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

    QString m_url;
    QString m_userName;
    QString m_password;
};

class FtpConnectionWorker : public QObject {
    Q_OBJECT
public:
    explicit FtpConnectionWorker(QObject *object = nullptr);
    ~FtpConnectionWorker() {
        closeSocket();
    }

    bool connect(FtpConnectionData ftp) {
        if (closeSocket())
            return false;

        const quint16 ftpPort {21};
        m_tcpSocker.connectToHost(ftp.m_userName, ftpPort);

        if (!m_tcpSocker.waitForConnected(m_timeout)) {
            emit message("Connection failed. Error "
                         + QString::number(m_tcpSocker.error()));
            return false;
        }

        emit message("Connected to " + ftp.m_url);

        getResponse();

        m_ftpConnectionData = ftp;

        return signIn();
    }

    void closeConnect() {
        if (!m_tcpSocker.isOpen())
            return;

        const QPair<QString, int> commandCode {"QUIT", 221};

        if (!sendCommand(commandCode.first)) {
            emit message("Failed to quit from FTP server. ");
        }
        if (getResponse(commandCode.second)) {}
    }

    void checkCommand(QPair<QString, int> command) {
        if (!m_tcpSocker.isOpen()) {
            emit message("There's no connection.");
            emit commandChecked(command.first, false);
            return;
        }

        if (!sendCommand(command.first)) {
            emit commandChecked(command.first, false);
            return;
        }

        if (!getResponse(command.second))
            emit commandChecked(command.first, false);

        emit commandChecked(command.first, true);
    }

private:
    bool signIn() {
        emit message("Trying to sign in with userName="
                     + m_ftpConnectionData.m_userName + "...");

        const QPair<QString, int> userCommand {
            "USER "+ m_ftpConnectionData.m_userName, 331
        };

        if (!sendCommand(userCommand.first) ||
                !getResponse(userCommand.second))
            return false;

        const QPair<QString, int> passCommand {
            "PASS "+ m_ftpConnectionData.m_password, 230
        };

        if (!sendCommand(passCommand.first) ||
                !getResponse(passCommand.second))
            return false;

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

    bool getResponse(int expectedResponseCode = -1) {
        if (expectedResponseCode != -1
                || expectedResponseCode < 100
                || expectedResponseCode > 599) {
            emit message("Wrong expected response code on command");
            return false;
        }

        if (!m_tcpSocker.isOpen()) {
            emit message("Socket is not open");
            return false;
        }

        if (m_tcpSocker.waitForReadyRead(m_timeout)) {
            auto responseData = m_tcpSocker.readAll();

            emit responseFromFtp(responseData);

            int codeSize = 3;

            if (expectedResponseCode != -1 && responseData.size() > 4
                    && responseData.mid(0, codeSize).toInt() != expectedResponseCode) {
                emit message("FAILED. Expected respons code is "
                             + QString::number(expectedResponseCode)
                             + ", but got " + responseData.mid(0, 3).toInt());
                return false;
            }


            if (responseData[codeSize] == '-')
                return getResponse(expectedResponseCode);

            return true;
        }
        else {
            emit message("Failed to get a response from the FTP server.");
            return false;
        }

    }

    bool closeSocket() {
        closeConnect();
        m_tcpSocker.close();
        if (!m_tcpSocker.waitForDisconnected(m_timeout)) {
            emit message("Disconnection failed. Error "
                         + QString::number(m_tcpSocker.error()));
            return false;
        }

        return true;
    }
public:
signals:
    void message(QString msg);
    void responseFromFtp(QString response);
    void commandChecked(QString command, bool success);

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

    // AbstractDeviceChecker interface
    void connectToDevice(const DeviceConnectionData &deviceData) override;
    void checkCommands(const QString &fileName) override;
    void closeConnection() override;

private:
    void onCommandChecked(QString command, bool success);

private:
    FtpConnectionWorker *m_connectionWorker = nullptr;
    QThread m_workerThread;
};

}

#endif // FTPSERVERCHECKER_H
