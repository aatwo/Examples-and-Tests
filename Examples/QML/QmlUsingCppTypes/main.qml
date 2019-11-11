import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import Whatever.common 1.0

Window {
    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("QmlUsingCppTypes")

    Column {
        Button {
            width: root.width;
            height: root.height / 3;
            text: "Call Test::TestFunction()"
            onClicked: {
                testObject.TestFunction();
            }
        }
        Button {
            y: root.height / 3;
            width: root.width;
            height: root.height / 3;
            text: "Call Test::TestFunctionEnum()"
            onClicked: {
                testObject.TestFunctionEnum(CommonNamespace.MyEnumValue2);
            }
        }
        Button {
            y: 2 * (root.height / 3);
            width: root.width;
            height: root.height / 3;
            text: "Call Test::TestFunctionStruct()"
            onClicked: {
                let myStruct = commonFactory.makeMyStruct();
                myStruct.Name = "FooBar";
                testObject.TestFunctionStruct(myStruct);
            }
        }
    }
}
