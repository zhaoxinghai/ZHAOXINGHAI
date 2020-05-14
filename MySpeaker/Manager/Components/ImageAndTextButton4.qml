import QtQuick 2.0

Rectangle {

    property int topMargin1: 14
    property int topMargin2: 1
    property alias imageSource: buttonImage.source
    property alias imgWidth: buttonImage.width
    property alias imgHeight: buttonImage.height
    property alias info: data.text
    property alias dataColor: data.color
    property alias enable: mouseArea.enabled
    property bool btnStatus: false
    property alias dataWidth: data.width
    property alias dataContentWidth: data.contentWidth

    signal selectSignal();
    signal undoSignal();

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
