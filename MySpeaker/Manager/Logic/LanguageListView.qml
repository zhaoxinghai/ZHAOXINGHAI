import QtQuick 2.0
import "../Components"

Rectangle {

    signal cancelSignal(var value);
    signal updateSignal();

    property string imgSrc: ""

    function getLanguages()
    {
        var tmp;
        var cout;

        cout = langStrObject.getMapSize();

        for (var i = 0; i < cout; i++)
        {
            tmp = langStrObject.getMapLanguage(i + 1);
            if (tmp !== "")
            {
                if (tmp === localConfigObject.readConfig("LocalConfig", "Language"))
                {
                    listModel.append({"index": i, "langName": tmp, "btnStatus": 1});
                }
                else
                {
                    listModel.append({"index": i, "langName": tmp, "btnStatus": 0});
                }
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

    color: "#e7e7e7"

    MouseArea{
        anchors.fill: parent
        onClicked: {

        }
    }

    ListModel{
        id: listModel
    }

    ListView {
        id: languageListView
        anchors.top: parent.top
        width: parent.width
        height: parent.height
        visible: true
        boundsBehavior: Flickable.StopAtBounds
        snapMode: ListView.NoSnap
        footer: Rectangle{height: 8}
        clip: true
        header: Rectangle{
            width: parent.width
            height: 22
            color: "#e7e7e7"
            visible: true
        }
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
                font.family: rootItem.fontName
                font.pixelSize: 18
                color: "#303030"
                text: langName
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
                    promptMask.messageShow(langStrObject.GetLangString("ETCS_LANGUAGE_SWITCHING"));
                    timeOutTimer.setInterval(20 * 1000);
                    timeOutTimer.start();
                    localConfigObject.writeConfig("LocalConfig", "Language", langName);
                    clearListModelBtnStatus();
                    listModel.setProperty(index, "btnStatus", 1);
                    languageTimer.start();
                }
            }
        }
    }

    ScrollBar{
        id: scrollBar
        anchors.top: languageListView.top
        anchors.right: parent.right
        anchors.rightMargin: 6
        height: languageListView.height
        position: languageListView.visibleArea.yPosition
        pageSize: languageListView.visibleArea.heightRatio
        scrollBarVisible: languageListView.visibleArea.heightRatio == 1 ? false : true
        z: 1
    }

    Timer{
        id: languageTimer
        interval: 1000
        repeat: false
        triggeredOnStart: false
        onTriggered: {
            var langName = localConfigObject.readConfig("LocalConfig", "Language");
            langStrObject.readLangConfigTable(langStrObject.getMapIndex(langName));
            rootItem.updateFontFromLanguage(langName);
            keyboardObject.initKeyMap(langName);
            updateSignal();
            rootItem.updateLanguage();
            promptMask.messageHide();
        }
    }

    Component.onCompleted: {
        getLanguages();
    }
}
