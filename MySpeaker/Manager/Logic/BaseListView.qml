import QtQuick 2.5
import QtQml.Models 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import "../Components"

Rectangle {

    property alias listViewModel: visualModel.model
    property int callId: 0
    property int prelistenId: 0
    property string titleBarText:""

    property int listViewTitleBarHeight: 50
    property int listViewHeight: height -  mainPage.g_titleBarHeight - listViewTitleBarHeight - volumeAdjustBarHeight - protocolBarHeight
    property int volumeAdjustBarHeight: 60
    property int protocolBarHeight: 100
    property int predefinedViewCallHeight: height
    property int scheduleSettingViewHeight: height

    property int umountId: 100

    property bool haveGong: false
    property string callStatus: dis_status
    property string prelistenStatus: dis_status

    property string wait_status: "wait"
    property string full_status: "full"
    property string part_status: "part"
    property string dis_status: "disconnect"

    function setUmountId(id)
    {
        umountId = id
    }

    function getUmountId()
    {
        return umountId;
    }

    function updateLanguageSlot()
    {
        usbView.titleBarText = langStrObject.GetLangString("ETCS_USB");
        prevTitleName.text = langStrObject.GetLangString("ETCS_SOURCE");
        sourceName.text = langStrObject.GetLangString("ETCS_SOURCE");
        listViewSelectAllBtnName.text = langStrObject.GetLangString("ETCS_SELECT_ALL");
        listViewClearBtnName.text = langStrObject.GetLangString("ETCS_CLEAR_ALL");
        propertyBtn.setInfoValue(langStrObject.GetLangString("ETCS_PROPERTY"));
        preListenBtn.setBtnText(langStrObject.GetLangString("ETCS_PRELISTEN"));
        callBtn.setBtnText(langStrObject.GetLangString("ETCS_CALL"));
        sendBtnName.setText(langStrObject.GetLangString("ETCS_SEND"));
        protocolMoreBtn.setInfoValue(langStrObject.GetLangString("ETCS_MORE"));
    }

    function changeUserSlot(value)
    {
        if (value === 0)
        {
            protocolMoreBtn.setBtnEnableValue(true);
            protocolMoreBtn.setImageSource(logicObject.getLocalImagePath() + "more_A.png");
            protocolMoreBtn.setDataColor("#5f5f5f");
        }
        else if (value === 1)
        {
            protocolMoreBtn.setBtnEnableValue(true);
            protocolMoreBtn.setImageSource(logicObject.getLocalImagePath() + "more_A.png");
            protocolMoreBtn.setDataColor("#5f5f5f");
        }
        else if (value === 2)
        {
            protocolMoreBtn.setBtnEnableValue(false);
            protocolMoreBtn.setImageSource(logicObject.getLocalImagePath() + "more_C.png");
            protocolMoreBtn.setDataColor("#40666666");
        }
    }

    function systemResultSlot(id, errCode)
    {
        var ret;
        if (id === getUmountId())
        {
            ret = udiskObject.isUDiskMounted()
            if (ret === false)
            {
                sourceManageObject.clearUsbSourceList();
                sourceManageObject.clearUsbSourceModel();
                statusBar.removeImg(logicObject.getLocalImagePath() + "USB_1.png");
                umountUDiskBtn.setVisibleValue(false);
                setListViewSelectAllBtnVisible(false);
                setListViewClearBtnVisible(false);
                promptMask.messageShow(langStrObject.GetLangString("ETCS_U_DISK_EJECT_SUCCESS"))
                timeOutTimer.setInterval(3*1000);
                timeOutTimer.start();
            }
            else
            {
                promptMask.messageShow(langStrObject.GetLangString("ETCS_U_DISK_EJECT_FAIL"))
                timeOutTimer.setInterval(3*1000);
                timeOutTimer.start();
            }
        }

        if (id === waitMask1.getUmountId())
        {
            ret = udiskObject.isUDiskMounted()
            if (ret === false)
            {
                sourceManageObject.clearUsbSourceList();
                sourceManageObject.clearUsbSourceModel();
                statusBar.removeImg(logicObject.getLocalImagePath() + "USB_1.png");
                umountUDiskBtn.setVisibleValue(false);
                setListViewSelectAllBtnVisible(false);
                setListViewClearBtnVisible(false);
            }
            else
            {

            }
        }
    }

    function initStatus()
    {
        zeroCallId();
        zeroPrelistenId();
        setVisibleValue(true);
    }

    function quitStatus()
    {
        var id;
        id = getCallId();
        if (id !== 0)
        {
            setCallStatusValue(dis_status);
            upDateCallBtnStatus();
            setPrelistenBtnEnable();
            zeroCallId();
            statusBar.setStatusBarMessageTextForTimer(langStrObject.GetLangString("ETCS_CALL_ERR_4"), "ETCS_CALL_ERR_4");
        }

        id = getPrelistenId();
        if (id !== 0)
        {
            pageObject.baseStopCallFromId("PreListen", getPrelistenId());
            zeroPrelistenId();
            setPrelistenStatusValue(dis_status);
            upDatePrelistenBtnStatus();
            statusBar.setStatusBarMessageTextForTimer(langStrObject.GetLangString("ETCS_CALL_ERR_10"), "ETCS_CALL_ERR_10");
        }

        setVisibleValue(false);
    }

    function insertUDiskSlot()
    {
        showWaitMask();
        sourceManageObject.setIsRun(true);
        waitMaskHideTimer.start();
    }

    function removeUDiskSlot()
    {
        sourceManageObject.clearUsbSourceList();
        sourceManageObject.clearUsbSourceModel();

        logicObject.clearAllUsbActivate();
        umountUDiskBtn.setVisibleValue(false);
        setListViewSelectAllBtnVisible(false);
        setListViewClearBtnVisible(false);
    }

    function finishUsbSourceCreateSlot()
    {
        sourceManageObject.clearUsbSourceList();
        sourceManageObject.createUsbSourceList();
        sourceManageObject.clearUsbSourceModel();
        sourceManageObject.createUsbSourceModel();
        var count = sourceManageObject.getUsbSourceModelCount();
        if (count <= 0)
        {
            setListViewSelectAllBtnVisible(false);
            setListViewClearBtnVisible(false);
        }
        else
        {
            setListViewSelectAllBtnVisible(true);
            setListViewClearBtnVisible(false);
        }

        umountUDiskBtn.setVisibleValue(true);
        hideWaitMask();
    }

    function timeOutSlot()
    {
        waitMaskHideTimer.stop();
        hideWaitMask();
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function showWaitMask()
    {
        waitMask.visible = true;
    }

    function hideWaitMask()
    {
        waitMask.visible = false;
    }

    function callSettingPagerRespondSlot(text)
    {
        callSettingPage.setView(text)
    }

    function listViewSelectAll()
    {
        var tran;
        var errorNum;
        var count = visualModel.model.rowCount();

        for (var i = 0; i < count; i++)
        {
            tran = visualModel.model.data(visualModel.model.index(i, 0), 5);
            errorNum = visualModel.model.data(visualModel.model.index(i, 0), 4);
            if (tran === true && errorNum === 0)
            {
                visualModel.model.setData(visualModel.model.index(i, 0), 1, 2);
            }
            else
            {
                ;
            }

        }
        return 0;
    }

    function listViewUnselectAll()
    {
        var tran;
        var errorNum;
        var count = visualModel.model.rowCount();

        for (var i = 0; i < count; i++)
        {
            tran = visualModel.model.data(visualModel.model.index(i, 0), 5);
            errorNum = visualModel.model.data(visualModel.model.index(i, 0), 4);
            if (tran === true && errorNum === 0)
            {
                visualModel.model.setData(visualModel.model.index(i, 0), 0, 2);
            }
            else
            {
                ;
            }
        }
        return 0;
    }

    function setListViewSelectAllBtnVisible(value)
    {
        listViewSelectAllBtn.visible = value;
    }

    function setListViewClearBtnVisible(value)
    {
        listViewClearBtn.visible = value;
    }

    function setVolumeSliderEnable()
    {
        volumeSlider.enabled = true;
    }

    function setVolumeSliderDisable()
    {
        volumeSlider.enabled = false;
    }

    function getVolumeSliderValue()
    {
        return volumeSlider.value;
    }

    function setGongTrueStatus(id)
    {
        if (id === getCallId())
        {
            setHaveGongValue(true);
            upDateCallBtnStatus();
        }
        if (id === getPrelistenId())
        {
            setHaveGongValue(true);
            upDatePrelistenBtnStatus();
        }
    }

    function setGongFalseStatus(id)
    {
        if (id === getCallId())
        {
            setHaveGongValue(false);
            upDateCallBtnStatus();
        }

        if (id === getPrelistenId())
        {
            setHaveGongValue(false);
            upDatePrelistenBtnStatus();
        }
    }

    function setPrelistenBtnEnable()
    {
        preListenBtn.setBtnEnable(logicObject.getLocalImagePath() + "prelisten_A.png", "#5f5f5f");
        preListenBtn.setBtnText(langStrObject.GetLangString("ETCS_PRELISTEN"));
    }

    function setPrelistenBtnDisable()
    {
        preListenBtn.setBtnDisable(logicObject.getLocalImagePath() + "preliten_D.png", "#40666666");
        preListenBtn.setBtnText(langStrObject.GetLangString("ETCS_PRELISTEN"));
    }

    function setPrelistenBtnWaitStatus()
    {
        preListenBtn.setBtnClickedStatus(logicObject.getLocalImagePath() + "prelisten_B.png", "#5f5f5f");
        preListenBtn.setBtnText(langStrObject.GetLangString("ETCS_WAIT"));
    }

    function setPrelistenBtnFullStatus()
    {
        preListenBtn.setBtnClickedStatus(logicObject.getLocalImagePath() + "prelisten_B.png", "#5f5f5f");
        preListenBtn.setBtnText(langStrObject.GetLangString("ETCS_FULL_CONNECT"));
    }

    function setPrelistenBtnFullGongStatus()
    {
        preListenBtn.setBtnEnable(logicObject.getLocalImagePath() + "prelisten_C.png", "#5f5f5f");
        preListenBtn.setImgPlayingValue(true);
        preListenBtn.setBtnText(langStrObject.GetLangString("ETCS_FULL_CONNECT"));
    }

    function setPrelistenBtnPartStatus()
    {
        preListenBtn.setBtnClickedStatus(logicObject.getLocalImagePath() + "prelisten_B.png", "#5f5f5f");
        preListenBtn.setBtnText(langStrObject.GetLangString("ETCS_PARTLY_CONNECT"));
    }

    function setPrelistenBtnPartGongStatus()
    {
        preListenBtn.setBtnEnable(logicObject.getLocalImagePath() + "prelisten_C.png", "#5f5f5f");
        preListenBtn.setImgPlayingValue(true);
        preListenBtn.setBtnText(langStrObject.GetLangString("ETCS_PARTLY_CONNECT"));
    }

    function setPrelistenBtnDisconnectStatus()
    {
        preListenBtn.setBtnUnclickedStatus(logicObject.getLocalImagePath() + "prelisten_A.png", "#5f5f5f");
        preListenBtn.setBtnText(langStrObject.GetLangString("ETCS_PRELISTEN"));
        resetPrelistenBtnStatus();
    }

    function resetPrelistenBtnStatus()
    {
        preListenBtn.resetButtonStatus();
    }

    function setPrelistenBtnText(text)
    {
        preListenBtn.setBtnText(text);
    }

    function setCallBtnWaitStatus()
    {
        callBtn.setBtnEnable(logicObject.getLocalImagePath() + "call_B.png", "#5f5f5f");
        callBtn.setBtnText(langStrObject.GetLangString("ETCS_WAIT"));
    }

    function setCallBtnFullStatus()
    {
        callBtn.setBtnEnable(logicObject.getLocalImagePath() + "call_B.png", "#5f5f5f");
        callBtn.setBtnText(langStrObject.GetLangString("ETCS_FULL_CONNECT"));
    }

    function setCallBtnFullGongStatus()
    {
        callBtn.setBtnEnable(logicObject.getLocalImagePath() + "call_D.png", "#5f5f5f");
        callBtn.setImgPlayingValue(true);
        callBtn.setBtnText(langStrObject.GetLangString("ETCS_FULL_CONNECT"));
    }

    function setCallBtnPartStatus()
    {
        callBtn.setBtnEnable(logicObject.getLocalImagePath() + "call_B.png", "#5f5f5f");
        callBtn.setBtnText(langStrObject.GetLangString("ETCS_PARTLY_CONNECT"));
    }

    function setCallBtnPartGongStatus()
    {
        callBtn.setBtnEnable(logicObject.getLocalImagePath() + "call_D.png", "#5f5f5f");
        callBtn.setImgPlayingValue(true);
        callBtn.setBtnText(langStrObject.GetLangString("ETCS_PARTLY_CONNECT"));
    }

    function setCallBtnDisconnectStatus()
    {
        callBtn.setBtnEnable(logicObject.getLocalImagePath() + "call_A.png", "#5f5f5f");
        callBtn.setBtnText(langStrObject.GetLangString("ETCS_CALL"));
        resetCallBtnStatus();
    }

    function resetCallBtnStatus()
    {
        callBtn.resetButtonStatus();
    }

    function setCallBtnText(text)
    {
        callBtn.setBtnText(text);
    }

    function setPrelistenId()
    {
        prelistenId = pageObject.getCallId();
    }

    function getPrelistenId()
    {
        return prelistenId;
    }

    function zeroPrelistenId()
    {
        prelistenId = 0;
    }

    function setCallId()
    {
        callId = pageObject.getCallId();
    }

    function zeroCallId()
    {
        callId = 0;
    }

    function getCallId()
    {
        return callId;
    }

    function syncSourceListSelect()
    {
        var count = visualModel.model.rowCount();
        var select;
        var tran;
        var errorNum;
        var index;

        sourceManageObject.clearAllUsbSourceSelected();
        for (var i = 0; i < count; i++)
        {
            errorNum = visualModel.model.data(visualModel.model.index(i, 0), 4);
            if (errorNum !== 0)
            {
                continue;
            }

            tran = visualModel.model.data(visualModel.model.index(i, 0), 5);
            if (tran === false)
            {
                continue;
            }

            select = visualModel.model.data(visualModel.model.index(i, 0), 2);
            if (Number(select) === 1)
            {
                index = visualModel.model.data(visualModel.model.index(i, 0), 1);
                sourceManageObject.setUsbSourceSelected(index, 1);
            }
        }
        return 0;
    }


    function preListenBtnClickSlot(status)
    { 
        if (status === true)
        {
            var ret = setMessageBarStatus();
            if (ret < 0)
            {
                setPrelistenStatusValue(dis_status);
                upDatePrelistenBtnStatus();
                return;
            }

            syncSourceListSelect();
            var errorCode = pageObject.baseStartCall("USB", "PreListen", callSettingPage.getgPrority(),
                                                        getVolumeSliderValue(),
                                                        callSettingPage.getgPreSignal(),
                                                        callSettingPage.getgReconnect(),
                                                        callSettingPage.getgPartlyConnect(),
                                                        callSettingPage.getgCycle(),
                                                        callSettingPage.getgGapTime(),
                                                        callSettingPage.getgTimeType()
                                                        );
            ret = rootItem.dealCallError(errorCode);
            if (ret !== 0)
            {
                setPrelistenStatusValue(dis_status);
                upDatePrelistenBtnStatus();
                zeroPrelistenId();
                setHaveGongValue(false);
                return;
            }

            setPrelistenId(pageObject.getCallId());
            setPrelistenStatusValue(wait_status);
            upDatePrelistenBtnStatus();
        }
        else if(status === false)
        {
            pageObject.baseStopCallFromId("PreListen", getPrelistenId());
            zeroPrelistenId();
            setHaveGongValue(false);
            setPrelistenStatusValue(dis_status);
            upDatePrelistenBtnStatus();
        }

        return 0;
    }

    function callBtnClickSlot(status)
    {
        if (status === true)
        {
            var ret = setMessageBarStatus();
            if (ret < 0)
            {
                setCallStatusValue(dis_status);
                upDateCallBtnStatus();
                return;
            }

            syncSourceListSelect();
            var errorCode = pageObject.baseStartCall("USB", "Call", callSettingPage.getgPrority(),
                                                    getVolumeSliderValue(),
                                                    callSettingPage.getgPreSignal(),
                                                    callSettingPage.getgReconnect(),
                                                    callSettingPage.getgPartlyConnect(),
                                                    callSettingPage.getgCycle(),
                                                    callSettingPage.getgGapTime(),
                                                    callSettingPage.getgTimeType()
                                                    );

            ret = rootItem.dealCallError(errorCode);
            if (ret !== 0)
            {
                setCallStatusValue(dis_status)
                upDateCallBtnStatus();
                zeroCallId();
                setHaveGongValue(false);
                return;
            }

            if (getPrelistenId() !== 0)
            {
                pageObject.baseStopCallFromId("PreListen", getPrelistenId());
                zeroPrelistenId();
            }

            setPrelistenBtnDisable();
            setCallId(pageObject.getCallId());
            setCallStatusValue(wait_status);
            upDateCallBtnStatus();
        }
        else if(status === false)
        {
            pageObject.baseStopCallFromId("Call", getCallId());
            setCallStatusValue(dis_status);
            upDateCallBtnStatus();
            setPrelistenBtnEnable();
            zeroCallId();
            setHaveGongValue(false);
        }

        return 0;
    }

    function activateUpdateSlot(id, text)
    {
        if  (id === getPrelistenId())
        {
            if (text === dis_status)
            {
                setPrelistenStatusValue(dis_status);
                upDatePrelistenBtnStatus();
                zeroPrelistenId();
                setHaveGongValue(false);
            }
            else if (text === full_status)
            {
                setPrelistenStatusValue(full_status);
                upDatePrelistenBtnStatus();
            }
            else if (text === part_status)
            {

                setPrelistenStatusValue(part_status);
                upDatePrelistenBtnStatus();
            }
            else if (text === wait_status)
            {
                setPrelistenStatusValue(wait_status);
                upDatePrelistenBtnStatus();
            }
        }
        else if (id === getCallId())
        {
            if (text === dis_status)
            {
                setCallStatusValue(dis_status)
                upDateCallBtnStatus();
                zeroCallId();
                setPrelistenBtnEnable();
                setHaveGongValue(false);
            }
            else if (text === full_status)
            {
                setCallStatusValue(full_status)
                upDateCallBtnStatus();
            }
            else if (text === part_status)
            {
                setCallStatusValue(part_status)
                upDateCallBtnStatus();
            }
            else if (text === wait_status)
            {
                setCallStatusValue(wait_status)
                upDateCallBtnStatus();
            }
        }
    }

    function setMessageBarStatus()
    {
        var count = visualModel.model.rowCount();
        var select;
        var index;
        sourceManageObject.clearAllUsbSourceSelected();
        for (var i = 0; i < count; i++)
        {
            select = visualModel.model.data(visualModel.model.index(i, 0), 2);
            if (Number(select) === 1)
            {
                messageBarHide();
                return 0;
            }
        }

        messageBarShow(langStrObject.GetLangString("ETCS_CALL_ERR_2"));
        return -1;
    }

    function messageBarShow(text)
    {
        messageBar.messageBarShow(text);
    }

    function messageBarHide()
    {
        messageBar.messageBarHide();
    }

    function setHaveGongValue(value)
    {
        haveGong = value;
    }

    function getHaveGongValue()
    {
        return haveGong;
    }

    function setCallStatusValue(value)
    {
        callStatus = value;
    }

    function getCallStatusValue()
    {
        return callStatus;
    }

    function setPrelistenStatusValue(value)
    {
        prelistenStatus = value;
    }

    function getPrelistenStatusValue()
    {
        return prelistenStatus;
    }

    function upDateCallBtnStatus()
    {
        if  (getCallId() === 0)
        {
            setCallBtnDisconnectStatus();
            return ;
        }

        if (getCallStatusValue() === wait_status)
        {
            setCallBtnWaitStatus();
        }
        else if (getCallStatusValue() === full_status)
        {
            if (getHaveGongValue() === true)
            {
                setCallBtnFullGongStatus();
            }
            else
            {
                setCallBtnFullStatus();
            }
        }
        else if (getCallStatusValue() === part_status)
        {
            if (getHaveGongValue() === true)
            {
                setCallBtnPartGongStatus();
            }
            else
            {
                setCallBtnPartStatus();
            }
        }
        else if (getCallStatusValue() === dis_status)
        {
            setCallBtnDisconnectStatus();
        }
    }

    function upDatePrelistenBtnStatus()
    {
        if  (getPrelistenId() === 0)
        {
            setPrelistenBtnDisconnectStatus();
            return ;
        }

        if (getPrelistenStatusValue() === wait_status)
        {
            setPrelistenBtnWaitStatus();
        }
        else if (getPrelistenStatusValue() === full_status)
        {
            if (getHaveGongValue() === true)
            {
                setPrelistenBtnFullGongStatus();
            }
            else
            {
                setPrelistenBtnFullStatus();
            }
        }
        else if (getPrelistenStatusValue() === part_status)
        {
            if (getHaveGongValue() === true)
            {
                setPrelistenBtnPartGongStatus();
            }
            else
            {
                setPrelistenBtnPartStatus();
            }
        }
        else if (getPrelistenStatusValue() === dis_status)
        {
            setPrelistenBtnDisconnectStatus();
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
            text: titleBarText
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
            clip: true
            z: 1
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
                text: langStrObject.GetLangString("ETCS_SOURCE")
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

    Rectangle{
        width: parent.width
        height: 1
        anchors.top: titleBar.bottom
        color: "#c0c0c0"
        z: 1
    }

    Rectangle{
        id: listViewTitleBar
        width: parent.width
        height: listViewTitleBarHeight
        anchors.top: titleBar.bottom
        color: "#ffffff"

        Rectangle{
            width: 4
            height: 24
            color: "#4f4f4f"
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 18
        }

        Text{
            id: sourceName
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 28
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_SOURCE")
            font.family: rootItem.fontName
            font.pixelSize: 22
            width: parent.width - 28 - umountUDiskBtn.width - listViewSelectAllBtn.width
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
            Component.onCompleted: {
            }
        }

        ImageButton{
            id: umountUDiskBtn
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: listViewSelectAllBtn.left
            width: 65
            height: parent.height
            buttonImageSource: logicObject.getLocalImagePath() + "eject.png"
            visible: false
            onClickedSignal: {
                promptMask.messageShow(langStrObject.GetLangString("ETCS_WAIT"));
                logicObject.clearAllUsbActivate();
                udiskObject.umountUDisk(umountId);
                timeOutTimer.setInterval(10*1000);
                timeOutTimer.start();
            }
        }

        Rectangle {

            id: listViewSelectAllBtn

            property bool listViewSelectAllBtnClicked: false

            width: 100
            height: 50
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            color: selectAllMouseArea.pressed ? "#DBE0E6" : parent.color
            visible: false
            Image{
                id: listViewSelectAllBtnImage
                anchors.left: parent.left
                anchors.leftMargin: (parent.width - listViewSelectAllBtnImage.width - listViewSelectAllBtnName.contentWidth) / 2
                anchors.verticalCenter: parent.verticalCenter
                source: listViewSelectAllBtn.listViewSelectAllBtnClicked ? logicObject.getLocalImagePath() + "ETCS_Target Zone_All_B.png" : logicObject.getLocalImagePath() + "select_all.png"
            }

            Text {
                id: listViewSelectAllBtnName
                font.family: rootItem.fontName
                font.pixelSize: 16
                anchors.left: listViewSelectAllBtnImage.right
                anchors.verticalCenter: parent.verticalCenter
                color: listViewSelectAllBtn.listViewSelectAllBtnClicked ? "#00c767" : "#303030"
                text: langStrObject.GetLangString("ETCS_SELECT_ALL")
                width: 110 - listViewSelectAllBtnImage.width
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
                Component.onCompleted: {
                }
            }

            MouseArea {
                id: selectAllMouseArea
                anchors.fill: parent
                onClicked: {
                    visualModel.items.addGroups(0, visualModel.items.count, ["selected"] );
                    listViewSelectAll();
                    setListViewSelectAllBtnVisible(false);
                    setListViewClearBtnVisible(true);
                }
            }
        }

        Rectangle {
            id: listViewClearBtn
            width: 100
            height: 50
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            color: clearMouseArea.pressed ? "#DBE0E6" : parent.color
            visible: false

            Image{
                id: listViewClearBtnImage
                anchors.left: parent.left
                anchors.leftMargin: (parent.width - listViewClearBtnImage.width - listViewClearBtnName.contentWidth) / 2
                anchors.verticalCenter: parent.verticalCenter
                source: logicObject.getLocalImagePath() + "clear_all.png"
            }

            Text {
                id: listViewClearBtnName
                font.family: rootItem.fontName
                font.pixelSize: 16
                anchors.left: listViewClearBtnImage.right
                anchors.verticalCenter: parent.verticalCenter
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_CLEAR_ALL")
                width: 110 - listViewClearBtnImage.width
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
                Component.onCompleted: {
                }
            }

            MouseArea {
                id: clearMouseArea
                anchors.fill: parent
                onClicked: {
                    if (visualModel.selectedItems.count > 0)
                    {
                        visualModel.selectedItems.remove(0, visualModel.selectedItems.count);
                    }
                    listViewUnselectAll();
                    setListViewSelectAllBtnVisible(true);
                    setListViewClearBtnVisible(false);
                }
            }
        }
    }

    Rectangle{
        z: 2
        width: parent.width
        height: 1
        anchors.top: listViewTitleBar.bottom
        color: "#c0c0c0"
    }

    ScrollBar{
        id: listViewVerticalScrollBar
        width: 6
        height: listView.height
        z: 1
        anchors.top: listView.top
        anchors.right: parent.right
        anchors.rightMargin: 6
        orientation: Qt.Vertical
        position: listView.visibleArea.yPosition
        pageSize: listView.visibleArea.heightRatio
        scrollBarVisible: listView.visibleArea.heightRatio == 1 ? false : true
    }

    ListView{
        id: listView
        width: parent.width - (2 * 18)
        height: listViewHeight
        anchors.top: listViewTitleBar.bottom
        anchors.left: parent.left
        anchors.leftMargin: 18
        clip: true
        boundsBehavior: Flickable.StopAtBounds
        snapMode: ListView.NoSnap
        footer: Rectangle{height: 8}
        model: visualModel
    }

    DelegateModel {
        id: visualModel

        property alias selectedItems: gp
        groups: [
            DelegateModelGroup{
                id: gp
                name: "selected"
            }
        ]

        delegate:Rectangle{
            id: listViewElement
            width: listView.width
            height: 72
            opacity: 1

            Rectangle{
                id: item
                width: listView.width
                height: 60
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                border.color: e_tran && e_errorNum === 0 ? (listViewElement.VisualDataModel.inSelected ? "#00c767" : "#c0c0c0") : "#40666666"
                border.width: e_tran && e_errorNum === 0 ? (listViewElement.VisualDataModel.inSelected ? 2 : 1) : 1

                CommonImage{
                    id: localErrorImage
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    source: logicObject.getLocalImagePath() + "error.png"
                    visible:  e_errorNum !== 0 ? true : false
                }

                Text{
                    text: e_sourceName
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: localErrorImage.right
                    anchors.leftMargin: 5
                    color: e_tran  && e_errorNum === 0  ? (listViewElement.VisualDataModel.inSelected ? "#00c767" : "#303030") : "#303030"
                    font.family: rootItem.fontName
                    font.bold: false
                    font.pixelSize: 18
                    width: parent.width - busyIndicator.width - 40
                    elide: Text.ElideRight
                    horizontalAlignment: Text.AlignLeft
                }

                BusyIndicator{

                    function setVisibleValue(value)
                    {
                        visible = value;
                    }

                    function setRunningValue(value)
                    {
                        running = value;
                    }

                    id: busyIndicator
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 15
                    running: !e_tran
                    visible: !e_tran
                    width: 30
                    height: 30
                    style: BusyIndicatorStyle {
                              indicator: Image {
                                  visible: control.running
                                  source: logicObject.getLocalImagePath() + "waiting.png"
                                  width: 30
                                  height: 30
                                  RotationAnimator on rotation {
                                      running: control.running
                                      loops: Animation.Infinite
                                      duration: 800
                                      from: 0 ; to: 360
                                  }
                              }
                          }
                }

                MouseArea{
                    anchors.fill: parent
                    enabled: e_tran
                    onClicked: {
                        if (e_errorNum !== 0)
                        {
                            localErrorMask.initStatus(langStrObject.GetLangString("ETCS_FILE_FORMAT_ERROR"));
                        }
                        else
                        {
                            messageBarHide();
                            listViewElement.VisualDataModel.inSelected = !listViewElement.VisualDataModel.inSelected;
                            if (listViewElement.VisualDataModel.inSelected === true)
                            {
                                visualModel.model.setData(visualModel.model.index(index, 0), 1 ,2);
                            }
                            else if (listViewElement.VisualDataModel.inSelected === false)
                            {
                               visualModel.model.setData(visualModel.model.index(index, 0), 0 ,2);
                            }

                            if (visualModel.selectedItems.count > 0)
                            {
                                setListViewSelectAllBtnVisible(false);
                                setListViewClearBtnVisible(true);
                            }
                            else if (visualModel.selectedItems.count == 0)
                            {
                                setListViewSelectAllBtnVisible(true);
                                setListViewClearBtnVisible(false);
                            }
                        }
                    }
                }
            }
        }
    }

    MessageBar{
        id: messageBar
        anchors.bottom: volumeAdjustBar.top
        width: parent.width
        height: 0
        color: "#dddddd"
        clip: true
        border.width: 1
        border.color: "#f4f4f4"
        z: 2      
    }

    Rectangle{
        z: 1
        width: parent.width
        height: 1
        anchors.top: volumeAdjustBar.top
        color: "#c6c6c6"
    }

    Rectangle{
        id: volumeAdjustBar
        anchors.bottom: protocolBar.top
        width: parent.width
        height: volumeAdjustBarHeight
        color: "#f5f5f5"

        CommonSlider{
            id: volumeSlider
            width: 350
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            stepSize: 0.5
            maximumValue: 20
            minimumValue: -20
            enabled: true

            onValueChanged: {
                if (getCallId() !== 0)
                {
                    logicObject.VolumeAjust(getCallId(), volumeSlider.value);
                }
                if (getPrelistenId() !== 0)
                {
                    logicObject.VolumeAjust(getPrelistenId(), volumeSlider.value);
                }

                pageObject.setLocalVolume("USB",volumeSlider.getValue());
            }

            Image{
                id: speakerImage
                anchors.verticalCenter : parent.verticalCenter
                anchors.right: parent.left
                anchors.rightMargin: 10
                source: volumeSlider.value == volumeSlider.minimumValue ? logicObject.getLocalImagePath() + "volume_1_A.png" : logicObject.getLocalImagePath() + "volume_1_B.png"
            }

            Text{
                id: volumeText
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: volumeSlider.right
                anchors.leftMargin: 10
                color: "#5f5f5f"
                text: volumeSlider.value + "dB"
                font.family: rootItem.fontName
                font.pixelSize: 14
                visible: !(volumeSlider.value == volumeSlider.minimumValue)
            }
        }
    }

    Rectangle{
        id: protocolBar
        width: parent.width
        height: protocolBarHeight
        anchors.bottom: parent.bottom
        ImageAndTextButton2{
            id: propertyBtn
            anchors.top: parent.top
            anchors.left: parent.left
            width: 96
            height: protocolBarHeight
            topMargin1: 9
            topMargin2: 9
            imageSource: logicObject.getLocalImagePath() + "property_A.png"
            info: langStrObject.GetLangString("ETCS_PROPERTY")
            onClickedSignal: {
                callSettingPage.syncOptions(pageObject.getLocalIntOption("USB", "Cycle"),
                                            pageObject.getLocalIntOption("USB", "Gap Time"),
                                            pageObject.getLocalIntOption("USB", "Priority"),
                                            pageObject.getLocalIntOption("USB", "Presignal"),
                                            pageObject.getLocalBoolOption("USB", "Regain"),
                                            pageObject.getLocalBoolOption("USB", "Partial"));
                callSettingPage.initCallStatus();
            }
        }

        ImageButtonFrame{
            id: preListenBtn
            anchors.top: parent.top
            anchors.left: propertyBtn.right
            width: 96
            height: protocolBarHeight
            margin1: 9
            margin2: 9
            buttonImageWidth: 38
            buttonImageHeight: 37
            imageSource: logicObject.getLocalImagePath() + "prelisten_A.png"
            textName: langStrObject.GetLangString("ETCS_PRELISTEN")
        }
        ImageButtonFrame{
            id: callBtn
            anchors.top: parent.top
            anchors.left: preListenBtn.right
            width: 96
            height: protocolBarHeight
            imgWidth: 58
            imgHeight: 56
            margin1: 0
            margin2: 0
            buttonImageWidth: 56
            buttonImageHeight: 54
            imageSource: logicObject.getLocalImagePath() + "call_A.png"
            textName: langStrObject.GetLangString("ETCS_CALL")
        }

        Rectangle{

            function topMessageHideSlot()
            {
                if (sendBtnMouseArea.getEnabledValue() === false)
                {
                    initStatus();
                }
            }

            function initStatus()
            {
                sendBtnImage.setSource(logicObject.getLocalImagePath() + "send_A.png");
                sendBtnName.setTextColor("#5f5f5f");
                sendBtnMouseArea.setEnabledValue(true);
            }

            function disableStatus()
            {
                sendBtnImage.setSource(logicObject.getLocalImagePath() + "send_C.png");
                sendBtnName.setTextColor("#40666666");
                sendBtnMouseArea.setEnabledValue(false);
            }

            id: sendBtn
            anchors.top: parent.top
            anchors.left: callBtn.right
            width: 96
            height: protocolBarHeight
            color: "#f5f5f5"

            CommonImage{
                id: sendBtnImage
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 9
                source: logicObject.getLocalImagePath() + "send_A.png"
            }

            CommonText{
                id: sendBtnName
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: sendBtnImage.bottom
                anchors.topMargin: 9
                font.family: rootItem.fontName
                font.pixelSize: 16
                color: "#5f5f5f"
                width: parent.width
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignHCenter
                text: langStrObject.GetLangString("ETCS_SEND")
            }

            MouseArea{

                function setEnabledValue(value)
                {
                    enabled = value;
                }

                function getEnabledValue()
                {
                    return enabled;
                }

                id: sendBtnMouseArea
                anchors.fill: parent
                onClicked: {
                    var ret = setMessageBarStatus();
                    if (ret < 0)
                    {
                        return;
                    }

                    syncSourceListSelect();
                    var errorCode = pageObject.baseStartCall("USB", "Call", callSettingPage.getgPrority(),
                                                            getVolumeSliderValue(),
                                                            callSettingPage.getgPreSignal(),
                                                            callSettingPage.getgReconnect(),
                                                            callSettingPage.getgPartlyConnect(),
                                                            callSettingPage.getgCycle(),
                                                            callSettingPage.getgGapTime(),
                                                            callSettingPage.getgTimeType()
                                                            );

                    ret = rootItem.dealCallError(errorCode);
                    if (ret !== 0)
                    {
                        return;
                    }

                    if (getPrelistenId() !== 0)
                    {
                        pageObject.baseStopCallFromId("PreListen", getPrelistenId());
                        zeroPrelistenId();
                    }

                    sendBtn.disableStatus();
                    topMessage.messagePlay();
                }
            }
        }

        ImageAndTextButton2{
            id: protocolMoreBtn
            anchors.top: parent.top
            anchors.left: sendBtn.right
            width: 96
            height: protocolBarHeight
            topMargin1: 9
            topMargin2: 9
            imageSource: logicObject.getLocalImagePath() + "more_A.png"
            info: langStrObject.GetLangString("ETCS_MORE")
            onClickedSignal: {
                var ret = setMessageBarStatus();
                if (ret === 0)
                {
                    moreFuncView.setVisibleValue(true);
                }
            }
        }
    }

    CallSettingPage{
        id : callSettingPage
        anchors.bottom: parent.bottom
        width: parent.width
        height: parent.height
        z: 11
        visible: false

        onQuitSignal: {
            pageObject.setLocalOption("USB",
                                      callSettingPage.getgPrority(),
                                      getVolumeSliderValue(),
                                     callSettingPage.getgPreSignal(),
                                     callSettingPage.getgReconnect(),
                                     callSettingPage.getgPartlyConnect(),
                                     callSettingPage.getgCycle(),
                                     callSettingPage.getgGapTime());
        }
    }

    Timer{
        id: waitMaskHideTimer
        interval: 60 * 1000
        repeat: false
        triggeredOnStart: false
        onTriggered: {
            hideWaitMask();
        }
    }

    Rectangle{
        id: waitMask
        anchors.top: listViewTitleBar.top
        anchors.left: parent.left
        z: 10
        width: parent.width
        height: parent.height - mainPage.g_titleBarHeight
        color: "#70666666"
        visible: false

        AnimatedImage{
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: -60
            source: logicObject.getLocalImagePath() + "waiting_1.gif"
            playing: true
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {

            }
        }
    }

    MoreFuncView{
        id: moreFuncView
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width
        height: parent.height
        z: 5
        visible: false
        onSendPredefineBtnSingal: {
            moreFuncView.setVisibleValue(false);
            predefineSetting.addInitStatus(langStrObject.GetLangString("ETCS_USB"))
        }
        onSendScheduleBtnSingal: {
            moreFuncView.setVisibleValue(false);
            scheduleSettingView.addInitStatus(callSettingPage.getgCycle(),
                                              callSettingPage.getgGapTime(),
                                              title.text);
        }
    }

    PredefineSetting{
        id: predefineSetting
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width
        height: predefinedViewCallHeight
        visible: false
        z: 5
        onCancelSignal: {
            predefineSetting.setVisibleValue(false);
            predefineSetting.quitStatus();
        }

        onSaveSignal: {

            if (logicObject.isOverPredefineMaxCount() === true)
            {
                promptMask.messageShow(langStrObject.GetLangString("ETCS_PREDEFINE_FULL"));
                timeOutTimer.setInterval(3000);
                timeOutTimer.start();
                return;
            }

            syncSourceListSelect();
            pageObject.baseMakePredefine("USB",
                                         predefineSetting.getName(),
                                     callSettingPage.getgPrority(),
                                     getVolumeSliderValue(),
                                    callSettingPage.getgPreSignal(),
                                    callSettingPage.getgReconnect(),
                                    callSettingPage.getgPartlyConnect(),
                                    callSettingPage.getgCycle(),
                                    callSettingPage.getgGapTime(),
                                    callSettingPage.getgTimeType());
            predefineSetting.setVisibleValue(false);
            predefineSetting.quitStatus();
        }
    }

    ScheduleSettingView{
        id: scheduleSettingView
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width
        height: scheduleSettingViewHeight
        visible: false
        z: 5
        onCancelSignal: {
            scheduleSettingView.setVisibleValue(false);
            scheduleSettingView.quitStatus();
        }
        onSaveSignal: {

            if (scheduleManage.isOverScheduleMaxCount() === true)
            {
                promptMask.messageShow(langStrObject.GetLangString("ETCS_SCHEDULE_FULL"));
                timeOutTimer.setInterval(3000);
                timeOutTimer.start();
                return;
            }

            scheduleSettingView.addSchedule();
            syncSourceListSelect();
            pageObject.baseMakeSchedule("USB",
                                        callSettingPage.getgPrority(),
                                        getVolumeSliderValue(),
                                        callSettingPage.getgPreSignal(),
                                        callSettingPage.getgReconnect(),
                                        callSettingPage.getgPartlyConnect(),
                                        callSettingPage.getgCycle(),
                                        callSettingPage.getgGapTime(),
                                        callSettingPage.getgTimeType());
            scheduleSettingView.setVisibleValue(false);
            scheduleSettingView.quitStatus();
        }
    }

	Component.onCompleted: {
        preListenBtn.buttonClicked.connect(preListenBtnClickSlot);
        callBtn.buttonClicked.connect(callBtnClickSlot);

        logicObject.signalCallStatus.connect(activateUpdateSlot);
        logicObject.gongPlayingTrueSignal.connect(setGongTrueStatus);
        logicObject.gongPlayingFalseSignal.connect(setGongFalseStatus);
        logicObject.systemResultSignal.connect(systemResultSlot);
        rootItem.updateLanguage.connect(updateLanguageSlot);
        rootItem.changeUserSignal.connect(changeUserSlot);
        udiskObject.insertUDiskSignal.connect(insertUDiskSlot);
        udiskObject.removeUDiskSignal.connect(removeUDiskSlot);
        sourceManageObject.finishUsbSourceCreateSignal.connect(finishUsbSourceCreateSlot);
        sourceManageObject.timeOutSignal.connect(timeOutSlot);
        topMessage.messageHideSignal.connect(sendBtn.topMessageHideSlot);

        callSettingPage.syncOptions(pageObject.getLocalIntOption("USB", "Cycle"),
                                    pageObject.getLocalIntOption("USB", "Gap Time"),
                                    pageObject.getLocalIntOption("USB", "Priority"),
                                    pageObject.getLocalIntOption("USB", "Presignal"),
                                    pageObject.getLocalBoolOption("USB", "Regain"),
                                    pageObject.getLocalBoolOption("USB", "Partial"));
        volumeSlider.value = pageObject.getLocalFloatOption("USB", "Volume");
    }
}
