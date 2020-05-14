import QtQuick 2.0
import QtQml.Models 2.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import "../Components"

/*
**0:Live    ---  Prelisten:NO Send:NO Volume:Yes
**1:Local   ---  Prelisten:YES Send:YES Volume:Yes
**2:Network ---  Prelisten:YES Send:YES Volume:NO(not real control)
*/

Rectangle {

    property int callInfoHeight: height - mainPage.g_titleBarHeight - volumeAdjustBarHeight - protocolBarHeight
    property int volumeAdjustBarHeight: 60
    property int protocolBarHeight: 100

    property int gid: 0
    property string gType: ""
    property string gLivePredefine: "Live"
    property string gLocalPredefine: "Local"
    property string gNetworkPredefine: "Network"
    property int prelistenId: 0
    property int callId: 0
    property bool haveGong: false
    property string callStatus: dis_status
    property string prelistenStatus: dis_status

    property string wait_status: "wait"
    property string full_status: "full"
    property string part_status: "part"
    property string dis_status: "disconnect"

    function updateLanguageSlot()
    {
        cycleInfoBtn.btnName = langStrObject.GetLangString("ETCS_CYCLE");
        //gaptimeInfoBtn.btnName = langStrObject.GetLangString("ETCS_GAPTIME");
        priorityInfoBtn.btnName = langStrObject.GetLangString("ETCS_PRIORITY");
        presignalInfoBtn.btnName = langStrObject.GetLangString("ETCS_PRESIGNAL");
        reconnectInfoBtn.btnName = langStrObject.GetLangString("ETCS_RECONNECT");
        partlyConnectInfoBtn.btnName = langStrObject.GetLangString("ETCS_PARTLY_CONNECT");
        volumeInfoBtn.btnName = langStrObject.GetLangString("ETCS_VOLUME");
        sourceInfoBtn.btnName = langStrObject.GetLangString("ETCS_SOURCE");
        zoneInfoBtn.btnName = langStrObject.GetLangString("ETCS_ZONE");
        propertyBtn.setInfoValue(langStrObject.GetLangString("ETCS_PROPERTY"));
        preListenBtn.setBtnText(langStrObject.GetLangString("ETCS_PRELISTEN"));
        callBtn.setBtnText(langStrObject.GetLangString("ETCS_CALL"));
        sendBtnName.setText(langStrObject.GetLangString("ETCS_SEND"));
        protocolMoreBtn.setInfoValue(langStrObject.GetLangString("ETCS_MORE"));
        title.text = langStrObject.GetLangString("ETCS_PREDEFINE_CALL");
        prevTitleName.text = langStrObject.GetLangString("ETCS_PREDEFINE");
        zoneDetail.prevTitleNameText = langStrObject.GetLangString("ETCS_PREDEFINE_CALL");
        sourceDetail.prevTitleNameText = langStrObject.GetLangString("ETCS_PREDEFINE_CALL");
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


    function mapStatusToStatusName(eStatus)
    {
        if (eStatus === "wait")
        {
            return langStrObject.GetLangString("ETCS_WAIT")
        }
        else if (eStatus === "full")
        {
            return langStrObject.GetLangString("ETCS_FULL_CONNECT")
        }
        else if (eStatus === "part")
        {
            return langStrObject.GetLangString("ETCS_PARTLY_CONNECT")
        }
        else if (eStatus === "disconnect")
        {
            return langStrObject.GetLangString("ETCS_DISCONNECT")
        }
        else
        {
            return "";
        }
    }

    function updateCallStatus(parentCallId, parentCallStatus, parentHaveGong)
    {
        if (parentCallId <= 0)
        {
            return;
        }
        else
        {
            setCallId(parentCallId);
            setCallStatusValue(parentCallStatus);
            setHaveGongValue(parentHaveGong);
            upDateCallBtnStatus();
        }
    }

    function setStatus(id)
    {
        setGid(id);
        setVisibleValue(true);
        setPredefineStatus();
        setCallId(0);
        zeroPrelistenId();
        zoneDetail.setStatus(gid);
        sourceDetail.setStatus(gid);
    }

    function quitStatus()
    {
        var id;
        id = getCallId();
        if (id !== 0)
        {
            if (gType == gLivePredefine)
            {
                pageObject.predefineStopCallFromId(getCallId());
                setCallId(0);
                statusBar.setStatusBarMessageTextForTimer(langStrObject.GetLangString("ETCS_CALL_ERR_9"), "ETCS_CALL_ERR_9");
            }
            else
            {
                statusBar.setStatusBarMessageTextForTimer(langStrObject.GetLangString("ETCS_CALL_ERR_8"), "ETCS_CALL_ERR_8");
            }

            setCallStatusValue(dis_status);
            upDateCallBtnStatus();
            if (gType != gLivePredefine)
            {
                setPrelistenBtnEnable();
            }
            if (gType == gNetworkPredefine)
            {
                setVolumeSliderEnable();
            }
        }

        id = getPrelistenId();
        if (id !== 0)
        {
            pageObject.predefineStopPreListen(getPrelistenId());
            zeroPrelistenId();
            statusBar.setStatusBarMessageTextForTimer(langStrObject.GetLangString("ETCS_CALL_ERR_13"), "ETCS_CALL_ERR_13");
            setPrelistenStatusValue(dis_status);
            upDatePrelistenBtnStatus();
            if (gType == gNetworkPredefine)
            {
                setVolumeSliderEnable();
            }
        }

        setVisibleValue(false);
    }

    function setPredefineStatus()
    {
        gType = getPredefineType();
        if  (gType == gLivePredefine)
        {
            setLivePredefineStatus();
        }
        else if (gType == gLocalPredefine)
        {
            setLocalPredefineStatus();
        }
        else if (gType == gNetworkPredefine)
        {
            setNetworkPredefineStatus();
        }
    }

    function setLivePredefineStatus()
    {
        setVolumeSliderEnable();
        setPrelistenBtnDisable();
        sendBtn.disableStatus();
    }

    function setLocalPredefineStatus()
    {
        setVolumeSliderEnable();
        setPrelistenBtnEnable();
        sendBtn.initStatus();
    }

    function setNetworkPredefineStatus()
    {
        setVolumeSliderEnable();
        setPrelistenBtnEnable();
        sendBtn.initStatus();
    }

    function getPredefineType()
    {
        var type
        type = logicObject.getPredefineStrValue(gid, "Type");
        return type;
    }

    function setGid(id)
    {
       gid = id;
    }

    function getGid()
    {
        return gid;
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function setVolumeSliderEnable()
    {
        volumeSlider.enabled = true;
    }

    function setVolumeSliderDisable()
    {
        volumeSlider.enabled = false;
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

    function setPrelistenBtnText(text)
    {
        preListenBtn.setBtnText(text);
    }

    function resetPrelistenBtnStatus()
    {
        preListenBtn.resetButtonStatus();
    }

    function setCallBtnWaitStatus()
    {
        callBtn.setBtnEnable(logicObject.getLocalImagePath() + "call_B.png", "#5f5f5f");
        callBtn.setBtnText(langStrObject.GetLangString("ETCS_WAIT"));
        callBtn.setButtonStatus(true);
    }

    function setCallBtnFullStatus()
    {
        callBtn.setBtnEnable(logicObject.getLocalImagePath() + "call_B.png", "#5f5f5f");
        callBtn.setBtnText(langStrObject.GetLangString("ETCS_FULL_CONNECT"));
        callBtn.setButtonStatus(true);
    }

    function setCallBtnFullGongStatus()
    {
        callBtn.setBtnEnable(logicObject.getLocalImagePath() + "call_D.png", "#5f5f5f");
        callBtn.setImgPlayingValue(true);
        callBtn.setBtnText(langStrObject.GetLangString("ETCS_FULL_CONNECT"));
        callBtn.setButtonStatus(true);
    }

    function setCallBtnPartStatus()
    {
        callBtn.setBtnEnable(logicObject.getLocalImagePath() + "call_B.png", "#5f5f5f");
        callBtn.setBtnText(langStrObject.GetLangString("ETCS_PARTLY_CONNECT"));
        callBtn.setButtonStatus(true);
    }

    function setCallBtnPartGongStatus()
    {
        callBtn.setBtnEnable(logicObject.getLocalImagePath() + "call_D.png", "#5f5f5fs");
        callBtn.setImgPlayingValue(true);
        callBtn.setBtnText(langStrObject.GetLangString("ETCS_PARTLY_CONNECT"));
        callBtn.setButtonStatus(true);
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

    function setPredefineListSelect()
    {
        var count = predefineModel.rowCount();
        var select;
        var index;

        logicObject.predefineUnselectAll();
        for (var i = 0; i < count; i++)
        {
            select = predefineModel.data(predefineModel.index(i, 0), 3);
            if (Number(select) === 1)
            {
                index = predefineModel.data(predefineModel.index(i, 0), 1);
                logicObject.setPredefineSelected(Number(index), 1);
            }
        }
        return 0;
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

    function setCallId(value)
    {
        callId = value;
    }

    function zeroCallId()
    {
        callId = 0;
    }

    function getCallId()
    {
        return callId;
    }

    function preListenBtnClickSlot(status)
    {
        if (status === true)
        {
            setPredefineListSelect();
            var errorCode = pageObject.predefinePreListen(volumeSlider.value);
            var ret = rootItem.dealCallError(errorCode);
            if (ret !== 0)
            {
                if (gType == gNetworkPredefine)
                {
                    setVolumeSliderEnable();
                }

                zeroPrelistenId();
                setHaveGongValue(false);
                setPrelistenStatusValue(dis_status);
                upDatePrelistenBtnStatus();
                return;
            }

            setPrelistenId();
            if (gType == gNetworkPredefine)
            {
                setVolumeSliderDisable();
            }
            setPrelistenStatusValue(wait_status);
            upDatePrelistenBtnStatus();
        }
        else if(status === false)
        {
            pageObject.predefineStopPreListen(getPrelistenId());
            zeroPrelistenId();
            if (gType == gNetworkPredefine)
            {
                setVolumeSliderEnable();
            }
            setHaveGongValue(false);
            setPrelistenStatusValue(dis_status);
            upDatePrelistenBtnStatus();
        }

        return 0;
    }

    function callBtnClickSlot(status)
    {
        var tmpId;

        if (status === true)
        {
            setPredefineListSelect();
            var errorCode = pageObject.predefineCall(volumeSlider.value);
            var ret = rootItem.dealCallError(errorCode);
            if (ret !== 0)
            {
                setCallId(0);
                if (gType != gLivePredefine)
                {
                    setPrelistenBtnEnable();
                }
                if (gType == gNetworkPredefine)
                {
                    setVolumeSliderEnable();
                }
                setHaveGongValue(false);
                setCallStatusValue(dis_status);
                upDateCallBtnStatus();
                return;
            }

            setCallId(pageObject.getCallId());

            if (gType != gLivePredefine)
            {
                if (getPrelistenId() !== 0)
                {
                    pageObject.predefineStopPreListen(getPrelistenId());
                    zeroPrelistenId();
                }

                setPrelistenBtnDisable();
            }

            if (gType == gNetworkPredefine)
            {
                setVolumeSliderDisable();
            }

            setCallStatusValue(wait_status);
            upDateCallBtnStatus();
        }
        else if(status === false)
        {
            pageObject.predefineStopCallFromId(getCallId());
            setCallId(0);
            if (gType != gLivePredefine)
            {
                setPrelistenBtnEnable();
            }
            if (gType == gNetworkPredefine)
            {
                setVolumeSliderEnable();
            }
            setHaveGongValue(false);
            setCallStatusValue(dis_status);
            upDateCallBtnStatus();
        }

        return 0;
    }

    function activateUpdateSlot(id, text)
    {
        if  (id === getPrelistenId())
        {
            if (text === dis_status)
            {
                if (gType == gNetworkPredefine)
                {
                    setVolumeSliderEnable();
                }

                zeroPrelistenId();
                setHaveGongValue(false);
                setPrelistenStatusValue(dis_status);
                upDatePrelistenBtnStatus();
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
                setCallId(0);
                if (gType != gLivePredefine)
                {
                    setPrelistenBtnEnable();
                }
                if (gType == gNetworkPredefine)
                {
                    setVolumeSliderEnable();
                }
                setHaveGongValue(false);
                setCallStatusValue(dis_status);
                upDateCallBtnStatus();
            }
            else if (text === full_status)
            {
                setCallStatusValue(full_status);
                upDateCallBtnStatus();
            }
            else if (text === part_status)
            {
                setCallStatusValue(part_status);
                upDateCallBtnStatus();
            }
            else if (text === wait_status)
            {
                setCallStatusValue(wait_status);
                upDateCallBtnStatus();
            }
        }
    }

    function changeView(index)
    {
        if (index === "zoneInfoBtn")
        {
            zoneDetail.setVisibleValue(true);
        }
        else if(index === "sourceInfoBtn")
        {
            sourceDetail.setVisibleValue(true);
        }
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
            text: langStrObject.GetLangString("ETCS_PREDEFINE_CALL");
            font.family: rootItem.fontName
            font.pixelSize: 26
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
                text: langStrObject.GetLangString("ETCS_PREDEFINE")
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
                    mainPage.showBusinessBar();
                    quitStatus();
                }
            }
        }
    }

    Rectangle{
        id: callInfo
        anchors.top: titleBar.bottom
        width: parent.width - 18
        height: callInfoHeight
        color: parent.color

        Flickable{
            id: callInfoFlickable
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: parent.height
            contentWidth: parent.width
            contentHeight: (65 * 9)+ 136
            flickableDirection: Flickable.VerticalFlick
            boundsBehavior: Flickable.StopAtBounds
            clip: true

            Rectangle{
                id: predefineCallDesc
                anchors.top: parent.top
                anchors.left: parent.left
                width: parent.width
                height: 136
                color: "#ffffff"

                Text{
                    id: predefineName
                    anchors.top: parent.top
                    anchors.topMargin: 15
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: parent.width - 80
                    color: "#303030"
                    text: logicObject.getPredefineStrValue(gid, "Name");
                    font.family: rootItem.fontName
                    font.pixelSize: 24
                    horizontalAlignment: Text.AlignHCenter
                    elide: Text.ElideRight
                }

                Text{
                    id: predefineDesc
                    anchors.top: predefineName.bottom
                    anchors.topMargin: 5
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: parent.width - 80
                    color: "#858585"
                    text: logicObject.getPredefineStrValue(gid, "Description")
                    font.family: rootItem.fontName
                    font.pixelSize: 18
                    wrapMode: Text.Wrap
                    maximumLineCount: 3
                    horizontalAlignment: Text.AlignHCenter
                    elide: Text.ElideRight
                    Component.onCompleted: {
                    }
                }
            }

            StripButton{
                id: cycleInfoBtn
                anchors.top: predefineCallDesc.bottom
                anchors.left: parent.left
                width: parent.width
                imgSrc: ""
                btnName: langStrObject.GetLangString("ETCS_CYCLE")
                infoText: logicObject.getPredefineIntValue(gid, "Cycle") === 0 ? langStrObject.GetLangString("ETCS_FOREVER") : logicObject.getPredefineIntValue(gid, "Cycle")
                isClickBtn: false
                clickImgSrc: ""
                btnValue: "cycleInfoBtn"
                line1Visible: true
                line2Visible: false
                buttonNameWidth: 300
                Component.onCompleted: {
                }
            }
            /*
            StripButton{
                id: gaptimeInfoBtn
                anchors.top: cycleInfoBtn.bottom
                anchors.left: parent.left
                width: parent.width
                imgSrc: ""
                btnName:  langStrObject.GetLangString("ETCS_GAPTIME")
                infoText: logicObject.getPredefineStrValue(gid, "Gap Time")
                isClickBtn: false
                clickImgSrc: ""
                btnValue: "gaptimeInfoBtn"
                line1Visible: true
                line2Visible: false
                buttonNameWidth: 300
                Component.onCompleted: {
                }
            }*/
            StripButton{
                id: priorityInfoBtn
                anchors.top: cycleInfoBtn.bottom
                anchors.left: parent.left
                width: parent.width
                imgSrc: ""
                btnName: langStrObject.GetLangString("ETCS_PRIORITY")
                infoText: logicObject.getPredefineIntValue(gid, "Priority")
                isClickBtn: false
                clickImgSrc: ""
                btnValue: "priorityInfoBtn"
                line1Visible: true
                line2Visible: false
                buttonNameWidth: 300
                Component.onCompleted: {
                }
            }
            StripButton{
                id: presignalInfoBtn
                anchors.top: priorityInfoBtn.bottom
                anchors.left: parent.left
                width: parent.width
                imgSrc: ""
                btnName: langStrObject.GetLangString("ETCS_PRESIGNAL")
                infoText: logicObject.getPredefineStrValue(gid, "Pre-signal")
                isClickBtn: false
                clickImgSrc: ""
                btnValue: "presignalInfoBtn"
                line1Visible: true
                line2Visible: false
                buttonNameWidth: 300
                Component.onCompleted: {
                }
            }
            StripButton{
                id: reconnectInfoBtn
                anchors.top: presignalInfoBtn.bottom
                anchors.left: parent.left
                width: parent.width
                imgSrc: ""
                btnName: langStrObject.GetLangString("ETCS_RECONNECT")
                infoText: logicObject.getPredefineBoolValue(gid, "Reconnect") === true ? langStrObject.GetLangString("ETCS_YES") : langStrObject.GetLangString("ETCS_NO")
                isClickBtn: false
                clickImgSrc: ""
                btnValue: "reconnectInfoBtn"
                line1Visible: true
                line2Visible: false
                buttonNameWidth: 300
                Component.onCompleted: {
                }
            }
            StripButton{
                id: partlyConnectInfoBtn
                anchors.top: reconnectInfoBtn.bottom
                anchors.left: parent.left
                width: parent.width
                imgSrc: ""
                btnName: langStrObject.GetLangString("ETCS_PARTLY_CONNECT")
                infoText: logicObject.getPredefineBoolValue(gid, "Partly Connect")  === true ? langStrObject.GetLangString("ETCS_YES") : langStrObject.GetLangString("ETCS_NO")
                isClickBtn: false
                clickImgSrc: ""
                btnValue: "partlyConnectInfoBtn"
                line1Visible: true
                line2Visible: false
                buttonNameWidth: 300
                Component.onCompleted: {
                }
            }
            StripButton{
                id: volumeInfoBtn
                anchors.top: partlyConnectInfoBtn.bottom
                anchors.left: parent.left
                width: parent.width
                imgSrc: ""
                btnName: langStrObject.GetLangString("ETCS_VOLUME")
                infoText: logicObject.getPredefineStrValue(gid, "Volume")
                isClickBtn: false
                clickImgSrc: ""
                btnValue: "volumeInfoBtn"
                line1Visible: true
                line2Visible: false
                buttonNameWidth: 300
                Component.onCompleted: {
                }
            }
            StripButton{
                id: sourceInfoBtn
                anchors.top: volumeInfoBtn.bottom
                anchors.left: parent.left
                width: parent.width
                imgSrc: ""
                btnName: langStrObject.GetLangString("ETCS_SOURCE")
                infoText: logicObject.getPredefineStrValue(gid, "Source")
                isClickBtn: logicObject.getPredefineBoolValue(gid, "isLive") === true ? false : true
                clickImgSrc: logicObject.getPredefineBoolValue(gid, "isLive") === true ? "": logicObject.getLocalImagePath() + "next_1.png"
                btnValue: "sourceInfoBtn"
                line1Visible: true
                line2Visible: false
                buttonNameWidth: 200
                Component.onCompleted: {
                }
            }
            StripButton{
                id: zoneInfoBtn
                anchors.top: sourceInfoBtn.bottom
                anchors.left: parent.left
                width: parent.width
                imgSrc: ""
                btnName: langStrObject.GetLangString("ETCS_ZONE");
                infoText: ""
                isClickBtn: true
                clickImgSrc: logicObject.getLocalImagePath() + "next_1.png"
                btnValue: "zoneInfoBtn"
                line1Visible: true
                line2Visible: false
                buttonNameWidth: 400
                Component.onCompleted: {
                }
            }
        }
    }

    ScrollBar{
        id: scrollBar
        anchors.top: callInfo.top
        anchors.right: parent.right
        anchors.rightMargin: 6
        width: 6
        height: callInfo.height
        orientation: Qt.Vertical
        position: callInfoFlickable.visibleArea.yPosition
        pageSize: callInfoFlickable.visibleArea.heightRatio
        scrollBarVisible: callInfoFlickable.visibleArea.heightRatio == 1 ? false : true
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

            function getValue()
            {
                return value;
            }

            id: volumeSlider
            width: 350
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            value: logicObject.getPredefineFloatValue(gid, "Volume")
            stepSize: 0.5
            maximumValue: 20
            minimumValue: -20
            enabled: true
            onValueChanged: {
                volumeInfoBtn.infoText = volumeSlider.value + "dB";
                if (getCallId() !== 0)
                {
                    logicObject.VolumeAjust(getCallId(), volumeSlider.value);
                }
                if (getPrelistenId() !== 0)
                {
                    logicObject.VolumeAjust(getPrelistenId(), volumeSlider.value);
                }

                logicObject.updatePredefineVolume(gid, logicObject.getPredefineBoolValue(gid, "MyConfig"), volumeSlider.value);
            }

            Image{
                id: speakerImage
                anchors.verticalCenter : parent.verticalCenter
                anchors.right: parent.left
                anchors.rightMargin: 10
                source: volumeSlider.value == volumeSlider.minimumValue ? logicObject.getLocalImagePath() + "volume_1_A.png" : logicObject.getLocalImagePath() + "volume_1_B.png"
            }

            Text{
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
            dataColor: "#40666666"
            imageSource: logicObject.getLocalImagePath() + "property_B.png"
            info: langStrObject.GetLangString("ETCS_PROPERTY")
            enable: false
            Component.onCompleted: {
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
            Component.onCompleted: {
            }
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
            visible: true
            imageSource: logicObject.getLocalImagePath() + "call_A.png"
            textName: langStrObject.GetLangString("ETCS_CALL")
            Component.onCompleted: {
            }
        }

        Rectangle{

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

                id: sendBtnMouseArea
                anchors.fill: parent
                onClicked: {

                    setPredefineListSelect();
                    var errorCode = pageObject.predefineCall(volumeSlider.value);
                    var ret = rootItem.dealCallError(errorCode);
                    if (ret !== 0)
                    {
                        return;
                    }
                    if (gType != gLivePredefine)
                    {
                        if (getPrelistenId() !== 0)
                        {
                            pageObject.predefineStopPreListen(getPrelistenId());
                            zeroPrelistenId();
                        }
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
                    moreFuncView.setVisibleValue(true);
            }
            Component.onCompleted: {
            }
        }
    }

    ZoneDetail{
        id: zoneDetail
        width: parent.width
        height: parent.height
        visible: false
        z: 2
        gType: "Predefine"
        prevTitleNameText: langStrObject.GetLangString("ETCS_PREDEFINE_CALL");
    }

    SourceDetail{
        id: sourceDetail
        width: parent.width
        height: parent.height
        visible: false
        z: 2
        gType: "Predefine"
        prevTitleNameText: langStrObject.GetLangString("ETCS_PREDEFINE_CALL");
    }

    MoreFuncView{
        id: moreFuncView
        anchors.bottom: parent.bottom
        width: parent.width
        height: parent.height
        z: 5
        visible: false
        sendPredefineBtnVisible: false
        onSendScheduleBtnSingal: {
            moreFuncView.setVisibleValue(false);
            var ret = logicObject.getPredefineBoolValue(gid, "isPhysical");

            if (ret === true)
            {
                scheduleSettingView.addLiveInitStatus("");
            }
            else
            {
                scheduleSettingView.addInitStatus(Number(logicObject.getPredefineIntValue(gid, "Cycle")),
                                                  Number(logicObject.getPredefineIntValue(gid, "Interval")),
                                                  langStrObject.GetLangString("ETCS_PREDEFINE"));
            }
        }
    }

    ScheduleSettingView{
        id: scheduleSettingView
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width
        height: parent.height
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
            pageObject.predefineMakeSchedule(volumeSlider.getValue());
            scheduleSettingView.setVisibleValue(false);
            scheduleSettingView.quitStatus();
        }
    }

    Component.onCompleted: {
        zoneInfoBtn.btnValueSignal.connect(changeView);
        sourceInfoBtn.btnValueSignal.connect(changeView);
        preListenBtn.buttonClicked.connect(preListenBtnClickSlot);
        callBtn.buttonClicked.connect(callBtnClickSlot);
        logicObject.signalCallStatus.connect(activateUpdateSlot);
        logicObject.gongPlayingTrueSignal.connect(setGongTrueStatus);
        logicObject.gongPlayingFalseSignal.connect(setGongFalseStatus);
        topMessage.messageHideSignal.connect(sendBtn.initStatus);
        rootItem.updateLanguage.connect(updateLanguageSlot);
        rootItem.changeUserSignal.connect(changeUserSlot);
    }
}
