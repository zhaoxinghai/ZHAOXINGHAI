import QtQuick 2.0
import "../Components"

Rectangle {

    property int childWidth: parent.width
    property int settingsAreaHeight: height - mainPage.g_titleBarHeight
    property int available: 0
    property int total: 0

    function updateLanguageSlot()
    {
        title.text = langStrObject.GetLangString("ETCS_SETTINGS");
        prevTitleName.text = langStrObject.GetLangString("ETCS_MORE");
        stationNameBtn.btnName = langStrObject.GetLangString("ETCS_STATION_NAME");
        stationNoteBtn.btnName = langStrObject.GetLangString("ETCS_SYSTEM_NUMBER");
        ipAddressBtn.btnName = langStrObject.GetLangString("ETCS_IP_ADDRESS");
        vlanBtn.btnName = langStrObject.GetLangString("ETCS_VLAN");
        userNameBtn.btnName = langStrObject.GetLangString("ETCS_USER");
        dkmNumBtn.btnName = langStrObject.GetLangString("ETCS_DKM_NUM");
        dkmNumBtn.infoText = langStrObject.GetLangString("ETCS_AVAIlABLE") + ": " + available +
                            " " + langStrObject.GetLangString("ETCS_PCS") + ", " +
                            langStrObject.GetLangString("ETCS_TOTAL") + ": " +
                            total + " " +langStrObject.GetLangString("ETCS_PCS") ;
        localStorageBtn.btnName = langStrObject.GetLangString("ETCS_LOCAL_STROAGE");
        lockScreenTimeBtn.btnName = langStrObject.GetLangString("ETCS_LOCK_SCREEN_TIME");
        uploadMasterBtn.btnName = langStrObject.GetLangString("ETCS_UPLOAD_MASTER");
        localMACBtn.btnName = langStrObject.GetLangString("ETCS_MAC");
        //regionBtn.btnName = langStrObject.GetLangString("ETCS_REGION");
        dayAndTimeBtn.btnName = langStrObject.GetLangString("ETCS_DAY_AND_TIME");
        languageBtn.btnName = langStrObject.GetLangString("ETCS_LANGUAGE");
        aboutBtn.btnName = langStrObject.GetLangString("ETCS_ABOUT");
        backlightBtn.btnName = langStrObject.GetLangString("ETCS_BACKLIGHT");
        updateSettingsValue();
    }

    function changeUserSlot(value)
    {
        if (value === 0)
        {
            ipAddressBtn.setBtnEnableValue(true);
            ipAddressBtn.setBtnClickImgSrc(logicObject.getLocalImagePath() + "next_1.png");
            vlanBtn.setBtnEnableValue(true);
            vlanBtn.setBtnClickImgSrc(logicObject.getLocalImagePath() + "next_1.png");
            lockScreenTimeBtn.setBtnEnableValue(true);
            lockScreenTimeBtn.setBtnClickImgSrc(logicObject.getLocalImagePath() + "next_1.png");
            dayAndTimeBtn.setBtnEnableValue(true);
            dayAndTimeBtn.setBtnClickImgSrc(logicObject.getLocalImagePath() + "next_1.png");
            languageBtn.setBtnEnableValue(true);
            languageBtn.setBtnClickImgSrc(logicObject.getLocalImagePath() + "next_1.png");
            //regionBtn.setBtnEnableValue(true);
            //regionBtn.setBtnClickImgSrc(logicObject.getLocalImagePath() + "next_1.png");
        }
        else if (value === 1)
        {
            ipAddressBtn.setBtnEnableValue(true);
            ipAddressBtn.setBtnClickImgSrc(logicObject.getLocalImagePath() + "next_1.png");
            vlanBtn.setBtnEnableValue(true);
            vlanBtn.setBtnClickImgSrc(logicObject.getLocalImagePath() + "next_1.png");
            lockScreenTimeBtn.setBtnEnableValue(true);
            lockScreenTimeBtn.setBtnClickImgSrc(logicObject.getLocalImagePath() + "next_1.png");
            dayAndTimeBtn.setBtnEnableValue(true);
            dayAndTimeBtn.setBtnClickImgSrc(logicObject.getLocalImagePath() + "next_1.png");
            languageBtn.setBtnEnableValue(true);
            languageBtn.setBtnClickImgSrc(logicObject.getLocalImagePath() + "next_1.png");
            //regionBtn.setBtnEnableValue(true);
            //regionBtn.setBtnClickImgSrc(logicObject.getLocalImagePath() + "next_1.png");
        }
        else if (value === 2)
        {
            ipAddressBtn.setBtnEnableValue(false);
            ipAddressBtn.setBtnClickImgSrc("");
            vlanBtn.setBtnEnableValue(false);
            vlanBtn.setBtnClickImgSrc("");
            lockScreenTimeBtn.setBtnEnableValue(true);
            lockScreenTimeBtn.setBtnClickImgSrc(logicObject.getLocalImagePath() + "next_1.png");
            dayAndTimeBtn.setBtnEnableValue(false);
            dayAndTimeBtn.setBtnClickImgSrc("");
            languageBtn.setBtnEnableValue(false);
            languageBtn.setBtnClickImgSrc("");
            //regionBtn.setBtnEnableValue(false);
            //regionBtn.setBtnClickImgSrc("");
        }

    }

    function initStatus()
    {
        setVisibleValue(true);
        settingsFlickable.setContentY(0);
        updateSettingsValue();
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function updateSettingsValue()
    {
        var note_id = localConfigObject.getLocalNode()
        stationNoteBtn.infoText = String(note_id)
        localMACBtn.infoText = logicObject.getRealMAC()
        stationNameBtn.infoText = localConfigObject.getStationName()
        userNameBtn.infoText = userManageObject.getUserListStrValue(userManageObject.getLoginUserId(), "name");
        ipAddressBtn.infoText = logicObject.getRealIp();
        vlanBtn.infoText = logicObject.getVLANInfo();
        lockScreenTimeBtn.infoText = mapLockTimeToName(userManageObject.getUserListIntValue(userManageObject.getLoginUserId(), "lockTime"));
        //regionBtn.infoText = langStrObject.GetLangString(localConfigObject.readConfig("LocalConfig", "Region"));
        languageBtn.infoText = localConfigObject.readConfig("LocalConfig", "Language");
    }

    function mapLockTimeToName(second)
    {
        if (second === 60)
        {
            return "1  " + langStrObject.GetLangString("ETCS_MINUTE");
        }
        else if (second === 120)
        {
            return "2  " + langStrObject.GetLangString("ETCS_MINUTE");
        }
        else if (second === 300)
        {
            return "5  " + langStrObject.GetLangString("ETCS_MINUTE");
        }
        else if (second === 600)
        {
            return "10 " + langStrObject.GetLangString("ETCS_MINUTE");
        }
        else if (second === 1800)
        {
            return "30 " + langStrObject.GetLangString("ETCS_MINUTE");
        }
        else if (second === 3600)
        {
            return "1  " + langStrObject.GetLangString("ETCS_HOUR")
        }
        else
        {
            return langStrObject.GetLangString("ETCS_OFF");
        }
    }

    function changeView(index)
    {
        detailSettingsView.showRect(index)
    }

    function updateDkmNum(value, setting)
    {
        available = value;
        total = setting;

        dkmNumBtn.infoText = langStrObject.GetLangString("ETCS_AVAIlABLE") + ": " + available +
                            " " + langStrObject.GetLangString("ETCS_PCS") + ", " +
                            langStrObject.GetLangString("ETCS_TOTAL") + ": " +
                            total + " " +langStrObject.GetLangString("ETCS_PCS") ;
    }

    color: "#ffffff"

    MouseArea{
        anchors.fill: parent
        onClicked: {

        }
    }

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
            text: langStrObject.GetLangString("ETCS_SETTINGS")
            font.family: rootItem.fontName
            font.pixelSize: mainPage.g_titleTextFontSize
            width: 200
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            z: 2
            Component.onCompleted: {
            }
        }

        Rectangle{
            id: prevBtnImgRect
            width: prevBtnImg.width + prevTitleName.contentWidth - 8 + 35
            height: parent.height
            anchors.top: parent.top
            anchors.left: parent.left
            color: mouseArea.pressed ? "#505050" : parent.color
            z: 1
            clip: true
            Image{
                id: prevBtnImg
                source: logicObject.getLocalImagePath() + "back.png"
                anchors.left: parent.left
                anchors.leftMargin: 14
                anchors.verticalCenter: parent.verticalCenter
            }
            Text{
                id: prevTitleName
                anchors.left: prevBtnImg.right
                anchors.leftMargin: -8
                anchors.verticalCenter: parent.verticalCenter
                color: mainPage.g_titleTextColor
                text: langStrObject.GetLangString("ETCS_MORE");
                font.family: rootItem.fontName
                font.pixelSize: mainPage.g_titleTextFontSize2
                width: 240 - (title.contentWidth / 2) - prevBtnImg.width - 14 + 8
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
                Component.onCompleted: {
                }
            }
            MouseArea{
                id: mouseArea
                anchors.fill: parent
                onClicked: {
                    setVisibleValue(false);
                    mainPage.showBusinessBar();
                }
            }
        }
    }

    Rectangle{
        id: settingsArea
        anchors.top: titleBar.bottom
        width: childWidth
        height: settingsAreaHeight
        color: parent.color

        Flickable{

            function setContentY(value)
            {
                contentY = value;
            }

            id: settingsFlickable
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: parent.height
            contentWidth: parent.width
            contentHeight: (65 * 14) + (22 * 3)
            flickableDirection: Flickable.VerticalFlick
            boundsBehavior: Flickable.StopAtBounds

            clip: true

            Rectangle{
                id: rect1
                width: parent.width
                height: 22
                anchors.top: parent.top
                anchors.left: parent.left
                color: "#e7e7e7"
            }
            StripButton{
                id: stationNameBtn
                anchors.top: rect1.bottom
                anchors.left: parent.left
                width: parent.width
                color: "#ffffff"
                imgSrc: ""
                btnName:  langStrObject.GetLangString("ETCS_STATION_NAME")
                infoText: localConfigObject.getStationName()
                isClickBtn: false
                clickImgSrc: ""
                btnValue: "stationNameBtn"
                line1Visible: false
                line2Visible: true
                line2Width: parent.width - 18
                buttonNameWidth: 300
                Component.onCompleted: {
                }
            }
            StripButton{
                id: userNameBtn
                anchors.top: stationNameBtn.bottom
                anchors.left: parent.left
                width: parent.width
                imgSrc: ""
                btnName:  langStrObject.GetLangString("ETCS_USER")
                infoText: userManageObject.getUserListStrValue(userManageObject.getLoginUserId(), "name")
                isClickBtn: false
                clickImgSrc: ""
                btnValue: "userNameBtn"
                line1Visible: false
                line2Visible: true
                buttonNameWidth: 250
                Component.onCompleted: {
                }
            }
            StripButton{
                id: stationNoteBtn
                anchors.top: userNameBtn.bottom
                anchors.left: parent.left
                width: parent.width
                imgSrc: ""
                btnName:  langStrObject.GetLangString("ETCS_SYSTEM_NUMBER")
                infoText: String(localConfigObject.getLocalNode())
                isClickBtn: false
                clickImgSrc: ""
                btnValue: "stationNoteBtn"
                line1Visible: false
                line2Visible: false
                buttonNameWidth: 250
                Component.onCompleted: {
                }
            }
            Rectangle{
                id: rect2
                width: parent.width
                height: 22
                anchors.top: stationNoteBtn.bottom
                anchors.left: parent.left
                color: "#e7e7e7"
            }
            StripButton{
                id: ipAddressBtn
                anchors.top: rect2.bottom
                anchors.left: parent.left
                width: parent.width
                imgSrc: ""
                btnName: langStrObject.GetLangString("ETCS_IP_ADDRESS")
                infoText: logicObject.getRealIp()
                isClickBtn: true
                clickImgSrc: logicObject.getLocalImagePath() + "next_1.png"
                btnValue: "ipAddressBtn"
                line1Visible: false
                line2Visible: true
                line2Width: parent.width - 18
                buttonNameWidth: 250
                Component.onCompleted: {
                }
            }
            StripButton{
                id: vlanBtn
                anchors.top: ipAddressBtn.bottom
                anchors.left: parent.left
                width: parent.width
                imgSrc: ""
                btnName: langStrObject.GetLangString("ETCS_VLAN")
                infoText: logicObject.getVLANInfo()
                isClickBtn: true
                clickImgSrc: logicObject.getLocalImagePath() + "next_1.png"
                btnValue: "vlanBtn"
                line1Visible: false
                line2Visible: true
                line2Width: parent.width - 18
                buttonNameWidth: 250
                Component.onCompleted: {
                }
            }
            StripButton{
                id: lockScreenTimeBtn
                anchors.top: vlanBtn.bottom
                anchors.left: parent.left
                width: parent.width
                imgSrc: ""
                btnName: langStrObject.GetLangString("ETCS_LOCK_SCREEN_TIME")
                infoText: mapLockTimeToName(userManageObject.getUserListIntValue(userManageObject.getLoginUserId(), "lockTime"))
                isClickBtn: true
                clickImgSrc: logicObject.getLocalImagePath() + "next_1.png"
                btnValue: "lockScreenTimeBtn"
                line1Visible: false
                line2Visible: true
                line2Width: parent.width - 18
                buttonNameWidth: 300
                Component.onCompleted: {
                }
            }
            StripButton{
                id: dayAndTimeBtn
                anchors.top: lockScreenTimeBtn.bottom
                anchors.left: parent.left
                width: parent.width
                buttonNameWidth: 380
                imgSrc: ""
                btnName: langStrObject.GetLangString("ETCS_DAY_AND_TIME")
                infoText: ""
                isClickBtn: true
                clickImgSrc: logicObject.getLocalImagePath() + "next_1.png"
                btnValue: "dayAndTimeBtn"
                line1Visible: false
                line2Visible: true
                line2Width: parent.width - 18
                Component.onCompleted: {
                }
            }
            StripButton{
                id: languageBtn
                anchors.top: dayAndTimeBtn.bottom
                anchors.left: parent.left
                width: parent.width
                buttonNameWidth: 250
                imgSrc: ""
                btnName: langStrObject.GetLangString("ETCS_LANGUAGE")
                infoFont: specialFontLoader.getSpecialFontName()
                infoText: localConfigObject.readConfig("LocalConfig", "Language")
                isClickBtn: true
                clickImgSrc: logicObject.getLocalImagePath() + "next_1.png"
                btnValue: "languageBtn"
                line1Visible: false
                line2Visible: true
                line2Width: parent.width - 18
                Component.onCompleted: {
                }
            }
            /*
            StripButton{
                id: regionBtn
                anchors.top: languageBtn.bottom
                anchors.left: parent.left
                width: parent.width
                buttonNameWidth: 250
                imgSrc: ""
                btnName: langStrObject.GetLangString("ETCS_REGION")
                infoFont: specialFontLoader.getSpecialFontName()
                infoText: langStrObject.GetLangString(localConfigObject.readConfig("LocalConfig", "Region"))
                isClickBtn: true
                clickImgSrc: logicObject.getLocalImagePath() + "next_1.png"
                btnValue: "regionBtn"
                line1Visible: false
                line2Visible: true
                Component.onCompleted: {
                }
            }
            */

            StripButton{
                id: backlightBtn
                anchors.top: languageBtn.bottom
                anchors.left: parent.left
                width: parent.width
                buttonNameWidth: 380
                imgSrc: ""
                btnName: langStrObject.GetLangString("ETCS_BACKLIGHT")
                infoText: ""
                isClickBtn: true
                clickImgSrc: logicObject.getLocalImagePath() + "next_1.png"
                btnValue: "backlightBtn"
                line1Visible: false
                line2Visible: false
                onBtnValueSignal: {
                    displaySettingView.initStatus();
                }
                Component.onCompleted: {
                }
            }

            Rectangle{
                id: rect4
                width: parent.width
                height: 22
                anchors.top: backlightBtn.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                color: "#e7e7e7"
            }
            StripButton{
                id: dkmNumBtn
                anchors.top: rect4.bottom
                anchors.left: parent.left
                width: parent.width
                buttonNameWidth: 100
                imgSrc: ""
                btnName: langStrObject.GetLangString("ETCS_DKM_NUM")
                infoText: dkmNumBtn.infoText = langStrObject.GetLangString("ETCS_AVAIlABLE") + ": " + available +
                          " " + langStrObject.GetLangString("ETCS_PCS") + ", " +
                          langStrObject.GetLangString("ETCS_TOTAL") + ": " +
                          total + " " +langStrObject.GetLangString("ETCS_PCS") ;
                isClickBtn: false
                clickImgSrc: ""
                btnValue: "dkmNumBtn"
                line1Visible: false
                line2Visible: true
                line2Width: parent.width - 18
                infoTextWidth: parent.width - btnNameContentWidth - 15 - 5
                Component.onCompleted: {
                }
            }
            StripButton{
                id: localStorageBtn
                anchors.top: dkmNumBtn.bottom
                anchors.left: parent.left
                width: parent.width
                imgSrc: ""
                btnName: langStrObject.GetLangString("ETCS_LOCAL_STROAGE")
                infoText: ""
                isClickBtn: true
                clickImgSrc: logicObject.getLocalImagePath() + "next_1.png"
                btnValue: "localStorageBtn"
                line1Visible: false
                line2Visible: true
                line2Width: parent.width - 18
                buttonNameWidth: 150
                onBtnStatusSignal: {
                    localStorage.initStatus();
                }
            }
            StripButton{
                id: uploadMasterBtn
                anchors.top: localStorageBtn.bottom
                anchors.left: parent.left
                width: parent.width
                imgSrc: ""
                btnName: langStrObject.GetLangString("ETCS_UPLOAD_MASTER")
                infoText: localConfigObject.getUploadMaster()
                isClickBtn: false
                clickImgSrc: ""
                btnValue: "uploadMasterBtn"
                line1Visible: false
                line2Visible: true
                line2Width: parent.width - 18
                buttonNameWidth: 200
                infoTextWidth: parent.width - btnNameContentWidth - 15 - 5
                Component.onCompleted: {
                }
            }
            StripButton{
                id: localMACBtn
                anchors.top: uploadMasterBtn.bottom
                anchors.left: parent.left
                width: parent.width
                imgSrc: ""
                btnName: langStrObject.GetLangString("ETCS_MAC")
                infoText: logicObject.getRealMAC()
                isClickBtn: false
                clickImgSrc: ""
                btnValue: "localMACBtn"
                line1Visible: false
                line2Visible: true
                line2Width: parent.width - 18
                buttonNameWidth: 200
                infoTextWidth: parent.width - btnNameContentWidth - 15 - 5
                Component.onCompleted: {
                }
            }
            StripButton{
                id: aboutBtn
                anchors.top: localMACBtn.bottom
                anchors.left: parent.left
                width: parent.width
                imgSrc: ""
                btnName: langStrObject.GetLangString("ETCS_ABOUT")
                infoText: ""
                isClickBtn: true
                clickImgSrc: logicObject.getLocalImagePath() + "next_1.png"
                btnValue: "aboutBtn"
                line1Visible: false
                line2Visible: false
                buttonNameWidth: 400
                Component.onCompleted: {
                }
            }
        }
    }

    ScrollBar{
        id: detailZoneScrollBar
        anchors.top: settingsArea.top
        anchors.right: parent.right
        anchors.rightMargin: 6
        width: 6
        height: settingsArea.height
        orientation: Qt.Vertical
        position: settingsFlickable.visibleArea.yPosition
        pageSize: settingsFlickable.visibleArea.heightRatio
        scrollBarVisible: settingsFlickable.visibleArea.heightRatio == 1 ? false : true
    }

    DetailSettingsView{
        id: detailSettingsView
        width: parent.width
        height: parent.height
        visible: false
    }

    DisplaySettingView{
        id: displaySettingView
        width: parent.width
        height: parent.height
        visible: false
    }

    LocalStorage{
        id: localStorage
        width: parent.width
        height: parent.height
        visible: false
    }

    Component.onCompleted: {
        ipAddressBtn.btnValueSignal.connect(changeView);
        vlanBtn.btnValueSignal.connect(changeView);
        languageBtn.btnValueSignal.connect(changeView);
        lockScreenTimeBtn.btnValueSignal.connect(changeView);
        dayAndTimeBtn.btnValueSignal.connect(changeView);
        //regionBtn.btnValueSignal.connect(changeView);
        aboutBtn.btnValueSignal.connect(changeView);
        detailSettingsView.updateSignal.connect(updateSettingsValue);
        logicObject.updateDkmNumberSignal.connect(updateDkmNum);
        rootItem.updateLanguage.connect(updateLanguageSlot);
        rootItem.changeUserSignal.connect(changeUserSlot);
    }
}
