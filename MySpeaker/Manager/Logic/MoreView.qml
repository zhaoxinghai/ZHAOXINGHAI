import QtQuick 2.0
import "../Components"

Rectangle {

    property int viewHeight: height + mainPage.businessBarHeight

    function updateLanguageSlot()
    {
        title.text = langStrObject.GetLangString("ETCS_MORE");
        settingsBtn.btnName = langStrObject.GetLangString("ETCS_SETTINGS");
        hardwareTestBtn.btnName = langStrObject.GetLangString("ETCS_HARDWARE_TEST");
        recordBtn.btnName = langStrObject.GetLangString("ETCS_RECORD");
        volumeBtn.btnName = langStrObject.GetLangString("ETCS_VOLUME_SETTING");
        vaBtn.btnName = langStrObject.GetLangString("ETCS_VA_BROADCAST");
        copyBtn.btnName = langStrObject.GetLangString("ETCS_LOG_EXPORT");
        logoutName.text = langStrObject.GetLangString("ETCS_LOGOUT");
    }

    function changeUserSlot(value)
    {
        if (value === 0)
        {
            hardwareTestBtn.setHeight(65);
            hardwareTestBtn.setVisibleValue(true);
            volumeBtn.setHeight(65);
            volumeBtn.setVisibleValue(true);
            copyBtn.setHeight(65);
            copyBtn.setVisibleValue(true);
        }
        else if (value === 1)
        {
            hardwareTestBtn.setHeight(65);
            hardwareTestBtn.setVisibleValue(true);
            volumeBtn.setHeight(65);
            volumeBtn.setVisibleValue(true);
            copyBtn.setHeight(65);
            copyBtn.setVisibleValue(true);
        }
        else if (value === 2)
        {
            hardwareTestBtn.setHeight(0);
            hardwareTestBtn.setVisibleValue(false);
            volumeBtn.setHeight(0);
            volumeBtn.setVisibleValue(false);
            copyBtn.setHeight(0);
            copyBtn.setVisibleValue(false);
        }
    }

    function initStatus()
    {
        settingsView.setVisibleValue(false);
        hardwareTestView.setVisibleValue(false);
        recordView.setVisibleValue(false);
        volumeView.setVisibleValue(false);
        vaPage.setVisibleValue(false);
        setVisibleValue(true);
        //mainPage.showBusinessBar();
    }

    function changeView(index)
    {
        if (index === "settingsBtn")
        {
            settingsView.initStatus();
            hardwareTestView.setVisibleValue(false);
            recordView.setVisibleValue(false);
            volumeView.setVisibleValue(false);
            vaPage.setVisibleValue(false);
            mainPage.hideBusinessBar();
        }
        else if (index === "hardwareTestBtn")
        {
            settingsView.setVisibleValue(false);
            hardwareTestView.setVisibleValue(true);
            recordView.setVisibleValue(false);
            volumeView.setVisibleValue(false);
            vaPage.setVisibleValue(false);
            mainPage.hideBusinessBar();
        }
        else if (index === "recordBtn")
        {
            settingsView.setVisibleValue(false);
            hardwareTestView.setVisibleValue(false);
            recordView.initStatus();
            volumeView.setVisibleValue(false);
            vaPage.setVisibleValue(false);
            mainPage.hideBusinessBar();
        }
        else if (index === "volumeBtn")
        {
            settingsView.setVisibleValue(false);
            hardwareTestView.setVisibleValue(false);
            recordView.setVisibleValue(false);
            volumeView.initStatus();
            vaPage.setVisibleValue(false);
            mainPage.hideBusinessBar();
        }

        else if (index === "vaBtn")
        {
            settingsView.setVisibleValue(false);
            hardwareTestView.setVisibleValue(false);
            recordView.setVisibleValue(false);
            volumeView.setVisibleValue(false);
            vaPage.initStatus();
            mainPage.hideBusinessBar();
        }
        else if (index === "logoutBtn")
        {
            rootItem.changePages("lockPage");
        }
    }

    function setVisibleValue(value)
    {
        visible = value;
        if (false === value)
        {
            settingsView.setVisibleValue(false);
            hardwareTestView.setVisibleValue(false);
            recordView.setVisibleValue(false);
            volumeView.setVisibleValue(false);
            vaPage.setVisibleValue(false);
            vaLogin.setVisibleValue(false);
        }
    }

    function getVAPageVisible()
    {
        var va_visible = vaPage.visible;
        if (va_visible)
        {
            return true;
        }

        return false;
    }

    color: "#e7e7e7"

    Rectangle{
        id: titleBar
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width
        height: mainPage.g_titleBarHeight
        color: mainPage.g_titleColor

        Text{
            id: title
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: mainPage.g_titleTextColor
            text: langStrObject.GetLangString("ETCS_MORE")
            font.family: rootItem.fontName
            font.pixelSize: mainPage.g_titleTextFontSize
            width: parent.width
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            Component.onCompleted: {
            }
        }
    }

    StripButton{
        id: settingsBtn
        anchors.top: titleBar.bottom
        anchors.topMargin: 25
        anchors.left: parent.left
        imgSrc: logicObject.getLocalImagePath() + "setting.png"
        btnName: langStrObject.GetLangString("ETCS_SETTINGS")
        isClickBtn: true
        clickImgSrc: logicObject.getLocalImagePath() + "next_1.png"
        btnValue: "settingsBtn"
        line1Visible: false
        line2Visible: false
        buttonImageWidth: 36
        buttonImageHeight: 35
        buttonNameWidth: 380
        Component.onCompleted: {
        }
    }

    StripButton{
        id: hardwareTestBtn
        anchors.top: settingsBtn.bottom
        anchors.left: parent.left
        imgSrc: logicObject.getLocalImagePath() + "hardware_test.png"
        btnName: langStrObject.GetLangString("ETCS_HARDWARE_TEST")
        isClickBtn: true
        clickImgSrc: logicObject.getLocalImagePath() + "next_1.png"
        btnValue: "hardwareTestBtn"
        line1Visible: true
        line2Visible: false
        buttonNameWidth: 380
        Component.onCompleted: {
        }
    }

    StripButton{
        id: recordBtn
        anchors.top: hardwareTestBtn.bottom
        anchors.left: parent.left
        imgSrc: logicObject.getLocalImagePath() + "record.png"
        btnName: langStrObject.GetLangString("ETCS_RECORD")
        isClickBtn: true
        clickImgSrc: logicObject.getLocalImagePath() + "next_1.png"
        btnValue: "recordBtn"
        line1Visible: true
        line2Visible: false
        buttonNameWidth: 380
        Component.onCompleted: {
        }
    }

    StripButton{
        id: volumeBtn
        anchors.top: recordBtn.bottom
        anchors.left: parent.left
        imgSrc: logicObject.getLocalImagePath() + "volume_1.png"
        btnName: langStrObject.GetLangString("ETCS_VOLUME_SETTING")
        isClickBtn: true
        clickImgSrc: logicObject.getLocalImagePath() + "next_1.png"
        btnValue: "volumeBtn"
        line1Visible: true
        line2Visible: false
        buttonNameWidth: 380
        Component.onCompleted: {
        }
    }

    StripButton{
        id: vaBtn
        anchors.top: volumeBtn.bottom
        anchors.left: parent.left
        imgSrc: logicObject.getLocalImagePath() + "VA.png"
        btnName: langStrObject.GetLangString("ETCS_VA_BROADCAST")
        isClickBtn: true
        clickImgSrc: logicObject.getLocalImagePath() + "next_1.png"
        btnValue: "vaBtn"
        line1Visible: true
        line2Visible: false
        buttonNameWidth: 380
        onBtnValueSignal: {
            vaLogin.initStatus();
            mainPage.hideBusinessBar();
        }
        Component.onCompleted: {
        }
    }

    StripButton{

        property bool isDetected: false

        function setIsDetected(value)
        {
            isDetected = value;
        }

        function getIsDetected()
        {
            return isDetected;
        }

        function copyLogResultSlot(id, ret)
        {
            if (getIsDetected() === false)
            {
                return;
            }

            if (id === 1)
            {
                if (ret === 256)
                {
                    waitMask1.promptModeShowMessage(langStrObject.GetLangString("ETCS_LOG_EXPORT_1"));
                }
                else if (ret === 0)
                {
                    logicObject.logicSync();
                    var ret = logicObject.isDataExist();
                    if (ret === true)
                    {
                        waitMask1.interModeShowMessage(langStrObject.GetLangString("ETCS_LOG_EXPORT_0"));
                    }
                    else
                    {
                        waitMask1.promptModeShowMessage(langStrObject.GetLangString("ETCS_LOG_EXPORT_1"));
                    }
                }
                else
                {
                    waitMask1.promptModeShowMessage(langStrObject.GetLangString("ETCS_LOG_EXPORT_1"));
                }
                setIsDetected(false)
                //copyLogTimeoutTimer.stop();
            }
        }

        id: copyBtn
        anchors.top: vaBtn.bottom
        anchors.left: parent.left
        imgSrc: logicObject.getLocalImagePath() + "log_export.png"
        btnName: langStrObject.GetLangString("ETCS_LOG_EXPORT")
        isClickBtn: true
        clickImgSrc: ""
        btnValue: "copyBtn"
        line1Visible: true
        line2Visible: false
        buttonNameWidth: 380
        onBtnValueSignal: {
            var ret;
            var status;
            var isEject;

            waitMask1.promptModeNoAniShowMessage(langStrObject.GetLangString("ETCS_LOG_EXPORT_2"));
            isEject = udiskObject.checkUDiskStatus();
            if (isEject === 0)
            {
                ret = udiskObject.isUDiskMounted();
                if (ret === true)
                {
                    setIsDetected(true);
                    logicObject.copyLog();
                    //copyLogTimeoutTimer.start();
                }
                else
                {
                    setIsDetected(false);
                    waitMask1.promptModeShowMessage(langStrObject.GetLangString("ETCS_U_DISK_ERROR_0"));
                }
            }
            else
            {
                setIsDetected(false);
                waitMask1.promptModeShowMessage(langStrObject.GetLangString("ETCS_U_DISK_ERROR_0"));
            }
        }
        Component.onCompleted: {

        }
    }

    RectButton{
        id: logoutBtn
        anchors.top: copyBtn.bottom
        anchors.topMargin: 25
        anchors.left: parent.left
        width: 480
        height: 65
        line1Visible: false
        line2Visible: false
        bkgPressedColor: "#dbe0e6"
        bkgReleasedColor: "#ffffff"

        Text {
            id: logoutName
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: rootItem.fontName
            font.pixelSize: 18
            color: "#f15b53"
            text: langStrObject.GetLangString("ETCS_LOGOUT")
            width: parent.width
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            Component.onCompleted: {
            }
        }

        onClickedSignal: {
            //logicObject.exitETCS();
            //Qt.quit();
            changeView("logoutBtn");
        }
    }

    SettingsView{
        id: settingsView
        width: parent.width
        height: viewHeight
        visible: false
    }

    HardwareTestView{
        id: hardwareTestView
        width: parent.width
        height: viewHeight
        visible: false
    }

    RecordView{
        id: recordView
        width: parent.width
        height: viewHeight
        visible: false
    }

    VolumeView{
        id: volumeView
        width: parent.width
        height: viewHeight
        visible: false
    }

    VALogin{
        id: vaLogin
        width: parent.width
        height: viewHeight
        visible: false

        onLoginSignal: {
            setVisibleValue(false);
            changeView("vaBtn");
        }

        onCancelSignal: {
            setVisibleValue(false);
            mainPage.showBusinessBar();
        }
    }

    VAPage{
        id: vaPage
        width: parent.width
        height: viewHeight
        visible: false
        z: 1
        onQuitSignal: {
            if (getId() !== 0)
            {
                pageObject.vaLiveStopCall(getId());
                setId(0);
                statusBar.setStatusBarMessageTextForTimer(langStrObject.GetLangString("ETCS_CALL_ERR_16"), "ETCS_CALL_ERR_16");
            }

            setVisibleValue(false);
            mainPage.showBusinessBar();
            zoneManageObject.ResetStateVA();
        }
        onLockSignal: {
            if (getId() !== 0)
            {
                pageObject.vaLiveStopCall(getId());
                setId(0);
                statusBar.setStatusBarMessageTextForTimer(langStrObject.GetLangString("ETCS_CALL_ERR_16"), "ETCS_CALL_ERR_16");
            }
            setVisibleValue(false);
            vaLogin.initStatus();
        }
    }
    /*
    Timer{
        id: copyLogTimeoutTimer
        interval: 60 * 1000
        repeat: false
        triggeredOnStart: false
        onTriggered: {
            copyBtn.setIsDetected(false);
            waitMask1.hideMessage();
        }
    }
    */
    Component.onCompleted: {
        settingsBtn.btnValueSignal.connect(changeView);
        hardwareTestBtn.btnValueSignal.connect(changeView);
        recordBtn.btnValueSignal.connect(changeView);
        volumeBtn.btnValueSignal.connect(changeView);
        rootItem.updateLanguage.connect(updateLanguageSlot);
        rootItem.changeUserSignal.connect(changeUserSlot);
        logicObject.systemResultSignal.connect(copyBtn.copyLogResultSlot);
    }
}

