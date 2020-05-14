import QtQuick 2.0

Text {

    function insertText(value)
    {
        text  = text + value;
    }

    function clearText()
    {
        text = "";
    }

    function setText(value)
    {
        text = value;
    }

    function getText()
    {
        return text;
    }

    function setTextColor(value)
    {
        color = value;
    }


    font.family: rootItem.fontName
    font.pixelSize: 18
    color: "#303030"
    horizontalAlignment: Text.AlignLeft
    wrapMode: Text.Wrap
}
