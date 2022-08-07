#ifndef ABSTRACTDEVICECHECKER_H
#define ABSTRACTDEVICECHECKER_H

#include <QObject>

namespace DeviceChecker {

enum DeviceType {
    Ftp
};
//-----------------------------------------------------
/*!
    \brief Структура с полями для инициализации подключения
    \brief к устройству.

    Поля определяются в структурах-наследника.
*/
struct DeviceConnectionData {
    DeviceType deviceType() const {return m_deviceType;}
protected:
    DeviceConnectionData(DeviceType deviceType)
        : m_deviceType(deviceType)
    {}

    DeviceType m_deviceType;
};
//-----------------------------------------------------
/*!
    \brief Наследник DeviceConnectionData.

    Определяет требуемые поля для подключения к FTP-серверу.
*/
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
//-----------------------------------------------------
/*!
    \brief Абстрактный класс, определяющий интерфейс взаимодейвия
    \brief с устройством.
*/
class AbstractDeviceChecker : public QObject
{
    Q_OBJECT
public:
    enum ConnectionStates {
        WaitConnection = 0,
        Connected,
        ConnectionError,
        SignInError
    };

    explicit AbstractDeviceChecker(QObject *parent = nullptr);
    virtual ~AbstractDeviceChecker(){}

    //! Подключится к устройству
    //! \param[in] deviceData - структура с полями, требуемые для подкючения
    virtual void connectToDevice(const DeviceConnectionData &deviceData) = 0;
    //! Провести проверку команд
    //! \param[in] deviceData - first - команда для устройства, second - ожидаемые ответ
    virtual void checkCommands(const QVector<QPair<QString, QString>> &fileName) = 0;
    //! Закрыть соединение с устройством
    virtual void closeConnection() = 0;

signals:
    //! Сигнал эмититься при изменения состояния взаимодействия с устройством
    void connectionStateChanged(AbstractDeviceChecker::ConnectionStates state);
    //! Сигнал для отправки сервисного сообщения
    void serviceMsg(const QString &msg);
    //! Сигнал для отправки ответа от устройства
    void deviceResponse(const QString &response);
    //! Сигнал c результатами проверки команд
    //! \param[in] counter.first - количество успешно прошедших команд по сценарию
    //! \param[in] counter.second - количество команд
    void allCommandsChecked(const QPair<int, int> &counter);
};

}

#endif // ABSTRACTDEVICECHECKER_H
