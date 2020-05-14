import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.4
import "../Components"

Rectangle {

    property int viewRectHeight: height - mainPage.g_titleBarHeight
    property string settingType: ""
    signal updateSignal();

    function updateLanguageSlot()
    {
        if (ipSettingRect.getVisibleValue() === true)
        {
            setTitleText(langStrObject.GetLangString("ETCS_NET_SETTINGS"));
        }
        else if (vlanSettingRect.visible == true)
        {
            setTitleText(langStrObject.GetLangString("ETCS_VLAN_SETTINGS"));
        }
        else if (languageSettingRect.visible == true)
        {
            setTitleText(langStrObject.GetLangString("ETCS_LANGUAGE"));
        }
        else if (lockScreenTimeSettingRect.visible == true)
        {
            setTitleText(langStrObject.GetLangString("ETCS_LOCK_SCREEN_TIME"));
        }
        else if (dayAndTimeSettingRect.visible == true)
        {
            setTitleText(langStrObject.GetLangString("ETCS_DAY_AND_TIME"));
        }
        else if (regionSettingRect.visible == true)
        {
            setTitleText(langStrObject.GetLangString("ETCS_REGION"));
        }
        else if (about.visible == true)
        {
            setTitleText(langStrObject.GetLangString("ETCS_ABOUT"));
        }

        prevTitleName.text = langStrObject.GetLangString("ETCS_SETTINGS");
    }

    function setTitleText(text)
    {
        title.text = text;
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function showRect(index)
    {
        setVisibleValue(true);
        if (index === "ipAddressBtn")
        {
            setTitleText(langStrObject.GetLangString("ETCS_NET_SETTINGS"));
            ipSettingRect.initStatus();
            vlanSettingRect.setVisibleValue(false);
            languageSettingRect.visible = false;
            lockScreenTimeSettingRect.setVisibleValue(false);
            dayAndTimeSettingRect.visible = false;
            regionSettingRect.visible = false;
            about.visible = false;
        }
        else if (index === "vlanBtn")
        {
            setTitleText(langStrObject.GetLangString("ETCS_VLAN_SETTINGS"));

            vlanSettingRect.setVisibleValue(true);
            ipSettingRect.setVisibleValue(false);
            vlanSettingRect.initStatus();
            lockScreenTimeSettingRect.setVisibleValue(false);
            languageSettingRect.visible = false;
            dayAndTimeSettingRect.visible = false;
            regionSettingRect.visible = false;
            about.visible = false;
        }
        else if (index === "languageBtn")
        {
            setTitleText(langStrObject.GetLangString("ETCS_LANGUAGE"));
            ipSettingRect.setVisibleValue(false);
            vlanSettingRect.setVisibleValue(false);
            lockScreenTimeSettingRect.setVisibleValue(false);
            languageSettingRect.visible = true;
            dayAndTimeSettingRect.visible = false;
            regionSettingRect.visible = false;
            about.visible = false;
        }
        else if (index === "lockScreenTimeBtn")
        {
            setTitleText(langStrObject.GetLangString("ETCS_LOCK_SCREEN_TIME"));
            ipSettingRect.setVisibleValue(false);
            vlanSettingRect.setVisibleValue(false);
            languageSettingRect.visible = false;
            lockScreenTimeSettingRect.initStatus();
            dayAndTimeSettingRect.visible = false;
            regionSettingRect.visible = false;
            about.visible = false;
        }
        else if (index === "dayAndTimeBtn")
        {
            setTitleText(langStrObject.GetLangString("ETCS_DAY_AND_TIME"));
            ipSettingRect.setVisibleValue(false);
            vlanSettingRect.setVisibleValue(false);
            languageSettingRect.visible = false;
            lockScreenTimeSettingRect.setVisibleValue(false);
            dayAndTimeSettingRect.visible = true;
            regionSettingRect.visible = false;
            about.visible = false;
        }
        /*
        else if (index === "regionBtn")
        {
            setTitleText(langStrObject.GetLangString("ETCS_REGION"));
            ipSettingRect.setVisibleValue(false);
            languageSettingRect.visible = false;
            lockScreenTimeSettingRect.setVisibleValue(false);
            dayAndTimeSettingRect.visible = false;
            regionSettingRect.initStatus();
            about.visible = false;
        }
        */
        else if (index === "aboutBtn")
        {
            setTitleText(langStrObject.GetLangString("ETCS_ABOUT"));
            ipSettingRect.setVisibleValue(false);
            vlanSettingRect.setVisibleValue(false);
            languageSettingRect.visible = false;
            lockScreenTimeSettingRect.setVisibleValue(false);
            dayAndTimeSettingRect.visible = false;
            regionSettingRect.visible = false;
            about.visible = true;
            about.initStatus();
        }
    }

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
            text: langStrObject.GetLangString("ETCS_DETAIL_SETTING")
            font.pixelSize: mainPage.g_titleTextFontSize
            font.family: rootItem.fontName
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
                text: langStrObject.GetLangString("ETCS_SETTINGS");
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
                    updateSignal();
                    setVisibleValue(false);
                }
            }
        }
    }

    NetSetting{
        id: ipSettingRect
        anchors.top: titleBar.bottom
        anchors.left: parent.left
        width: parent.width
        height: viewRectHeight
        visible: false
    }

    VLANSetting{
        id: vlanSettingRect
        anchors.top: titleBar.bottom
        anchors.left: parent.left
        width: parent.width
        height: viewRectHeight
        visible: false
    }

    LanguageListView{
        id: languageSettingRect
        anchors.top: titleBar.bottom
        anchors.left: parent.left
        width: parent.width
        height: viewRectHeight
        visible: false
    }

    LockTimeListView{
        id: lockScreenTimeSettingRect
        anchors.top: titleBar.bottom
        anchors.left: parent.left
        width: parent.width
        height: viewRectHeight
        visible: false
    }

    DayAndTimeSettingView{
        id: dayAndTimeSettingRect
        anchors.top: titleBar.bottom
        anchors.left: parent.left
        width: parent.width
        height: viewRectHeight
        visible: false
    }
    RegionSetting{
        id: regionSettingRect
        anchors.top: titleBar.bottom
        anchors.left: parent.left
        width: parent.width
        height: viewRectHeight
        visible: false
    }

    About{
        id: about
        anchors.top: titleBar.bottom
        anchors.left: parent.left
        width: parent.width
        height: viewRectHeight
        visible: false
    }

    Component.onCompleted: {
        rootItem.updateLanguage.connect(updateLanguageSlot);
    }
}
