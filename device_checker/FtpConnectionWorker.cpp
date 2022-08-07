#include "FtpConnectionWorker.h"

DeviceChecker::FtpConnectionWorker::FtpConnectionWorker(QObject *parent)
    : QObject(parent)
    , m_tcpSocker(this)
{}

DeviceChecker::FtpConnectionWorker::~FtpConnectionWorker() {
    closeSocket();
}

bool DeviceChecker::FtpConnectionWorker::connect(FtpConnectionData ftpData) {
    if (!closeSocket())
        return false;

    const quint16 ftpPort {21};

    m_tcpSocker.connectToHost(ftpData.m_url, ftpPort);

    if (!m_tcpSocker.waitForConnected(m_timeout)) {
        m_tcpSocker.close();
        emit message("Connection failed. Error "
                     + QString::number((int)m_tcpSocker.error())
                     + ". Check url/ip of server.");
        emit connectionError();
        return false;
    }

    emit message("Connected to " + ftpData.m_url);

    getResponse("220");

    m_ftpConnectionData = ftpData;

    if (!signIn()) {
        emit message("Sing In error.");
        emit connectionError();
        return false;
    }

    emit message("Signed in! Now you can test your scenario.");
    return true;
}

void DeviceChecker::FtpConnectionWorker::closeConnect() {
    if (!m_tcpSocker.isOpen())
        return;

    const QPair<QString, QString> commandCode {"QUIT", "221"};

    if (!sendCommand(commandCode.first)) {
        emit message("Failed to quit from FTP server. ");
    }

    getResponse();
}

void DeviceChecker::FtpConnectionWorker::checkCommand(QPair<QString, QString> command) {
    if (!m_tcpSocker.isOpen()) {
        emit message("There's no connection.");
        emit commandChecked(command.first, false);
        return;
    }

    emit message("\nChecking command: " + command.first + "...");
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

void DeviceChecker::FtpConnectionWorker::clearBuffer() {
    m_tcpSocker.readAll();
}

bool DeviceChecker::FtpConnectionWorker::signIn() {
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

bool DeviceChecker::FtpConnectionWorker::sendCommand(const QString &command) {
    m_tcpSocker.write((command + "\r\n").toUtf8().constData());
    if (!m_tcpSocker.waitForBytesWritten(m_timeout)) {
        emit message("Failed send command: " + command);
        return false;
    }

    return true;
}

bool DeviceChecker::FtpConnectionWorker::getResponse(const QString &expectedResponseCode) {
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
            emit message("FAILED. Expected response was \""
                         + expectedResponseCode
                         + "\", but got \"" + responseData.mid(0, expectedResponseCode.size())
                         + "\" in the response from the FTP server.");
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

bool DeviceChecker::FtpConnectionWorker::closeSocket() {
    if (!m_tcpSocker.isOpen())
        return true;

    closeConnect();
    m_tcpSocker.close();

    return true;
}

