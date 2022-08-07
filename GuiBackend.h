#ifndef GUIBACKEND_H
#define GUIBACKEND_H

#include <QObject>

/*!
    \brief Класс для взаимодействия с qml-слоем
*/
class GuiBackend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString serverUrl READ serverUrl WRITE setServerUrl NOTIFY serverUrlChanged)
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QString fullFileName READ fullFileName WRITE setFullFileName NOTIFY fullFileNameChanged)
public:
    explicit GuiBackend(QObject *parent = nullptr);

    // From GUI
    //! Эмитет сигнал для подключения к устройству
    Q_INVOKABLE void emitConnectToDevice();
    //! Эмитет сигнал для запуска подготовленного
    //! сценария тестирования устройства
    Q_INVOKABLE void emitStartOperation();

    const QString &serverUrl() const;
    void setServerUrl(const QString &newServerUrl);

    const QString &userName() const;
    void setUserName(const QString &newUserName);

    const QString &password() const;
    void setPassword(const QString &newPassword);

    const QString &fullFileName() const;
    void setFullFileName(const QString &newFullFileName);

    // To GUI
    //! Отправить на GUI сервисное сообщение (не от устройтва)
    void setServiceMessage(const QString &msg);
    //! Отправить на GUI ответ от устройства
    void setDeviceResponse(const QString &response);

signals:
    void serverUrlChanged();
    void userNameChanged();
    void passwordChanged();
    void fullFileNameChanged();

    //! Сигнал для подключения к устройству
    void connectToDevice();
    //! Сигнал для запуска подготовленного
    //! сценария тестирования устройства
    //! \param[in] comandsFileName - полное имя файла со сценарием
    void startOperation(const QString &comandsFileName);

    //! Отправиляет на GUI сервисное сообщение (не от устройтва)
    //! \param[in] msg - сообщение
    void serviceMessage(const QString &msg);
    //! Отправиляет на GUI ответ от устройства
    //! \param[in] msg - ответ от устройства
    void deviceResponse(const QString &response);

private:
    QString m_serverUrl{"ftp.gnu.org"};
    QString m_userName{"anonymous"};
    QString m_password{"test@mail.ru"};
    QString m_fullFileName;
};

#endif // GUIBACKEND_H
