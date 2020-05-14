import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

TextInput{

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

    function setText(value)
    {
        text = value;
    }

    function getText()
    {
        return text;
    }

    function getTrimText()
    {
        return text.trim();
    }

    signal focusSignal();
    signal textChangeSignal();

    width: 200
    font.family: rootItem.fontName
    font.pixelSize: 20
    verticalAlignment: TextInput.AlignVCenter
    horizontalAlignment: TextInput.AlignLeft

    onFocusChanged: {
        focusSignal();
    }
    onTextChanged: {
        textChangeSignal();
    }
}
