import QtQuick 2.0
import "../Components"

Rectangle {

    signal cancelSignal(var value);
    signal updateSignal();

    function updateLanguageSlot()
    {
        listModel.clearListModel();
        listModel.createListModel();
        clearListModelBtnStatus();
        setClickedBtnStatus();
    }

    function initStatus()
    {
        listModel.clearListModel();
        listModel.createListModel();
        clearListModelBtnStatus();
        setClickedBtnStatus();
        setVisibleValue(true);
    }

    function setClickedBtnStatus()
    {
        var regionKey;
        regionKey = localConfigObject.readConfig("LocalConfig", "Region");

        for (var i = 0; i < listModel.count; i++)
        {
            if (regionKey === listModel.get(i).key)
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
            listModel.append({"nIndex": 0, "key": "ETCS_GERMANY", "btnStatus": 0});
            listModel.append({"nIndex": 1, "key": "ETCS_CHINA", "btnStatus": 0});
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

    ListView {
        id: regionListView
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
                visible: index == 0 ? false : true
            }

            Text {
                id: buttonName
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 31
                font.family: specialFontLoader.getSpecialFontName()
                font.pixelSize: 18
                color: "#303030"
                text: langStrObject.GetLangString(key)
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
                    localConfigObject.writeConfig("LocalConfig", "Region", key);
                    clearListModelBtnStatus();
                    setClickedBtnStatus();
                    listModel.setProperty(index, "btnStatus", 1);
                    cancelSignal(false);
                    updateSignal();
                }
            }
        }
    }

    Component.onCompleted: {
        rootItem.updateLanguage.connect(updateLanguageSlot);
    }
}
