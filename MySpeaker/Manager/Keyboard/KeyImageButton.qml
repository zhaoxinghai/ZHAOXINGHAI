import QtQuick 2.0

Rectangle {

    property string bkgPressedColor: "#dbe0e6"
    property string bkgReleasedColor: "#00ffffff"
    property alias imageSource: image.source
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

    function setButtonImageSource(source)
    {
        imageSource = source;
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function setEnabled(value)
    {
        enabled = value;
    }

    color: mouseArea.pressed ? bkgPressedColor : bkgReleasedColor
    width: 120
    height: 80

    Image{
        id: image
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            clickedSignal();
        }
    }
}
