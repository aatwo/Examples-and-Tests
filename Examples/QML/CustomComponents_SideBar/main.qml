import QtQuick 2.9
import QtQuick.Controls 2.2

ApplicationWindow {

    id: windowId
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Button
    {
        anchors.centerIn: parent
        text: "Toggle side bar"
        onClicked:
        {
            if( sideBarId.isOpen() )
                sideBarId.close()

            else
                sideBarId.open()
        }
    }

    SideBar
    {
        id: sideBarId
    }


    // The following code uses the Qt Quick Drawer component to provide a side bar
    //    Drawer
    //    {
    //        property int maxWidth: Math.min( 300, windowId.width )

    //        width: maxWidth
    //        height: windowId.height

    //        Text
    //        {
    //            anchors.centerIn: parent
    //            text: "DRAWER"
    //        }
    //    }
}
