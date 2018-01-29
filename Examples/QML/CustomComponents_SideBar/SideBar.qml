import QtQuick 2.0
import QtQuick.Controls 2.2

MouseArea
{
    property int openAnimationDurationMs: 500
    property int closeAnimationDurationMs: 500
    property int maxBarWidth: 300

    id: sideBarId
    state: "CLOSED"

    //x: privates.progress > 0.0
    //width: parent.width
    height: parent.height
    x: -privates.maxWidth + ( privates.realProgress * privates.maxWidth )
    width: privates.maxWidth

    function isOpen()
    {
        return privates.isOpen
    }

    function open()
    {
        if( privates.isOpen )
            return

        privates.isOpen = true
        sideBarId.state = "OPEN"

        print( "Side bar opening..." )
    }

    function close()
    {
        if( !privates.isOpen )
            return

        privates.isOpen = false
        sideBarId.state = "CLOSED"

        print( "Side bar closing..." )
    }

    QtObject
    {
        id: privates

        property int maxWidth: Math.min( sideBarId.maxBarWidth, sideBarId.parent.width )
        property int progress: 0
        property int progressMax: 1000
        property real realProgress: progress / progressMax
        property bool isOpen: false
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


    states: [

        State {

            name: "CLOSED"
        },

        State {

            name: "OPEN"
        }

    ]

    transitions: [

        Transition
        {
            from: "CLOSED"
            to: "OPEN"
            NumberAnimation
            {
                target: privates
                property: "progress"
                to: privates.progressMax
                duration: Math.abs( 1.0 - privates.realProgress ) * openAnimationDurationMs
                easing.type: Easing.OutQuad
            }
        },

        Transition
        {
            from: "OPEN"
            to: "CLOSED"
            NumberAnimation
            {
                target: privates
                property: "progress"
                to: 0
                duration: Math.abs( 0.0 - privates.realProgress ) * openAnimationDurationMs
                easing.type: Easing.OutQuad
            }
        }

    ]

}
