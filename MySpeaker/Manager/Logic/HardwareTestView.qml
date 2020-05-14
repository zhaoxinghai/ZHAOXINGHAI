import QtQuick 2.0
import "../Components"

Rectangle {

    property int count: 0

    function updateLanguageSlot()
    {
        title.text = langStrObject.GetLangString("ETCS_HARDWARE_TEST");
        hwSelfTestBtnName.setText(langStrObject.GetLangString("ETCS_HW_SELF_TEST"));
        prevTitleName.text = langStrObject.GetLangString("ETCS_MORE");
        netTestName.setText(langStrObject.GetLangString("ETCS_NET_TEST"));
        mainLanName.setText(langStrObject.GetLangString("ETCS_MAIN_ETHENET"));
        backupEthName.setText(langStrObject.GetLangString("ETCS_BACKUP_ETHERNET"));
        if (startTextBtn.getEnableValue() === true)
        {
            startTextBtn.setTextInfo(langStrObject.GetLangString("ETCS_START_TEST"));
        }
        else
        {
            startTextBtn.setTextInfo(langStrObject.GetLangString("ETCS_TESTING"));
        }
    }

    function lampTestSlot(status)
    {
        if (status === true)
        {
            logicObject.gpioTest(true);
        }
        else
        {
            logicObject.gpioTest(false);
        }
    }

    function netTestSlot(id, err)
    {
        count++;
        if (id === 0)
        {
            if (err === 0)
            {
                mainEthImage.setSource(logicObject.getLocalImagePath() + "net_ok.png");
            }
            else if (err === 1)
            {
                mainEthImage.setSource(logicObject.getLocalImagePath() + "net_error.png");
            }
        }
        else if (id === 1)
        {
            if (err === 0)
            {
                backupEthImage.setSource(logicObject.getLocalImagePath() + "net_ok.png");
            }
            else if (err === 1)
            {
                backupEthImage.setSource(logicObject.getLocalImagePath() + "net_error.png");
            }
        }

        if (count === 2)
        {
            count = 0;
            startTextBtn.initStatus();
        }
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function initStatus()
    {
        setVisibleValue(true);

    }

    function quitStatus()
    {
        setVisibleValue(false);
        mainPage.showBusinessBar();
        logicObject.gpioTest(false);
    }

    color: "#e7e7e7"

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
            text: langStrObject.GetLangString("ETCS_HARDWARE_TEST")
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
                text: langStrObject.GetLangString("ETCS_MORE")
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
                    quitStatus();
                }
            }
        }
    }

    StripFrame{
        id: hwSelfTestBtn
        anchors.top: titleBar.bottom
        anchors.topMargin: 22
        anchors.left: parent.left
        width: parent.width
        height: 65
        line1Visible: true
        line2Visible: true
        bkSwitch: false
        bkgPressedColor: "#dbe0e6"
        bkgReleasedColor: "#ffffff"
        visible: true
        CommonText{
            id: hwSelfTestBtnName
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 32
            width: parent.width - mainEthImage.width - 44
            font.pixelSize: 18
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_HW_SELF_TEST")
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
        }

        onClickedSignal: {
            hardwareSelfTest.initStatus();
            logicObject.lampTest(true);
        }
    }

    StripFrame{
        id: netTestFrame
        anchors.top: hwSelfTestBtn.bottom
        anchors.topMargin: 22
        anchors.left: parent.left
        width: parent.width
        height: 65
        line1Visible: false
        line2Visible: true
        bkSwitch: false
        bkgPressedColor: "#dbe0e6"
        bkgReleasedColor: "#ffffff"
        CommonText{
            id: netTestName
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 34
            width: parent.width - 34
            font.pixelSize: 18
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_NET_TEST")
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
            Component.onCompleted: {

            }
        }

        TextButton2{

            function initStatus()
            {
                setTextInfo(langStrObject.GetLangString("ETCS_START_TEST"));
                setEnabledValue(true);
            }

            function testingStatus()
            {
                setTextInfo(langStrObject.GetLangString("ETCS_TESTING") + "...");
                mainEthImage.setSource("");
                backupEthImage.setSource("");
                setEnabledValue(false);
            }

            id: startTextBtn
            width: nameContentWidth < parent.height ? parent.height : nameContentWidth
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 18
            fontSize: 18
            info: langStrObject.GetLangString("ETCS_START_TEST")
            textColor: "#00c960"
            visible: true
            bkgPressedColor: "#dbe0e6"
            onClickedSignal: {
                testingStatus();
                logicObject.lanTest();
            }
        }
    }

    StripFrame{
        id: mainEthFrame
        anchors.top: netTestFrame.bottom
        anchors.left: parent.left
        width: parent.width
        height: 65
        line1Visible: false
        line2Visible: true
        bkSwitch: false
        bkgPressedColor: "#dbe0e6"
        bkgReleasedColor: "#ffffff"
        CommonText{
            id: mainLanName
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 50
            width: parent.width - mainEthImage.width - 44
            font.pixelSize: 18
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_MAIN_ETHENET")
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
            Component.onCompleted: {

            }
        }

        CommonImage{
            id: mainEthImage
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 25
            width: 36
            height: 35
            source: ""
        }
    }

    StripFrame{
        id: backupEthFrame
        anchors.top: mainEthFrame.bottom
        anchors.left: parent.left
        width: parent.width
        height: 65
        line1Visible: false
        line2Visible: true
        bkSwitch: false
        bkgPressedColor: "#dbe0e6"
        bkgReleasedColor: "#ffffff"
        CommonText{
            id: backupEthName
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 50
            width: parent.width - mainEthImage.width - 44
            font.pixelSize: 18
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_BACKUP_ETHERNET")
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
        }
        CommonImage{
            id: backupEthImage
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 25
            width: 36
            height: 35
            source: ""
        }
    }

    StripFrame{
        id: imageBrowserBtn
        anchors.top: backupEthFrame.bottom
        anchors.topMargin: 22
        anchors.left: parent.left
        width: parent.width
        height: 65
        line1Visible: false
        line2Visible: true
        bkSwitch: false
        bkgPressedColor: "#dbe0e6"
        bkgReleasedColor: "#ffffff"
        visible: false
        CommonText{
            id: imageBrowserBtnName
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 50
            width: parent.width - mainEthImage.width - 44
            font.pixelSize: 18
            color: "#303030"
            text: "Image browser"
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
        }

        onClickedSignal: {
            //imageBrowser.initStatus();
        }
    }

    Component.onCompleted: {
        rootItem.updateLanguage.connect(updateLanguageSlot);
        logicObject.netTestSignal.connect(netTestSlot);
    }
}

