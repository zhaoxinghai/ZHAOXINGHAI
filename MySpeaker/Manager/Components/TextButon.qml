import QtQuick 2.0

Rectangle {

    property string bkgPressedColor: "#505050"
    property string bkgReleasedColor: "#00ffffff"
    property alias textWidth: name.width
    property int fontSize: 18
    property string info: ""
    property string textColor: "#858585"
    property alias enabled: mouseArea.enabled

    signal clickedSignal();

    function setBkgPressedColor(color)
    {
        bkgPressedColor = color;
    }

    function setBkgReleasedColor(color)
    {
        bkgReleasedColor = color;
    }

    function setTextAttributes(width, size, color)
    {
        textWidth = width;
        fontSize = size;
        textColor = color;
    }

    function setTextInfo(text)
    {
        info = text;
    }

    function setTextColor(color)
    {
        text.color = color;
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    color: mouseArea.pressed ? bkgPressedColor : bkgReleasedColor
    clip: true

    Text {
        id: name
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        font.family: rootItem.fontName
        font.pixelSize: fontSize
        color: textColor
        width: parent.width
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
        text: info
    }
    MouseArea{
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            clickedSignal();
        }
    }
}
