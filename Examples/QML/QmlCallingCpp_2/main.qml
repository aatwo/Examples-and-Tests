import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

Window {

    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Button {

        text:       "Button One"
        x:          parent.x
        y:          parent.y
        width:      parent.width * 0.5
        height:     parent.height * 0.5

        onClicked: {

            testObject.testFunctionOne()
        }
    }

    Button {

        text:       "Button Two"
        x:          parent.x + parent.width * 0.5
        y:          parent.y
        width:      parent.width * 0.5
        height:     parent.height * 0.5

        onClicked: {

            testObject.testFunctionOne()
            testObject.testFunctionTwo()
        }
    }

    Button {

        property real widthMultiplier: testObject.myProperty
        property real animatedWidthMultiplier: 1.0

        id:         buttonThree
        text:       "Button Three"
        x:          parent.width * 0.5 - parent.width * animatedWidthMultiplier * 0.5
        y:          parent.y + parent.height * 0.5
        width:      parent.width * animatedWidthMultiplier
        height:     parent.height * 0.5

        onWidthMultiplierChanged: {

            widthAnimation.start()
        }

        SequentialAnimation on animatedWidthMultiplier {

            id: widthAnimation
            NumberAnimation { loops: 1; easing.type: Easing.OutElastic; to: buttonThree.widthMultiplier; duration: 1000 }
        }
    }
}
