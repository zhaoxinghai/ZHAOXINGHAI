import QtQuick 2.0

Rectangle {

    property string bkgPressedColor: "#dbe0e6"
    property string bkgReleasedColor: "#00ffffff"
    property alias fontSize: data.font.pixelSize
    property alias dataText: data.text
    property alias dataColor: data.color
    property alias enabled: mouseArea.enabled
    property alias bottomLineVisible: bottomLine.visible
    property alias leftLineVisible: leftLine.visible
    property alias rightLineVisible: rightLine.visible
    property string key: ""


    signal clickedSignal();

    function setKey(value)
    {
        key = value
    }

    function getKey()
    {
        return key;
    }

    function setBkgPressedColor(color)
    {
        bkgPressedColor = color;
    }

    function setBkgReleasedColor(color)
    {
        bkgReleasedColor = color;
    }

    function setFontSize(size)
    {
        fontSize = size;
    }

    function setDataText(text)
    {
        dataText = text;
    }

    function getDataText()
    {
        return dataText;
    }

    function setDataColor(color)
    {
        dataColor = color;
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function setEnabled(value)
    {
        enabled = value;
    }

    width: 120
    height: 80
    color: mouseArea.pressed ? bkgPressedColor : bkgReleasedColor
    clip: true

    Rectangle{
        id: bottomLine
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        z:  1
        width: parent.width
        height: 1
        color: "#e0e0e0"
        visible: false
    }
    Rectangle{
        id: leftLine
        anchors.top: parent.top
        anchors.left: parent.left
        z:  1
        width: 1
        height: parent.height
        color: "#e0e0e0"
        visible: false
    }
    Rectangle{
        id: rightLine
        anchors.top: parent.top
        anchors.right: parent.right
        z:  1
        width: 1
        height: parent.height
        color: "#e0e0e0"
        visible: false
    }
    Text {
        id: data
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        font.family: rootItem.fontName
        font.pixelSize: 20
        color: "#303030"
        width: parent.width
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
    }
    MouseArea{
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            clickedSignal();
        }
    }
}
