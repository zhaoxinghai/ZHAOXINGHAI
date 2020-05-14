import QtQuick 2.0

Rectangle {

    property string bkgPressedColor: "#DBE0E6"
    property string bkgReleasedColor: "#ffffff"
    property alias dataColor: data.color
    property alias dataText: data.text
    property alias nameColor: name.color
    property alias nameText: name.text
    property alias enabled: mouseArea.enabled

    signal clickedSignal();

    width: 120
    height: 84

    function btnEnableStatus(text1, color1, text2, color2)
    {
        setBtnEnableValue(true);
        setDataText(text1);
        setDataColor(color1);
        setNameText(text2);
        setNameColor(color2);
    }

    function btnDisableStatus(text1, color1, text2, color2)
    {
        setBtnEnableValue(false);
        setDataText(text1);
        setDataColor(color1);
        setNameText(text2);
        setNameColor(color2);
    }

    function setBkgPressedColor(color)
    {
        bkgPressedColor = color;
    }

    function setBkgReleasedColor(color)
    {
        bkgReleasedColor = color;
    }

    function setBtnEnableValue(value)
    {
        enabled = value;
    }

    function setDataText(text)
    {
        dataText = text;
    }

    function setDataColor(color)
    {
        dataColor = color;
    }

    function setNameText(text)
    {
        nameText = text;
    }

    function setNameColor(color)
    {
        nameColor = color;
    }

    color: mouseArea.pressed ? bkgPressedColor : bkgReleasedColor
    clip: true

    Text{
        id: data
        anchors.top: parent.top
        anchors.topMargin: 15
        anchors.horizontalCenter: parent.horizontalCenter
        font.family: rootItem.fontName
        font.pixelSize: 20
        width: parent.width
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
    }

    Text {
        id: name
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: data.bottom
        anchors.topMargin: 5
        font.family: rootItem.fontName
        font.bold: false
        font.pixelSize:16
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
