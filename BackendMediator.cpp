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
    qRegisterMetaType<FtpConnectionData>();
    qRegisterMetaType<QPair<QString,QString>>();
    qmlContext->setContextProperty("GuiBackend", m_guiBackend);

    connectInit();
}

void BackendMediator::connectInit()
{
    //---------FROM-GUI-----------------
    connect(m_guiBackend, &GuiBackend::connectToDevice,
            this, [&](){
        FtpConnectionData connectionData{m_guiBackend->serverUrl(),
                    m_guiBackend->userName(),
                    m_guiBackend->password()};

        m_deviceChecker->connectToDevice(connectionData);
    });
    connect(m_guiBackend, &GuiBackend::startOperation,
            m_scriptParser, &ScriptFileParser::parseFile);

    //---------FROM-FILE-PARSER----------
    connect(m_scriptParser, &ScriptFileParser::fileParsed,
            m_deviceChecker, &AbstractDeviceChecker::checkCommands);
    connect(m_scriptParser, &ScriptFileParser::serviceMsg,
            m_guiBackend, &GuiBackend::setServiceMessage);

    //---------FROM-DEVICE---------------
    connect(m_deviceChecker, &AbstractDeviceChecker::serviceMsg,
            m_guiBackend, &GuiBackend::setServiceMessage);
    connect(m_deviceChecker, &AbstractDeviceChecker::deviceResponse,
            m_guiBackend, &GuiBackend::setDeviceResponse);
    connect(m_deviceChecker, &AbstractDeviceChecker::allCommandsChecked,
            this, [&](const QPair<int, int> &counter){
        m_guiBackend->setServiceMessage("\nAll commands are checked: "
                                        + QString::number(counter.first) + "/"
                                        + QString::number(counter.second) + " PASSED");
    });
}
