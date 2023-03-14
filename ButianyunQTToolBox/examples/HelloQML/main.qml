import QtQuick
import QtQuick.Controls
import "butianyun.js" as Butianyun


Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Button {
        x: 100
        y: 100
        text: "Click here"
        onClicked: {
            Butianyun.work()
            Butianyun.work_all()
            status.text = "clicked";
        }
    }

    Label {
        id: status
        x: 100
        y: 200
    }
}
