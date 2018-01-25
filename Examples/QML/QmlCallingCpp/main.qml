import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

Window {

    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Button {

        text:       "Button One"
        x:          parent.x
        y:          parent.y
        width:      parent.width
        height:     parent.height * 0.5

        onClicked: {

            testObject.testFunctionOne()
        }
    }

    Button {

        text:       "Button Two"
        x:          parent.x
        y:          parent.y + parent.height * 0.5
        width:      parent.width
        height:     parent.height * 0.5

        onClicked: {

            testObject.testFunctionTwo()
        }
    }
}
