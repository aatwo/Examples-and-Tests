import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

Window {

    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    function myFunction() {

        myButton.text = "Hello, World!"
    }

    Button {

        id: myButton
        anchors.fill: parent
        text: "Click me"

        onClicked: {

            myFunction()
        }
    }
}
