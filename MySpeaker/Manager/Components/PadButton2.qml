import QtQuick 2.0

Rectangle {

    property alias text: textItem.text
    property string buttonBackgroundColor: "#eaedf2"
    property string buttonPressBackgroundColor: "#d0d0d0"
    property string textColor: "#363636"

    property bool topLineVisible: false
    property bool bottomLineVisible: false
    property bool leftLineVisible: false
    property bool rightLineVisible: false
    property bool textVisible: true
    property bool imageVisible: false

    signal clickSignal();

    function getText()
    {
        return text;
    }

    width: 160
    height: 74
    color: mouse.pressed ? buttonPressBackgroundColor : buttonBackgroundColor
    clip: true

    Rectangle{
        z: 1
        anchors.top: parent.top
        anchors.topMargin: 1
        anchors.left: parent.left
        width: parent.width
        height: 1
        color: "#FFFFFF"
        visible: topLineVisible
    }
    Rectangle{
        z: 1
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 1
        anchors.left: parent.left
        width: parent.width
        height: 1
        color: "#FFFFFF"
        visible: bottomLineVisible
    }
    Rectangle{
        z: 1
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 1
        width: 1
        height: parent.height
        color: "#FFFFFF"
        visible: leftLineVisible
    }
    Rectangle{
        z: 1
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.rightMargin: 1
        width: 1
        height: parent.height
        color: "#FFFFFF"
        visible: rightLineVisible
    }

    CommonText {
        id: textItem
        font.family: rootItem.fontName
        font.pixelSize: 26
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
        onClicked: {
            clickSignal();
        }
    }
}
