import QtQuick 2.9
import QtQuick.Window 2.2

Window {

    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Text
    {
        id: textId
        anchors.centerIn: parent
        text: "Waiting..."
    }

    // Connection class is used to connect C++ signals to our QML code
    // In this case our C++ signal is called "somethingHappened( QString something )"
    // and so the signal handler is "onSomethingHappened"
    Connections
    {
        target: testCppClass
        onSomethingHappened:
        {
            textId.text = something
        }
    }
}
