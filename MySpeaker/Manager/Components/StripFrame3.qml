import QtQuick 2.0

Rectangle {

    property string bkgPressedColor: "#DBE0E6"
    property string bkgReleasedColor: "#ffffff"
    property alias line1Visible: line1.visible
    property alias line2Visible: line2.visible
    property bool btnStatus: true

    signal selectSignal();
    signal undoSignal();

    function setBkgPressedColor(color)
    {
        bkgPressedColor = color;
    }

    function setBkgReleasedColor(color)
    {
        bkgReleasedColor = color;
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
