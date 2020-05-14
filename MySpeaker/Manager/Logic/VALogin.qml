import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import "../Components"

Rectangle{

    property int userNameComboboxHeight: 74
    property int passwordEditHeight: 74
    property int loginBtnHeight: 80
    property int numPadHeight: 296

    signal loginSignal();
    signal cancelSignal();

    function updateLanguageSlot()
    {
        cancelName.setText(langStrObject.GetLangString("ETCS_CANCEL"));
        loginName.setText(langStrObject.GetLangString("ETCS_LOGIN"));
    }

    function initStatus()
    {
        setVisibleValue(true);
        userCombobox.initStatus();
        passwordEdit.initStatus();
        passWordLine.setLineColor("#b3b3b3");
        clearImageButton.setVisibleValue(false);
        message.showMessage("");
        numPad.setVisibleValue(false);
        dragUnit.setDragUnitEnableValue(false);
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function quitStatus()
    {
        setVisibleValue(false);
    }

    function getVALoginVisible()
    {
        return visible;
    }

    function comfirmUserNameAndPassword()
    {
        var count = userManageObject.getUserListCount();
        var id;
        var tmpUserName = "";
        var tmpPassword = "";
        var encryptPw = "";

        var comUserName = "";

        encryptPw = userManageObject.encryptPassword(passwordEdit.getText().toString());
        comUserName = userCombobox.getText();
        for (var i = 0; i < count; i++)
        {
            id = userManageObject.getUserId(i);
            tmpUserName = userManageObject.getUserListStrValue(id, "name");
            tmpPassword = userManageObject.getUserListStrValue(id, "passwd");

            if ((comUserName.toString() === String(tmpUserName)) && (encryptPw !== String(tmpPassword)) )
            {
                return -1;
            }
            else if ((comUserName.toString() === String(tmpUserName)) && (encryptPw === String(tmpPassword)) )
            {
                userManageObject.setVALoginUserId(id);
                return 0;
            }
        }

        return -2;
    }

    function okSlot()
    {
        numPad.setVisibleValue(false);
        passwordEdit.setFocus(false);
        passWordLine.setLineColor("#b3b3b3");
    }

    clip: true

    MouseArea{
        anchors.fill: parent
        onClicked: {

        }
    }

    Image{
        id: loginPageImage
        source: logicObject.getLocalImagePath() + "Honeywell_logo.png"
        anchors.top: parent.top
        anchors.topMargin: 100 - 36
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Combobox3 {
        id: userCombobox
        anchors.top: parent.top
        anchors.topMargin: 200
        anchors.horizontalCenter: parent.horizontalCenter
        width: 400
        height: userNameComboboxHeight
        z: 1
        onSelectSignal: {
            passwordEdit.setText("");
            passwordEdit.setFocus(true);
            passWordLine.setLineColor("#cd3429");
            numPad.setVisibleValue(true);
        }

        onIsEmptySignal: {
            message.showMessage(langStrObject.GetLangString("ETCS_LOGIN_ERR_0"))
        }
    }

    PasswordEdit2{

        function addCharSlot(op)
        {
            passwordEdit.setText(passwordEdit.getText() + op);
        }

        function delCharSlot()
        {
            passwordEdit.setText(passwordEdit.getText().substring(0, passwordEdit.getText().length - 1));
        }

        id: passwordEdit
        width: 370
        height: 74
        anchors.top: userCombobox.bottom
        anchors.left: userCombobox.left

        HorizontalLine {
            id: passWordLine
            anchors.top: parent.bottom
            anchors.left: parent.left
            width: 400
            height: 1
            color: "#b3b3b3"
        }

        ImageButton{
            id: clearImageButton
            anchors.top: parent.top
            anchors.topMargin: 20
            anchors.right: parent.right
            anchors.rightMargin: -35
            width: parent.height - 20
            height: parent.height - 20
            buttonImageSource: logicObject.getLocalImagePath() + "delete_2.png"
            onClickedSignal: {
                passwordEdit.setText("");
                message.showMessage("");
            }
        }

        Keys.onPressed: {
            if (event.key === 16777220 || event.key === Qt.Key_Enter)
            {
                if (getVALoginVisible() === true)
                {
                    var ret = comfirmUserNameAndPassword();
                    if (ret === 0)
                    {
                        passwordEdit.initStatus();
                        passWordLine.setLineColor("#b3b3b3");
                        message.showMessage("");
                        numPad.setVisibleValue(false);
                        loginSignal();
                    }
                    else if (ret === -1)
                    {
                        message.showMessage(langStrObject.GetLangString("ETCS_LOGIN_ERR_2"));
                    }
                }
            }
         }

        onTextChanged: {
            message.showMessage("");
            if (getText().length === 0)
            {
                clearImageButton.setVisibleValue(false);
            }
            else
            {
                clearImageButton.setVisibleValue(true);
            }
        }

        onFocusChanged: {
            if (getFocus() === true)
            {
                passWordLine.setLineColor("#cd3429");
                numPad.setVisibleValue(true);
            }
        }
    }

    CommonText{

        function showMessage(text)
        {
            setVisibleValue(true);
            setText(text);
        }

        id: message
        anchors.bottom: loginBtn.top
        anchors.bottomMargin: 12
        anchors.left: parent.left
        anchors.leftMargin:40        
        font.family: rootItem.fontName
        color: "#ee3124"
        font.pointSize: 14
        visible: true
        width: 400
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignLeft
    }

    RectButton3{
        id: cancelBtn
        width: 190
        height: 56
        anchors.bottom: numPad.top
        anchors.bottomMargin: 12
        anchors.left: parent.left
        anchors.leftMargin: 40
        bkgPressedColor: "#b3b3b3"
        bkgReleasedColor: "#303030"
        CommonText{
            id: cancelName
            color: "#ffffff"
            text: langStrObject.GetLangString("ETCS_CANCEL")
            font.family: rootItem.fontName
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 28
            width: parent.width
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            Component.onCompleted: {
            }
        }

        onClickedSignal: {
            cancelSignal();
            dragUnit.setDragUnitEnableValue(true);
        }
    }

    RectButton3{
        id: loginBtn
        width: 190
        height: 56
        anchors.bottom: numPad.top
        anchors.bottomMargin: 12
        anchors.left: cancelBtn.right
        anchors.leftMargin: 20
        bkgPressedColor: "#b3b3b3"
        bkgReleasedColor: "#303030"
        CommonText{
            id: loginName
            color: "#ffffff"
            text: langStrObject.GetLangString("ETCS_LOGIN")
            font.family: rootItem.fontName
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 28
            width: parent.width
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            Component.onCompleted: {
            }
        }

        onClickedSignal: {
            var ret = comfirmUserNameAndPassword();
            if (ret === 0)
            {
                passwordEdit.initStatus();
                passWordLine.setLineColor("#b3b3b3");
                message.showMessage("");
                numPad.setVisibleValue(false);
                loginSignal();
                dragUnit.setDragUnitEnableValue(true);
            }
            else if (ret === -1)
            {
                message.showMessage(langStrObject.GetLangString("ETCS_LOGIN_ERR_2"));
            }
        }
    }

   NumberPad2 {
       id: numPad
       width: parent.width
       height: numPadHeight
       anchors.bottom: parent.bottom
       anchors.horizontalCenter: parent.horizontalCenter
       visible: false
   }

    Component.onCompleted: {
        numPad.addCharSignal.connect(passwordEdit.addCharSlot);
        numPad.delCharSignal.connect(passwordEdit.delCharSlot);
        numPad.okSignal.connect(okSlot);
        rootItem.updateLanguage.connect(updateLanguageSlot);
    }
}
