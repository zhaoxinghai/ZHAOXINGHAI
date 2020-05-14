import QtQuick 2.0

Rectangle {

    property alias text: textItem.text
    property string btnValue: ""
    property string buttonBackgroundColor: "#f0f0f0"
    property string buttonPressBackgroundColor: "#bbbbbb"
    property string textColor: "#303030"
    property bool btnEnable: true
    property bool textVisible: true
    property bool imageVisible: false

    signal buttonPressed(var data);
    signal buttonReleased();

    color: btnEnable ? (mouse.pressed ? buttonPressBackgroundColor : buttonBackgroundColor) : "#40666666"

    Text {
        id: textItem
        font.family: rootItem.fontName
        font.pixelSize: 28
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        color: textColor
        visible: textVisible
        width: parent.width
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
    }

    Image {
        id: image
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        source: logicObject.getLocalImagePath() + "key_del_1.png"
        visible: imageVisible
    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        enabled: btnEnable

        onReleased: {
            buttonReleased();
        }

        onPressed: {
            buttonPressed(btnValue);
        }
    }
}
