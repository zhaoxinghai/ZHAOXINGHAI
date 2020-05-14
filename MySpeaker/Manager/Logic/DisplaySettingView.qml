import QtQuick 2.0
import "../Components"

Rectangle {

    function updateLanguageSlot()
    {
        title.text = langStrObject.GetLangString("ETCS_BACKLIGHT");
        //backlightOnName.setText(langStrObject.GetLangString("ETCS_BACKLIGHT_ON"));
        prevTitleName.text = langStrObject.GetLangString("ETCS_SETTINGS");
    }

    function initStatus()
    {
        setVisibleValue(true);
        backlightSlider.setValue(Number(localConfigObject.readConfig("LocalConfig", "Backlight")));
    }

    function quitStatus()
    {
        setVisibleValue(false);
    }

    function setVisibleValue(value)
    {
        visible = value;
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
            text: langStrObject.GetLangString("ETCS_BACKLIGHT")
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
                text: langStrObject.GetLangString("ETCS_SETTINGS")
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
                    //imgSwitch.offStatus();
                    //backlightTimer.setTimerEnabledValue(true);
                    quitStatus();
                }
            }
        }
    }

    Rectangle{
        id: rect1
        width: parent.width
        height: 22
        anchors.top: titleBar.bottom
        anchors.left: parent.left
        color: "#e7e7e7"
    }

    /*
    CommonRect{
        id: backlightOnRect
        anchors.top: rect1.bottom
        anchors.left: parent.left
        width: parent.width
        height: 65
        line1Visible: false
        line2Visible: false
        CommonText{
            id: backlightOnName
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 32
            width: parent.width - imgSwitch.width - 32
            font.pixelSize: 18
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_BACKLIGHT_ON")
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
        }
        ImageSwitch{

            function offStatus()
            {
                setBtnClickedValue(false);
                updateSwitchStatus();
            }

            id: imgSwitch
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 30
            visible: true
            btnClicked: false
            onSwitchOpenSignal: {
                backlightTimer.setTimerEnabledValue(false);
            }
            onSwitchCloseSignal: {
                backlightTimer.setTimerEnabledValue(true);
            }
        }
    }
    */

    CommonRect{
        id: backlightBtn
        anchors.top: rect1.bottom
        //anchors.topMargin: 22
        anchors.left: parent.left
        width: parent.width
        height: 65
        line1Visible: false
        line2Visible: false

        CommonSlider{
            id: backlightSlider
            width: 360
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            stepSize: 1.0
            maximumValue: 255
            minimumValue: 30
            updateValueWhileDragging: false
            value: Number(localConfigObject.readConfig("LocalConfig", "Backlight"))
           onValueChanged: {
                    localConfigObject.writeConfig("LocalConfig", "Backlight", value);
                    gpioObject.setBacklight(value);
            }
        }


        CommonImage{
            id: darkImage
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: backlightSlider.left
            width: 38
            height: 37
            source: logicObject.getLocalImagePath() + "backlight_dark.png"
        }
        CommonImage{
            id: brightImage
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: backlightSlider.right
            width: 38
            height: 37
            source: logicObject.getLocalImagePath() + "backlight_bright.png"
        }
    }

    Component.onCompleted: {
        rootItem.updateLanguage.connect(updateLanguageSlot);
    }
}
