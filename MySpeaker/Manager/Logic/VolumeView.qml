import QtQuick 2.0
import "../Components"

Rectangle {

    property int nodeListViewHeight: height - mainPage.g_titleBarHeight

    function updateLanguageSlot()
    {
        title.text = langStrObject.GetLangString("ETCS_NODE");
        prevTitleName.text = langStrObject.GetLangString("ETCS_MORE");
        localName.setText(langStrObject.GetLangString("ETCS_LOCAL_VOLUME"));
    }

    function initStatus()
    {
        listModel.clearListModel();
        listModel.createListModel();
        setVisibleValue(true);
    }

    function quitStatus()
    {
        setVisibleValue(false);
    }

    function setVisibleValue(value)
    {
        visible = value;
    }


    color: "#e7e7e7"

    MouseArea{
        anchors.fill: parent
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
            text: langStrObject.GetLangString("ETCS_NODE")
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
        contentHeight: 1*65 + networkNodeView.height + 25
        flickableDirection: Flickable.VerticalFlick
        boundsBehavior: Flickable.StopAtBounds
        clip: true

        Rectangle{
            anchors.fill: parent
            color: "#e7e7e7"
        }

        StripFrame{
            id: localBtn
            anchors.top: parent.top
            anchors.topMargin: 25
            anchors.left: parent.left
            width: parent.width
            height: 65
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            line1Visible: false
            line2Visible: false

            CommonImage{
                id: localImg
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 16
                source: logicObject.getLocalImagePath() + "local.png"
            }

            CommonText{
                id: localName
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: localImg.right
                anchors.leftMargin: 15
                width: parent.width - localImg.width - 16 - clickImg.width - 20
                font.pixelSize: 18
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_LOCAL_VOLUME")
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
                Component.onCompleted: {
                }
            }

            CommonImage{
                id: clickImg
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 20
                source: logicObject.getLocalImagePath() + "next_1.png"
            }

            onClickedSignal: {
                sourceManageObject.updateVolumeControl(localConfigObject.getLocalNode());
                volumeLocal.initStatus();
            }
        }

        ListView {
            id: networkNodeView
            anchors.top: localBtn.bottom
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
                            width: 330
                            color: sourceManageObject.getNodeBoolValue(nIndex, "enable") ? "#303030" : "#40666666"
                            text: sourceManageObject.getNodeStrValue(nIndex, "name")
                        }
                        CommonText{
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: btnEnableImage.left
                            anchors.rightMargin: 5
                            width: 50
                            color:  sourceManageObject.getNodeBoolValue(nIndex, "enable") ? "#858585" : "#40666666"
                            horizontalAlignment: Text.AlignRight
                            text: sourceManageObject.getNodeIntValue(nIndex, "node")
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
                            sourceManageObject.updateVolumeControl(sourceManageObject.getNodeIntValue(nIndex, "node"));
                            volumeDOM.setGNode(sourceManageObject.getNodeIntValue(nIndex, "node"));
                            volumeDOM.initStatus();
                        }
            }
        }
    }

    ListModel {

        function createListModel()
        {
            var count = sourceManageObject.getNodeCount();
            for (var i = 0; i < count; i++)
            {
                if (sourceManageObject.isDOM(i) === 0)
                {
                    listModel.append({"nIndex": i});
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

    VolumeLocal{
        id: volumeLocal
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width
        height: parent.height
        visible: false
    }

    VolumeDom{
        id: volumeDOM
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width
        height: parent.height
        visible: false
    }

    Component.onCompleted: {
        rootItem.updateLanguage.connect(updateLanguageSlot);
    }
}
