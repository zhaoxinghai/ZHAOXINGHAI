import QtQuick 2.0

Rectangle {

    property string bkgPressedColor: "#dbe0e6"
    property string bkgReleasedColor: "#f0f0f0"
    property bool line1Visible: false
    property bool line2Visible: true
    property alias btnEnable: mouseArea.enabled
    signal clickedSignal();

    function setBkgPressedColor(color)
    {
        bkgPressedColor = color;
    }

    function setBkgReleasedColor(color)
    {
        bkgReleasedColor = color;
    }

    function setBtnEnable(value)
    {
        btnEnable = value;
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    width: 480
    height: 65
    color: mouseArea.pressed ? bkgPressedColor : bkgReleasedColor
    clip: true

    HorizontalLine{
        id: line1
        z: 1
        anchors.top: parent.top
        anchors.left: parent.left
        visible: line1Visible
    }

    HorizontalLine{
        id: line2
        z: 1
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        visible: line2Visible
    }

    MouseArea{
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            clickedSignal();
        }
    }
}
