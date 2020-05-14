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
        }
        else
        {
            switchCloseStatus();
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
        switchImage.setSource(logicObject.getLocalImagePath() + "switch_on.png");
    }

    function switchCloseStatus()
    {
        switchImage.setSource(logicObject.getLocalImagePath() + "switch_off.png");
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

    Image{

        function setSource(value)
        {
            source = value;
        }

        id: switchImage
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
    }

    MouseArea{
        anchors.fill: parent
        onClicked: {
            updateSlot();
        }
    }

    Component.onCompleted: {
        updateSwitchStatus();
    }
}
