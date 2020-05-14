import QtQuick 2.0

Rectangle {

    property string imageSource: ""
    property string textName: ""
    property string textColor: "black"
    property bool buttonStatus: false

    property alias buttonEnable: mouseArea.enabled
    property alias buttonPressedStatus: mouseArea.pressed

    signal buttonClicked(var status);
    signal buttonPressed();

    color: mouseArea.pressed ? "#dddddd" : "#ffffff"

    Image{
        id: buttonImage
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 20
        source: imageSource
    }

    Text {
        id: buttonName
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: buttonImage.right
        anchors.leftMargin: 30
        font.family: rootItem.fontName
        font.bold: false
        font.pixelSize:24
        color: textColor
        text: textName
    }

    MouseArea {
        id:mouseArea
        anchors.fill: parent
        onClicked: {
            parent.buttonStatus = !parent.buttonStatus;
            parent.buttonClicked(parent.buttonStatus);
        }
        onPressed: {
            parent.buttonPressed();
        }
    }
}

