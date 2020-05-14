import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.4
import "../Components"

Item {

    property string passwordLineInitColor: "#b3b3b3"
    property string passwordLineSelectColor: "#cd3429"

    property alias passWordEdtiText: passwdField.text
    property alias passwdFieldFocus : passwdField.focus

    function updateLanguageSlot()
    {
        passwdField.placeholderText = langStrObject.GetLangString("ETCS_PASSWORD");
    }

    function setPasswordEnableValue(value)
    {
        passwdField.enabled = value;
    }

    Column {
        id: passwordColumn

        TextField{
            id: passwdField
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
            Rectangle{
                width: 50
                height: 50

                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.rightMargin: -40

                color: mouseArea.pressed ? "#dbe0e6" : "#00ffffff"
                Image{

                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter

                    source: logicObject.getLocalImagePath() + "delete_2.png"
                    visible: passwdField.length == 0 ? false : true
                }
                MouseArea{
                    id: mouseArea
                    anchors.fill: parent
                    onClicked: {
                        loginInPage.clearPassword();
                        loginInPage.messageClear();
                    }
                }
            }

            Keys.onPressed: {
                if (event.key === 16777220 || event.key === Qt.Key_Enter)
                {
                    if (loginInPage.visible == true)
                    {
                        var ret;
                        ret = loginInPage.comfirmUserNameAndPassword();
                        if (ret === 0)
                        {
                            rootItem.changePages("mainPage");
                            loginInPage.messageClear();
                            loginInPage.clearPassword();
                        }
                        else
                        {

                        }
                    }
                }
             }

            onTextChanged: {
                //if (passwdField.text.length == 1)
                //{
                    loginInPage.messageClear();
               // }
            }

            Component.onCompleted: {
            }
         }

        Rectangle {
            id: passWordLine
            width: 400
            height: 1
            color: passwdField.focus ? passwordLineSelectColor : passwordLineInitColor
        }


    }  

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
