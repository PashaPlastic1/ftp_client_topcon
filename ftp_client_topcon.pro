QT += quick network qml

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    GuiBackend.h \
    BackendMediator.h \
    device_checker/AbstractDeviceChecker.h \
    device_checker/FtpServerChecker.h \
    device_checker/FtpConnectionWorker.h \
    device_checker/ScriptFileParser.h

SOURCES += \
    main.cpp \
    GuiBackend.cpp \
    BackendMediator.cpp \
    device_checker/AbstractDeviceChecker.cpp \
    device_checker/FtpServerChecker.cpp \
    device_checker/FtpConnectionWorker.cpp \
    device_checker/ScriptFileParser.cpp

RESOURCES += qml.qrc

BUILD_DIR = $$OUT_PWD
DESTDIR = $$BUILD_DIR/bin
message($$DESTDIR)

UI_DIR = uics
MOC_DIR = mocs
OBJECTS_DIR = objs

TARGET = ftp_client_topcon

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
