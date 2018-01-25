import QtQuick 2.4
import QtQuick.Controls 2.2

Item {
    id: item1
    width: 400
    height: 400
    property alias myButton: myButton

    Button {
        id: myButton
        x: 67
        y: 132
        width: 334
        height: 137

        text: "TestButton"
        anchors.right: parent.right
        anchors.rightMargin: 33
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        checkable: false
    }
}
