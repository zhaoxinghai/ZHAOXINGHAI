import QtQuick 2.0
import "../Components"

Rectangle {

    property int gid: 0
    property int g_sourceIndex: 10000
    property int g_tmpId: 0
    property int g_tmpSourceIndex: 999
    property string gType: ""
    property string prevTitleNameText: ""
    property int sourceTitleRectHeight: 60
    property int areaHeight: height -  mainPage.g_titleBarHeight

    function updateLanguageSlot()
    {
        title.text = langStrObject.GetLangString("ETCS_SOURCE");
    }

    function updateSourceSlot(id, index)
    {
        setTmpId(id);
        setTmpSourceIndex(index);

        if (getGid() === id)
        {
            setSourceIndex(index);
        }
    }

    function activateDeleteSlot(id)
    {
        if (getGid() === id)
        {
            setTmpId(0);
            setTmpSourceIndex(9999);
            setSourceIndex(10000);
        }
    }

    function setSourceIndex(value)
    {
        g_sourceIndex = value
    }

    function getSourceIndex()
    {
        return g_sourceIndex;
    }

    function setStatus(id)
    {
        setGid(id);
        getSources();
        if (getGid() === getTmpId())
        {
            setSourceIndex(getTmpSourceIndex());
        }
    }

    function quitStatus()
    {
        setGid(0);
        setSourceIndex(1000);
        setVisibleValue(false);
    }

    function setGid(id)
    {
        gid = id;
    }

    function getGid()
    {
        return gid;
    }

    function getSources()
    {
        var tmp;
        var count;

        listModel.clear();

        var i = 0;

        if (gType == "Predefine")
        {
            count = logicObject.getPredefineSourceCount(getGid());
            for (i = 0; i < count; i++)
            {
                tmp = logicObject.getPredefineSource(getGid(), i);
                listModel.append({"sourceIndex": i, "sourceName": tmp});
            }
        }
        else if (gType == "Activate")
        {
            if (logicObject.getActivateBoolValue(getGid(), "haveGong") === true)
            {
                count = logicObject.getActivateSourceCount(getGid());
                for (i = 0; i < count; i++)
                {
                    tmp = logicObject.getActivateSource(getGid(), i);
                    listModel.append({"sourceIndex": i + 1, "sourceName": tmp});
                }
            }
            else
            {
                count = logicObject.getActivateSourceCount(getGid());
                for (i = 0; i < count; i++)
                {
                    tmp = logicObject.getActivateSource(getGid(), i);
                    listModel.append({"sourceIndex": i, "sourceName": tmp});
                }
            }
        }
        else if (gType == "History")
        {
            count = historyObject.getHistorySourceCount(getGid());
            for (i = 0; i < count; i++)
            {
                tmp = historyObject.getHistorySource(getGid(), i);
                listModel.append({"sourceIndex": i, "sourceName": tmp});
            }
        }
    }

    function setTmpId(id)
    {
        g_tmpId = id
    }

    function getTmpId()
    {
        return g_tmpId;
    }

    function setTmpSourceIndex(sourceIndex)
    {
        g_tmpSourceIndex = sourceIndex;
    }

    function getTmpSourceIndex()
    {
        return g_tmpSourceIndex;
    }

    function setVisibleValue(value)
    {
        visible = value;
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

        Text{
            id: title
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: mainPage.g_titleTextColor
            text: langStrObject.GetLangString("ETCS_SOURCE")
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
                text: prevTitleNameText
                font.family: rootItem.fontName
                font.pixelSize: mainPage.g_titleTextFontSize2
                width: 240 - (title.contentWidth / 2) - prevBtnImg.width - 14 + 8
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
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

    ListModel{
        id: listModel
    }

    ScrollBar{
        id: scrollBar
        anchors.top: sourceListView.top
        anchors.right: parent.right
        anchors.rightMargin: 6
        width: 6
        height: sourceListView.height
        orientation: Qt.Vertical
        position: sourceListView.visibleArea.yPosition
        pageSize: sourceListView.visibleArea.heightRatio
        scrollBarVisible: sourceListView.visibleArea.heightRatio == 1 ? false : true
        z: 1
    }

    ListView {
        id: sourceListView
        anchors.top: titleBar.bottom
        width: parent.width
        height: areaHeight
        visible: true
        boundsBehavior: Flickable.StopAtBounds
        snapMode: ListView.NoSnap
        footer: Rectangle{height: 8}
        clip: true
        model: listModel
        delegate: Rectangle {
            width: 480
            height: 65
            color: "#ffffff"

            Rectangle{
                anchors.top: parent.top
                anchors.left: parent.left
                width: parent.width
                height: 1
                color: "#e0e0e0"
                z: 1
                visible: index == 0 ? false : true
            }

            Text {
                id: buttonName
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 31
                font.family: rootItem.fontName
                font.pixelSize: 18
                color: "#303030"
                text: index + 1
            }

            Text {
                id: source
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: isPlayingImage.left
                anchors.rightMargin: 5
                width: parent.width - buttonName.width - isPlayingImage.width - 31 - 15
                font.family: rootItem.fontName
                font.pixelSize: 18
                color: "#858585"
                text: sourceName
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
            }

            CommonImage{
                id: isPlayingImage
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 15
                visible: sourceIndex === getSourceIndex() ? true : false
                source: logicObject.getLocalImagePath() + "playing.png"
            }

            MouseArea {
                anchors.fill: parent
                enabled: true
                onClicked: {

                }
            }
        }
    }

    Component.onCompleted: {
        rootItem.updateLanguage.connect(updateLanguageSlot);
        logicObject.updateSourceSignal.connect(updateSourceSlot);
        logicObject.activateDeleteSignal.connect(activateDeleteSlot);
    }
}
