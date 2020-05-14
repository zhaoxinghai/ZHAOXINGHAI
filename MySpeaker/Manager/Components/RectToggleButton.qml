import QtQuick 2.0

Rectangle{

    property bool btnStatus: false
    property string bkgPressedColor: "#dbe0e6"
    property string bkgReleasedColor: "#ffffff"
    property string borderSelectColor: "#00c767"
    property string borderUndoColor: "#c0c0c0"
    property bool bkSwitch: false

    signal selectSignal();
    signal undoSignal();

    width: 140
    height: 60
    color: bkSwitch ? (mouseArea.pressed ? bkgPressedColor : bkgReleasedColor) : bkgReleasedColor
    clip: true

    function getBtnStatus()
    {
        return btnStatus;
    }

    function setBorderWidth(width)
    {
        border.width = width;
    }

    function setBorderColor(color)
    {
        border.color = color;
    }

    function setColor(value)
    {
        color = value;
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
