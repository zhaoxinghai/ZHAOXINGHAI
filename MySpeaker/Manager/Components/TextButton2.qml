import QtQuick 2.0

Rectangle {

    property string bkgPressedColor: "#505050"
    property string bkgReleasedColor: "#00ffffff"
    property alias info: name.text
    property alias textColor: name.color
    property alias fontSize: name.font.pixelSize
    property alias nameContentWidth: name.contentWidth
    property alias nameWidth: name.width
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

    function setTextInfo(value)
    {
        info = value;
    }

    function setTextColor(color)
    {
        name.color = color;
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function setEnabledValue(value)
    {
        enabled = value;
    }

    function getEnableValue()
    {
        return enabled;
    }

    color: mouseArea.pressed ? bkgPressedColor : bkgReleasedColor
    clip: true

    Text {
        id: name
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        font.family: rootItem.fontName
        font.pixelSize: 18
        color: "#858585"
        width: 150
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
        Component.onCompleted: {

        }
    }
    MouseArea{
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            clickedSignal();
        }
    }
}
