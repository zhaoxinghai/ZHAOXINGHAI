import QtQuick 2.0
import "../Components"

Rectangle {

    property int gCycle: 0
    property int gMin: 0
    property int gSec: 0
    property int gGapTime: 0
    property bool gTimeType: true
    property int gPrority: 0
    property int gPreSignal: 0
    property int gEndSignal: 0
    property int gReconnect: 0
    property int gPartlyConnect: 0

    property int maskItemHeight: height - childAreaHeight
    property int childAreaHeight: 360

    signal quitSignal();

    function updateLanguageSlot()
    {
        title.setText(langStrObject.GetLangString("ETCS_CALL_PARA"));
        cycleText.setText(langStrObject.GetLangString("ETCS_CYCLE"));
        cycleData.setText(getgCycle() === 0 ? langStrObject.GetLangString("ETCS_FOREVER") : getgCycle());
        gapTimeText.setText(langStrObject.GetLangString("ETCS_GAPTIME"));
        priorityText.setText(langStrObject.GetLangString("ETCS_PRIORITY"));
        preSignalText.setText(langStrObject.GetLangString("ETCS_PRESIGNAL"));
        reconnectText.setText(langStrObject.GetLangString("ETCS_RECONNECT"))
        partlyConnectText.setText(langStrObject.GetLangString("ETCS_PARTLY_CONNECT"))
    }

    function syncOptions(cycle, gapTime, priority, presignal, regain, partial)
    {
        setgCycle(cycle);
        setGGapTime(gapTime);
        setGMin(gapTime / 60);
        setGSec(gapTime % 60);
        setgPrority(priority);
        setgPreSignal(presignal);
        setgReconnect(regain);
        setgPartlyConnect(partial);
    }

    function initAllText()
    {
        cycleData.setText(getgCycle() === 0 ? langStrObject.GetLangString("ETCS_FOREVER") : getgCycle());
        //gapTimeData.setText(logicObject.getMinAndSec(getgMin(), getgSec()));
        priorityData.setText(getgPrority());
        preSignalData.setText(getgPreSignal() === 0 ? "--" : getgPreSignal());
        reconnectSwitch.setBtnClickedValue(getgReconnect());
        reconnectSwitch.updateSwitchStatus();
        partlyConnectSwitch.setBtnClickedValue(getgPartlyConnect());
        partlyConnectSwitch.updateSwitchStatus();
    }

    function initCallStatus()
    {
        setVisibleValue(true);
        cycleBtn.setVisibleValue(true);
        //gapTimeBtn.setVisibleValue(false);
        priorityBtn.setVisibleValue(true);
        preSignalBtn.setVisibleValue(true);
        reconnectBtn.setVisibleValue(true);
        partlyConnectBtn.setVisibleValue(true);
        initAllText();

        /*
        if (getgCycle() === 1)
        {
           gapTimeBtn.disableStatus();
        }
        else
        {
            gapTimeBtn.enableStatus();
        }
        */
    }

    function initLiveCallStatus()
    {
        setVisibleValue(true);
        cycleBtn.setVisibleValue(false);
        //gapTimeBtn.setVisibleValue(false);
        priorityBtn.setVisibleValue(true);
        priorityBtn.setAnchorsTop();
        preSignalBtn.setVisibleValue(true);
        reconnectBtn.setVisibleValue(true);
        partlyConnectBtn.setVisibleValue(true);
        partlyConnectBtn.setLine2VisibleValue(true);
        initAllText();

        /*
        if (getgCycle() === 1)
        {
            gapTimeBtn.disableStatus();
        }
        else
        {
            gapTimeBtn.enableStatus();
        }
        */
    }

    function setgCycle(value)
    {
        gCycle = value;
    }

    function getgCycle()
    {
        return gCycle;
    }

    function setGMin(min)
    {
        gMin = min;
    }

    function setGSec(sec)
    {
        gSec = sec;
    }

    function setGGapTime(gapTime)
    {
        gGapTime = gapTime;
    }

    function setGTimeType(type)
    {
        gTimeType = type;
    }

    function setgTimeType(type)
    {
        gTimeType = type;
    }

    function getgMin()
    {
        return gMin;
    }

    function getgSec()
    {
        return gSec;
    }

    function getgGapTime()
    {
        return gGapTime;
    }

    function getgTimeType()
    {
        return gTimeType;
    }

    function setgPrority(value)
    {
        gPrority = value;
    }

    function getgPrority()
    {
        return gPrority;
    }

    function setgPreSignal(value)
    {
        gPreSignal = value;
    }

    function getgPreSignal()
    {
        return gPreSignal;
    }
    function getgEndSignal()
    {
        return gEndSignal;
    }

    function setgReconnect(value)
    {
        gReconnect = value;
    }

    function getgReconnect()
    {
        return gReconnect;
    }

    function setgPartlyConnect(value)
    {
        gPartlyConnect = value;
    }

    function getgPartlyConnect()
    {
        return gPartlyConnect;
    }

    function setView(index)
    {
        if (index === "Cycle")
        {
            maskItem.setVisibleValue(true);
            cycleView.initStatus(getgCycle());
            gapTimeView.setVisibleValue(false);
            priorityView.setVisibleValue(false);
            preSignalView.setVisibleValue(false);
        }
        else if (index === "GapTime")
        {  
            maskItem.setVisibleValue(true);
            cycleView.setVisibleValue(false);
            gapTimeView.initStatus(getgMin(), getgSec(), getgTimeType());
            priorityView.setVisibleValue(false);
            preSignalView.setVisibleValue(false);
        }
        else if (index === "Priority")
        {
            maskItem.setVisibleValue(true);
            cycleView.setVisibleValue(false);
            gapTimeView.setVisibleValue(false);
            priorityView.initStatus(getgPrority());
            preSignalView.setVisibleValue(false);
        }
        else if (index === "PreSignal")
        {
            maskItem.setVisibleValue(true);
            cycleView.setVisibleValue(false);
            gapTimeView.setVisibleValue(false);
            priorityView.setVisibleValue(false);
            preSignalView.initStatus(getgPreSignal());
        }
    }

    function setViewHide()
    {
        cycleBtn.setVisibleValue(false);
        //gapTimeBtn.setVisibleValue(false);
        prorityBtn.setVisibleValue(false);
        preSignalBtn.setVisibleValue(false);
        setVisibleValue(false);
        updateDataSignal();
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function quitStatus()
    {
        setVisibleValue(false);
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

        CommonText{
            id: title
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: mainPage.g_titleTextColor
            text: langStrObject.GetLangString("ETCS_CALL_PARA")
            font.family: rootItem.fontName
            font.pixelSize: mainPage.g_titleTextFontSize
            width: (240 - prevBtn.width) * 2
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            Component.onCompleted: {
            }
        }
        ImageButton{
            id: prevBtn
            width: parent.height
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            buttonImageSource: logicObject.getLocalImagePath() + "back.png"
            visible: true
            bkgPressedColor: "#505050"
            onClickedSignal: {
                quitStatus();
                quitSignal();
            }
        }
    }


    StripFrame{
        id: cycleBtn
        anchors.top: titleBar.bottom
        anchors.topMargin: 25
        line1Visible: false
        line2Visible: true
        bkgPressedColor: "#dbe0e6"
        bkgReleasedColor: "#ffffff"
        CommonImage{
            id: cycleImg
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 16
            source: logicObject.getLocalImagePath() + "cycle.png"
        }
        CommonText{
            id: cycleText
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: cycleImg.right
            anchors.leftMargin: 15
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_CYCLE")
            width: parent.width - (cycleImg.width + 16) - (cycleBtnEnableImg.width + 20) - (cycleData.contentWidth + 5)
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
            Component.onCompleted: {
            }
        }
        CommonImage{
            id: cycleBtnEnableImg
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 20
            source: logicObject.getLocalImagePath() + "next_1.png"
        }
        CommonText{
            id: cycleData
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: cycleBtnEnableImg.left
            anchors.rightMargin: 5
            color: "#858585"
            text: getgCycle() === 0 ? langStrObject.GetLangString("ETCS_FOREVER") : getgCycle()
            width: 100
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignRight
            Component.onCompleted: {
            }
        }
        onClickedSignal: {
            setView("Cycle");
        }
    }
/*
    StripFrame{

        function enableStatus()
        {
            gapTimeBtn.setEnabledValue(true);
            gapTimeBtnEnableImg.setVisibleValue(true);
        }

        function disableStatus()
        {
            gapTimeBtn.setEnabledValue(false);
            gapTimeBtnEnableImg.setVisibleValue(false);
        }

        id: gapTimeBtn
        visible: false
        anchors.top: cycleBtn.bottom
        line1Visible: false
        line2Visible: true
        bkgPressedColor: "#dbe0e6"
        bkgReleasedColor: "#ffffff"
        CommonImage{
            id: gapTimeImg
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 16
            width: 36
            height: 35
            source: logicObject.getLocalImagePath() + "gaptime.png"
        }
        CommonText{
            id: gapTimeText
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: gapTimeImg.right
            anchors.leftMargin: 15
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_GAPTIME");
            width:  parent.width - (gapTimeImg.width + 16)- (gapTimeBtnEnableImg.width + 20) - (gapTimeData.contentWidth + 5)
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
            Component.onCompleted: {
            }
        }
        CommonImage{
            id: gapTimeBtnEnableImg
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 20
            source: logicObject.getLocalImagePath() + "next_1.png"
        }
        CommonText{
            id: gapTimeData
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: gapTimeBtnEnableImg.left
            anchors.rightMargin: 5
            color: "#858585"
            text: logicObject.getMinAndSec(getgMin(), getgSec())
            width: 80
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignRight
        }
        onClickedSignal: {
            setView("GapTime");
        }
    }
*/
    StripFrame{

        function setAnchorsTop()
        {
            anchors.top = titleBar.bottom
            anchors.topMargin = 25;
        }

        id: priorityBtn
        anchors.top: cycleBtn.bottom
        line1Visible: false
        line2Visible: true
        bkgPressedColor: "#dbe0e6"
        bkgReleasedColor: "#ffffff"
        CommonImage{
            id: priorityImg
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 16
            source: logicObject.getLocalImagePath() + "priority.png"
        }
        CommonText{
            id: priorityText
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: priorityImg.right
            anchors.leftMargin: 15
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_PRIORITY")
            width:  parent.width - (priorityImg.width + 16) - (priorityBtnEnableImg.width + 20) - (priorityData.contentWidth + 10)
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
            Component.onCompleted: {
            }
        }
        CommonImage{
            id: priorityBtnEnableImg
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 20
            source: logicObject.getLocalImagePath() + "next_1.png"
        }
        CommonText{
            id: priorityData
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: priorityBtnEnableImg.left
            anchors.rightMargin: 5
            color: "#858585"
            text: getgPrority()
            width: 50
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignRight
        }
        onClickedSignal: {
            setView("Priority");
        }
    }

    StripFrame{
        id: preSignalBtn
        anchors.top: priorityBtn.bottom
        line1Visible: false
        line2Visible: false
        bkgPressedColor: "#dbe0e6"
        bkgReleasedColor: "#ffffff"
        CommonImage{
            id: preSignalImg
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 16
            source: logicObject.getLocalImagePath() + "presignal.png"
        }
        CommonText{
            id: preSignalText
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: preSignalImg.right
            anchors.leftMargin: 15
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_PRESIGNAL")
            width: parent.width - (preSignalImg.width + 16) - (preSignalBtnEnableImg.width + 20) - (preSignalData.contentWidth + 10)
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
            Component.onCompleted: {
            }
        }
        CommonImage{
            id: preSignalBtnEnableImg
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 20
            source: logicObject.getLocalImagePath() + "next_1.png"
        }
        CommonText{
            id: preSignalData
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: preSignalBtnEnableImg.left
            anchors.rightMargin: 5
            color: "#858585"
            text: getgPreSignal() === 0 ? "--" : getgPreSignal()
            width: 50
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignRight
        }
        onClickedSignal: {
            setView("PreSignal");
        }
    }

    StripFrame{
        id: reconnectBtn
        anchors.top: preSignalBtn.bottom
        anchors.topMargin: 25
        line1Visible: false
        line2Visible: true
        bkgPressedColor: "#dbe0e6"
        bkgReleasedColor: "#ffffff"
        bkSwitch: false
        CommonImage{
            id: reconnectImg
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 16
            source: logicObject.getLocalImagePath() + "reconnect.png"
        }
        CommonText{
            id: reconnectText
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: reconnectImg.right
            anchors.leftMargin: 15
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_RECONNECT")
            width: parent.width - (reconnectImg.width + 16) - 80 - 22
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
            Component.onCompleted: {
            }
        }
        ImageSwitch{
            id: reconnectSwitch
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 22
            width: 60
            height: 65
            onSwitchOpenSignal: {
                setgReconnect(true);
            }
            onSwitchCloseSignal: {
                setgReconnect(false);
            }
        }
    }

    StripFrame{
        id: partlyConnectBtn
        anchors.top: reconnectBtn.bottom
        line1Visible: false
        line2Visible: false
        bkgPressedColor: "#dbe0e6"
        bkgReleasedColor: "#ffffff"
        bkSwitch: false
        CommonImage{
            id: partlyConnectImg
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 16
            source: logicObject.getLocalImagePath() + "part_Connect.png"
        }
        CommonText{
            id: partlyConnectText
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: partlyConnectImg.right
            anchors.leftMargin: 15
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_PARTLY_CONNECT")
            width: parent.width - (partlyConnectImg.width + 16) - 80 - 22
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
            Component.onCompleted: {
            }
        }
        ImageSwitch{
            id: partlyConnectSwitch
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 22
            width: 60
            height: 65
            onSwitchOpenSignal: {
                setgPartlyConnect(true)
            }
            onSwitchCloseSignal: {
                setgPartlyConnect(false);
            }
        }
    }

    Rectangle{

        function setVisibleValue(value)
        {
            visible = value;
        }

        id: maskItem
        width: parent.width
        height: maskItemHeight
        anchors.top: parent.top
        color: "#70666666"
        visible: false
        MouseArea{
            anchors.fill: parent
        }
    }  

    Cycle{
        id: cycleView
        anchors.top: maskItem.bottom
        width: parent.width
        height: childAreaHeight
        visible: false
        color: "#ffffff"
        z: 5
        onCancelSignal: {
            maskItem.setVisibleValue(false);
            cycleView.setVisibleValue(false);
        }
        onOkSignal: {
            setgCycle(cycleView.getCurrentIndex());

            if (getgCycle() === 0)
            {
               cycleData.setText(langStrObject.GetLangString("ETCS_FOREVER"));
            }
            else
            {
                cycleData.setText(getgCycle());
            }

            /*
            if (getgCycle() === 1)
            {
                gapTimeBtn.disableStatus();
            }
            else
            {
                gapTimeBtn.enableStatus();
            }
            */
            maskItem.setVisibleValue(false);
            cycleView.setVisibleValue(false);
        }
    }

    GapTime{
        id: gapTimeView
        anchors.top: maskItem.bottom
        width: parent.width
        height: childAreaHeight
        visible: false
        color: "#ffffff"
        z: 5
        onCancelSignal: {
            maskItem.setVisibleValue(false);
            gapTimeView.setVisibleValue(false);
        }
        onOkSignal: {
            setGMin(getCurrentMinIndex());
            setGSec(getCurrentSecondIndex());
            setGGapTime(getGapTimeFormMinAndSec());
            setGTimeType(getTimeType());
            gapTimeData.setText(logicObject.getMinAndSec(getCurrentMinIndex(), getCurrentSecondIndex()));
            maskItem.setVisibleValue(false);
            gapTimeView.setVisibleValue(false);
        }
    }

    Prority{
        id: priorityView
        anchors.top: maskItem.bottom
        width: parent.width
        height: childAreaHeight
        visible: false
        color: "#ffffff"
        z: 5
        onCancelSignal: {
            maskItem.setVisibleValue(false);
            priorityView.setVisibleValue(false);
        }
        onOkSignal: {
            setgPrority(getProrityFromCurrentIndex());
            priorityData.setText(getgPrority());
            maskItem.setVisibleValue(false);
            priorityView.setVisibleValue(false);
        }
    }

    PreSignal{
        id: preSignalView
        anchors.top: maskItem.bottom
        width: parent.width
        height: childAreaHeight
        visible: false
        color: "#ffffff"
        z: 5
        onOkSignal: {
            setgPreSignal(getCurrentIndex());
            preSignalData.setText(getgPreSignal() === 0 ? "--" : getgPreSignal());
            maskItem.setVisibleValue(false);
            preSignalView.setVisibleValue(false);
        }
    }  

    Component.onCompleted: {
        rootItem.updateLanguage.connect(updateLanguageSlot);
    }
}
