import QtQuick 2.0
import QtQuick.Controls 2.2

FocusScope
{

    id: sideBarId

    property int openAnimationDurationMs: 500
    property int closeAnimationDurationMs: 500
    property int maxBarWidth: 300

    //------------------------------------------------------------------------------

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
        property bool isFullyClosed: progress == 0
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
        sideBarId.focus = true
    }

    function close()
    {
        if( !privates.isOpen )
            return

        privates.isOpen = false
        sideBarId.state = "CLOSED"
        sideBarId.focus = false
    }

    //------------------------------------------------------------------------------

    MouseArea
    {
        anchors.fill: parent
        visible: !privates.isFullyClosed

        Rectangle
        {
            id: barRectId
            x: -width + ( privates.realProgress * width )
            width: privates.maxWidth
            height: parent.height
            color: "red"


            // Contents
            Flickable
            {
                anchors.fill: parent
                contentHeight: contentsColumnId.height

                Column
                {
                    id: contentsColumnId
                    width: parent.width

                    Button{ text: "Close sidebar"; width: parent.width; onClicked: { close() } }
                    Button{ text: "Button 2"; width: parent.width }
                    Button{ text: "Button 3"; width: parent.width }
                    Button{ text: "Button 4"; width: parent.width }
                    Button{ text: "Button 5"; width: parent.width }
                    Button{ text: "Button 6"; width: parent.width }
                    Button{ text: "Button 7"; width: parent.width }
                    Button{ text: "Button 8"; width: parent.width }
                    Button{ text: "Button 9"; width: parent.width }
                    Button{ text: "Button 10"; width: parent.width }
                    Button{ text: "Button 11"; width: parent.width }
                    Button{ text: "Button 12"; width: parent.width }
                    Button{ text: "Button 13"; width: parent.width }
                    Button{ text: "Button 14"; width: parent.width }
                    Button{ text: "Button 15"; width: parent.width }
                    Button{ text: "Button 16"; width: parent.width }
                    Button{ text: "Button 17"; width: parent.width }
                    Button{ text: "Button 18"; width: parent.width }
                    Button{ text: "Button 19"; width: parent.width }
                    Button{ text: "Button 20"; width: parent.width }
                }
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
