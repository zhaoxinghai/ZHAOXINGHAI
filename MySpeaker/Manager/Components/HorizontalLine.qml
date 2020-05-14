import QtQuick 2.0

Rectangle{

    function setLineWidth(value)
    {
        width = value;
    }

    function setLineHeight(value)
    {
        height = value
    }

    function setLineColor(value)
    {
        color = value;
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    z:  1
    width: 480
    height: 1
    color: "#e0e0e0"
    visible: true
}
