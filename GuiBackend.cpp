#include "GuiBackend.h"

GuiBackend::GuiBackend(QObject *parent)
    : QObject{parent}
{

}

void GuiBackend::emitConnectToDevice()
{
    emit connectToDevice();
}

void GuiBackend::emitStartOperation()
{
    emit startOperation(m_fullFileName);
}


const QString &GuiBackend::serverUrl() const
{
    return m_serverUrl;
}

void GuiBackend::setServerUrl(const QString &newServerUrl)
{
    if (m_serverUrl == newServerUrl)
        return;

    m_serverUrl = newServerUrl;
    emit serverUrlChanged();
}

const QString &GuiBackend::userName() const
{
    return m_userName;
}

void GuiBackend::setUserName(const QString &newUserName)
{
    if (m_userName == newUserName)
        return;

    m_userName = newUserName;
    emit userNameChanged();
}

const QString &GuiBackend::password() const
{
    return m_password;
}

void GuiBackend::setPassword(const QString &newPassword)
{
    if (m_password == newPassword)
        return;

    m_password = newPassword;
    emit passwordChanged();
}

const QString &GuiBackend::fullFileName() const
{
    return m_fullFileName;
}

void GuiBackend::setFullFileName(const QString &newFullFileName)
{
    if (m_fullFileName == newFullFileName)
        return;

    m_fullFileName = newFullFileName;
    emit fullFileNameChanged();
}

void GuiBackend::setServiceMessage(const QString &msg)
{
    emit serviceMessage(msg);
}

void GuiBackend::setDeviceResponse(const QString &response)
{
    emit deviceResponse(response);
}

