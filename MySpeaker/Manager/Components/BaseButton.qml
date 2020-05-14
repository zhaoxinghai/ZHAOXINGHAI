import QtQuick 2.0

Rectangle {

    property string textName: buttonName.text
    property color textColor: buttonName.color
    property string imageSource: buttonImage.source
    property bool buttonClicked: false
    property string keyValue: ""
    property bool buttonEnable: true
    property int margin1: 0
    property int margin2: 0

    property alias btnNameWidth: buttonName.width
    property alias btnNameContentWidth: buttonName.contentWidth

    property alias buttonImageWidth: buttonImage.width
    property alias buttonImageHeight: buttonImage.height

    signal keyValueSignal(var value);

    function setButtonClickedValue(value)
    {
        buttonClicked = value;
    }

    Image{
        id: buttonImage
        anchors.top: parent.top
        anchors.topMargin: margin1
        anchors.horizontalCenter: parent.horizontalCenter
        source: imageSource
    }

    Text {
        id: buttonName
        font.family: rootItem.fontName
        font.pixelSize: 16
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: buttonImage.bottom
        anchors.topMargin: margin2
        color: textColor
        text: textName
        width: parent.width
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
    }

    MouseArea {
        anchors.fill: parent
        enabled: buttonEnable
        onClicked: {
            keyValueSignal(keyValue);
        }
    }
}
