import QtQuick 2.9
import QtQuick.Controls 2.2

ApplicationWindow {

    id: windowId
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    // Contents
    Rectangle
    {
        anchors.fill: parent
        color: "yellow"
        Button
        {
            text: "Open sidebar"
            anchors.centerIn: parent
            onClicked:
            {
                if( sideBarId.isOpen() )
                    sideBarId.close()
                else
                    sideBarId.open()
            }
        }
    }

    SideBar
    {
        id: sideBarId
    }
}
