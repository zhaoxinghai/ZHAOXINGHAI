import QtQuick 2.0
import "../Components"

Rectangle {

    property alias sendPredefineBtnVisible: sendPredefineBtn.visible

    signal sendPredefineBtnSingal();
    signal sendScheduleBtnSingal();

    function updateLanguageSlot()
    {
        sendPredefineName.setText(langStrObject.GetLangString("ETCS_MAKE_PREDEFINE"));
        sendScheduleName.setText(langStrObject.GetLangString("ETCS_MAKE_SCHEDULE"));
        closeBtnName.setText(langStrObject.GetLangString("ETCS_CANCEL"));
    }

    function initStatus()
    {
        setVisibleValue(true);
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function closeBtnClickSlot()
    {
        setVisibleValue(false);
    }

    width: logicObject.getRealWidth()
    height: logicObject.getRealWidth()
    color: "#70666666"

    MouseArea{
        anchors.fill: parent
        onClicked: {

        }
    }

    RectButton{
        id: sendPredefineBtn
        anchors.bottom: sendScheduleBtn.top
        anchors.left: parent.left
        width: 480
        height: 65
        line1Visible: true
        line2Visible: false
        Image{
            id: sendPredefineImg
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 18
            source: logicObject.getLocalImagePath() + "predefine_A.png"
        }
        CommonText{
            id: sendPredefineName
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: sendPredefineImg.right
            anchors.leftMargin: 18
            font.family: rootItem.fontName
            font.pixelSize: 18
            color: "#5f5f5f"
            text: langStrObject.GetLangString("ETCS_MAKE_PREDEFINE")
            width: parent.width - sendPredefineImg.width - 18 - 18
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
            Component.onCompleted: {
            }
        }

        onClickedSignal: {
            sendPredefineBtnSingal()
        }
    }
    RectButton{
        id: sendScheduleBtn
        anchors.bottom: closeBtn.top
        anchors.left: parent.left
        width: 480
        height: 65
        line1Visible: true
        line2Visible: false
        Image{
            id: sendScheduleImg
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 18
            width: 38
            height: 37
            source: logicObject.getLocalImagePath() + "schedule_A.png"
        }
        CommonText{
            id: sendScheduleName
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: sendScheduleImg.right
            anchors.leftMargin: 18
            font.family: rootItem.fontName
            font.pixelSize: 18
            color: "#5f5f5f"
            text: langStrObject.GetLangString("ETCS_MAKE_SCHEDULE")
            width: parent.width - sendScheduleImg.width - 18 - 18
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
            Component.onCompleted: {
            }
        }
        onClickedSignal: {
            sendScheduleBtnSingal()
        }
    }
    RectButton{
        id: closeBtn
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        height: 86
        line1Visible: true
        line2Visible: true
        CommonText{
            id: closeBtnName
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: rootItem.fontName
            font.pixelSize: 24
            color: "#5f5f5f"
            text: langStrObject.GetLangString("ETCS_CANCEL")
            width: parent.width
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            Component.onCompleted: {
            }
        }
        onClickedSignal: {
            closeBtnClickSlot();
        }
    }

    Component.onCompleted: {
        rootItem.updateLanguage.connect(updateLanguageSlot);
    }
}
