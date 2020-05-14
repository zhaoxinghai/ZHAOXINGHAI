import QtQuick 2.0
import "../Components"

Rectangle {

    property int gid:0
    property int callInfoHeight: height - mainPage.g_titleBarHeight

    function updateLanguageSlot()
    {
        title.text = langStrObject.GetLangString("ETCS_DETAILS");
        nameBtn.btnName = langStrObject.GetLangString("ETCS_NAME");
        timeBtn.btnName = langStrObject.GetLangString("ETCS_TIME");
        statusInfoBtn.btnName = langStrObject.GetLangString("ETCS_STATUS");
        cycleInfoBtn.btnName = langStrObject.GetLangString("ETCS_CYCLE");
        currentCycleInfoBtn.btnName = langStrObject.GetLangString("ETCS_CURRENT_CYCLE");
        //gaptimeInfoBtn.btnName = langStrObject.GetLangString("ETCS_GAPTIME");
        priorityInfoBtn.btnName = langStrObject.GetLangString("ETCS_PRIORITY");
        presignalInfoBtn.btnName = langStrObject.GetLangString("ETCS_PRESIGNAL");
        reconnectInfoBtn.btnName = langStrObject.GetLangString("ETCS_RECONNECT");
        partlyConnectInfoBtn.btnName = langStrObject.GetLangString("ETCS_PARTLY_CONNECT");
        volumeInfoBtn.btnName = langStrObject.GetLangString("ETCS_VOLUME");
        sourceInfoBtn.btnName = langStrObject.GetLangString("ETCS_SOURCE");
        zoneInfoBtn.btnName = langStrObject.GetLangString("ETCS_ZONE");
        zoneDetail.prevTitleNameText = langStrObject.GetLangString("ETCS_DETAILS");
        sourceDetail.prevTitleNameText = langStrObject.GetLangString("ETCS_DETAILS");
    }

    function changeView(index)
    {
        if (index === "zoneInfoBtn")
        {
            zoneDetail.setVisibleValue(true);
        }
        else if(index === "sourceInfoBtn")
        {
            sourceDetail.setStatus(gid);
            sourceDetail.setVisibleValue(true);
        }
    }

    function show(id)
    {
        gid = id;
        initStatus();
        setVisibleValue(true);
        dragUnit.setDragUnitEnableValue(false);
        zoneDetail.setStatus(gid);
        sourceDetail.setStatus(gid);
    }

    function initStatus()
    {
        nameBtn.infoText = logicObject.getActivateStrValue(gid, "Name") === "Prelisten" ? langStrObject.GetLangString("ETCS_PRELISTEN") : logicObject.getActivateStrValue(gid, "Name")
        timeBtn.infoText = logicObject.getActivateStrValue(gid, "Time");
        statusInfoBtn.infoText = mapStatusToStatusName(logicObject.getActivateStrValue(gid, "Status"));
        cycleInfoBtn.infoText = logicObject.getActivateIntValue(gid, "Cycle") === 0 ? langStrObject.GetLangString("ETCS_FOREVER") : logicObject.getActivateIntValue(gid, "Cycle")
        currentCycleInfoBtn.infoText = logicObject.getActivateIntValue(gid, "Current Cycle");
        //gaptimeInfoBtn.infoText = logicObject.getActivateStrValue(gid, "Gap Time");
        priorityInfoBtn.infoText = logicObject.getActivateIntValue(gid, "Priority");
        presignalInfoBtn.infoText = logicObject.getActivateStrValue(gid, "Pre-signal");
        reconnectInfoBtn.infoText = logicObject.getActivateBoolValue(gid, "Reconnect") === true ? langStrObject.GetLangString("ETCS_YES") : langStrObject.GetLangString("ETCS_NO")
        partlyConnectInfoBtn.infoText = logicObject.getActivateBoolValue(gid, "Partly Connect") === true ? langStrObject.GetLangString("ETCS_YES") : langStrObject.GetLangString("ETCS_NO")
        volumeInfoBtn.infoText = logicObject.getActivateStrValue(gid, "Volume")
        sourceInfoBtn.infoText = logicObject.getActivateStrValue(gid, "SourceType");
        sourceInfoBtn.isClickBtn = logicObject.getActivateBoolValue(gid, "isLive") === true ? false : true;
        sourceInfoBtn.clickImgSrc =logicObject.getActivateBoolValue(gid, "isLive") === true ? "": logicObject.getLocalImagePath() + "next_1.png";
    }

    function hide()
    {
        setVisibleValue(false);
        dragUnit.setDragUnitEnableValue(true);
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

    function syncDetailInfoSignalSlot(id, current, status)
    {
        if (id === gid)
        {
            currentCycleInfoBtn.infoText = current;
            statusInfoBtn.infoText = mapStatusToStatusName(status);
        }
    }

    function setVisibleValue(value)
    {
        visible = value;
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
            text: langStrObject.GetLangString("ETCS_DETAILS")
            font.family: rootItem.fontName
            font.pixelSize: mainPage.g_titleTextFontSize
            width: (240 - prevBtnImgRect.width) * 2
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            Component.onCompleted: {
            }
        }

        Rectangle{
            id: prevBtnImgRect
            width: parent.height
            height: parent.height
            anchors.top: parent.top
            anchors.left: parent.left
            color: mouseArea.pressed ? "#505050" : parent.color
            z: 1
            clip: true
            Image{
                id: prevBtnImg
                source: logicObject.getLocalImagePath() + "back.png"
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }

            MouseArea{
                id: mouseArea
                anchors.fill: parent
                onClicked: {
                    hide();
                }
            }
        }
    }

    Rectangle{
        id: callInfo
        anchors.top: titleBar.bottom
        width: parent.width
        height: callInfoHeight
        color: "#e7e7e7"

        Flickable{
            id: callInfoFlickable
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: parent.height
            contentWidth: parent.width
            contentHeight: 65 * 13 + 2 * 22
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
                id: nameBtn
                anchors.top: rect1.bottom
                anchors.left: parent.left
                width: parent.width
                color: "#ffffff"
                imgSrc: ""
                btnName: langStrObject.GetLangString("ETCS_NAME");
                infoText: logicObject.getActivateStrValue(gid, "Name") === "Prelisten" ? langStrObject.GetLangString("ETCS_PRELISTEN") : logicObject.getActivateStrValue(gid, "Name")
                isClickBtn: false
                clickImgSrc: ""
                btnValue: "nameBtn"
                line1Visible: false
                line2Visible: true
                line2Width: parent.width - 18
                buttonNameWidth: 250
            }
            StripButton{
                id: timeBtn
                anchors.top: nameBtn.bottom
                anchors.left: parent.left
                width: parent.width
                color: "#ffffff"
                imgSrc: ""
                btnName: langStrObject.GetLangString("ETCS_TIME")
                infoText: logicObject.getActivateStrValue(gid, "Time");
                isClickBtn: false
                clickImgSrc: ""
                btnValue: "timeBtn"
                line1Visible: false
                line2Visible: true
                line2Width: parent.width - 18
                buttonNameWidth: 250
            }
            StripButton{
                id: statusInfoBtn
                anchors.top: timeBtn.bottom
                anchors.left: parent.left
                width: parent.width
                imgSrc: ""
                btnName: langStrObject.GetLangString("ETCS_STATUS")
                infoText: mapStatusToStatusName(logicObject.getActivateStrValue(gid, "Status"))
                isClickBtn: false
                clickImgSrc: ""
                btnValue: "statusInfoBtn"
                line1Visible: false
                line2Visible: true
                line2Width: parent.width - 18
                buttonNameWidth: 200
            }
            StripButton{
                id: cycleInfoBtn
                anchors.top: statusInfoBtn.bottom
                anchors.left: parent.left
                width: parent.width
                imgSrc: ""
                btnName: langStrObject.GetLangString("ETCS_CYCLE")
                infoText: logicObject.getActivateIntValue(gid, "Cycle") === 0 ? langStrObject.GetLangString("ETCS_FOREVER") : logicObject.getActivateIntValue(gid, "Cycle")
                isClickBtn: false
                clickImgSrc: ""
                btnValue: "cycleInfoBtn"
                line1Visible: false
                line2Visible: true
                line2Width: parent.width - 18
                buttonNameWidth: 250
            }
            StripButton{
                id: currentCycleInfoBtn
                anchors.top: cycleInfoBtn.bottom
                anchors.left: parent.left
                width: parent.width
                imgSrc: ""
                btnName: langStrObject.GetLangString("ETCS_CURRENT_CYCLE")
                infoText: logicObject.getActivateIntValue(gid, "Current Cycle")
                isClickBtn: false
                clickImgSrc: ""
                btnValue: "currentCycleInfoBtn"
                line1Visible: false
                line2Visible: true
                line2Width: parent.width - 18
                buttonNameWidth: 250
            }
            /*
            StripButton{
                id: gaptimeInfoBtn
                anchors.top: currentCycleInfoBtn.bottom
                anchors.left: parent.left
                width: parent.width
                imgSrc: ""
                btnName:  langStrObject.GetLangString("ETCS_GAPTIME")
                infoText: logicObject.getActivateStrValue(gid, "Gap Time")
                isClickBtn: false
                clickImgSrc: ""
                btnValue: "gaptimeInfoBtn"
                line1Visible: false
                line2Visible: true
                line2Width: parent.width - 18
                buttonNameWidth: 250
            }*/
            StripButton{
                id: priorityInfoBtn
                anchors.top: currentCycleInfoBtn.bottom
                anchors.left: parent.left
                width: parent.width
                imgSrc: ""
                btnName: langStrObject.GetLangString("ETCS_PRIORITY")
                infoText: logicObject.getActivateIntValue(gid, "Priority")
                isClickBtn: false
                clickImgSrc: ""
                btnValue: "priorityInfoBtn"
                line1Visible: false
                line2Visible: true
                line2Width: parent.width - 18
                buttonNameWidth: 250
            }
            StripButton{
                id: presignalInfoBtn
                anchors.top: priorityInfoBtn.bottom
                anchors.left: parent.left
                width: parent.width
                imgSrc: ""
                btnName: langStrObject.GetLangString("ETCS_PRESIGNAL")
                infoText: logicObject.getActivateStrValue(gid, "Pre-signal")
                isClickBtn: false
                clickImgSrc: ""
                btnValue: "presignalInfoBtn"
                line1Visible: false
                line2Visible: true
                line2Width: parent.width - 18
                buttonNameWidth: 250
            }
            StripButton{
                id: reconnectInfoBtn
                anchors.top: presignalInfoBtn.bottom
                anchors.left: parent.left
                width: parent.width
                imgSrc: ""
                btnName: langStrObject.GetLangString("ETCS_RECONNECT")
                infoText: logicObject.getActivateBoolValue(gid, "Reconnect") === true ? langStrObject.GetLangString("ETCS_YES") : langStrObject.GetLangString("ETCS_NO")
                isClickBtn: false
                clickImgSrc: ""
                btnValue: "reconnectInfoBtn"
                line1Visible: false
                line2Visible: true
                line2Width: parent.width - 18
                buttonNameWidth: 300
            }
            StripButton{
                id: partlyConnectInfoBtn
                anchors.top: reconnectInfoBtn.bottom
                anchors.left: parent.left
                width: parent.width
                imgSrc: ""
                btnName: langStrObject.GetLangString("ETCS_PARTLY_CONNECT")
                infoText: logicObject.getActivateBoolValue(gid, "Partly Connect") === true ? langStrObject.GetLangString("ETCS_YES") : langStrObject.GetLangString("ETCS_NO")
                isClickBtn: false
                clickImgSrc: ""
                btnValue: "partlyConnectInfoBtn"
                line1Visible: false
                line2Visible: true
                line2Width: parent.width - 18
                buttonNameWidth: 300
            }
            StripButton{
                id: volumeInfoBtn
                anchors.top: partlyConnectInfoBtn.bottom
                anchors.left: parent.left
                width: parent.width
                imgSrc: ""
                btnName: langStrObject.GetLangString("ETCS_VOLUME")
                infoText: logicObject.getActivateStrValue(gid, "Volume")
                isClickBtn: false
                clickImgSrc: ""
                btnValue: "volumeInfoBtn"
                line1Visible: false
                line2Visible: true
                line2Width: parent.width - 18
                buttonNameWidth: 250
            }
            StripButton{
                id: sourceInfoBtn
                anchors.top: volumeInfoBtn.bottom
                anchors.left: parent.left
                width: parent.width
                imgSrc: ""
                btnName: langStrObject.GetLangString("ETCS_SOURCE")
                infoText: logicObject.getActivateStrValue(gid, "SourceType")
                isClickBtn: logicObject.getActivateBoolValue(gid, "isLive") === true ? false : true
                clickImgSrc: logicObject.getActivateBoolValue(gid, "isLive") === true ? "": logicObject.getLocalImagePath() + "next_1.png"
                btnValue: "sourceInfoBtn"
                line1Visible: false
                line2Visible: true
                line2Width: parent.width - 18
                buttonNameWidth: 250
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
                line1Visible: false
                line2Visible: false
                buttonNameWidth: 250
            }
            Rectangle{
                id: rect2
                width: parent.width
                height: 22
                anchors.top: zoneInfoBtn.bottom
                anchors.left: parent.left
                color: "#e7e7e7"
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

    ZoneDetail{
        id: zoneDetail
        width: parent.width
        height: logicObject.getRealHeight()
        visible: false
        z: 2
        gType: "Activate"
        prevTitleNameText: langStrObject.GetLangString("ETCS_DETAILS");
    }

    SourceDetail{
        id: sourceDetail
        width: parent.width
        height: logicObject.getRealHeight()
        visible: false
        z: 2
        gType: "Activate"
        prevTitleNameText: langStrObject.GetLangString("ETCS_DETAILS");
    }


    Component.onCompleted: {
        rootItem.updateLanguage.connect(updateLanguageSlot);
        logicObject.syncDetailInfoSignal.connect(syncDetailInfoSignalSlot);
        zoneInfoBtn.btnValueSignal.connect(changeView);
        sourceInfoBtn.btnValueSignal.connect(changeView);
    }
}
