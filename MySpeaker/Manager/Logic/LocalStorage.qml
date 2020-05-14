import QtQuick 2.0
import "../Components"

Rectangle {

    function updateLanguageSlot()
    {
        title.text = langStrObject.GetLangString("ETCS_LOCAL_STROAGE");
        prevTitleName.text = langStrObject.GetLangString("ETCS_MORE");
        audioStorageName.setText(langStrObject.GetLangString("ETCS_AUDIO"));
        vaaudioStorageName.setText(langStrObject.GetLangString("ETCS_VA_AUDIO"));
        recordStorageName.setText(langStrObject.GetLangString("ETCS_RECORD"));
    }

    function initStatus()
    {
        audioStorageMes.setText(localConfigObject.getUsedStorage(0) + "MB" + " / " + localConfigObject.getTotalStorage(0) + "MB");
        vaaudioStorageMes.setText(localConfigObject.getUsedStorage(1) + "MB" + " / " + localConfigObject.getTotalStorage(1) + "MB");
        recordStorageMes.setText(localConfigObject.getUsedStorage(2) + "MB" + " / " + localConfigObject.getTotalStorage(2) + "MB");
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
            text: langStrObject.GetLangString("ETCS_LOCAL_STROAGE")
            font.family: rootItem.fontName
            font.pixelSize: mainPage.g_titleTextFontSize
            width: 200
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            z: 2
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
                }
            }
        }
    }

    CommonRect{
        id: audioStorageItem
        anchors.top: titleBar.bottom
        anchors.topMargin: 25
        anchors.left: parent.left
        width: parent.width
        height: 65
        line1Visible: false
        line2Visible: true
        CommonText{
            id: audioStorageName
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 34
            width: 100
            font.pixelSize: 18
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_AUDIO")
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
        }
        CommonText{
            id: audioStorageMes
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 25
            width: parent.width - audioStorageName.contentWidth
            font.pixelSize: 18
            color: "#858585"
            text: localConfigObject.getUsedStorage(0) + "MB" + " / " + localConfigObject.getTotalStorage(0) + "MB"
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignRight
        }
    }

    CommonRect{
        id: vaaudioStorageItem
        anchors.top: audioStorageItem.bottom
        anchors.left: parent.left
        width: parent.width
        height: 65
        line1Visible: false
        line2Visible: true
        CommonText{
            id: vaaudioStorageName
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 34
            width: 100
            font.pixelSize: 18
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_VA_AUDIO")
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
        }
        CommonText{
            id: vaaudioStorageMes
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 25
            width: parent.width - vaaudioStorageName.contentWidth
            font.pixelSize: 18
            color: "#858585"
            text: localConfigObject.getUsedStorage(1) + "MB" + " / " + localConfigObject.getTotalStorage(1) + "MB"
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignRight
        }
    }

    CommonRect{
        id: recordStorageItem
        anchors.top: vaaudioStorageItem.bottom
        anchors.left: parent.left
        width: parent.width
        height: 65
        line1Visible: false
        line2Visible: true
        CommonText{
            id: recordStorageName
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 34
            width: 100
            font.pixelSize: 18
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_RECORD")
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
        }
        CommonText{
            id: recordStorageMes
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 25
            width: parent.width - recordStorageName.width
            font.pixelSize: 18
            color: "#858585"
            text: localConfigObject.getUsedStorage(2) + "MB" + " / " + localConfigObject.getTotalStorage(2) + "MB"
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignRight
        }
    }

    /*
    CommonRect{
        id: varecordItem
        anchors.top: recordStorageItem.bottom
        anchors.left: parent.left
        width: parent.width
        height: 65
        line1Visible: false
        line2Visible: true
        CommonText{
            id: varecordName
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 34
            width: 100
            font.pixelSize: 18
            color: "#303030"
            text: langStrObject.GetLangString("VA Record")
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
        }
        CommonText{
            id: varecordMes
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 25
            width: parent.width - varecordName.contentWidth
            font.pixelSize: 18
            color: "#858585"
            text: localConfigObject.getUsedStorage(3) + "MB" + " / " + localConfigObject.getTotalStorage(3) + "MB"
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignRight
        }
    }
    */

    Component.onCompleted: {
        rootItem.updateLanguage.connect(updateLanguageSlot);
    }
}
