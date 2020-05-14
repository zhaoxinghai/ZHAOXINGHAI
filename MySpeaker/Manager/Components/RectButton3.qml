import QtQuick 2.0

Rectangle {

    property string bkgPressedColor: "#dbe0e6"
    property string bkgReleasedColor: "#f0f0f0"
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

    width: 480
    height: 65
    color: mouseArea.pressed ? bkgPressedColor : bkgReleasedColor
    clip: true

    MouseArea{
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            clickedSignal();
        }
    }
}
