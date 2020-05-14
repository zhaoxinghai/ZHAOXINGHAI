import QtQuick 2.0

Rectangle {
    property string bkgPressedColor: "#DBE0E6"
    property string bkgReleasedColor: "#f0f0f0"
    property bool line1Visible: false
    property bool line2Visible: true
    property alias line1Color: line1.color
    property alias line2Color: line2.color
    property alias btnEnable: mouseArea.enabled
    property int clickStatus: 0

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

    function setBtnColor(value)
    {
        color = value;
    }

    function setClickStatus(status)
    {
        clickStatus = status;
    }

    function getClickStatus()
    {
        return clickStatus;
    }

    function setBtnValidStatus()
    {
        setBtnColor(bkgPressedColor);
        setClickStatus(1);
    }

    function resetBtnStatus()
    {
        setBtnColor(bkgReleasedColor);
        setClickStatus(0);
    }

    width: 480
    height: 65
    clip: true
    color: bkgReleasedColor

    HorizontalLine{
        id: line1
        z: 1
        anchors.top: parent.top
        anchors.left: parent.left
        color: "#c0c0c0"
        visible: line1Visible
    }

    HorizontalLine{
        id: line2
        z: 1
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        color: "#c0c0c0"
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
