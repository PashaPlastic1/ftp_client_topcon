#ifndef BACKENDMEDIATOR_H
#define BACKENDMEDIATOR_H

#include <QObject>
#include <QQmlContext>

class GuiBackend;

namespace DeviceChecker {
class AbstractDeviceChecker;
}

class ScriptFileParser;

class BackendMediator : public QObject
{
    Q_OBJECT
public:
    explicit BackendMediator(QQmlContext *qmlContext, QObject *parent = nullptr);

    void connectInit();

signals:

private:
    GuiBackend *m_guiBackend = nullptr;
    DeviceChecker::AbstractDeviceChecker *m_deviceChecker = nullptr;
    ScriptFileParser *m_scriptParser = nullptr;
};

#endif // BACKENDMEDIATOR_H
