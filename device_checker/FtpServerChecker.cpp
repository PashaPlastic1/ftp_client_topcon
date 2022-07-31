#include "FtpServerChecker.h"

using namespace DeviceChecker;

FtpServerChecker::FtpServerChecker(QObject *parent)
    : AbstractDeviceChecker{parent}
{

}

void DeviceChecker::FtpServerChecker::connectToDevice(const DeviceConnectionData &deviceData)
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
            this, &FtpServerChecker::onCommandChecked);
}

void FtpServerChecker::checkCommands(const QString &fileName)
{

}

void FtpServerChecker::closeConnection()
{

}

void FtpServerChecker::onCommandChecked(QString command, bool success)
{

}
