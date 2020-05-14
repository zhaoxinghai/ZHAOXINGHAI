import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.4
import "../Components"

TextField{

    function updateLanguageSlot()
    {
        placeholderText = langStrObject.GetLangString("ETCS_PASSWORD");
    }

    function initStatus()
    {
        setFocus(false);
        setText("");
    }

    function setEnabledValue(value)
    {
        enabled = value;
    }

    function setFocus(value)
    {
        focus = value;
    }

    function getFocus()
    {
        return focus;
    }

    function setText(value)
    {
        text = value;
    }

    function getText()
    {
        return text;
    }

    width: 360
    height: 74
    placeholderText: langStrObject.GetLangString("ETCS_PASSWORD")
    echoMode: TextInput.Password
    verticalAlignment: TextInput.AlignBottom
    maximumLength: 18
    validator: RegExpValidator
    {
        regExp: /^[0-9]*$/
    }

    style: touchStyle

    Component {
        id: touchStyle
        TextFieldStyle {
            placeholderTextColor: "#b3b3b3"
            font{
                family: specialFontLoader.getSpecialFontName()
                pointSize: 18
                bold: false
            }

            background: Rectangle {
                border.width: 0
                width: 400
                height: 50
            }
        }
    }

    Component.onCompleted: {
        rootItem.updateLanguage.connect(updateLanguageSlot);
    }
}
