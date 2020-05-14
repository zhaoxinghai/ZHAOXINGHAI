import QtQuick 2.0

Rectangle {

    property int textDataWidth:36
    property int textDataHeight:36
    property string textDataStr: ""
    property string textDataColor: "#303030"
    property bool textDataVisible:false
    property string textName: ""
    property string textColor: "#5f5f5f"
    property bool buttonStatus: false
    property bool buttonEnable: true
    property var pressed: mouseArea.pressed

    property int textTopMargin1: 15
    property int textTopMargin2: 5
    property alias textOne:textData
    property string keyValue: ""

    property alias dataWidth: textData.width
    property alias dataContentWidth: textData.contentWidth

    signal buttonClicked(var status);
    signal buttonKeyValueSignal(var value);

    function setButtonStatus(value)
    {
        buttonStatus = value;
    }

    function getButtonStatus()
    {
        return buttonStatus;
    }

    clip: true

    Text{
        id: textData
        anchors.top: parent.top
        anchors.topMargin: textTopMargin1
        anchors.horizontalCenter: parent.horizontalCenter
        font.family: rootItem.fontName
        font.bold: false
        font.pixelSize: 20
        color: textDataColor
        text: textDataStr
        width: parent.width
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
    }

    Text {
        id: buttonName
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: textData.bottom
        anchors.topMargin: textTopMargin2
        font.family: rootItem.fontName
        font.bold: false
        font.pixelSize:16
        color: textColor
        text: textName
        width: parent.width
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        enabled: buttonEnable
        onClicked: {
            buttonStatus = !buttonStatus;
            buttonClicked(buttonStatus);
            buttonKeyValueSignal(keyValue);
        }
    }
}
