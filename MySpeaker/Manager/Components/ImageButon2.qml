import QtQuick 2.0

Rectangle {

    property alias imageSource: buttonImage.source
    property alias enable: mouseArea.enabled
    property bool btnStatus: false
    property alias buttonImageWidth: buttonImage.width
    property alias buttonImageHeight: buttonImage.height

    signal selectSignal();
    signal undoSignal();

    function setImageSource(source)
    {
        imageSource = source;
    }

    function setBtnEnableValue(value)
    {
        enable = value;
    }

    function setBtnStatus(value)
    {
        btnStatus = value;
    }

    function getBtnStatus()
    {
        return btnStatus;
    }

    width: 120
    height: 86

    clip: true
    Image{
        id: buttonImage
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            btnStatus = !btnStatus;
            if (btnStatus == true)
            {
                selectSignal();
            }
            else
            {
                undoSignal();
            }
        }
    }
}
