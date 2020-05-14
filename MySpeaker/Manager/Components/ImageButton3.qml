import QtQuick 2.0


import QtQuick 2.0

Rectangle {

    property string bkgPressedColor: "#DBE0E6"
    property string bkgReleasedColor: "#00ffffff"
    property alias imageSource: buttonImage.source
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

    function setImgPlayingValue(value)
    {
        buttonImage.playing = value;
    }

    function setEnabledValue(value)
    {
        enabled = value;
    }

    color: bkgReleasedColor
    width: buttonImage.width
    height: buttonImage.height
    clip: true

    AnimatedImage{
        id: buttonImage
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
