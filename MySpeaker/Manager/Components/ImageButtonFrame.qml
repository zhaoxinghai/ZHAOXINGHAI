import QtQuick 2.0

Rectangle {

    property string imageSource: ""
    property string textName: ""
    property string textColor: "#5f5f5f"
    property bool buttonStatus: false
    property bool buttonEnable: true
    property var pressed: mouseArea.pressed
    property string keyValue: ""
    property int margin1: 0
    property int margin2: 0

    property alias btnNameWidth: buttonName.width
    property alias btnNameContentWidth: buttonName.contentWidth
    property alias imgWidth: buttonImage.width
    property alias imgHeight: buttonImage.height
    property alias buttonImageWidth: buttonImage.width
    property alias buttonImageHeight: buttonImage.height

    signal buttonClicked(var status);
    signal buttonPressed();
    signal buttonReleased();
    signal buttonKeyValueSignal(var value);

    function resetButtonStatus()
    {
        buttonStatus = false;
    }

    function setButtonStatus(status)
    {
        buttonStatus = status;
    }

    function getButtonStatus()
    {
        return buttonStatus;
    }

    function setBtnEnable(img, color)
    {
        mouseArea.enabled = true;
        imageSource = img;
        textColor = color;
    }

    function setBtnDisable(img, color)
    {
        mouseArea.enabled = false;
        imageSource = img;
        textColor = color;
    }

    function setBtnClickedStatus(img, color)
    {
        imageSource = img;
        textColor = color;
    }

    function setBtnUnclickedStatus(img, color)
    {
        imageSource = img;
        textColor = color;
    }

    function setBtnPressedStatus(img, color)
    {
        imageSource = img;
        textColor = color;
    }

    function setBtnReleasedStatus(img, color)
    {
        imageSource = img;
        textColor = color;
    }

    function setBtnText(text)
    {
        buttonName.text = text;
    }

    function setImgPlayingValue(value)
    {
        buttonImage.playing = value;
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function getVisibleValue()
    {
        return visible;
    }

    color: "#f5f5f5"

    AnimatedImage{
        id: buttonImage
        anchors.top: parent.top
        anchors.topMargin: margin1
        anchors.horizontalCenter: parent.horizontalCenter
        source: imageSource
    }

    Text {
        id: buttonName
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: buttonImage.bottom
        anchors.topMargin: margin2
        font.family: rootItem.fontName
        font.bold: false
        font.pixelSize: 16
        color: textColor
        text: textName
        width: parent.width + 5
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        enabled: buttonEnable
        onClicked: {
            parent.buttonStatus = !parent.buttonStatus;
            parent.buttonClicked(parent.buttonStatus);
            buttonKeyValueSignal(keyValue);
        }
        onPressed: {
            parent.buttonPressed();
        }

        onReleased: {
            parent.buttonReleased();
        }
    }
}

