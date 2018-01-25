import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import com.MyCompany.qmlComponents 1.0


Item {

    property int buttonHeight: 60

    ListView {

        x: 4
        y: 4
        width: parent.width - 8
        height: parent.height - buttonHeight - 8

        clip: true

        model: TodoListModel {

            list: todoListData
        }

        delegate: RowLayout {

            width: parent.width
            CheckBox {

                checked: model.done
                onClicked: model.done = checked
            }
            TextField {

                Layout.fillWidth: true
                text: model.text
                onEditingFinished: model.text = text
            }
        }
    }

    Button {

        x: 0
        y: parent.height - buttonHeight
        width: parent.width * 0.5
        height: buttonHeight

        text: "Add new item"
        Layout.fillWidth: true
        onClicked: {
            todoListData.appendItem()
        }
    }

    Button {

        x: parent.width * 0.5
        y: parent.height - buttonHeight
        width: parent.width * 0.5
        height: buttonHeight

        text: "remove completed"
        Layout.fillWidth: true
        onClicked: {
            todoListData.removeCompletedItems()
        }
    }

}
