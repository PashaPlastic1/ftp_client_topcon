#ifndef BACKENDMEDIATOR_H
#define BACKENDMEDIATOR_H

#include <QObject>
#include "GuiBackend.h"

class BackendMediator : public QObject
{
    Q_OBJECT
public:
    explicit BackendMediator(QObject *parent = nullptr);

signals:

private:
    GuiBackend guiBackend;

};

#endif // BACKENDMEDIATOR_H
