import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import Qt.labs.platform 1.1

ApplicationWindow {
    width: 640
    height: 500
    visible: true
    title: qsTr("ftp client")

    Connections {
        target: GuiBackend
        function onServiceMessage(msg) {
            outputArea.text = outputArea.text + msg + "\n";
        }
        function onDeviceResponse(msg) {
            outputArea.text = outputArea.text + ">> " + msg + "\n";
        }
    }

    GridLayout {
        property int rowHeight: 44
        anchors.fill: parent
        anchors.margins: 5

        columns: 3

        TextField {
            id: serverUrlField
            Layout.fillWidth: true
            Layout.preferredHeight: parent.rowHeight
            Layout.columnSpan: 2
            placeholderTextColor : Material.color(Material.Grey)
            selectByMouse: true
            text: GuiBackend.serverUrl
            onEditingFinished: GuiBackend.serverUrl = text

            placeholderText: "FTP server ip or url"
        }

        Button {
            id: signInButton
            text: "sign in"
            Layout.fillWidth: true
            enabled: serverUrlField.text
                        && userNameField.text
                        && passwordField.text

            Layout.rowSpan: 2

            onClicked: GuiBackend.emitConnectToDevice()
        }

        TextField {
            id: userNameField
            Layout.fillWidth: true
            Layout.preferredHeight: parent.rowHeight
            placeholderTextColor : Material.color(Material.Grey)
            selectByMouse: true
            text: GuiBackend.userName
            onEditingFinished: GuiBackend.userName = text

            placeholderText: "user name"
        }

        TextField {
            id: passwordField
            Layout.fillWidth: true
            Layout.preferredHeight: parent.rowHeight
            placeholderTextColor : Material.color(Material.Grey)
            selectByMouse: true
            text: GuiBackend.password
            onEditingFinished: GuiBackend.password = text

            placeholderText: "password"
        }


        TextField {
            id: fileNameField
            Layout.fillWidth: true
            Layout.columnSpan: 2
            Layout.preferredHeight: parent.rowHeight
            placeholderTextColor : Material.color(Material.Grey)
            selectByMouse: true
            text: GuiBackend.fullFileName
            onTextChanged: GuiBackend.fullFileName = text
            onEditingFinished: GuiBackend.fullFileName = text

            placeholderText: "enter the full file name"
        }

        Button {
            id: browseButton
            text: "browse..."
            Layout.fillWidth: true

            onClicked: fileDialog.open()

            FileDialog {
                id: fileDialog
                defaultSuffix: "fdk"
                fileMode: FileDialog.OpenFile
                title: "Выберите файл"
                nameFilters: ["(*.txt)"]
                onAccepted: {
                    fileNameField.text = file.toString().slice(8);
                }
            }
        }

        Button {
            id: startButton
            text: "start"
            Layout.fillWidth: true
            Layout.columnSpan: 3

            enabled: serverUrlField.text
                        && userNameField.text
                        && passwordField.text
                        && fileNameField.text

            onClicked: GuiBackend.emitStartOperation()
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.columnSpan: 3
            TextArea {
                id: outputArea

                selectByMouse: true

                readOnly: true

            }
        }
    }
}
