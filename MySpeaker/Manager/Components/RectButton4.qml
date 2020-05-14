import QtQuick 2.0

Rectangle {

    property alias btnEnable: mouseArea.enabled
    signal clickedSignal();

    function getClickStatus()
    {
        return clickStatus;
    }

    function setColor(value)
    {
        color = value;
    }

    function setEnabled(value)
    {
        btnEnable = value;
    }

    width: 480
    height: 65
    clip: true

    MouseArea{
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            clickedSignal();
        }
    }
}
