import QtQuick 2.0

Rectangle {

    property bool line1Visible: false
    property bool line2Visible: true

    function setColor(value)
    {
        color = value;
    }

    width: 480
    height: 65
    color: "#ffffff"
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
}
