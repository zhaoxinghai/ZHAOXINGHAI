import QtQuick 2.0
import "../Components"

Rectangle {

    signal isEmptySignal();
    signal selectSignal();

    function initStatus()
    {
        var ret;
        var id;
        var name;

        ret = isComboboxEnable();
        if (ret === true)
        {
            mouseArea.setEnabledValue(true);
            id = userManageObject.getUserId(0);
            name = userManageObject.getUserListStrValue(id, "name");
            userName.setText(name);
            userName.setTextColor("#303030");
        }
        else
        {
            mouseArea.setEnabledValue(false);
            isEmpty();
        }
    }

    function getText()
    {
        return userName.text;
    }

    function isComboboxEnable()
    {
        if (userListModel.getListModelCount() <= 0)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    width: 400
    height: 74

    ListModel{

        function createListModel()
        {
            var count = userManageObject.getUserListCount();
            for (var i = 0; i < count; i++)
            {
                userListModel.append({"nId": userManageObject.getUserId(i)});
            }
        }

        function clearListModel()
        {
           if (userListModel.count > 0)
           {
                remove(0, userListModel.count);
           }
        }

        function getListModelCount()
        {
            return userListModel.count;
        }

        id: userListModel
    }

    CommonText {
        id: userName
        anchors.left: parent.left
        anchors.leftMargin: 12
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 3
        color: "#b3b3b3"
        text: langStrObject.GetLangString("ETCS_SELECT_USER")
        font.family: rootItem.fontName
        font.bold: false
        font.pixelSize: 25
        width: parent.width - image.width - 20
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignLeft
        Component.onCompleted: {
        }
    }

    Image{
        id: image
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 10
        source: logicObject.getLocalImagePath() + "arrow_down_1.png"
    }

    HorizontalLine {
        id: usrNameLine
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.bottom
        width: 400
        height: 1
        color: "#b3b3b3"
        z: 3
    }

    MouseArea {

        function setEnabledValue(value)
        {
            enabled = value;
        }

        id: mouseArea
        anchors.fill: parent
        onClicked: {
            if (list.getVisibleValue() === false)
            {
                list.setVisibleValue(true);
                listRect.setVisibleValue(true);
                list.resetList();
                usrNameLine.setLineColor("#cd3429")
            }
            else
            {
                list.setVisibleValue(false);
                listRect.setVisibleValue(false);
                usrNameLine.setLineColor("#b3b3b3");
                selectSignal();
            }
        }
    }


    Rectangle{

        function setVisibleValue(value)
        {
           visible = value;
        }

        id: listRect
        width: 400
        height: 182
        anchors.top:list.top
        anchors.left: list.left
        color: "#f5f5f5"
        z: 1
        visible: false
    }

    ListView {

        function resetList()
        {
            list.contentY = 0;
        }

        function setVisibleValue(value)
        {
            visible = value;
        }

        function getVisibleValue()
        {
            return visible;
        }

        id: list
        anchors.top: parent.bottom
        anchors.left: parent.left
        width: 400
        height: 182
        visible: false
        z: 2
        boundsBehavior: Flickable.StopAtBounds
        snapMode: ListView.NoSnap
        clip: true
        model: userListModel

        delegate: RectButton{
            width: 400
            height: 60
            line1Visible: false
            line2Visible: true
            color: "#f5f5f5"
            CommonText{
                id: menuText
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                color: "#303030"
                text: userManageObject.getUserListStrValue(nId, "name")
                font.family: rootItem.fontName
                font.pixelSize: 26
                width: parent.width
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
            onClickedSignal: {
                userName.setText(menuText.getText());
                userName.setTextColor("#303030");
                usrNameLine.setLineColor("#b3b3b3");
                list.setVisibleValue(false);
                listRect.setVisibleValue(false);
                selectSignal();
            }
        }

        ScrollBar{
            id: userScrollBar
            anchors.top: list.top
            anchors.right: list.right
            anchors.rightMargin: 2
            width: 6
            height: list.height
            orientation: Qt.Vertical
            position: list.visibleArea.yPosition
            pageSize: list.visibleArea.heightRatio
            scrollBarVisible: list.visibleArea.heightRatio == 1 ? false : true
            z: 2
        }
    }

    Component.onCompleted: {
        userListModel.clearListModel();
        userListModel.createListModel();

        initStatus();
    }
}
