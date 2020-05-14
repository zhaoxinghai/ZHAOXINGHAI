import QtQuick 2.0
import QtQml.Models 2.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

import "../Components"

Rectangle {

    property int protocolBarHeight: 100
    property int sourceAddViewHeight: 86
    property int listViewHeight: height - mainPage.g_titleBarHeight - volumeAdjustBarHeight - protocolBarHeight
    property int volumeAdjustBarHeight: 60

    property int callId: 0
    property int prelistenId: 0
    property bool haveGong: false
    property string callStatus: dis_status
    property string prelistenStatus: dis_status

    property string wait_status: "wait"
    property string full_status: "full"
    property string part_status: "part"
    property string dis_status: "disconnect"

    property int g_node;
    property int g_channel;
    property int g_title;

    function updateLanguageSlot()
    {
        title.setText(langStrObject.GetLangString("ETCS_NETWORK_LIVE"));
        prevBtn.setDataText(langStrObject.GetLangString("ETCS_SOURCE"));
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

    function initStatus()
    {
        zeroCallId();
        zeroPrelistenId();
        setVisibleValue(true);
        inputListModel.clearListModel();
        inputListModel.createListModel();
        volumeSlider.setValue(pageObject.getNetworkFloatOption("Volume"));
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
            statusBar.setStatusBarMessageTextForTimer(langStrObject.GetLangString("ETCS_CALL_ERR_6"), "ETCS_CALL_ERR_6");
        }

        id = getPrelistenId();
        if (id !== 0)
        {
            pageObject.baseStopCallFromId("PreListen", getPrelistenId());
            zeroPrelistenId();
            setPrelistenStatusValue(dis_status);
            upDatePrelistenBtnStatus();
            statusBar.setStatusBarMessageTextForTimer(langStrObject.GetLangString("ETCS_CALL_ERR_12"), "ETCS_CALL_ERR_12");
        }

        setNode(0);
        inputListModel.clearListModel();
        lineListModel.clearListModel();
        setVisibleValue(false);
    }

    function setNode(value)
    {
        g_node = value;
    }

    function getNode()
    {
        return g_node;
    }

    function setChannel(value)
    {
        g_channel = value;
    }

    function getChannel()
    {
        return g_channel;
    }

    function setTitle(value)
    {
        g_title = value;
    }

    function getTitle()
    {
        return g_title;
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
                return;
            }

            var errorCode = pageObject.networkPageStartLiveCall("PreListen", getNode(),
                                                        getChannel(),
                                                        getTitle(),
                                                        callSettingPage.getgPrority(),
                                                        volumeSlider.getValue(),
                                                        callSettingPage.getgPreSignal(),
                                                        callSettingPage.getgReconnect(),
                                                        callSettingPage.getgPartlyConnect(),
                                                        0,
                                                        0,
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
            pageObject.networkPageStopCallFromId("PreListen", getPrelistenId());
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
                return;
            }

            var errorCode = pageObject.networkPageStartLiveCall("Call", getNode(),
                                                    getChannel(),
                                                    getTitle(),
                                                    callSettingPage.getgPrority(),
                                                    volumeSlider.getValue(),
                                                    callSettingPage.getgPreSignal(),
                                                    callSettingPage.getgReconnect(),
                                                    callSettingPage.getgPartlyConnect(),
                                                    0,
                                                    0,
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
                pageObject.networkPageStopCallFromId("PreListen", getPrelistenId());
                zeroPrelistenId();
            }

            setPrelistenBtnDisable();
            setCallId(pageObject.getCallId());
            setCallStatusValue(wait_status);
            upDateCallBtnStatus();
        }
        else if(status === false)
        {
            pageObject.networkPageStopCallFromId("Call", getCallId());
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
        if (getChannel() !== 0)
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
            volumeSlider.enableSlider();
            setCallBtnDisconnectStatus();
            return ;
        }

        if (getCallStatusValue() === wait_status)
        {
            volumeSlider.disableSlider();
            setCallBtnWaitStatus();
        }
        else if (getCallStatusValue() === full_status)
        {
            volumeSlider.disableSlider();
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
            volumeSlider.disableSlider();
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
            volumeSlider.enableSlider();
            setCallBtnDisconnectStatus();
        }
    }

    function upDatePrelistenBtnStatus()
    {
        if  (getPrelistenId() === 0)
        {
            volumeSlider.enableSlider();
            setPrelistenBtnDisconnectStatus();
            return ;
        }

        if (getPrelistenStatusValue() === wait_status)
        {
            volumeSlider.disableSlider();
            setPrelistenBtnWaitStatus();
        }
        else if (getPrelistenStatusValue() === full_status)
        {
            volumeSlider.disableSlider();
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
            volumeSlider.disableSlider();
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
            volumeSlider.enableSlider();
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
            text: langStrObject.GetLangString("ETCS_NETWORK_LIVE")
            font.pixelSize: mainPage.g_titleTextFontSize
            width: 200
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            z: 1
            Component.onCompleted: {

            }
        }

        ImageAndTextButton{
            id: prevBtn
            width: imageWidth + dataContentWidth - 8 + 35
            height: parent.height
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

    VerticalLline{
        anchors.bottom: volumeAdjustBar.top
        anchors.left: parent.left
        anchors.leftMargin: 170
        height: inputListView.height
    }

    ListModel {
        function createListModel()
        {
            var dal;
            var count;
            var type;

            type = sourceManageObject.getNodeDeviceType(getNode());
            if (type === 12)
            {
                inputListModel.append({"inputType": "ETCS_INPUT", "dal": 1});
            }
            else
            {
                count = sourceManageObject.getNodeInputCount(getNode());
                if (count <= 0)
                {
                    return;
                }
                for (var i = 0; i < count; i++)
                {
                    dal = sourceManageObject.getNodeInputDal(getNode(), i);
                    inputListModel.append({"inputType": "DAL_INPUT", "dal": dal});
                }
            }
        }

        function clearListModel()
        {
           if (inputListModel.count > 0)
           {
                remove(0, inputListModel.count);
           }
        }

        id: inputListModel
    }

    ListView{
        id: inputListView
        width: 170
        height: listViewHeight
        anchors.top: titleBar.bottom
        anchors.left: parent.left
        clip: true
        boundsBehavior: Flickable.StopAtBounds
        snapMode: ListView.NoSnap
        footer: Rectangle{height: 8}
        model: inputVisualModel
    }

    DelegateModel {
        id: inputVisualModel

        property alias selectedItems: gp
        groups: [
            DelegateModelGroup{
                id: gp
                name: "selected"
            }
        ]

        model: inputListModel

        delegate:Rectangle{
                    id: listViewElement
                    width: 170
                    height: 68
                    ToggleButton3{
                        width: 140
                        height: 60
                        anchors.bottom: parent.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                        dataColor: listViewElement.VisualDataModel.inSelected ? "#00c767" : "#303030"
                        border.width: listViewElement.VisualDataModel.inSelected ? 2 : 1
                        border.color: listViewElement.VisualDataModel.inSelected ? "#00c767" : "#c0c0c0"

                        onSelectSignal: {
                            if (inputVisualModel.selectedItems.count > 0)
                            {
                                inputVisualModel.selectedItems.remove(0, inputVisualModel.selectedItems.count);
                            }

                            listViewElement.VisualDataModel.inSelected = true;
                            lineListModel.clearListModel();
                            lineListModel.createListModel(getNode(), inputType, dal);
                        }
                        Component.onCompleted: {
                            if (index === 0)
                            {
                                listViewElement.VisualDataModel.inSelected = true;
                                lineListModel.clearListModel();
                                lineListModel.createListModel(getNode(), inputType, dal);
                            }
                            else
                            {
                                listViewElement.VisualDataModel.inSelected = false;
                            }

                            if (inputType === "ETCS_INPUT")
                            {
                                setText(langStrObject.GetLangString("ETCS_SOURCE"));
                            }
                            else
                            {
                                setText(sourceManageObject.getNodeInputStrValue(getNode(), dal, "name"));
                            }
                        }
                    }
        }
    }

    ScrollBar{
        id: inputScrollBar
        anchors.top: inputListView.top
        anchors.right: inputListView.right
        anchors.rightMargin: 6
        height: inputListView.height
        position: inputListView.visibleArea.yPosition
        pageSize: inputListView.visibleArea.heightRatio
        scrollBarVisible: inputListView.visibleArea.heightRatio === 1 ? false : true
    }

    ListModel {
        function createListModel(node, inputType, dal)
        {
            if (inputType === "ETCS_INPUT")
            {
                lineListModel.append({"lineType": "other",  "linePort": "micPort", "portNumber": 41});
                lineListModel.append({"lineType": "other", "linePort": "lineinPort", "portNumber": 42});
            }
            else
            {
                var dalType;
                var micPort;
                var lineinPort;
                var count;

                dalType = sourceManageObject.getNodeInputStrValue(node, dal, "type");
                micPort = sourceManageObject.getNodeInputIntValue(node, dal, "micPort");
                lineinPort = sourceManageObject.getNodeInputIntValue(node, dal, "lineinPort");

                count = sourceManageObject.getNodeCount(node);
                if (count <= 0)
                {
                    return;
                }

                if (dalType === "UIM")
                {
                    lineListModel.append({"lineType": "UIM", "linePort": "micPort", "portNumber": micPort});
                    lineListModel.append({"lineType": "UIM", "linePort": "lineinPort","portNumber": lineinPort});
                }
                else
                {
                    if (micPort !== -1)
                    {
                        lineListModel.append({"lineType": "other",  "linePort": "micPort", "portNumber": micPort});
                    }
                    if (lineinPort !== -1)
                    {
                        lineListModel.append({"lineType": "other", "linePort": "lineinPort", "portNumber": lineinPort});
                    }
                }
            }
        }

        function clearListModel()
        {
           if (lineListModel.count > 0)
           {
                remove(0, lineListModel.count);
           }
        }

        id: lineListModel
    }

    ListView{
        id: lineListView
        width: 310
        height: listViewHeight
        anchors.top: inputListView.top
        anchors.left: inputListView.right
        clip: true
        boundsBehavior: Flickable.StopAtBounds
        snapMode: ListView.NoSnap
        footer: Rectangle{height: 8}
        model: lineVisualModel
    }

    DelegateModel {
        id: lineVisualModel

        property alias selectedItems: gp2
        groups: [
            DelegateModelGroup{
                id: gp2
                name: "selected"
            }
        ]

        model: lineListModel

        delegate:Rectangle{
                id: listViewElement2
                width: 310
                height: 68

                ToggleButton3{
                    width: 280
                    height: 60
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    dataColor: listViewElement2.VisualDataModel.inSelected ? "#00c767" : "#303030"
                    border.width: listViewElement2.VisualDataModel.inSelected ? 2 : 1
                    border.color: listViewElement2.VisualDataModel.inSelected ? "#00c767" : "#c0c0c0"

                    onSelectSignal: {
                        if (lineVisualModel.selectedItems.count > 0)
                        {
                            lineVisualModel.selectedItems.remove(0, lineVisualModel.selectedItems.count);
                        }

                        listViewElement2.VisualDataModel.inSelected = true;
                        setChannel(portNumber);
                        setTitle(1);
                    }

                    Component.onCompleted: {

                        if (index === 0)
                        {
                            listViewElement2.VisualDataModel.inSelected = true;
                            setChannel(portNumber);
                            setTitle(1);
                        }
                        else
                        {
                            listViewElement2.VisualDataModel.inSelected = false;
                        }

                        if (lineType === "UIM" && linePort === "micPort")
                        {
                            setText(langStrObject.GetLangString("ETCS_LINEIN") + 1);
                        }
                        else if (lineType === "UIM" && linePort === "lineinPort")
                        {
                            setText(langStrObject.GetLangString("ETCS_LINEIN") + 2);
                        }

                        if (lineType === "other" && linePort === "micPort")
                        {
                            setText(langStrObject.GetLangString("ETCS_MIC"));
                        }
                        else if (lineType === "other" && linePort === "lineinPort")
                        {
                            setText(langStrObject.GetLangString("ETCS_LINEIN"));
                        }
                    }
                }
        }
    }

    ScrollBar{
        id: lineScrollBar
        anchors.top: lineListView.top
        anchors.right: lineListView.right
        anchors.rightMargin: 6
        height: lineListView.height
        position: lineListView.visibleArea.yPosition
        pageSize: lineListView.visibleArea.heightRatio
        scrollBarVisible: lineListView.visibleArea.heightRatio === 1 ? false : true
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
            value: 0

            CommonImage{
                id: speakerImage
                anchors.verticalCenter : parent.verticalCenter
                anchors.right: parent.left
                anchors.rightMargin: 10
                source: volumeSlider.value == volumeSlider.minimumValue ? logicObject.getLocalImagePath() + "volume_1_A.png" : logicObject.getLocalImagePath() + "volume_1_B.png"
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
            onValueChanged: {
                    if (getCallId() !== 0)
                    {
                        logicObject.VolumeAjust(getCallId(), volumeSlider.value);
                    }
                    if (getPrelistenId() !== 0)
                    {
                        logicObject.VolumeAjust(getPrelistenId(), volumeSlider.value);
                    }

                    pageObject.setNetworkVolume(volumeSlider.getValue());
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
                callSettingPage.syncOptions(pageObject.getNetworkIntOption("Cycle"),
                                            pageObject.getNetworkIntOption("Gap Time"),
                                            pageObject.getNetworkIntOption("Priority"),
                                            pageObject.getNetworkIntOption("Presignal"),
                                            pageObject.getNetworkBoolOption("Regain"),
                                            pageObject.getNetworkBoolOption("Partial"));
                callSettingPage.initLiveCallStatus();
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

                    var errorCode = pageObject.networkPageStartLiveCall("Call", getNode(),
                                                            getChannel(),
                                                            getTitle(),
                                                            callSettingPage.getgPrority(),
                                                            volumeSlider.getValue(),
                                                            callSettingPage.getgPreSignal(),
                                                            callSettingPage.getgReconnect(),
                                                            callSettingPage.getgPartlyConnect(),
                                                            0,
                                                            0,
                                                            callSettingPage.getgTimeType()
                                                            );

                    ret = rootItem.dealCallError(errorCode);
                    if (ret !== 0)
                    {
                        return;
                    }

                    if (getPrelistenId() !== 0)
                    {
                        pageObject.networkPageStopCallFromId("PreListen", getPrelistenId());
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
            pageObject.setNetworkOption(getNode(),
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
            predefineSetting.addInitStatus(langStrObject.GetLangString("ETCS_NETWORK_LIVE"))
        }
        onSendScheduleBtnSingal: {
            moreFuncView.setVisibleValue(false);
            scheduleSettingView.addLiveInitStatus(title.getText());

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

            pageObject.networkLiveMakePredefine(getNode(),
                                                getChannel(),
                                                getTitle(),
                                            predefineSetting.getName(),
                                            callSettingPage.getgPrority(),
                                            volumeSlider.getValue(),
                                            callSettingPage.getgPreSignal(),
                                            callSettingPage.getgReconnect(),
                                            callSettingPage.getgPartlyConnect(),
                                            0,
                                            0,
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
            pageObject.networkLiveMakeSchedule(getNode(),
                                               getChannel(),
                                               getTitle(),
                                        callSettingPage.getgPrority(),
                                        volumeSlider.getValue(),
                                        callSettingPage.getgPreSignal(),
                                        callSettingPage.getgReconnect(),
                                        callSettingPage.getgPartlyConnect(),
                                        0,
                                        0,
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
        rootItem.updateLanguage.connect(updateLanguageSlot);
        rootItem.changeUserSignal.connect(changeUserSlot);
        topMessage.messageHideSignal.connect(sendBtn.topMessageHideSlot);

        callSettingPage.syncOptions(pageObject.getNetworkIntOption("Cycle"),
                                    pageObject.getNetworkIntOption("Gap Time"),
                                    pageObject.getNetworkIntOption("Priority"),
                                    pageObject.getNetworkIntOption("Presignal"),
                                    pageObject.getNetworkBoolOption("Regain"),
                                    pageObject.getNetworkBoolOption("Partial"));
        volumeSlider.setValue(pageObject.getNetworkFloatOption("Volume"));
    }
}
