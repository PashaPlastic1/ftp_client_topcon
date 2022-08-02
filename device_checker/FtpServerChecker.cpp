#include "FtpServerChecker.h"

using namespace DeviceChecker;

FtpServerChecker::FtpServerChecker(QObject *parent)
    : AbstractDeviceChecker{parent}
{

}

FtpServerChecker::~FtpServerChecker()
{
    closeConnection();
}

void FtpServerChecker::connectToDevice(const DeviceConnectionData &deviceData)
{
    Q_ASSERT(deviceData.deviceType() == DeviceType::Ftp);

    if (!m_connectionWorker)
        m_connectionWorker = new FtpConnectionWorker;

    m_connectionWorker->moveToThread(&m_workerThread);
    connect(&m_workerThread, &QThread::finished,
            m_connectionWorker, &QObject::deleteLater);
    connect(m_connectionWorker, &FtpConnectionWorker::message,
            this, [&](QString msg) {emit serviceMsg(msg);});
    connect(m_connectionWorker, &FtpConnectionWorker::responseFromFtp,
            this, [&](QString response) {emit deviceResponse(response);});
    connect(m_connectionWorker, &FtpConnectionWorker::commandChecked,
            this, &FtpServerChecker::onCommandChecked);

    connect(this, &FtpServerChecker::signalConnectToDevice,
            m_connectionWorker, &FtpConnectionWorker::connect);
    connect(this, &FtpServerChecker::signalDisconnect,
            m_connectionWorker, &FtpConnectionWorker::closeConnect);
    connect(this, &FtpServerChecker::signalCheckCommand,
            m_connectionWorker, &FtpConnectionWorker::checkCommand);

    m_workerThread.start();

    emit signalConnectToDevice(static_cast<const FtpConnectionData&>(deviceData));
}

void FtpServerChecker::checkCommands(const QVector<QPair<QString, QString>> &commands)
{
    for (auto &command : commands)
        m_commandQueue.push(command);

    m_commandSize = commands.size();
    m_commandCounter = 0;

    emit signalCheckCommand(m_commandQueue.front());
}

void FtpServerChecker::closeConnection()
{
    m_workerThread.quit();
    m_workerThread.wait();

    m_connectionWorker = nullptr;
}

void FtpServerChecker::onCommandChecked(QString command, bool success)
{
    if (success)
        ++m_commandCounter;

    m_commandQueue.pop();
    if (!m_commandQueue.empty())
        emit signalCheckCommand(m_commandQueue.front());
    else {
        emit allCommandsChecked({m_commandCounter, m_commandSize});
        m_commandCounter = 0;
        m_commandSize = 0;
        closeConnection();
    }
}
