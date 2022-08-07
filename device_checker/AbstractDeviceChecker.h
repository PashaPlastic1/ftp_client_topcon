#ifndef ABSTRACTDEVICECHECKER_H
#define ABSTRACTDEVICECHECKER_H

#include <QObject>

namespace DeviceChecker {

enum DeviceType {
    Ftp
};

struct DeviceConnectionData {
    DeviceType deviceType() const {return m_deviceType;}
protected:
    DeviceConnectionData(DeviceType deviceType)
        : m_deviceType(deviceType)
    {}

    DeviceType m_deviceType;
};

class AbstractDeviceChecker : public QObject
{
    Q_OBJECT
public:
    enum ConnectionStates {
        WaitConnection = 0,
        Connected,
        ConnectionError,
        SignInError,
    };

    explicit AbstractDeviceChecker(QObject *parent = nullptr);
    virtual ~AbstractDeviceChecker(){}

    virtual void connectToDevice(const DeviceConnectionData &deviceData) = 0;
    virtual void checkCommands(const QVector<QPair<QString, QString>> &fileName) = 0;
    virtual void closeConnection() = 0;

signals:
    void connectionStateChanged(ConnectionStates state);
    void serviceMsg(const QString &msg);
    void deviceResponse(const QString &response);
    void allCommandsChecked(const QPair<int, int> &counter);
};

}

#endif // ABSTRACTDEVICECHECKER_H
