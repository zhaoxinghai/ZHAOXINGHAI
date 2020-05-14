import QtQuick 2.0

Text {
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

    function setFontPixelSize(size)
    {
        font.pixelSize = size;
    }

    function setFontBold(bold)
    {
        font.bold = bold;
    }

    function setFontStrikeOutValue(value)
    {
        font.strikeout = value;
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    font.family: rootItem.fontName
    font.pixelSize: 18
    color: "#5f5f5f"
    elide: Text.ElideRight
    horizontalAlignment: Text.AlignLeft
}
