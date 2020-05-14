import QtQuick 2.0
import "../Components"

Rectangle {

    signal cancelSignal(var value);
    signal updateSignal();

    property string imgSrc: ""

    function initStatus()
    {
        listModel.clearListModel();
        listModel.createListModel();
        setClickedBtnStatus();
        setVisibleValue(true);
    }

    function setClickedBtnStatus()
    {
        var sec;
        sec = userManageObject.getUserListIntValue(userManageObject.getLoginUserId(), "lockTime");

        clearListModelBtnStatus();
        for (var i = 0; i < listModel.count; i++)
        {
            if (sec === listModel.get(i).sec)
            {
                listModel.setProperty(i, "btnStatus", 1);
            }
        }
    }

    function clearListModelBtnStatus()
    {
        for (var i = 0; i < listModel.count; i++)
        {
            listModel.setProperty(i, "btnStatus", 0)
        }
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    color: "#e7e7e7"

    ListModel{

        function createListModel()
        {
            listModel.append({"nIndex": 0, "name": "1  " + langStrObject.GetLangString("ETCS_MINUTE"), "sec": 60, "btnStatus": 0});
            listModel.append({"nIndex": 1, "name": "2  " + langStrObject.GetLangString("ETCS_MINUTE"), "sec": 120, "btnStatus": 0});
            listModel.append({"nIndex": 2, "name": "5  " + langStrObject.GetLangString("ETCS_MINUTE"), "sec": 300, "btnStatus": 0});
            listModel.append({"nIndex": 3, "name": "10 " + langStrObject.GetLangString("ETCS_MINUTE"), "sec": 600, "btnStatus": 0});
            listModel.append({"nIndex": 4, "name": "30 " + langStrObject.GetLangString("ETCS_MINUTE"), "sec": 1800, "btnStatus": 0});
            listModel.append({"nIndex": 5, "name": "1  " + langStrObject.GetLangString("ETCS_HOUR"), "sec": 3600, "btnStatus": 0});
            listModel.append({"nIndex": 6, "name": langStrObject.GetLangString("ETCS_OFF"), "sec": 0, "btnStatus": 0});        }

        function clearListModel()
        {
            if (listModel.count > 0)
            {
                 remove(0, listModel.count);
            }
        }

        id: listModel
    }

    ListView {
        id: languageListView
        anchors.top: parent.top
        anchors.topMargin: 22
        width: parent.width
        height: parent.height
        visible: true
        boundsBehavior: Flickable.StopAtBounds
        snapMode: ListView.NoSnap
        footer: Rectangle{height: 8}
        clip: true
        model: listModel
        delegate: Rectangle {
            width: 480
            height: 65
            color: mouseArea.pressed ? "#dbe0e6" : "#ffffff"

            Rectangle{
                anchors.top: parent.top
                anchors.left: parent.left
                width: parent.width
                height: 1
                color: "#e0e0e0"
                z: 1
                visible: nIndex == 0 ? false : true
            }

            Text {
                id: buttonName
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 31
                font.family: rootItem.fontName
                font.pixelSize: 18
                color: "#303030"
                text: name
                width: parent.width - statusImg.width - 28
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }

            Image{
                id: statusImg
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 28
                visible: true
                source:  btnStatus == 1 ? logicObject.getLocalImagePath() + "repeat.png" : ""
            }

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                enabled: true
                onClicked: {
                    userManageObject.setUserListIntValue(userManageObject.getLoginUserId(), sec);
                    //userManageObject.printUserList();
                    clearListModelBtnStatus();
                    listModel.setProperty(index, "btnStatus", 1);
                    cancelSignal(false);
                    updateSignal();
                    rootItem.upDateLockScreenTimer();
                }
            }
        }
    }
}
