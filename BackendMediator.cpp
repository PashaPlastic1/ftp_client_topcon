#include "BackendMediator.h"

#include "GuiBackend.h"
#include "device_checker/FtpServerChecker.h"
#include "device_checker/ScriptFileParser.h"

using namespace DeviceChecker;

BackendMediator::BackendMediator(QQmlContext *qmlContext, QObject *parent)
    : QObject{parent}
    , m_guiBackend{new GuiBackend(this)}
    , m_deviceChecker{new FtpServerChecker(this)}
    , m_scriptParser(new ScriptFileParser(this))
{
    qRegisterMetaType<GuiBackend*>();
    qmlContext->setContextProperty("GuiBackend", m_guiBackend);

    connectInit();
}

void BackendMediator::connectInit()
{
    connect(m_guiBackend, &GuiBackend::connectToDevice,
            this, [&](){
        FtpConnectionData connectionData{m_guiBackend->serverUrl(),
                    m_guiBackend->userName(),
                    m_guiBackend->password()};

        m_deviceChecker->connectToDevice(connectionData);
    });
}
