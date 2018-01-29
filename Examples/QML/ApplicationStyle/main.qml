import QtQuick 2.10
import QtQuick.Window 2.10
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Window {

    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Flickable
    {
        anchors.fill: parent
        flickableDirection: Flickable.VerticalFlick


        Column
        {
            Button
            {
                text: "Test button"
            }

            Switch
            {
                text: "Test switch"
            }

            Slider
            {

            }

            Frame
            {
                Column
                {
                    CheckBox
                    {
                        text: "Checkbox one"
                    }

                    CheckBox
                    {
                        text: "Checkbox two"
                    }

                    CheckBox
                    {
                        text: "Checkbox three"
                    }
                }
            }


        }

    }
}
