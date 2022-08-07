#ifndef FTPCONNECTIONWORKER_H
#define FTPCONNECTIONWORKER_H

#include <QTcpSocket>
#include <QThread>

#include "AbstractDeviceChecker.h"

namespace DeviceChecker {
/*!
    \brief Рабочий для взаимодействия с FTP-сервером

*/
class FtpConnectionWorker : public QObject {
    Q_OBJECT
public:
    explicit FtpConnectionWorker(QObject *parent = nullptr);
    ~FtpConnectionWorker();

    //! Подключится к FTP-серверу
    //! \param[in] ftp - параметры подключения
    //! \param[out] успех
    bool connect(FtpConnectionData ftpData);
    //! Закрыть соединение с FTP-сервером
    void closeConnect();
    //! Проверить команду
    //! \param[in] command.first - команда
    //! \param[in] command.second - ожидаемый ответ (используются коды или их часть)
    void checkCommand(QPair<QString, QString> command);
    //! Очистить буффер TCP-сокета
    void clearBuffer();

private:
    //! Авторизоваться на FTP-сервере
    //! \param[out] успех
    bool signIn();
    //! Отправить команду на FTP-сервер
    //! \param[in] command - команда
    //! \param[out] успех
    bool sendCommand(const QString &command);
    //! Получить ответ от FTP-сервера
    //! \param[in] expectedResponseCode - ожидаемый код первого сообщения (или его часть)
    //! \param[out] успех
    bool getResponse(const QString &expectedResponseCode = "");
    //! Закрыть соединение с сервером и сокет
    bool closeSocket();

private:
signals:
    //! Сигнал с сервисным сообщением
    //! \param[in] msg - сервисное сообщение
    void message(QString msg);
    //! Сигнал с ответом от FTP-сервера
    //! \param[in] response - ответ от FTP-сервера
    void responseFromFtp(QString response);
    //! Сигнал с результатом тестирования команды после sendCommand()
    //! \param[in] command - проверенная команда
    //! \param[in] success - успех проверки
    void commandChecked(QString command, bool success);

    //! Сигнал эмитится в случае провала во время подключения к FTP-серверу
    void connectionError();

private:
    QTcpSocket m_tcpSocker;
    FtpConnectionData m_ftpConnectionData;
    int m_timeout {3000};
};

}

#endif // FTPCONNECTIONWORKER_H
