import QtQuick 2.0
import "../Components"

Rectangle {

    property int gid: 0
    property string gType: ""
    property string prevTitleNameText: ""
    property int zoneTitleRectHeight: 60
    property int areaHeight: height -  mainPage.g_titleBarHeight

    function updateLanguageSlot()
    {
        title.text = langStrObject.GetLangString("ETCS_ZONE");
    }

    function setStatus(id)
    {
        setGid(id);
        getZones()
    }

    function quitStatus()
    {
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

    function getZones()
    {
        var tmp;
        var count;
        var ret;
        listModel.clear();

        var i = 0;

        if (gType == "Predefine")
        {
            count = logicObject.getPredefineZoneCount(getGid());
            for (i = 0; i < count; i++)
            {
                tmp = logicObject.getPredefineZone(getGid(), i);
                listModel.append({"index": i + 1, "zoneName": tmp});
            }
        }
        else if (gType == "Activate")
        {         
            ret = logicObject.getActivateBoolValue(getGid(), "prelisten");
            if (ret === true)
            {
                listModel.append({"index":1, "zoneName": langStrObject.GetLangString("ETCS_SPEAKER")});
                return;
            }

            count = logicObject.getActivateZoneCount(getGid());
            for (i = 0; i < count; i++)
            {
                tmp = logicObject.getActivateZone(getGid(), i);
                listModel.append({"index": i + 1, "zoneName": tmp});
            }
        }
        else if (gType == "History")
        {
            count = historyObject.getHistoryZoneCount(getGid());
            for (i = 0; i < count; i++)
            {
                tmp = historyObject.getHistoryZone(getGid(), i);
                listModel.append({"index": i + 1, "zoneName": tmp});
            }
        }
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
            text: langStrObject.GetLangString("ETCS_ZONE")
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
        anchors.top: zoneListView.top
        anchors.right: parent.right
        anchors.rightMargin: 6
        width: 6
        height: zoneListView.height
        orientation: Qt.Vertical
        position: zoneListView.visibleArea.yPosition
        pageSize: zoneListView.visibleArea.heightRatio
        scrollBarVisible: zoneListView.visibleArea.heightRatio == 1 ? false : true
        z: 1
    }

    ListView {
        id: zoneListView
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
                text: index
            }

            Text {
                id: zone
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 31
                width: parent.width - buttonName.width - 2 * 31 - 5
                font.family: rootItem.fontName
                font.pixelSize: 18
                color: "#858585"
                text: zoneName
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
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
    }
}
