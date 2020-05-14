import QtQuick 2.0
import "../Components"

Rectangle {

    property var userNameStr: langStrObject.GetLangString("ETCS_SELECT_USER")
    property string userNameStrInitColor: "#b3b3b3"
    property string userNameStrSelectColor: "#303030"
    property string userNameLineInitColor: "#b3b3b3"
    property string userNameListSelectColor: "#cd3429"
    property string listViewBackgroundColor: "#f5f5f5"
    property string listViewPressedBackgroundColor: "#dbe0e6"
    property string listViewBorderColor:"#dcdcdc"
    property string listViewTextColor: "#303030"

    signal isEmptySignal();

    function initUserName()
    {
        var count = userListModel.getListModelCount();
        var id;

        for (var i = 0; i < count; i++)
        {
            id = userManageObject.getUserId(i);
            userNameStr = userManageObject.getUserListStrValue(id, "name");
            userName.color = userNameStrSelectColor;
            return 0;
        }

        userNameStr = langStrObject.GetLangString("ETCS_SELECT_USER");
        userName.color = userNameStrInitColor;
        return -1;
    }

    function setComboxEnableValue(value)
    {
        mouseArea1.enabled = value;
    }

    function setListVisibleValue(value)
    {
        listRect.visible = value;
    }

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

    Column {
        id: box

        Rectangle {
            id: selectionBox
            width: 400
            height: 74

            Text {
                id: userName
                anchors.left: parent.left
                anchors.leftMargin: 12
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 3
                color: userNameStrInitColor
                text: userNameStr
                font.family: rootItem.fontName
                font.bold: false
                font.pixelSize: 25
                width: parent.width - image.width - 20
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
            Image{
                id: image
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                anchors.right: parent.right
                anchors.rightMargin: 11
                source: logicObject.getLocalImagePath() + "arrow_down_1.png"
            }

            MouseArea {
                id: mouseArea1
                anchors.fill: parent
                onClicked: {
                    if (listRect.visible == false)
                    {
                        listRect.visible = true;
                        list.resetList();
                        usrNameLine.color = userNameListSelectColor;
                        passwordEdit.passwdFieldFocus = false;
                        loginInPage.clearPassword();
                        loginInPage.messageClear();
                    }
                    else
                    {
                        listRect.visible = false;
                        usrNameLine.color = userNameLineInitColor;
                        passwordEdit.passwdFieldFocus = false;
                        loginInPage.clearPassword();
                        loginInPage.messageClear();
                    }
                }
            }

        }

        Rectangle {
            id: usrNameLine
            width: 400
            height: 1
            color: userNameLineInitColor
            z: 1
        }

        Rectangle{
            id: listRect
            width: 400
            height: 182
            color: listViewBackgroundColor
            visible: false

            ListView {

                function resetList()
                {
                    list.contentY = 0;
                }

                id: list
                anchors.top: parent.top
                anchors.left: parent.left
                width: 400
                height: 182
                visible: true
                boundsBehavior: Flickable.StopAtBounds
                snapMode: ListView.NoSnap
                clip: true
                model: userListModel
                delegate: Rectangle{
                        width: 400
                        height:60
                        color: mouseArea.pressed ? listViewPressedBackgroundColor : listViewBackgroundColor

                        Rectangle{
                            anchors.bottom: parent.bottom
                            anchors.left: parent.left
                            width: parent.width
                            height: 1
                            color: listViewBorderColor
                            z: 1
                            visible: true
                        }

                        Text {
                            id: menuText
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            anchors.top: parent.top
                            anchors.topMargin: 10
                            color: listViewTextColor
                            text: userManageObject.getUserListStrValue(nId, "name")
                            font.family: rootItem.fontName
                            font.bold: false
                            font.pixelSize: 26
                            width: parent.width
                            elide: Text.ElideRight
                            horizontalAlignment: Text.AlignLeft
                        }

                        MouseArea {
                            id: mouseArea
                            anchors.fill: parent
                            onClicked: {
                                userNameStr = menuText.text;
                                userName.color = listViewTextColor;
                                usrNameLine.color = userNameLineInitColor;
                                passwordEdit.passwdFieldFocus = true;
                                listRect.visible = false;
                            }
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
        }
    }

    Component.onCompleted: {
        userListModel.clearListModel();
        userListModel.createListModel();
        initUserName();
    }
}
