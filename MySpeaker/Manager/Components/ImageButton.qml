import QtQuick 2.0

Rectangle {

    property string bkgPressedColor: "#dbe0e6"
    property string bkgReleasedColor: "#00ffffff"
    property string buttonImageSource: ""
    property alias enabled: mouseArea.enabled
    property alias buttonImageWidth: buttonImage.width
    property alias buttonImageHeight: buttonImage.height

    signal clickedSignal();
    signal releasedSignal();

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
        buttonImageSource = source;
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function setEnabledValue(value)
    {
        enabled = value;
    }

    color: mouseArea.pressed ? bkgPressedColor : bkgReleasedColor
    width: buttonImage.width
    height: buttonImage.height

    Image{
        id: buttonImage
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        source: buttonImageSource
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            clickedSignal();
        }
        onReleased: {
            releasedSignal();
        }
    }
}
