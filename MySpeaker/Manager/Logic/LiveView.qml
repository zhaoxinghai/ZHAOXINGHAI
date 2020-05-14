import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import "../Components"

Rectangle {

    property int callId:0
    property int livePort: 41
    property string titleBarText: ""

    property int btnHeight: 66
    property int volumeAdjustBarHeight: 60
    property int protocolBarHeight: 100

    property bool haveGong: false
    property string callBtnStatus: dis_status

    property string wait_status: "wait"
    property string full_status: "full"
    property string part_status: "part"
    property string dis_status: "disconnect"

    function updateLanguageSlot()
    {
        title.text = langStrObject.GetLangString("ETCS_LIVE");
        prevTitleName.text = langStrObject.GetLangString("ETCS_SOURCE");
        micName.setText(langStrObject.GetLangString("ETCS_MIC"));
        lineInName.setText(langStrObject.GetLangString("ETCS_LINEIN"));
        propertyBtn.setInfoValue(langStrObject.GetLangString("ETCS_PROPERTY"));
        preListenBtn.textName = langStrObject.GetLangString("ETCS_PRELISTEN");
        callBtn.textName = langStrObject.GetLangString("ETCS_CALL");
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
        setVisibleValue(true);
    }

    function quitStatus()
    {
        var id;
        id = getCallId()
        if  (id > 0)
        {
            pageObject.livePageStopCall(getCallId());
            setCallBtnStatusValue(dis_status);
            upDateCallBtnStatus();
            sourceSwitchRect.inactiveStatus();
            liveViewImageInactiveStatus();
            statusBar.setStatusBarMessageTextForTimer(langStrObject.GetLangString("ETCS_CALL_ERR_7"), "ETCS_CALL_ERR_7");
            zeroCallId();
        }
        setVisibleValue(false);
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function setCallBtnWaitStatus()
    {
        setCallBtnImageSource(logicObject.getLocalImagePath() + "call_B.png");
        setCallBtnTextColor("#5f5f5f");
        setCallBtnText(langStrObject.GetLangString("ETCS_WAIT"));
    }

    function setCallBtnFullStatus()
    {
        setCallBtnImageSource(logicObject.getLocalImagePath() + "call_B.png");
        setCallBtnTextColor("#5f5f5f");
        setCallBtnText(langStrObject.GetLangString("ETCS_FULL_CONNECT"));
    }

    function setCallBtnFullGongStatus()
    {
        setCallBtnImageSource(logicObject.getLocalImagePath() + "call_D.png");
        setCallBtnTextColor("#5f5f5f");
        callBtn.setImgPlayingValue(true);
        setCallBtnText(langStrObject.GetLangString("ETCS_FULL_CONNECT"));
    }

    function setCallBtnPartStatus()
    {
        setCallBtnImageSource(logicObject.getLocalImagePath() + "call_B.png");
        setCallBtnTextColor("#5f5f5f");
        setCallBtnText(langStrObject.GetLangString("ETCS_PARTLY_CONNECT"));
    }

    function setCallBtnPartGongStatus()
    {
        setCallBtnImageSource(logicObject.getLocalImagePath() + "call_D.png");
        setCallBtnTextColor("#5f5f5f");
        callBtn.setImgPlayingValue(true);
        setCallBtnText(langStrObject.GetLangString("ETCS_PARTLY_CONNECT"));
    }

    function setCallBtnDisconnectStatus()
    {
        setCallBtnImageSource(logicObject.getLocalImagePath() + "call_A.png");
        setCallBtnTextColor("#5f5f5f");
        setCallBtnText(langStrObject.GetLangString("ETCS_CALL"));
        callBtn.setButtonStatus(false);
    }

    function setCallBtnImageSource(source)
    {
        callBtn.imageSource = source;
    }

    function setCallBtnTextColor(color)
    {
        callBtn.textColor = color;
    }

    function setCallBtnText(text)
    {
        callBtn.textName = text;
    }

    function setCallId(id)
    {
        callId = id;
    }

    function zeroCallId()
    {
        callId = 0;
    }

    function getCallId()
    {
        return callId;
    }

    function setLivePort(value)
    {
        livePort = value;
    }

    function getLivePort()
    {
        return livePort;
    }

    function startAnimatedImageFromId(id)
    {
        if (id === getCallId())
        {
            liveViewImageActiveStatus();
        }
    }

    function stopAnimatedImageFromId(id)
    {
        if (id === getCallId())
        {
            liveViewImageInactiveStatus();
        }
    }

    function liveViewImageInactiveStatus()
    {
        if (getLivePort() === 41)
        {
            setMicOffAnimatedImage();
        }
        else if (getLivePort() === 42)
        {
            setLineInOffAnimatedImage();
        }
        else
        {
            ;
        }
    }

    function liveViewImageActiveStatus()
    {
        if (getLivePort() === 41)
        {
            setMicOnAnimatedImage();
        }
        else if (getLivePort() === 42)
        {
            setLineInOnAnimatedImage();
        }
        else
        {
            ;
        }
    }

    function setLineInOffAnimatedImage()
    {
        liveViewImage.source = logicObject.getLocalImagePath() + "linein_B.png";
    }
    function setLineInOnAnimatedImage()
    {
        liveViewImage.source = logicObject.getLocalImagePath() + "linein_A.png";
    }
    function setMicOffAnimatedImage()
    {
        liveViewImage.source = logicObject.getLocalImagePath() + "mic_B.png";
    }
    function setMicOnAnimatedImage()
    {
        liveViewImage.source = logicObject.getLocalImagePath() + "mic_A.png";
    }

    function activateUpdateSlot(id, text)
    {
        if  (id === getCallId())
        {
            if (text === dis_status)
            {
                setCallBtnStatusValue(dis_status);
                upDateCallBtnStatus();
                stopAnimatedImageFromId(id);
                zeroCallId();
                sourceSwitchRect.inactiveStatus();
                liveViewImageInactiveStatus();
                setHaveGongValue(false);
            }

            else if (text === full_status)
            {
                setCallBtnStatusValue(full_status);
                upDateCallBtnStatus();
            }
            else if (text === part_status)
            {
                setCallBtnStatusValue(part_status);
                upDateCallBtnStatus();
            }
            else if (text === wait_status)
            {
                setCallBtnStatusValue(wait_status);
                upDateCallBtnStatus();
            }
        }
    }

    function callBtnClickSlot(status)
    {
        if (status === true)
        {
            var errorCode = pageObject.livePageStartCall(callSettingPage.getgPrority(),
                                         getVolumeSliderValue(),
                                         callSettingPage.getgPreSignal(),
                                         callSettingPage.getgReconnect(),
                                         callSettingPage.getgPartlyConnect(),
                                         getLivePort()
                                         );

            var ret = rootItem.dealCallError(errorCode);
            if (ret !== 0)
            {
                setCallBtnStatusValue(dis_status);
                upDateCallBtnStatus();
                zeroCallId();
                setHaveGongValue(false);
                return;
            }

            setCallId(pageObject.getCallId());
            setCallBtnStatusValue(wait_status);
            upDateCallBtnStatus();
            sourceSwitchRect.activeStatus();
        }
        else
        {
            pageObject.livePageStopCall(getCallId());
            setCallBtnStatusValue(dis_status);
            upDateCallBtnStatus();
            sourceSwitchRect.inactiveStatus();
            liveViewImageInactiveStatus();
            setHaveGongValue(false);
        }
        return 0;
    }

    function setGongTrueStatus(id)
    {
        if (id === getCallId())
        {
            setHaveGongValue(true);
            upDateCallBtnStatus();
        }
    }

    function setGongFalseStatus(id)
    {
        if (id === getCallId())
        {         
            setHaveGongValue(false);
            upDateCallBtnStatus();
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

    function setCallBtnStatusValue(value)
    {
        callBtnStatus = value;
    }

    function getCallBtnStatusValue()
    {
        return callBtnStatus;
    }

    function upDateCallBtnStatus()
    {
        if  (getCallId() === 0)
        {
            setCallBtnDisconnectStatus();
            return ;
        }

        if (getCallBtnStatusValue() === wait_status)
        {
            setCallBtnWaitStatus();
        }
        else if (getCallBtnStatusValue() === full_status)
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
        else if (getCallBtnStatusValue() === part_status)
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
        else if (getCallBtnStatusValue() === dis_status)
        {
            setCallBtnDisconnectStatus();
        }
    }

    function upDatePrelistenBtnStatus()
    {

    }

    function getgPrority()
    {
        return callSettingPage.getgPrority();
    }

    function getVolumeSliderValue()
    {
        return volumeSlider.value;
    }

    function getgPreSignal()
    {
        return callSettingPage.getgPreSignal();
    }

    function getgReconnect()
    {
        return callSettingPage.getgReconnect();
    }

    function getgPartlyConnect()
    {
        return callSettingPage.getgPartlyConnect();
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
        anchors.top: titleBar.bottom
        width: parent.width
        height: 1
        color: "#c0c0c0"
        z: 1
    }

    Rectangle{

        function inactiveStatus()
        {
            if (getLivePort() === 41)
            {
                micBtn.selectStatus()
                lineInBtn.unSelectStatus();
            }
            else if (getLivePort() === 42)
            {
                micBtn.unSelectStatus()
                lineInBtn.selectStatus();
            }
            else
            {
                ;
            }
        }

        function activeStatus()
        {
            if (getLivePort() === 41)
            {
                micBtn.selectDisableStatus();
                lineInBtn.unselecDisableStatus();
            }
            else if (getLivePort() === 42)
            {
                micBtn.unselecDisableStatus();
                lineInBtn.selectDisableStatus();
            }
            else
            {
                ;
            }
        }

        id: sourceSwitchRect
        anchors.top: titleBar.bottom
        anchors.topMargin: 40
        anchors.horizontalCenter: parent.horizontalCenter
        width: 350
        height: 41
        border.width: 1
        border.color: "#808080"
        clip: true
        RectButton4{

            function selectStatus()
            {
                micBtn.setColor("#1792e5");
                micName.setTextColor("#ffffff");
                micBtn.setEnabled(true);
            }

            function unSelectStatus()
            {
                micBtn.setColor("#ffffff");
                micName.setTextColor("#303030");
                micBtn.setEnabled(true);
            }

            function selectDisableStatus()
            {
                micBtn.setColor("#b0b0b0");
                micName.setTextColor("#303030");
                micBtn.setEnabled(false);
            }

            function unselecDisableStatus()
            {
                micBtn.setColor("#ffffff");
                micName.setTextColor("#303030");
                micBtn.setEnabled(false);
            }

            id: micBtn
            anchors.top: parent.top
            anchors.topMargin: 1
            anchors.left: parent.left
            anchors.leftMargin: 1
            width: parent.width / 2 - 1
            height: parent.height - 2

            CommonText {
                id: micName
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                font.family: rootItem.fontName
                font.pixelSize: 18
                text: langStrObject.GetLangString("ETCS_MIC")
                width: parent.width
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignHCenter
                Component.onCompleted: {

                }
            }

            onClickedSignal: {
                setLivePort(41);
                sourceSwitchRect.inactiveStatus();
                liveViewImageInactiveStatus();
            }
        }

        RectButton4{

            function selectStatus()
            {
                lineInBtn.setColor("#1792e5");
                lineInName.setTextColor("#ffffff");
                lineInBtn.setEnabled(true);
            }

            function unSelectStatus()
            {
                lineInBtn.setColor("#ffffff");
                lineInName.setTextColor("#303030");
                lineInBtn.setEnabled(true);
            }

            function selectDisableStatus()
            {
                lineInBtn.setColor("#b0b0b0");
                lineInName.setTextColor("#303030");
                lineInBtn.setEnabled(false);
            }

            function unselecDisableStatus()
            {
                lineInBtn.setColor("#ffffff");
                lineInName.setTextColor("#303030");
                lineInBtn.setEnabled(false);
            }

            id: lineInBtn
            anchors.top: parent.top
            anchors.topMargin: 1
            anchors.left: micBtn.right
            width: parent.width / 2 - 1
            height: parent.height - 2

            CommonText {
                id: lineInName
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                font.family: rootItem.fontName
                font.pixelSize: 18
                text: langStrObject.GetLangString("ETCS_LINEIN")
                width: parent.width
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignHCenter
                Component.onCompleted: {

                }
            }

            onClickedSignal: {
                setLivePort(42);
                sourceSwitchRect.inactiveStatus();
                liveViewImageInactiveStatus();
            }
        }
    }

    AnimatedImage{
        id: liveViewImage
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: sourceSwitchRect.bottom
        anchors.topMargin: 120
        width: 190
        height: 176
        source: logicObject.getLocalImagePath() + "mic_B.png"
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

            function setValue(data)
            {
                value = data;
            }

            function getValue()
            {
                return value;
            }

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

                pageObject.setLiveVolume(volumeSlider.getValue());
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
                callSettingPage.syncOptions(0,
                                            0,
                                            pageObject.getLiveIntOption("Priority"),
                                            pageObject.getLiveIntOption("Presignal"),
                                            pageObject.getLiveBoolOption("Regain"),
                                            pageObject.getLiveBoolOption("Partial"));
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
            imageSource: logicObject.getLocalImagePath() + "preliten_D.png"
            textName: langStrObject.GetLangString("ETCS_PRELISTEN")
            textColor: "#40666666"
            Component.onCompleted: {
            }
        }
        ImageButtonFrame{
            id: callBtn
            anchors.top: parent.top
            anchors.left: preListenBtn.right
            width: 96
            height: protocolBarHeight
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

    CallSettingPage{
        id : callSettingPage
        anchors.bottom: parent.bottom
        width: parent.width
        height: parent.height
        z: 5
        visible: false
        onQuitSignal: {
            pageObject.setLiveOption(
                        callSettingPage.getgPrority(),
                        getVolumeSliderValue(),
                        callSettingPage.getgPreSignal(),
                        callSettingPage.getgReconnect(),
                        callSettingPage.getgPartlyConnect(),
                        getLivePort());
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
            predefineSetting.addInitStatus(langStrObject.GetLangString("ETCS_LIVE"))
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

            var errorCode = pageObject.liveMakePredefine(predefineSetting.getName(),
                                        callSettingPage.getgPrority(),
                                         getVolumeSliderValue(),
                                         callSettingPage.getgPreSignal(),
                                         callSettingPage.getgReconnect(),
                                         callSettingPage.getgPartlyConnect(),
                                         getLivePort()
                                         );

            var ret = rootItem.dealCallError(errorCode);
            if (ret !== 0)
            {
                return;
            }

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
            pageObject.liveMakeSchedule(callSettingPage.getgPrority(),
                                        getVolumeSliderValue(),
                                        callSettingPage.getgPreSignal(),
                                        callSettingPage.getgReconnect(),
                                        callSettingPage.getgPartlyConnect(),
                                        getLivePort());
            scheduleSettingView.setVisibleValue(false);
            scheduleSettingView.quitStatus();
        }
    }

    Component.onCompleted: {
        callBtn.buttonClicked.connect(callBtnClickSlot);
        sendBtn.disableStatus();

        logicObject.signalCallStatus.connect(activateUpdateSlot);
        logicObject.micActivateSignal.connect(startAnimatedImageFromId);
        logicObject.micInactiveSignal.connect(stopAnimatedImageFromId);
        logicObject.gongPlayingTrueSignal.connect(setGongTrueStatus);
        logicObject.gongPlayingFalseSignal.connect(setGongFalseStatus);
        rootItem.updateLanguage.connect(updateLanguageSlot);
        rootItem.changeUserSignal.connect(changeUserSlot);

        callSettingPage.syncOptions(0,
                                    0,
                                    pageObject.getLiveIntOption("Priority"),
                                    pageObject.getLiveIntOption("Presignal"),
                                    pageObject.getLiveBoolOption("Regain"),
                                    pageObject.getLiveBoolOption("Partial"));
        volumeSlider.setValue(pageObject.getLiveFloatOption("Volume"));
        setLivePort(41);
        sourceSwitchRect.inactiveStatus();
        liveViewImageInactiveStatus();
    }
}

