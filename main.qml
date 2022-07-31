import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15

ApplicationWindow {
    width: 640
    height: 500
    visible: true
    title: qsTr("ftp client")

    GridLayout {
        property int rowHeight: 44
        anchors.fill: parent

        columns: 3

        TextField {
            id: serverUrlField
            Layout.fillWidth: true
            Layout.preferredHeight: parent.rowHeight
            Layout.columnSpan: 2
            placeholderTextColor : Material.color(Material.Grey)

            placeholderText: "FTP server ip or url"
        }

        Button {
            id: signInButton
            text: "sign in"
            Layout.fillWidth: true

//            Layout.row: 0
//            Layout.column: 2
            Layout.rowSpan: 2
        }

        TextField {
            id: userNameField
            Layout.fillWidth: true
            Layout.preferredHeight: parent.rowHeight
            placeholderTextColor : Material.color(Material.Grey)

            placeholderText: "user name"
        }

        TextField {
            id: passwordField
            Layout.fillWidth: true
            Layout.preferredHeight: parent.rowHeight
            placeholderTextColor : Material.color(Material.Grey)

            placeholderText: "password"
        }


        TextField {
            id: fileNameField
            Layout.fillWidth: true
            Layout.columnSpan: 2
            Layout.preferredHeight: parent.rowHeight
            placeholderTextColor : Material.color(Material.Grey)


            placeholderText: "enter the full file name"
        }

        Button {
            id: browseButton
            text: "browse..."
            Layout.fillWidth: true
        }

        Button {
            id: startButton
            text: "start"
            Layout.fillWidth: true
            Layout.columnSpan: 3
        }

        TextArea {
            id: outputArea
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.columnSpan: 3

            readOnly: true

        }
    }
}
