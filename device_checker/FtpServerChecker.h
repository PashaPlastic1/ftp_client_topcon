#ifndef FTPSERVERCHECKER_H
#define FTPSERVERCHECKER_H

#include "AbstractDeviceChecker.h"
#include <QThread>
#include <queue>

namespace DeviceChecker {

class FtpConnectionData;
class FtpConnectionWorker;

/*!
    \brief Наследник от AbstractDeviceChecker для проверки FTP-сервера

*/
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
    // Сигналы для общения с рабочим
    //! Подключится к FTP-серваку
    //! \param[in] ftpData - параметры подключения
    void signalConnectToDevice(DeviceChecker::FtpConnectionData);
    //! Закрыть соединение с FTP-сервером
    void signalDisconnect();
    //! Проверить команду
    //! \param[in] command.first - команда
    //! \param[in] command.second - ожидаемый ответ (используются коды или их часть)
    void signalCheckCommand(QPair<QString, QString>);
    //! Очистить буффер TCP-сокета
    void clearBuffer();

private:
    //! Слот для обработки результата тестирования команды после signalCheckCommand()
    //! \param[in] command - проверенная команда
    //! \param[in] success - успех проверки
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
