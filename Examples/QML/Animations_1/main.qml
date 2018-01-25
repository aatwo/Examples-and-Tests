import QtQuick 2.9
import QtQuick.Window 2.2

Window {

    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Rectangle {

        property real animationProgress: 0.0

        x:          parent.width * 0.5 - parent.width * animationProgress * 0.5
        y:          parent.y
        width:      parent.width * animationProgress
        height:     parent.height
        color:      "#BBFF63"

        SequentialAnimation on animationProgress {

            loops: Animation.Infinite
            NumberAnimation { easing.type: Easing.OutElastic; to: 1; duration: 1500 }
            NumberAnimation { easing.type: Easing.OutElastic; to: 0; duration: 1500 }
        }

        Text {

            anchors.centerIn: parent
            text: "Hello, World!"

            RotationAnimation on rotation {

                loops: Animation.Infinite
                from: 0
                to: 360
                duration: 1000

            }

        }

    }

}
