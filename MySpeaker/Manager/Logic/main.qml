import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import "../Components"

Rectangle {
    property string fontName: ""
    property int statusBarHeight: 36
    property int mainPageHeight: height - statusBarHeight
    signal updateLanguage();
    signal changeUserSignal(var per);

    function updateLanguageSlot()
    {
        clearConfirmPrompt.setText(langStrObject.GetLangString("ETCS_CLEAR_OR_NOT"));
        cancleBtn.setTextInfo(langStrObject.GetLangString("ETCS_CANCEL"));
        okBtn.setTextInfo(langStrObject.GetLangString("ETCS_OK"));
        commonCancelBtn.setTextInfo(langStrObject.GetLangString("ETCS_CANCEL"));
        commonOKBtn.setTextInfo(langStrObject.GetLangString("ETCS_OK"));
        topMessage.setText(langStrObject.GetLangString("ETCS_SEND_QUEUE"));
        confirmPrompt.setText(langStrObject.GetLangString("ETCS_REMOVE_ITEM"));
        cancelButton.setTextInfo(langStrObject.GetLangString("ETCS_CANCEL"));
        okButton.setTextInfo(langStrObject.GetLangString("ETCS_OK"));

        confirmResetPwdDesc.setText(langStrObject.GetLangString("RESET_CONFIG_PWD"));
        resetPwdCancelBtn.setTextInfo(langStrObject.GetLangString("ETCS_CANCEL"));
        resetPwdOKBtn.setTextInfo(langStrObject.GetLangString("ETCS_OK"));

        noticeEditModePrompt.setText(
                    langStrObject.GetLangString("ETCS_PREDEFINE_EDIT_MODE"));
        noticeEditModeOkButton.setTextInfo(
                    langStrObject.GetLangString("ETCS_OK"));
    }

    function systemResultSlot(id, errCode)
    {
        if (id === waitMask1.getUmountId())
        {
            var ret;
            ret = udiskObject.isUDiskMounted();
            if (ret === false)
            {
                waitMask1.promptModeShowMessage(langStrObject.GetLangString("ETCS_U_DISK_EJECT_SUCCESS"));
            }
            else
            {
                waitMask1.promptModeShowMessage(langStrObject.GetLangString("ETCS_U_DISK_EJECT_FAIL"));
            }
        }

    }

    function setFontName(font)
    {
        fontName = font;
    }

    function loaderFont(font)
    {
        var target_font = font;
        if ("" === target_font)
        {
            target_font = "Roboto-Regular.ttf";
        }

        if (target_font === "SourceHanSans-Normal.otf")
        {
            target_font = "SourceHanSansCN-Regular.otf";
        }


        var abPath;
        abPath = logicObject.getFullFontPath(target_font);

        fontLoader.source = abPath;
        if (fontLoader.status == FontLoader.Ready)
        {
            setFontName(fontLoader.name);
        }
    }

    function updateFontFromLanguage(language)
    {
        var tmpFont;
        tmpFont = fontSettingObject.getFontFromLanguage(language)
        loaderFont(tmpFont);
    }

    function changePages(index)
    {
        if (index === "loginPage")
        {
            loginInPage.initStatus();
            mainPage.visible = false;
            lockPage.setVisibleValue(false);
            dragUnit.setDragUnitEnableValue(false);
        }
        else if (index === "mainPage")
        {
            loginInPage.setVisibleValue(false);
            mainPage.initStatus();
            lockPage.setVisibleValue(false);
            dragUnit.setDragUnitEnableValue(true);
        }
        else if (index === "lockPage")
        {
            loginInPage.setVisibleValue(false);
            mainPage.visible = false;
            lockPage.initStatus();
            dragUnit.setDragUnitEnableValue(false);
        }
    }

    function getCurrentPage()
    {
        if ( loginInPage.visible == true)
        {
            return "loginPage";
        }
        else if (mainPage.visible == true)
        {
            return "mainPage";
        }
        else if (lockPage.visible == true)
        {
            return "lockPage";
        }
    }

    function setScreenRotation()
    {
        rootItem.transform = screenRotation;
    }

    function upDateLockScreenTimer()
    {
        setLockScreenTimerInterval();
    }

    function setLockScreenTimerInterval()
    {
        var tmp;
        tmp = userManageObject.getUserListIntValue(userManageObject.getLoginUserId(), "lockTime");
        if (tmp > 0)
        {
            lockScreenTimer.interval = tmp * 1000;
            backlightTimer.interval = tmp * 1000;
            lockScreenTimer.restart();
        }
        else if (tmp === 0)
        {
            backlightTimer.interval = 300 * 1000;
            backlightTimer.restart();
            lockScreenTimer.stop();
        }
        else
        {
            backlightTimer.interval = 300 * 1000;
            backlightTimer.restart();
            lockScreenTimer.stop();
        }
    }

    function touchEventSlot()
    {
        var tmp;

        screenProtectorPage.quitStatus();
        if (backlightTimer.running == false)
        {
            gpioObject.setBacklight(Number(localConfigObject.readConfig("LocalConfig", "Backlight")));
        }

        var tmp;
        tmp = userManageObject.getUserListIntValue(userManageObject.getLoginUserId(), "lockTime");
        if (tmp > 0)
        {
            lockScreenTimer.restart();
            backlightTimer.restart();
        }
        else if (tmp === 0)
        {
            lockScreenTimer.stop();
            backlightTimer.restart();
        }
        else
        {
            lockScreenTimer.restart();
            backlightTimer.restart();
        }
    }

    function updateUDiskIconSlot(status)
    {
        if (status === 1)
        {
            statusBar.insertImg(logicObject.getLocalImagePath() + "USB_1.png");
        }
        else if (status === 2)
        {
            statusBar.removeImg(logicObject.getLocalImagePath() + "USB_1.png");
        }
        else
        {
            ;
        }
    }

    function logOutUser()
    {
        zoneManageObject.unSelectAll();
        mainPage.changeHomeView("Predefined");
        mainPage.setClickedButton("Predefine");
    }

    function dealCallError(errorCode)
    {
        if (errorCode === 0)
        {
            return 0;
        }
        if (errorCode === 34)
        {
            promptMask.messageShow(langStrObject.GetLangString("ETCS_MAX_AUDIO_JOB"));
            timeOutTimer.setInterval(3000);
            timeOutTimer.start();
            return -1;
        }
        else if (errorCode === 35)
        {
            promptMask.messageShow(langStrObject.GetLangString("ETCS_ACTIVATE_QUEUE_FULL"));
            timeOutTimer.setInterval(3000);
            timeOutTimer.start();
            return -1;
        }
        else
        {
            return 0;
        }
    }

    id: rootItem

    x:0
    y:0
    width: logicObject.getRealWidth()
    height: logicObject.getRealHeight()
    color: "#ffffff"
    visible: true
    clip: true

    Rotation{
        id: screenRotation
        angle: -90
        origin.x: 240
        origin.y: 240
    }

    FontLoader{
        id: fontLoader
    }

    FontLoader{

        function initFont(font)
        {
            var abPath;
            abPath = logicObject.getFullFontPath(font);
            if (abPath === "")
            {
                return false;
            }

            specialFontLoader.source = abPath;
            if (specialFontLoader.status == FontLoader.Ready)
            {
                return true;
            }

            return true;
        }

        function getSpecialFontName()
        {
            return specialFontLoader.name;
        }

        id: specialFontLoader

    }

    Rectangle{
        id: borderRect
        width: parent.width
        height: parent.height
        anchors.fill: parent
        visible: true
        color: "#00000000"
        border.width: 2
        border.color: "#000000"
        z: 10
    }

    DragUnit{
        id: dragUnit
        width: parent.width
        height: 80
        z: 3
        opacity: 0
        color: "green"
        errorScope: 1
        spaceFromYAndHeight: 50
        targetWindowHeight: parent.height
    }

    PullDownPage{
        id: pullDownPage
        z: 2
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width
        height: 0
        baseHeight: parent.height
        visible: true
        clip: true
    }

    StatusBar{
        id: statusBar
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width
        height: statusBarHeight
        visible: true
        z: 1
    }

    LockPage{
        id: lockPage
        width: parent.width
        height: parent.height
        lockImgSrc: localConfigObject.getLockImg()
        visible: false
    }

    LoginIn{
        id: loginInPage
        width: parent.width
        height: parent.height
        anchors.fill: parent
        visible: false
    }

    MainPage{
        id: mainPage
        anchors.fill: parent
        width: parent.width
        height: parent.height
        visible: true
    }

    HardwareSelfTest{
        id: hardwareSelfTest
        anchors.fill: parent
        width: parent.width
        height: parent.height
        visible: false
        z: 10
    }

    ImageBrowser{
        id: imageBrowser
        anchors.fill: parent
        width: parent.width
        height: parent.height
        visible: false
        z: 10
    }

    ScreenProtectorPage{
        id: screenProtectorPage
        anchors.fill: parent
        width: parent.width
        height: parent.height
        visible: false
        z: 11
    }

    TopMessageFrame{
        id: topMessage
        anchors.left: parent.left
        anchors.top: parent.top
        width: 478
        height: 0
        imageSource: logicObject.getLocalImagePath() + "top_message.png"
        mesText: langStrObject.GetLangString("ETCS_SEND_QUEUE")
        visible: true
        z: 4
    }

    Rectangle{

        function initStatus(value)
        {
            localError.setText(value);
            setVisibleValue(true);
            localErrorTimer.start();
        }

        function quitStatus()
        {
            localError.setText("");
            setVisibleValue(false);
        }

        function setVisibleValue(value)
        {
            visible = value;
        }

        id: localErrorMask
        anchors.top: parent.top
        anchors.left: parent.left
        z: 4
        width: parent.width
        height: parent.height
        color: "#70666666"
        visible: false

        MouseArea{
            anchors.fill: parent
            onClicked: {
                localErrorTimer.stop();
                localErrorMask.quitStatus();
            }
        }

        Rectangle{
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            width: 420
            height: 200
            color: "#f0f0f0"

            CommonText{
                id: localError
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width - 60
                text: ""
                color: "#ef574f"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.Wrap
                maximumLineCount: 2
            }
        }

        Timer{
            id: localErrorTimer
            interval: 1500
            repeat: false
            triggeredOnStart: false
            onTriggered: {
                localErrorMask.quitStatus();
            }
        }
    }

    Rectangle{

        signal okSignal();

        function setVisibleValue(value)
        {
            visible = value;
        }

        id: clearConfirmMask
        anchors.top: parent.top
        anchors.left: parent.left
        z: 4
        width: parent.width
        height: parent.height
        color: "#70666666"
        visible: false

        MouseArea{
            anchors.fill: parent
            onClicked: {
            }
        }

        Rectangle{
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            width: 420
            height: 200
            color: "#f0f0f0"

            CommonText{
                id: clearConfirmPrompt
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 60
                width: parent.width - 60
                text: langStrObject.GetLangString("ETCS_CLEAR_OR_NOT")
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.Wrap
                maximumLineCount: 2
            }

            Rectangle{
                width: parent.width
                height: 1
                anchors.bottom: cancleBtn.top
                color: "#c0c0c0"
                z: 2
            }

            Rectangle{
                width: 1
                height: cancleBtn.height
                anchors.bottom: cancleBtn.bottom
                anchors.left: cancleBtn.right
                color: "#c0c0c0"
                z: 2
            }

            TextButon{
                id: cancleBtn
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                width: 210
                height: 53
                info: langStrObject.GetLangString("ETCS_CANCEL")
                textColor:"#57a0fe"
                z: 1
                bkgReleasedColor: "#f0f0f0"
                bkgPressedColor: "#DBE0E6"
                onClickedSignal: {
                    clearConfirmMask.setVisibleValue(false);
                }
            }

            TextButon{
                id: okBtn
                anchors.left: cancleBtn.right
                anchors.bottom: parent.bottom
                width: 210
                height: 53
                info: langStrObject.GetLangString("ETCS_OK");
                textColor:"#57a0fe"
                z: 1
                bkgReleasedColor: "#f0f0f0"
                bkgPressedColor: "#DBE0E6"
                onClickedSignal: {
                    clearConfirmMask.okSignal();
                    clearConfirmMask.setVisibleValue(false);
                }
            }
        }
    }

    Rectangle{

        signal cancelSignal();
        signal okSignal();

        function setVisibleValue(value)
        {
            visible = value;
        }

        id: confirmMask
        anchors.top: parent.top
        anchors.left: parent.left
        z: 4
        width: parent.width
        height: parent.height
        color: "#70666666"
        visible: false

        MouseArea{
            anchors.fill: parent
            onClicked: {
            }
        }

        Rectangle{
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            width: 420
            height: 200
            color: "#f0f0f0"

            CommonText{
                id: confirmPrompt
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 60
                width: parent.width - 60
                text: langStrObject.GetLangString("ETCS_REMOVE_ITEM")
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.Wrap
                maximumLineCount: 2
            }

            Rectangle{
                width: parent.width
                height: 1
                anchors.bottom: cancelButton.top
                color: "#c0c0c0"
                z: 2
            }

            Rectangle{
                width: 1
                height: cancelButton.height
                anchors.bottom: cancelButton.bottom
                anchors.left: cancelButton.right
                color: "#c0c0c0"
                z: 2
            }

            TextButon{
                id: cancelButton
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                width: 210
                height: 53
                info: langStrObject.GetLangString("ETCS_CANCEL")
                textColor:"#57a0fe"
                z: 1
                bkgReleasedColor: "#f0f0f0"
                bkgPressedColor: "#DBE0E6"
                onClickedSignal: {
                    confirmMask.cancelSignal();
                    confirmMask.setVisibleValue(false);
                }
            }

            TextButon{
                id: okButton
                anchors.left: cancelButton.right
                anchors.bottom: parent.bottom
                width: 210
                height: 53
                info: langStrObject.GetLangString("ETCS_OK")
                textColor:"#57a0fe"
                z: 1
                bkgReleasedColor: "#f0f0f0"
                bkgPressedColor: "#DBE0E6"
                onClickedSignal: {
                    confirmMask.okSignal();
                    confirmMask.setVisibleValue(false);
                }
            }
        }
    }

    Rectangle{
        function setVisibleValue(value)
        {
            visible = value;
        }

        id: noticeEditMode
        anchors.top: parent.top
        anchors.left: parent.left
        z: 4
        width: parent.width
        height: parent.height
        color: "#70666666"
        visible: false

        MouseArea{
            anchors.fill: parent
            onClicked: {
            }
        }

        Rectangle{
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            width: 420
            height: 200
            color: "#f0f0f0"

            CommonText{
                id: noticeEditModePrompt
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 60
                width: parent.width - 60
                text: langStrObject.GetLangString("ETCS_PREDEFINE_EDIT_MODE")
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.Wrap
                maximumLineCount: 2
            }

            Rectangle{
                width: parent.width
                height: 1
                anchors.bottom: noticeEditModeOkButton.top
                color: "#c0c0c0"
                z: 2
            }

            TextButon{
                id: noticeEditModeOkButton
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                width: 420
                height: 53
                info: langStrObject.GetLangString("ETCS_OK")
                textColor:"#57a0fe"
                z: 1
                bkgReleasedColor: "#f0f0f0"
                bkgPressedColor: "#DBE0E6"
                onClickedSignal: {
                    noticeEditMode.setVisibleValue(false);
                }
            }
        }
    }

    Rectangle{
        signal cancelSignal();
        signal okSignal();

        function setVisibleValue(value)
        {
            visible = value;
        }

        id: confirmResetPwd
        anchors.top: parent.top
        anchors.left: parent.left
        z: 4
        width: parent.width
        height: parent.height
        color: "#70666666"
        visible: false

        MouseArea{
            anchors.fill: parent
            onClicked: {
            }
        }

        Rectangle{
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            width: 420
            height: 200
            color: "#f0f0f0"

            CommonText{
                id: confirmResetPwdDesc
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 60
                width: parent.width - 60
                text: langStrObject.GetLangString("RESET_CONFIG_PWD")
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.Wrap
                maximumLineCount: 2
            }

            Rectangle{
                width: parent.width
                height: 1
                anchors.bottom: resetPwdCancelBtn.top
                color: "#c0c0c0"
                z: 2
            }

            Rectangle{
                width: 1
                height: resetPwdCancelBtn.height
                anchors.bottom: resetPwdCancelBtn.bottom
                anchors.left: resetPwdCancelBtn.right
                color: "#c0c0c0"
                z: 2
            }

            TextButon{
                id: resetPwdCancelBtn
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                width: 210
                height: 53
                info: langStrObject.GetLangString("ETCS_CANCEL")
                textColor:"#57a0fe"
                z: 1
                bkgReleasedColor: "#f0f0f0"
                bkgPressedColor: "#DBE0E6"
                onClickedSignal: {
                    confirmResetPwd.setVisibleValue(false);
                }
            }

            TextButon{
                id: resetPwdOKBtn
                anchors.left: resetPwdCancelBtn.right
                anchors.bottom: parent.bottom
                width: 210
                height: 53
                info: langStrObject.GetLangString("ETCS_OK")
                textColor:"#57a0fe"
                z: 1
                bkgReleasedColor: "#f0f0f0"
                bkgPressedColor: "#DBE0E6"
                onClickedSignal: {
                    confirmResetPwd.okSignal();
                    confirmResetPwd.setVisibleValue(false);
                }
            }
        }
    }

    Rectangle {

        function setMessage(text)
        {
            message.setText(text);
        }

        function setVisibleValue(value)
        {
            visible = value;
        }

        function messageShow(text)
        {
            message.setText(text);
            setVisibleValue(true);
        }

        function messageHide()
        {
            message.setText("");
            setVisibleValue(false);
        }

        id: promptMask
        anchors.top: parent.top
        anchors.left: parent.left
        z: 11
        width: parent.width
        height: parent.height
        color: "#70666666"
        visible: false
        clip: true

        MouseArea{
            anchors.fill: parent
        }

        Rectangle{
            id: prompt
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            width: 420
            height: 200
            color: "#f0f0f0"
            Text{
                function setText(value)
                {
                    text = value;
                }

                id: message
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                font.family: rootItem.fontName
                color: "#ef574f"
                font.pointSize: 12
                text: ""
                width: parent.width - 60
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.Wrap
                maximumLineCount: 2
            }

            Timer{

                function setInterval(value)
                {
                    interval = value;
                }

                id: timeOutTimer
                interval: 10 * 1000
                repeat: false
                triggeredOnStart : false
                onTriggered: {
                    promptMask.messageHide();
                }
            }
        }
    }

    Rectangle{

        property int umountId: 110

        function getUmountId()
        {
            return umountId;
        }

        function promptModeNoAniShowMessage(value)
        {
            promptMode();
            logMessage.setText(value);
            setVisibleValue(true);
        }

        function promptModeShowMessage(value)
        {
            promptMode();
            logMessage.setText(value);
            setVisibleValue(true);
            waitMask1Ani.start();
        }

        function interModeShowMessage(value)
        {
            interMode();
            logMessage.setText(value);
            setVisibleValue(true);
        }

        function hideMessage()
        {
            promptMode();
            waitMask1Ani.stop();
            logMessage.setText("");
            setVisibleValue(false);
        }

        function setVisibleValue(value)
        {
            visible = value;
        }

        function promptMode()
        {
            commonline1.setVisibleValue(false);
            commonline2.setVisibleValue(false);
            commonCancelBtn.setVisibleValue(false);
            commonOKBtn.setVisibleValue(false);
        }

        function interMode()
        {
            commonline1.setVisibleValue(true);
            commonline2.setVisibleValue(true);
            commonCancelBtn.setVisibleValue(true);
            commonOKBtn.setVisibleValue(true);
        }

        id: waitMask1
        anchors.top: parent.top
        anchors.left: parent.left
        z: 11
        width: parent.width
        height: parent.height
        color: "#70666666"
        visible: false
        MouseArea{
            anchors.fill: parent
            onClicked: {

            }
        }
        Rectangle{
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            width: 420
            height: 200
            color: "#f0f0f0"
            CommonText{
                id: logMessage
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 60
                width: parent.width - 60
                color: "#ef574f"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.Wrap
                maximumLineCount: 2
            }

            Rectangle{
                function setVisibleValue(value)
                {
                    visible = value;
                }

                id: commonline1
                width: parent.width
                height: 1
                anchors.bottom: commonCancelBtn.top
                color: "#c0c0c0"
                z: 2
            }

            Rectangle{
                function setVisibleValue(value)
                {
                    visible = value;
                }

                id: commonline2
                width: 1
                height: commonCancelBtn.height
                anchors.bottom: commonCancelBtn.bottom
                anchors.left: commonCancelBtn.right
                color: "#c0c0c0"
                z: 2
            }

            TextButon{
                id: commonCancelBtn
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                width: 210
                height: 53
                info: langStrObject.GetLangString("ETCS_CANCEL")
                textColor:"#57a0fe"
                z: 1
                bkgReleasedColor: "#f0f0f0"
                bkgPressedColor: "#DBE0E6"
                onClickedSignal: {
                    waitMask1.hideMessage();
                }
            }

            TextButon{
                id: commonOKBtn
                anchors.left: commonCancelBtn.right
                anchors.bottom: parent.bottom
                width: 210
                height: 53
                info: langStrObject.GetLangString("ETCS_OK")
                textColor:"#57a0fe"
                z: 1
                bkgReleasedColor: "#f0f0f0"
                bkgPressedColor: "#DBE0E6"
                onClickedSignal: {
                    waitMask1.promptModeNoAniShowMessage(langStrObject.GetLangString("ETCS_WAIT"));
                    waitMask1HideTimer.start();
                    logicObject.clearAllUsbActivate();
                    udiskObject.umountUDisk(waitMask1.umountId);
                }
            }
        }

        PropertyAnimation{
            id: waitMask1Ani
            target: waitMask1
            properties: "visible"
            to: false
            duration: 1500
        }

        Timer{
            id: waitMask1HideTimer
            interval: 10 * 1000
            repeat: false
            triggeredOnStart: true
            onTriggered: {
                waitMask1.hideMessage();
            }
        }
    }

    Timer{
        id: lockScreenTimer
        interval: 300 * 1000
        repeat: true
        onTriggered: {
            var ret = getCurrentPage();
            if  (ret !== "lockPage")
            {
                dragUnit.initTargetPageStatus();
                changePages("lockPage");
            }
        }
    }

    Timer{

        property bool timerEnabled: true

        function setTimerEnabledValue(value)
        {
            timerEnabled = value;
        }

        function getTimerEnabledValue()
        {
            return timerEnabled;
        }

        id: backlightTimer
        interval: 300 * 1000
        repeat: false
        onTriggered: {
            if (getTimerEnabledValue() === true)
            {
                gpioObject.setBacklight(0);
                screenProtectorPage.initStatus();
            }
            else
            {
                ;
            }
        }
    }

    Timer{
        id: watchdogTimer
        interval: 10 * 1000
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            gpioObject.feadWatchdog();
        }
    }

    Timer{
        id: testTimer
        interval: 200
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            logicObject.timerUpdate200();
        }
    }

    Component.onCompleted: {
        if ((Screen.width == 800) && (Screen.height == 480))
        {
            setScreenRotation();
        }

        updateFontFromLanguage(localConfigObject.readConfig("LocalConfig", "Language"));

        specialFontLoader.initFont("Roboto-Regular.ttf");
        specialFontLoader.initFont("SourceHanSans-Normal.otf");
        if (!specialFontLoader.initFont("SourceHanSans-Normal.otf"))
        {
            specialFontLoader.initFont("SourceHanSansCN-Regular.otf");
        }


        changePages("loginPage");
        upDateLockScreenTimer();
        gpioObject.touchEventSignal.connect(touchEventSlot);
        logicObject.insertImgSignal.connect(statusBar.insertImg);
        logicObject.removeImgSignal.connect(statusBar.removeImg);
        logicObject.setStatusBarMessageSignal.connect(statusBar.setStatusBarMessageTextByFault);
        logicObject.removeStatusBarMessageSignal.connect(statusBar.clearBarMessageTextFromTextKey);
        logicObject.systemResultSignal.connect(rootItem.systemResultSlot);
        rootItem.updateLanguage.connect(updateLanguageSlot);
        dragUnit.dragUnitIsvalid.connect(pullDownPage.setPullBtnStatus);
        dragUnit.dragUnitStatusSignal.connect(pullDownPage.setPullBtnImgStatus);
        gpioObject.firstSetBacklight(localConfigObject.readConfig("LocalConfig", "Backlight"));
        udiskObject.updateUDiskIconSignal.connect(updateUDiskIconSlot);
        backlightTimer.start();
        logicObject.runLogic();
        udiskObject.init();
        gpioObject.openWatchdog();
        watchdogTimer.start();
        testTimer.start();
    }
}
