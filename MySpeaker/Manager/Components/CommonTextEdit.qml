import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

TextEdit{

    function setFocus(value)
    {
        focus = value;
    }

    function setCursorPostion(pos)
    {
        cursorPosition = pos;
    }

    function setNameTitleText(txt)
    {
        text = txt;
    }

    function addCharSlot(ch)
    {
        insert(cursorPosition, ch);
    }

    function delCharSlot()
    {
        remove(cursorPosition - 1, cursorPosition);
    }

    function clearSlot()
    {
        text = "";
    }

    function getText()
    {
        return text;
    }

    signal focusSignal();
    signal textChangeSignal();

    width: 200
    font.family: rootItem.fontName
    font.pixelSize: 24
    color: "#303030"
    text: ""
    horizontalAlignment: Text.AlignLeft
    onFocusChanged: {
        focusSignal();
    }
    onTextChanged: {
        textChangeSignal();
    }
}
