import QtQuick 2.9
import QtQuick.Window 2.2

Window {

    id: windowId
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    function testFunction( value )
    {
        print( "Hello from javascript: " + value )
        textId.text = value
        return 10
    }

    Text
    {
        id: textId
        anchors.centerIn: parent
    }
}
