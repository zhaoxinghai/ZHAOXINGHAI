import QtQuick 2.0
import "../Components"

Rectangle {

    property int nodeListViewHeight: height - mainPage.g_titleBarHeight

    function updateLanguageSlot()
    {
        title.text = langStrObject.GetLangString("ETCS_SOURCE");
        prevTitleName.text = langStrObject.GetLangString("ETCS_ZONE");
        usbButton.btnName = langStrObject.GetLangString("ETCS_USB");
        localButton.btnName = langStrObject.GetLangString("ETCS_LOCAL");
        liveButton.btnName = langStrObject.GetLangString("ETCS_LIVE");
        usbView.titleBarText = langStrObject.GetLangString("ETCS_USB");
        liveView.titleBarText = langStrObject.GetLangString("ETCS_LIVE");
    }

    function updateConnectResultSlot()
    {
        listModel.clearListModel();
        listModel.createListModel();
    }

    function initStatus()
    {
        setVisibleValue(true);
        listModel.clearListModel();
        listModel.createListModel();
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function changeSourceView(index)
    {
        if (index === "USB")
        {
            usbView.initStatus();
            localView.setVisibleValue(false);
            networkView.setVisibleValue(false);
            networkLiveView.setVisibleValue(false);
            liveView.setVisibleValue(false);
        }
        else if (index === "Local")
        {
            usbView.setVisibleValue(false);
            localView.initStatus();
            networkView.setVisibleValue(false);
            networkLiveView.setVisibleValue(false);
            liveView.setVisibleValue(false);
        }
        else if (index === "Network")
        {
            usbView.setVisibleValue(false);
            localView.setVisibleValue(false);
            networkView.initStatus();
            networkLiveView.setVisibleValue(false);
            liveView.setVisibleValue(false);
        }
        else if (index === "NetworkLive")
        {
            usbView.setVisibleValue(false);
            localView.setVisibleValue(false);
            networkView.setVisibleValue(false);
            networkLiveView.initStatus();
            liveView.setVisibleValue(false);
        }
        else if (index === "Live")
        {
            usbView.setVisibleValue(false);
            localView.setVisibleValue(false);
            networkView.setVisibleValue(false);
            liveView.initStatus();
        }
    }

    function getgPrority()
    {
        return liveView.getgPrority();
    }

    function getVolumeSliderValue()
    {
        return liveView.getVolumeSliderValue();
    }

    function getgPreSignal()
    {
        return liveView.getgPreSignal();
    }

    function getgReconnect()
    {
        return liveView.getgReconnect();
    }

    function getgPartlyConnect()
    {
        return liveView.getgPartlyConnect();
    }

    function getLivePort()
    {
        return liveView.getLivePort();
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
            text: langStrObject.GetLangString("ETCS_SOURCE");
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
                text: langStrObject.GetLangString("ETCS_ZONE")
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
                    changeView("zoneView");
                    mainPage.showBusinessBar();
                }
            }
        }
    }

    Flickable{
        id: nodeListView
        anchors.top: titleBar.bottom
        anchors.left: parent.left
        width: parent.width
        height: nodeListViewHeight
        contentWidth: width
        contentHeight: 4*65 + networkNodeView.height + 25
        flickableDirection: Flickable.VerticalFlick
        boundsBehavior: Flickable.StopAtBounds
        clip: true

        Rectangle{
            anchors.fill: parent
            color: "#e7e7e7"
        }

        StripButton{
            id: usbButton
            anchors.top: parent.top
            anchors.topMargin: 25
            anchors.left: parent.left
            width: parent.width
            buttonNameWidth: 300
            imgSrc: logicObject.getLocalImagePath() + "USB.png"
            btnName: langStrObject.GetLangString("ETCS_USB")
            infoText: ""
            isClickBtn: true
            clickImgSrc: logicObject.getLocalImagePath() + "next_1.png"
            btnValue: "USB"
            line1Visible: false
            line2Visible: true
            Component.onCompleted: {
            }
        }
        StripButton{
            id: localButton
            anchors.top: usbButton.bottom
            anchors.left: parent.left
            width: parent.width
            buttonNameWidth: 300
            imgSrc: logicObject.getLocalImagePath() + "local.png"
            btnName: langStrObject.GetLangString("ETCS_LOCAL")
            infoText: ""
            isClickBtn: true
            clickImgSrc: logicObject.getLocalImagePath() + "next_1.png"
            btnValue: "Local"
            line1Visible: false
            line2Visible: true
            Component.onCompleted: {
            }
        }
        StripButton{
            id: liveButton
            anchors.top: localButton.bottom
            anchors.left: parent.left
            width: parent.width
            buttonNameWidth: 300
            imgSrc: logicObject.getLocalImagePath() + "live.png"
            btnName: langStrObject.GetLangString("ETCS_LIVE")
            infoText: ""
            isClickBtn: true
            clickImgSrc: logicObject.getLocalImagePath() + "next_1.png"
            btnValue: "Live"
            line1Visible: false
            line2Visible: false
            Component.onCompleted: {
            }
        }

        ListView {
            id: networkNodeView
            anchors.top: liveButton.bottom
            anchors.topMargin: 25
            width: parent.width
            height: listModel.count * 65 + 30
            visible: true
            boundsBehavior: Flickable.StopAtBounds
            snapMode: ListView.NoSnap
            footer: Rectangle{height: 8}
            model: listModel
            delegate: RectButton{
                        width: 480
                        height: 65
                        bkgReleasedColor: "#ffffff"
                        line1Visible: index == 0 ? false : true
                        line2Visible: false
                        btnEnable: sourceManageObject.getNodeBoolValue(nIndex, "enable")

                        CommonImage{
                            id: btnTypeImage
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            anchors.leftMargin: 16
                            source: sourceManageObject.getNodeBoolValue(nIndex, "enable") ?
                                    logicObject.getLocalImagePath() + "network_A.png" :
                                    logicObject.getLocalImagePath() + "network_B.png"
                        }
                        CommonText{
                            id: nodeName
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: btnTypeImage.right
                            anchors.leftMargin: 15              
                            color: sourceManageObject.getNodeBoolValue(nIndex, "enable") ? "#303030" : "#40666666"
                            text: sourceManageObject.getNodeStrValue(nIndex, "name") + "   " + devType
                            width: 300
                            elide: Text.ElideRight
                            horizontalAlignment: Text.AlignLeft
                        }
                        CommonImage{
                            id: btnEnableImage
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            anchors.rightMargin: 20
                            source: logicObject.getLocalImagePath() + "next_1.png"
                            visible: sourceManageObject.getNodeBoolValue(nIndex, "enable")
                        }

                        onClickedSignal: {
                            if (devType === "Audio")
                            {
                                var strPath;
                                networkView.setNode(sourceManageObject.getNodeIntValue(nIndex, "node"));
                                strPath = sourceManageObject.loadNetIOMAP(sourceManageObject.getNodeIntValue(nIndex, "node"));
                                networkView.setPath(strPath);
                                changeSourceView("Network");
                            }
                            else if (devType === "DAL")
                            {
                                networkLiveView.setNode(sourceManageObject.getNodeIntValue(nIndex, "node"));
                                changeSourceView("NetworkLive");
                            }
                            else if (devType === "Input")
                            {
                                networkLiveView.setNode(sourceManageObject.getNodeIntValue(nIndex, "node"));
                                changeSourceView("NetworkLive");
                            }
                        }
            }
        }
    }

    ListModel {

        function createListModel()
        {
            var ret;
            var count = sourceManageObject.getNodeCount();
            var tmpNode = 0;
            var localNode = -1;
            var isSourceVisible;

            for (var i = 0; i < count; i++)
            {
                isSourceVisible = sourceManageObject.getNodeBoolValue(i, "sourceVisible");
                if (isSourceVisible === false)
                {
                    continue;
                }

                ret = sourceManageObject.getDeviceType(i);
                if (ret === 1 || ret === 2
                      || ret === 6 || ret === 7 || ret === 8 || ret === 9) //DOM and Comprio
                {
                    listModel.append({"nIndex": i, "devType": "DAL"});
                }
                else if (ret === 10) //SCU
                {
                    listModel.append({"nIndex": i, "devType": "Audio"});
                }
                else if (ret === 12) //ETCS
                {
                    tmpNode = sourceManageObject.getNode(i);
                    localNode = localConfigObject.getLocalNode();
                    if (tmpNode === localNode)
                    {
                        ;
                    }
                    else
                    {
                        listModel.append({"nIndex": i, "devType": "Input"});
                        listModel.append({"nIndex": i, "devType": "Audio"});
                    }
                }
            }
        }

        function clearListModel()
        {
           if (listModel.count > 0)
           {
                remove(0, listModel.count);
           }
        }

        id: listModel
    }

    ScrollBar{
        id: scrollBar
        anchors.top: nodeListView.top
        anchors.right: nodeListView.right
        anchors.rightMargin: 2
        height: nodeListView.height
        position: nodeListView.visibleArea.yPosition
        pageSize: nodeListView.visibleArea.heightRatio
        scrollBarVisible: nodeListView.visibleArea.heightRatio == 1 ? false : true
        z: 3
    }

    BaseListView{
        id: usbView
        width: parent.width
        height: parent.height
        anchors.top: parent.top
        z: 5
        visible: false
        listViewModel: usbSourceModel
        titleBarText: langStrObject.GetLangString("ETCS_USB");
    }

    LocalView{
        id:localView
        width: parent.width
        height: parent.height
        anchors.top: parent.top
        z: 5
        visible: false
    }

    LiveView{
        id:liveView
        width: parent.width
        height: parent.height
        anchors.top: parent.top
        z: 5
        visible: false
        titleBarText: langStrObject.GetLangString("ETCS_LIVE");
    }

    NetworkView{
        id: networkView
        width: parent.width
        height: parent.height
        anchors.top: parent.top
        z: 5
        visible: false
    }

    NetworkLiveView{
        id: networkLiveView
        width: parent.width
        height: parent.height
        anchors.top: parent.top
        z: 5
        visible: false
    }

    Component.onCompleted: {
        rootItem.updateLanguage.connect(updateLanguageSlot);
        localButton.btnValueSignal.connect(changeSourceView);
        usbButton.btnValueSignal.connect(changeSourceView);
        liveButton.btnValueSignal.connect(changeSourceView);

        logicObject.updateConnectResultSignal.connect(updateConnectResultSlot);
    } 
}
