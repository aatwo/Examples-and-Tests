import QtQuick 2.0
import QtQuick.Controls 2.2

MouseArea
{
    property int openAnimationDurationMs: 500
    property int closeAnimationDurationMs: 500
    property int maxBarWidth: 300

    //------------------------------------------------------------------------------

    id: sideBarId
    state: "CLOSED"
    width: parent.width
    height: parent.height
    x: 0
    y: 0

    QtObject
    {
        id: privates

        property int maxWidth: Math.min( sideBarId.maxBarWidth, sideBarId.parent.width )
        property int progress: 0
        property int progressMax: 1000
        property real realProgress: progress / progressMax
        property bool isOpen: false
    }

    onClicked:
    {
        if( isOpen() )
            close()

        else
            open()
    }

    //------------------------------------------------------------------------------

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

    //------------------------------------------------------------------------------

    Rectangle
    {
        id: barRectId

        x: -width + ( privates.realProgress * width )
        width: privates.maxWidth
        height: parent.height

        color: "red"

        Text
        {
            anchors.centerIn: parent
            color: "white"
            text: parent.width
        }
    }

    Rectangle
    {
        id: shadowRectId

        x: barRectId.x + barRectId.width
        width: parent.width - ( privates.realProgress * barRectId.width )
        height: parent.height
        color: "black"
        opacity: privates.realProgress * 0.5

        Text
        {
            anchors.centerIn: parent
            color: "white"
            text: parent.width
        }
    }

    //------------------------------------------------------------------------------

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
