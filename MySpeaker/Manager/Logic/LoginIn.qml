import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import "../Components"

Rectangle{

    property int ret: 0
    property string projectNameColor: "#222222"
    property string messageColor: "#ee3124"
    property string loginBtnBackgroudInitColor: "#303030"
    property string loginBtnBackgroudSelectColor: "#b3b3b3"
    property string loginBtnTextColor: "#ffffff"
    property bool loginBtnStatus: false

    property int userNameComboboxHeight: 74
    property int passwordEditHeight: 74
    property int loginBtnHeight: 80
    property int numPadHeight: 296

    function updateLanguageSlot()
    {
        loginBtnText.text = langStrObject.GetLangString("ETCS_LOGIN");
    }

    function initStatus()
    {
        setVisibleValue(true);
        if (userManageObject.getUserListCount() === 0)
        {
            userNameCombobox.setComboxEnableValue(false);
            passwordEdit.setPasswordEnableValue(false);
            loginBtn.setLoginBtnEnableValue(false);
            messageShow(langStrObject.GetLangString("ETCS_LOGIN_ERR_0"));
        }
        ipText.setText(logicObject.getRealIp());

        logicObject.setButtonEnable("alarm", false);
        logicObject.setButtonEnable("DKM", false);
    }

    function digitPressed(op)
    {
        if (op >= "0" && op <= "9")
        {
            passwordEdit.passWordEdtiText = passwordEdit.passWordEdtiText + op.toString();
        }
        else if (op === "DEL")
        {
            passwordEdit.passWordEdtiText = passwordEdit.passWordEdtiText.substring(0, passwordEdit.passWordEdtiText.length - 1);
        }
        else if (op === "OK")
        {
            passwordEdit.passwdFieldFocus = false;
        }
    }

    function clearPassword()
    {
        passwordEdit.passWordEdtiText = "";
    }

    function checkResetPasswd()
    {
        var selectUser = userNameCombobox.userNameStr.toString();
        var selectUserPwd = passwordEdit.passWordEdtiText.toString();
        var isReset = userManageObject.isNeedResetAccount(selectUser, selectUserPwd);
        if (false === isReset)
        {
            return false;
        }
        confirmResetPwd.setVisibleValue(true);
        return true;
    }

    function confirmResetPwdOKSlot()
    {
        logicObject.resetConfigPwd();
    }

    function comfirmUserNameAndPassword()
    {
        var id;
        var tmpUserName = "";
        var tmpPassword = "";
        var encryptPw = "";

        var selectUser = userNameCombobox.userNameStr.toString();
        var selectUserPwd = passwordEdit.passWordEdtiText.toString();

        var count = userManageObject.getUserListCount();
        encryptPw = userManageObject.encryptPassword(selectUserPwd);

        for (var i = 0; i < count; i++)
        {        
            id = userManageObject.getUserId(i);
            tmpUserName = userManageObject.getUserListStrValue(id, "name");
            tmpPassword = userManageObject.getUserListStrValue(id, "passwd");

            if ((selectUser === String(tmpUserName)) && (encryptPw !== String(tmpPassword)) )
            {
                return -1;
            }
            else if ((selectUser === String(tmpUserName)) && (encryptPw === String(tmpPassword)) )
            {   
                return id;
            }
        }

        return -1;
    }

    function messageClear()
    {
        message.text = "";
        message.visible = false;
    }

    function messageShow(text)
    {
        message.visible = true;
        message.text = text;
    }

    function setVisibleValue(value)
    {
        visible = value;
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
        anchors.topMargin: 100
        anchors.horizontalCenter: parent.horizontalCenter
    }

    CommonText{
        id: ipText
        anchors.top: loginPageImage.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        text: logicObject.getRealIp()
        color: projectNameColor
        width: parent.width
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
    }

    Combobox2 {
        id: userNameCombobox
        width: 400
        z: 1
        height: userNameComboboxHeight
        anchors.bottom: passwordEdit.top
        anchors.bottomMargin: 0
        anchors.horizontalCenter: parent.horizontalCenter
        onIsEmptySignal: {
            messageShow(langStrObject.GetLangString("ETCS_LOGIN_ERR_0"));
        }
    }

    PasswordEdit{
        id: passwordEdit
        width: 400
        height: passwordEditHeight
        anchors.bottom: loginBtn.top
        anchors.bottomMargin: 40
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Text{
        id: message
        anchors.bottom: loginBtn.top
        anchors.left: parent.left
        anchors.leftMargin:40
        font.family: rootItem.fontName
        color: messageColor
        font.pointSize: 14
		visible:false
        width: 400
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignLeft
        Component.onCompleted: {

        }
    }

    Rectangle{

        function setLoginBtnEnableValue(value)
        {
            mouseArea.enabled = value;
        }

        id: loginBtn
        width: 400
        height: loginBtnHeight
        anchors.bottom: numPad.top
        anchors.horizontalCenter: parent.horizontalCenter  

        Rectangle{
            width: 400
            height: 56
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: mouseArea.pressed ? loginBtnBackgroudSelectColor : loginBtnBackgroudInitColor
        }
        Text{
            id: loginBtnText
            color: loginBtnTextColor
            text: langStrObject.GetLangString("ETCS_LOGIN")
            font.family: rootItem.fontName
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCentern
            font.pixelSize: 30
            width: parent.width
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            Component.onCompleted: {
            }
        }
        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: {
                var isReset = checkResetPasswd();
                if (true === isReset)
                {
                    return;
                }

                var ret = comfirmUserNameAndPassword();
                if (ret >= 0){
                    messageClear();
                    passwordEdit.passwdFieldFocus = false;
                    clearPassword();

                    logicObject.setButtonEnable("DKM", true);

                    var id = ret;
                    if (userManageObject.getLoginUserId() !== id)
                    {
                        rootItem.logOutUser();
                        userManageObject.setLoginUserId(id);
                        userManageObject.setUser(userManageObject.getLoginUserId());
                        rootItem.setLockScreenTimerInterval();
                        var tmpPermission;
                        var mapPermission;
                        tmpPermission = userManageObject.getUserListStrValue(userManageObject.getLoginUserId(), "permission");
                        if (tmpPermission === "root")
                        {
                            mapPermission = 0;
                        }
                        else if (tmpPermission === "poweruser")
                        {
                            mapPermission = 1;
                        }
                        else if (tmpPermission === "user")
                        {
                            mapPermission = 2;
                        }
                        rootItem.changeUserSignal(mapPermission);
                        changePages("mainPage");
                    }
                    else
                    {
                        mainPage.keepStatus();
                        dragUnit.setDragUnitEnableValue(true);

                        if (true === mainPage.getVAPageVisible())
                        {
                            logicObject.setButtonEnable("alarm", true);
                        }
                    }
                }
                else
                {
                    messageShow(langStrObject.GetLangString("ETCS_LOGIN_ERR_2"));
                }

                userNameCombobox.setListVisibleValue(false);
            }
        }
    }

   NumberPad {
       id: numPad
       width: parent.width
       height: numPadHeight
       anchors.bottom: parent.bottom
       anchors.horizontalCenter: parent.horizontalCenter
       visible: passwordEdit.passwdFieldFocus
   }

    Component.onCompleted: {
        rootItem.updateLanguage.connect(updateLanguageSlot);
        confirmResetPwd.okSignal.connect(confirmResetPwdOKSlot);
    }
}
