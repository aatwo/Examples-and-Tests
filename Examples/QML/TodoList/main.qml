import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

ApplicationWindow {

    visible: true
    width: 640
    height: 480
    title: qsTr("Todo List")

    TodoList {

        anchors.fill: parent
    }
}
