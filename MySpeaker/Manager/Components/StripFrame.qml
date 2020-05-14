import QtQuick 2.0

Rectangle {

    property string bkgPressedColor: "#dbe0e6"
    property string bkgReleasedColor: "#00ffffff"
    property alias line1Visible: line1.visible
    property alias line2Visible: line2.visible
    property alias enabled: mouseArea.enabled
    property bool bkSwitch: true

    signal clickedSignal();


    function setBkgPressedColor(color)
    {
        bkgPressedColor = color;
    }

    function setBkgReleasedColor(color)
    {
        bkgReleasedColor = color;
    }

    function setLine1VisibleValue(value)
    {
        line1Visible = value;
    }

    function setLine2VisibleValue(value)
    {
        line2Visible = value;
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function setEnabledValue(value)
    {
        enabled = value;
    }

    width: 480
    height: 65

    color: bkSwitch ? (mouseArea.pressed ? bkgPressedColor : bkgReleasedColor) : bkgReleasedColor
    clip: true

    HorizontalLine{
        id: line1
        z: 1
        anchors.top: parent.top
        anchors.left: parent.left
    }

    HorizontalLine{
        id: line2
        z: 1
        anchors.bottom: parent.bottom
        anchors.left: parent.left
    }

    MouseArea{
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            clickedSignal();
        }
    }
}
