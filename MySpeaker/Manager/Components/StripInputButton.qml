import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.4

Rectangle {

    property alias btnNameWidth: buttonName.width
    property alias btnNameContentWidth: buttonName.contentWidth

    signal focusChange(var value);

    function setInputText(text)
    {
        inputTextField.text = text;
    }

    function getInputText()
    {
        return inputTextField.text;
    }

    function setButtonNameText(text)
    {
        buttonName.text = text;
    }

    function getButtonNameText()
    {
        return buttonName.text;
    }

    function setInputTextFieldFocus(value)
    {
        inputTextField.focus = value;
    }

    function setLine1VisibleValue(value)
    {
        line1.visible = value;
    }

    function setLine1Width(value)
    {
        line1.width = value;
    }

    function setLine2VisibleValue(value)
    {
        line2.visible = value;
    }

    function setLine2Width(value)
    {
        line2.width = value;
    }

    function setMaxLength()
    {
        inputTextField.maximumLength = 15;
    }

    function getInputTextFieldFocus()
    {
        return inputTextField.focus;
    }

    function insertData(data)
    {
        inputTextField.insert(inputTextField.cursorPosition, data);
    }

    function removeData(data)
    {
        inputTextField.remove(inputTextField.cursorPosition - 1, inputTextField.cursorPosition);
    }

    width: 480
    height: 65
    color: "#ffffff"

    Rectangle{
        id: line1
        z:  1
        anchors.top: parent.top
        height: 1
        color: "#e0e0e0"
    }

    Text {
        id: buttonName
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 31
        font.family: rootItem.fontName
        font.pixelSize: 18
        color: "#303030"
        width: 200
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignLeft
        Component.onCompleted: {

        }
    }

    Component {
        id: touchStyle
        TextFieldStyle {
            font{
                family: rootItem.fontName
                pointSize: 12
                bold: false
            }

            background: Rectangle {
                border.width: 0
                width: 280
                height: inputTextField.height
            }

        }
    }

    TextField{
        id: inputTextField
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: buttonName.right
        width: parent.width - buttonName.width
        height: parent.height
        textColor: "#303030"
        verticalAlignment: TextInput.AlignVCenter
        style: touchStyle
        selectByMouse: false
        menu: null

        onFocusChanged: {
            focusChange(true);
        }
    }

    Rectangle{
        id: line2
        z:  2
        anchors.bottom : parent.bottom
        height: 1
        color: "#e0e0e0"
    }


}
