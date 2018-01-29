import QtQuick 2.0
import QtQuick.Controls 2.2

MouseArea
{
    property int openAnimationDurationMs: 500
    property int closeAnimationDurationMs: 500
    property int maxBarWidth: 300

    id: sideBarId
    x: -privates.maxWidth + ( privates.progress * privates.maxWidth )
    width: privates.maxWidth
    height: parent.height

    function isOpen()
    {
        return privates.isOpen
    }

    function open()
    {
        if( privates.isOpen )
            return

        if( closeAnimationId.running )
            closeAnimationId.stop()

        openAnimationId.start()
        privates.isOpen = true

        print( "Side bar opening..." )
    }

    function close()
    {
        if( !privates.isOpen )
            return

        if( openAnimationId.running )
            openAnimationId.stop()

        closeAnimationId.start()
        privates.isOpen = false
        print( "Side bar closing..." )
    }

    QtObject
    {
        id: privates

        property bool isOpen: false
        property real progress: 0.0
        property int maxWidth: Math.min( sideBarId.maxBarWidth,
                                        sideBarId.parent.width )
    }


    Rectangle
    {
        anchors.fill: parent
        color: "red"

        Text
        {
            anchors.centerIn: parent
            text: "CUSTOM DRAWER"
        }
    }


    NumberAnimation
    {
        id: openAnimationId
        target: privates
        property: "progress"
        to: 1.0
        duration: Math.abs( 1.0 - privates.progress ) * openAnimationDurationMs
        easing.type: Easing.OutQuad
    }

    NumberAnimation
    {
        id: closeAnimationId
        target: privates
        property: "progress"
        to: 0.0
        duration: Math.abs( 0.0 - privates.progress ) * openAnimationDurationMs
        easing.type: Easing.OutQuad
    }

    //    SequentialAnimation
    //    {
    //        running: true
    //        loops: Animation.Infinite

    //        NumberAnimation
    //        {
    //            target: privates
    //            property: "progress"
    //            to: 0.0
    //            duration: 1000
    //        }

    //        NumberAnimation
    //        {
    //            target: privates
    //            property: "progress"
    //            to: 1.0
    //            duration: 1000
    //        }
    //    }

}
