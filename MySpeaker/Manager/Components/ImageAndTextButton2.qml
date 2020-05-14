import QtQuick 2.0

Rectangle {

    property string bkgPressedColor: "#DBE0E6"
    property string bkgReleasedColor: "#f5f5f5"
    property int topMargin1: 14
    property int topMargin2: 1
    property alias imageSource: buttonImage.source
    property alias info: data.text
    property alias dataColor: data.color
    property alias enable: mouseArea.enabled
    property alias dataWidth: data.width
    property alias dataContentWidth: data.contentWidth

    signal clickedSignal();

    function setBkgPressedColor(color)
    {
        bkgPressedColor = color;
    }

    function setBkgReleasedColor(color)
    {
        bkgReleasedColor = color;
    }

    function setImageSource(source)
    {
        imageSource = source;
    }

    function setDataColor(color)
    {
        dataColor = color;
    }

    function setInfoValue(value)
    {
        info = value;
    }

    function setBtnEnableValue(value)
    {
        enable = value;
    }

    color: mouseArea.pressed ? bkgPressedColor : bkgReleasedColor
    width: 120
    height: 86
    clip: true

    Image{
        id: buttonImage
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: topMargin1
    }

    Text {
        id: data
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: buttonImage.bottom
        anchors.topMargin: topMargin2
        font.family: rootItem.fontName
        font.pixelSize: 16
        color: "#5f5f5f"
        width: parent.width
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            clickedSignal();
        }
    }
}
