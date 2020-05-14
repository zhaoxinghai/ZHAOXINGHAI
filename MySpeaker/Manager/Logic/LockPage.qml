import QtQuick 2.0
import "../Components"

Rectangle {

    property string lockImgSrc: ""
    property int oldY: 0
    property int newY: 0

    function updateLanguageSlot()
    {
        unLockMessage.text = langStrObject.GetLangString("ETCS_UNLOCK_WAY");
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function setLockImage(value)
    {
        lockImgSrc = value;
    }

    function initStatus()
    {
        setLockImage(localConfigObject.getLockImg());
        setVisibleValue(true);

        logicObject.setButtonEnable("alarm", false);
        logicObject.setButtonEnable("DKM", false);
    }

    clip: true

    Text{
        id: hourAndMin
        anchors.top: parent.top
        anchors.topMargin: 130
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#000000"
        text: logicObject.getHourAndMinFromSystemTime()
        font.family: rootItem.fontName
        font.bold: false
        font.pixelSize: 80
        z: 1
        width: parent.width
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
    }

    Text{
        id: yearAndMonthAndDay
        anchors.top: hourAndMin.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#000000"
        text: logicObject.getYearAndMonthAndDayFromSystemTime()
        font.family: rootItem.fontName
        font.bold: false
        font.pixelSize: 26
        z: 1
        width: parent.width
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
    }

    Text{
        id: unLockMessage
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 50
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#ffffff"
        text: langStrObject.GetLangString("ETCS_UNLOCK_WAY")
        font.family: rootItem.fontName
        font.bold: false
        font.pixelSize: 22
        z: 1
        width: parent.width
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
        Component.onCompleted: {
        }
    }


    Image{
        id: lockImg
        anchors.fill: parent
        source: lockImgSrc
    }

    Timer{
        id: flushTimeTimer
        interval: 1000
        repeat: true
        onTriggered: {
            hourAndMin.text = logicObject.getHourAndMinFromSystemTime();
            yearAndMonthAndDay.text = logicObject.getYearAndMonthAndDayFromSystemTime();
        }
    }

    MouseArea{
        id: mouseArea
        anchors.fill: parent
        onDoubleClicked: {

        }
        onPressed: {
            oldY = mouseArea.mouseY;
        }
        onReleased: {
            newY = mouseArea.mouseY;
            if (oldY - newY > 50)
            {
               changePages("loginPage");
            }
        }
    }

    Component.onCompleted: {
        flushTimeTimer.start();
        rootItem.updateLanguage.connect(updateLanguageSlot);
    }
}

