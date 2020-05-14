import QtQuick 2.0

Rectangle {

    property string bkgPressedColor: "#505050"
    property string bkgReleasedColor: "#00ffffff"
    property alias imageSource: buttonImage.source
    property alias dataText: data.text
    property alias fontSize: data.font.pixelSize
    property alias imageWidth: buttonImage.width
    property alias dataWidth: data.width
    property alias dataContentWidth: data.contentWidth
    property int imgLeftMargin: 0
    property int dataLeftMargin: 0

    signal clickedSignal();

    function setBkgPressedColor(color)
    {
        bkgPressedColor = color;
    }

    function setBkgReleasedColor(color)
    {
        bkgReleasedColor = color;
    }

    function setDataText(text)
    {
        dataText = text;
    }

    color: mouseArea.pressed ? bkgPressedColor : bkgReleasedColor
    width: 130
    height: 60
    clip: true

    Image{
        id: buttonImage
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: imgLeftMargin
    }

    Text {
        id: data
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: buttonImage.right
        anchors.leftMargin: dataLeftMargin
        font.family: rootItem.fontName
        font.pixelSize: 20
        color: "#f1f1f1"
        width: parent.width - buttonImage.width
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignLeft
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            clickedSignal();
        }
    }
}
