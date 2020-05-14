import QtQuick 2.0
import "../Components"

Rectangle {

    property int gid: 0
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
            sourceDetail.setVisibleValue(true);
        }
    }

    function show(id)
    {
        gid = id;
        setVisibleValue(true);
        dragUnit.setDragUnitEnableValue(false);
        initStatus();
        zoneDetail.setStatus(gid);
        sourceDetail.setStatus(gid);
    }

    function initStatus()
    {
        nameBtn.infoText = historyObject.getHistoryStrValue(gid, "Name") === "Prelisten" ? langStrObject.GetLangString("ETCS_PRELISTEN") : historyObject.getHistoryStrValue(gid, "Name");
        timeBtn.infoText = historyObject.getHistoryStrValue(gid, "Time");
        statusInfoBtn.infoText = mapStatusToStatusName(historyObject.getHistoryStrValue(gid, "Status"));
        cycleInfoBtn.infoText = historyObject.getHistoryStrValue(gid, "Cycle") === "0" ? langStrObject.GetLangString("ETCS_FOREVER") : historyObject.getHistoryIntValue(gid, "Cycle")
        currentCycleInfoBtn.infoText = historyObject.getHistoryIntValue(gid, "Current Cycle");
        //gaptimeInfoBtn.infoText = historyObject.getHistoryStrValue(gid, "Gap Time");
        priorityInfoBtn.infoText = historyObject.getHistoryIntValue(gid, "Priority");
        presignalInfoBtn.infoText = historyObject.getHistoryStrValue(gid, "Pre-signal");
        reconnectInfoBtn.infoText = historyObject.getHistoryBoolValue(gid, "Reconnect") === true ? langStrObject.GetLangString("ETCS_YES") : langStrObject.GetLangString("ETCS_NO")
        partlyConnectInfoBtn.infoText = historyObject.getHistoryBoolValue(gid, "Partly Connect")  === true ? langStrObject.GetLangString("ETCS_YES") : langStrObject.GetLangString("ETCS_NO")
        volumeInfoBtn.infoText = historyObject.getHistoryStrValue(gid, "Volume");
        sourceInfoBtn.infoText = historyObject.getHistoryStrValue(gid, "Source");
        sourceInfoBtn.isClickBtn = historyObject.getHistoryBoolValue(gid, "isLive") === true ? false : true;
        sourceInfoBtn.clickImgSrc =historyObject.getHistoryBoolValue(gid, "isLive") === true ? "": logicObject.getLocalImagePath() + "next_1.png";
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
                btnName: langStrObject.GetLangString("ETCS_NAME")
                infoText: historyObject.getHistoryStrValue(gid, "Name")
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
                infoText: historyObject.getHistoryStrValue(gid, "Time");
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
                infoText: mapStatusToStatusName(historyObject.getHistoryStrValue(gid, "Status"))
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
                infoText: historyObject.getHistoryIntValue(gid, "Cycle") === "0" ? langStrObject.GetLangString("ETCS_FOREVER") : historyObject.getHistoryIntValue(gid, "Cycle")
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
                infoText: historyObject.getHistoryIntValue(gid, "Current Cycle")
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
                infoText: historyObject.getHistoryStrValue(gid, "Gap Time")
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
                infoText: historyObject.getHistoryIntValue(gid, "Priority")
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
                infoText: historyObject.getHistoryStrValue(gid, "Pre-signal")
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
                infoText: historyObject.getHistoryStrValue(gid, "Reconnect") ? langStrObject.GetLangString("ETCS_YES") : langStrObject.GetLangString("ETCS_NO")
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
                infoText: historyObject.getHistoryStrValue(gid, "Partly Connect") ? langStrObject.GetLangString("ETCS_YES") : langStrObject.GetLangString("ETCS_NO")
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
                infoText: historyObject.getHistoryStrValue(gid, "Volume")
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
                infoText: historyObject.getHistoryStrValue(gid, "Source")
                isClickBtn: historyObject.getHistoryStrValue(gid, "Type") === "Live" ? false : true
                clickImgSrc: historyObject.getHistoryStrValue(gid, "Type") === "Live" ? "": logicObject.getLocalImagePath() + "next_1.png"
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
        gType: "History"
        prevTitleNameText: langStrObject.GetLangString("ETCS_DETAILS");
    }

    SourceDetail{
        id: sourceDetail
        width: parent.width
        height: logicObject.getRealHeight()
        visible: false
        z: 2
        gType: "History"
        prevTitleNameText: langStrObject.GetLangString("ETCS_DETAILS");
    }


    Component.onCompleted: {
        rootItem.updateLanguage.connect(updateLanguageSlot);
        zoneInfoBtn.btnValueSignal.connect(changeView);
        sourceInfoBtn.btnValueSignal.connect(changeView);
    }
}
