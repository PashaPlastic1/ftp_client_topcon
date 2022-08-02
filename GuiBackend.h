#ifndef GUIBACKEND_H
#define GUIBACKEND_H

#include <QObject>

class GuiBackend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString serverUrl READ serverUrl WRITE setServerUrl NOTIFY serverUrlChanged)
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QString fullFileName READ fullFileName WRITE setFullFileName NOTIFY fullFileNameChanged)
public:
    explicit GuiBackend(QObject *parent = nullptr);

    Q_INVOKABLE void emitConnectToDevice();
    Q_INVOKABLE void emitStartOperation();

    const QString &serverUrl() const;
    void setServerUrl(const QString &newServerUrl);

    const QString &userName() const;
    void setUserName(const QString &newUserName);

    const QString &password() const;
    void setPassword(const QString &newPassword);

    const QString &fullFileName() const;
    void setFullFileName(const QString &newFullFileName);

signals:
    void serverUrlChanged();
    void userNameChanged();
    void passwordChanged();
    void fullFileNameChanged();

    void connectToDevice();
    void startOperation(const QString &comandsFileName);

private:
    QString m_serverUrl;
    QString m_userName;
    QString m_password;
    QString m_fullFileName;
};

#endif // GUIBACKEND_H
