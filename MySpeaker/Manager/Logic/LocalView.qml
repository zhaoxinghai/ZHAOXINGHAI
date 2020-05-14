import QtQuick 2.0
import QtQml.Models 2.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

import "../Components"

Rectangle {

    property int protocolBarHeight: 100
    property int sourceAddViewHeight: 86
    property int listViewHeight: height - mainPage.g_titleBarHeight - sourceAddViewHeight - colorRect.height - volumeAdjustBarHeight - protocolBarHeight
    property int volumeAdjustBarHeight: 60

    property int groupIndex: 0
    property int callId: 0
    property int prelistenId: 0
    property bool haveGong: false
    property string callStatus: dis_status
    property string prelistenStatus: dis_status

    property string wait_status: "wait"
    property string full_status: "full"
    property string part_status: "part"
    property string dis_status: "disconnect"

    function updateLanguageSlot()
    {
        title.setText(langStrObject.GetLangString("ETCS_LOCAL"));
        prevBtn.setDataText(langStrObject.GetLangString("ETCS_SOURCE"));
        propertyBtn.setInfoValue(langStrObject.GetLangString("ETCS_PROPERTY"));
        preListenBtn.setBtnText(langStrObject.GetLangString("ETCS_PRELISTEN"));
        callBtn.setBtnText(langStrObject.GetLangString("ETCS_CALL"));
        sendBtnName.setText(langStrObject.GetLangString("ETCS_SEND"))
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

    function initStatus()
    {
        zeroCallId();
        zeroPrelistenId();
        iomapListModel.clearListModel();
        nodeListModel.clearListModel();
        nodeListModel.createListModel();
        sources.clearText();
        sourceManageObject.clearLocalFragment();
        setVisibleValue(true);
        sourceDelBtn.setVisibleValue(false);
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
            statusBar.setStatusBarMessageTextForTimer(langStrObject.GetLangString("ETCS_CALL_ERR_5"), "ETCS_CALL_ERR_5");
        }

        id = getPrelistenId();
        if (id !== 0)
        {
            pageObject.baseStopCallFromId("PreListen", getPrelistenId());
            zeroPrelistenId();
            setPrelistenStatusValue(dis_status);
            upDatePrelistenBtnStatus();
            statusBar.setStatusBarMessageTextForTimer(langStrObject.GetLangString("ETCS_CALL_ERR_11"), "ETCS_CALL_ERR_11");
        }

        setVisibleValue(false);
    }

    function setVisibleValue(value)
    {
        visible = value;
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

            var errorCode = pageObject.baseStartCall("Local", "PreListen", callSettingPage.getgPrority(),
                                                        volumeSlider.getValue(),
                                                        callSettingPage.getgPreSignal(),
                                                        callSettingPage.getgEndSignal(),
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

            var errorCode = pageObject.baseStartCall("Local", "Call", callSettingPage.getgPrority(),
                                                    volumeSlider.getValue(),
                                                    callSettingPage.getgPreSignal(),
                                                    callSettingPage.getgEndSignal(),
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

    function activateDelSlot(id, text)
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
        if (sources.getText() !== "")
        {
            messageBarHide();
            return 0;
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

    function setGongFalseStatus()
    {
        var id;
        id = getCallId();
        if (id > 0)
        {
            setHaveGongValue(false);
            upDateCallBtnStatus();
        }

        id = getPrelistenId();
        if (id > 0)
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

        CommonText{
            id: title
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: mainPage.g_titleTextColor
            text: langStrObject.GetLangString("ETCS_LOCAL");
            font.pixelSize: mainPage.g_titleTextFontSize
            width: 200
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            Component.onCompleted: {
            }
        }

        ImageAndTextButton{
            id: prevBtn
            width: imageWidth + dataContentWidth - 8 + 35
            height: 60
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            imgLeftMargin: 14
            dataLeftMargin: -8
            fontSize: mainPage.g_titleTextFontSize2
            imageSource: logicObject.getLocalImagePath() + "back.png"
            dataText: langStrObject.GetLangString("ETCS_SOURCE")
            dataWidth: 240 - (title.contentWidth/2) - imageWidth - 14 + 8
            onClickedSignal: {
                quitStatus();
            }
            Component.onCompleted: {
            }
        }
    }

    HorizontalLine{
        anchors.top: titleBar.bottom
    }

    Flickable{
        id: sourceAddView
        anchors.top: titleBar.bottom
        anchors.left: parent.left
        anchors.leftMargin: 15
        width: 380
        height: sourceAddViewHeight
        contentWidth: width
        contentHeight: sources.contentHeight
        flickableDirection: Flickable.VerticalFlick
        boundsBehavior: Flickable.StopAtBounds
        clip: true

        CommonText2{
            id: sources
            width: parent.width
            height: parent.height
            anchors.top: parent.top
            anchors.left: parent.left

            onTextChanged: {
                if (sourceAddView.contentHeight - sourceAddView.height > 0)
                {
                    sourceAddView.contentY = sourceAddView.contentHeight - sourceAddView.height;
                }
            }
        }
    }

    ImageButton{
        id: sourceDelBtn
        width: 60
        height: 60
        anchors.top: sourceAddView.top
        anchors.right: parent.right
        buttonImageWidth: 28
        buttonImageHeight: 27
        buttonImageSource: logicObject.getLocalImagePath() + "delete_4.png"
        visible: sourceManageObject.getLocalFragmentSize() === 0 ? false : true
        onClickedSignal: {
            sources.clearText();
            sourceManageObject.clearLocalFragment();
            setVisibleValue(false);
        }
    }

    HorizontalLine{
        anchors.top: sourceAddView.bottom
    }

    HorizontalLine{
        anchors.top: nodeListView.top
    }

    VerticalLline{
        anchors.bottom: volumeAdjustBar.top
        anchors.left: parent.left
        anchors.leftMargin: 170
        height: nodeListView.height
    }

    ListModel {
        function createListModel()
        {
            var count = sourceManageObject.getLocalGroupCount();
            for (var i = 0; i < count; i++)
            {
                nodeListModel.append({"nIndex": i});
            }
            if (count > 0)
            {
                nodeListView.positionViewAtBeginning();
                nodeListView.currentIndex = 0;
            }
        }

        function clearListModel()
        {
           if (nodeListModel.count > 0)
           {
                remove(0, nodeListModel.count);
           }
        }

        id: nodeListModel
    }

    Rectangle{
        id: colorRect
        anchors.top: sourceAddView.bottom
        anchors.left: parent.left
        width: 480
        height: 15
        color: "#e7e7e7"
    }

    ListView{
        id: nodeListView
        width: 170
        height: listViewHeight
        anchors.bottom: volumeAdjustBar.top
        anchors.left: parent.left
        clip: true
        boundsBehavior: Flickable.StopAtBounds
        snapMode: ListView.NoSnap
        footer: Rectangle{height: 8}
        model: nodeVisualModel
    }

    DelegateModel {

        id: nodeVisualModel

        property alias selectedItems: gp
        groups: [
            DelegateModelGroup{
                id: gp
                name: "selected"
            }
        ]

        model: nodeListModel

        delegate:Rectangle{
                    id: listViewElement
                    width: 170
                    height: 68
                    ToggleButton3{
                        width: 140
                        height: 60
                        anchors.bottom: parent.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                        dataText: sourceManageObject.getLocalGroupName(nIndex) === "Record" ? langStrObject.GetLangString("ETCS_RECORD") : sourceManageObject.getLocalGroupName(nIndex)
                        dataColor: listViewElement.VisualDataModel.inSelected ? "#00c767" : "#303030"
                        border.width: listViewElement.VisualDataModel.inSelected ? 2 : 1
                        border.color: listViewElement.VisualDataModel.inSelected ? "#00c767" : "#c0c0c0"

                        onSelectSignal: {
                            if (nodeVisualModel.selectedItems.count > 0)
                            {
                                nodeVisualModel.selectedItems.remove(0, nodeVisualModel.selectedItems.count);
                            }
                            listViewElement.VisualDataModel.inSelected = true;
                            groupIndex = nIndex;
                            iomapListModel.clearListModel();
                            iomapListModel.createListModel(groupIndex);
                        }
                    }

                    Component.onCompleted: {
                        if (nodeVisualModel.selectedItems.count > 0)
                        {
                            return;
                        }

                        if (index === 0)
                        {
                            listViewElement.VisualDataModel.inSelected = true;

                            groupIndex = index;
                            iomapListModel.clearListModel();
                            iomapListModel.createListModel(groupIndex);
                        }
                    }
        }
    }

    ScrollBar{
        id: nodeScrollBar
        anchors.top: nodeListView.top
        anchors.right: nodeListView.right
        anchors.rightMargin: 6
        height: nodeListView.height
        position: nodeListView.visibleArea.yPosition
        pageSize: nodeListView.visibleArea.heightRatio
        scrollBarVisible: nodeListView.visibleArea.heightRatio == 1 ? false : true
    }

    ListModel {
        function createListModel(index)
        {
            var count = sourceManageObject.getLocalIOMapCount(index);
            for (var i = 0; i < count; i++)
            {
                iomapListModel.append({"nIndex": i});
            }
        }

        function clearListModel()
        {
           if (iomapListModel.count > 0)
           {
                remove(0, iomapListModel.count);
           }
        }

        id: iomapListModel
    }

    ListView{
        id: iomapListView
        width: 310
        height: listViewHeight
        anchors.top: nodeListView.top
        anchors.left: nodeListView.right
        clip: true
        boundsBehavior: Flickable.StopAtBounds
        snapMode: ListView.NoSnap
        footer: Rectangle{height: 8}
        model: iomapVisualModel
    }

    DelegateModel {
        id: iomapVisualModel

        property alias selectedItems: gp2
        groups: [
            DelegateModelGroup{
                id: gp2
                name: "selected"
            }
        ]

        model: iomapListModel

        delegate:Rectangle{
                id: listViewElement2
                width: 310
                height: 68
                TextButon{
                    width: 280
                    height: 60
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    info: sourceManageObject.getLocalGroupValue(groupIndex, nIndex, "Desc")
                    textColor: "#303030"
                    border.color: "#c0c0c0"
                    bkgPressedColor: "#dbe0e6"
                    textWidth: 210
                    onClickedSignal: {
                        var errorNum;
                        errorNum = sourceManageObject.getLocalGroupIntValue(groupIndex, nIndex, "errorNum");
                        if (errorNum === 0)
                        {
                            if (iomapVisualModel.selectedItems.count > 0)
                            {
                                iomapVisualModel.selectedItems.remove(0, iomapVisualModel.selectedItems.count);
                            }

                            listViewElement2.VisualDataModel.inSelected = true;
                            if (sourceManageObject.isOverLocalSourceMaxCount() === true)
                            {
                                promptMask.messageShow(langStrObject.GetLangString("ETCS_SOURCE_FULL"));
                                timeOutTimer.setInterval(3000);
                                timeOutTimer.start();
                                return;
                            }

                            sources.insertText("\"" + info + "\"" + " ");
                            sourceManageObject.addLocalFragment(groupIndex, nIndex);
                            sourceDelBtn.setVisibleValue(true);
                        }
                        else if(errorNum === 4)
                        {        
                            localErrorMask.initStatus(langStrObject.GetLangString("ETCS_FILE_NOT_EXIST_ERROR"));
                        }
                        else if (errorNum === 5)
                        {
                            localErrorMask.initStatus(langStrObject.GetLangString("ETCS_FILE_FORMAT_ERROR"));
                        }
                    }

                    CommonImage{
                        id: localErrorImage
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 5
                        source: logicObject.getLocalImagePath() + "error.png"
                        visible:  sourceManageObject.getLocalGroupIntValue(groupIndex, nIndex, "errorNum") !== 0 ? true : false
                    }
                }
        }
    }

    ScrollBar{
        id: iomapScrollBar
        anchors.top: iomapListView.top
        anchors.right: iomapListView.right
        anchors.rightMargin: 6
        height: iomapListView.height
        position: iomapListView.visibleArea.yPosition
        pageSize: iomapListView.visibleArea.heightRatio
        scrollBarVisible: iomapListView.visibleArea.heightRatio == 1 ? false : true
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

    HorizontalLine{
        z: 1
        anchors.top: volumeAdjustBar.top
    }


    Rectangle{
        id: volumeAdjustBar
        width: parent.width
        height: volumeAdjustBarHeight
        anchors.bottom: protocolBar.top
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#f5f5f5"
        CommonSlider{
            id: volumeSlider
            width: 350
            height: parent.height
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

                pageObject.setLocalVolume("Local",volumeSlider.getValue());
            }
            CommonImage{
                id: speakerImage
                anchors.verticalCenter : parent.verticalCenter
                anchors.right: parent.left
                anchors.rightMargin: 10
                source: volumeSlider.value == volumeSlider.minimumValue ?
                     logicObject.getLocalImagePath() + "volume_1_A.png"
                     : logicObject.getLocalImagePath() + "volume_1_B.png"
            }
            CommonText{
                id: volumeText
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.right
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
                callSettingPage.syncOptions(pageObject.getLocalIntOption("Local", "Cycle"),
                            pageObject.getLocalIntOption("Local", "Gap Time"),
                            pageObject.getLocalIntOption("Local", "Priority"),
                            pageObject.getLocalIntOption("Local", "Presignal"),
                            pageObject.getLocalBoolOption("Local", "Regain"),
                            pageObject.getLocalBoolOption("Local", "Partial"));
                callSettingPage.initCallStatus();
            }
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
            imageSource: logicObject.getLocalImagePath() + "call_A.png"
            textName: langStrObject.GetLangString("ETCS_CALL")
            Component.onCompleted: {
            }
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

                    var errorCode = pageObject.baseStartCall("Local", "Call", callSettingPage.getgPrority(),
                                                            volumeSlider.getValue(),
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
            Component.onCompleted: {
            }
        }
    }

    CallSettingPage{
        id : callSettingPage
        anchors.bottom: parent.bottom
        width: parent.width
        height: parent.height
        z: 5
        visible: false

        onQuitSignal: {
            pageObject.setLocalOption("Local",
                                      callSettingPage.getgPrority(),
                                      volumeSlider.getValue(),
                                     callSettingPage.getgPreSignal(),
                                     callSettingPage.getgReconnect(),
                                     callSettingPage.getgPartlyConnect(),
                                     callSettingPage.getgCycle(),
                                     callSettingPage.getgGapTime());
        }
    }

    MoreFuncView{
        id: moreFuncView
        anchors.bottom: parent.bottom
        width: parent.width
        height: parent.height
        z: 5
        visible: false
        onSendPredefineBtnSingal: {
            moreFuncView.setVisibleValue(false);
            predefineSetting.addInitStatus(langStrObject.GetLangString("ETCS_LOCAL"));
        }
        onSendScheduleBtnSingal: {
            moreFuncView.setVisibleValue(false);
            scheduleSettingView.addInitStatus(callSettingPage.getgCycle(),
                                              callSettingPage.getgGapTime(),
                                              title.getText());
        }
    }

    PredefineSetting{
        id: predefineSetting
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width
        height: parent.height
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

            pageObject.baseMakePredefine("Local",
                                         predefineSetting.getName(),
                                         callSettingPage.getgPrority(),
                                            volumeSlider.getValue(),
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
            pageObject.baseMakeSchedule("Local", callSettingPage.getgPrority(),
                                        volumeSlider.getValue(),
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

        logicObject.signalCallStatus.connect(activateDelSlot);
        logicObject.gongPlayingTrueSignal.connect(setGongTrueStatus);
        logicObject.gongPlayingFalseSignal.connect(setGongFalseStatus);
        rootItem.updateLanguage.connect(updateLanguageSlot);
        rootItem.changeUserSignal.connect(changeUserSlot);
        topMessage.messageHideSignal.connect(sendBtn.topMessageHideSlot);

        callSettingPage.syncOptions(pageObject.getLocalIntOption("Local", "Cycle"),
                    pageObject.getLocalIntOption("Local", "Gap Time"),
                    pageObject.getLocalIntOption("Local", "Priority"),
                    pageObject.getLocalIntOption("Local", "Presignal"),
                    pageObject.getLocalBoolOption("Local", "Regain"),
                    pageObject.getLocalBoolOption("Local", "Partial"));
        volumeSlider.setValue(pageObject.getLocalFloatOption("Local", "Volume"));
    }
}
