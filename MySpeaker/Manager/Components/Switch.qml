import QtQuick 2.0

Rectangle{

    property bool btnClicked: false

    signal switchOpenSignal();
    signal switchCloseSignal();

    function updateSlot()
    {
        var tmp;
        tmp = getBtnClickedValue();
        tmp = !tmp;
        setBtnClickedValue(tmp);
        if (tmp === true)
        {
            switchOpenStatus();
            switchOpenSignal();
        }
        else
        {
            switchCloseStatus();
            switchCloseSignal();
        }
    }

    function updateSwitchStatus()
    {
        var tmp = getBtnClickedValue();
        if (tmp === true)
        {
            switchOpenStatus();
            switchOpenSignal();
        }
        else
        {
            switchCloseStatus();
            switchCloseSignal();
        }
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function setBtnClickedValue(value)
    {
        btnClicked = value;
    }

    function getBtnClickedValue()
    {
        return btnClicked;
    }

    function switchOpenStatus()
    {
        setGrooveColor("#a1d6b9");
        setHandleColor("#14974f");
        setHandleBorderColor("#14974f");
        setHandlePosition(29);
    }

    function switchCloseStatus()
    {
        setGrooveColor("#d6d6d6");
        setHandleColor("#ffffff");
        setHandleBorderColor("#c4c4c4");
        setHandlePosition(-15);
    }

    function setGrooveColor(color)
    {
        groove.color = color;
    }

    function setHandleColor(color)
    {
        handle.color = color;
    }

    function setHandleBorderColor(color)
    {
        handle.border.color = color;
    }

    function setHandlePosition(x)
    {
        handle.x = x;
    }

    width: 60
    height: 65
    color: "#ffffff"

    Rectangle{
        id: groove
        width: 44
        height: 8
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        color: btnClicked == true ? "#a1d6b9" : "#d6d6d6"

        Rectangle{
            id: handle
            x: btnClicked  == true ? 29 : -15
            y: -11
            width: 30
            height: 30
            radius: height / 2
            color: btnClicked  == true ? "#14974f": "#ffffff"
            border.width: 1
            border.color: btnClicked  == true ? "#14974f": "#c4c4c4"
        }
    }

    MouseArea{
        anchors.fill: parent
        onClicked: {
            updateSlot();
        }
    }

    Component.onCompleted: {

    }
}
