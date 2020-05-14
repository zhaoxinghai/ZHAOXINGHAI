import QtQuick 2.0

import QtQuick 2.0

Rectangle {

    property bool clickStatus0: false
    property bool clickStatus1: false
    property bool clickStatus2: false
    property bool clickStatus3: false
    property int currentIndex: 0


    property int preSignalTitleHeight: 50
    property int preSignalsHeight: 50
    property int btnHeight: 60

    signal okSignal();
    signal cancelSignal();

    function initStatus(index)
    {
        setVisibleValue(true);
        setClickStatus(index);
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function setCurrentIndex(index)
    {
        currentIndex = index;
    }

    function getCurrentIndex()
    {
        return currentIndex;
    }

    function updateLanguageSlot()
    {
        preSignalTitleText.text = langStrObject.GetLangString("ETCS_PRESIGNAL");
        preSignalNone.text = langStrObject.GetLangString("ETCS_NULL");
        preSignal1.text = langStrObject.GetLangString("ETCS_SIGNAL_1");
        preSignal2.text = langStrObject.GetLangString("ETCS_SIGNAL_2");
        preSignal3.text = langStrObject.GetLangString("ETCS_SIGNAL_3");
        okBtnText.text = langStrObject.GetLangString("ETCS_OK");
    }

    function setClickStatus(index)
    {
        switch(index)
        {
        case 0:
            clickStatus0 = true;
            clickStatus1 = false;
            clickStatus2 = false;
            clickStatus3 = false;
            break;
        case 1:
            clickStatus0 = false;
            clickStatus1 = true;
            clickStatus2 = false;
            clickStatus3 = false;
            break;
        case 2:
            clickStatus0 = false;
            clickStatus1 = false;
            clickStatus2 = true;
            clickStatus3 = false;
            break;
        case 3:
            clickStatus0 = false;
            clickStatus1 = false;
            clickStatus2 = false;
            clickStatus3 = true;
            break;
        default:
            break
        }

        setCurrentIndex(index);
    }

    Rectangle{
        id: preSignalTitle
        width: parent.width
        height: preSignalTitleHeight
        anchors.top: parent.top
        color: "#ffffff"
        Text{
            id: preSignalTitleText
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_PRESIGNAL")
            font.family: rootItem.fontName
            font.pixelSize: 22
            width: parent.width
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
        }
    }

    Rectangle{
        z: 1
        width: parent.width
        height: 1
        anchors.top: preSignalTitle.bottom
        color: "#c0c0c0"
    }

    Rectangle{
        id: zeroSignal
        width: parent.width
        height: preSignalsHeight
        anchors.top: parent.top
        anchors.topMargin: 50
        color: clickStatus0 == true ? "#00c767" : parent.color
        Text{
            id: preSignalNone
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_NULL")
            font.family: rootItem.fontName
            font.pixelSize: 22
            width: parent.width
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                setClickStatus(0);
            }
        }
    }

    Rectangle{
        z: 1
        width: parent.width
        height: 1
        anchors.top: zeroSignal.bottom
        color: "#c0c0c0"
    }

    Rectangle{
        id: oneSignal
        width: parent.width
        height: preSignalsHeight
        anchors.top: parent.top
        anchors.topMargin: 100
        color: clickStatus1 == true ? "#00c767" : parent.color
        Text{
            id: preSignal1
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_SIGNAL_1")
            font.family: rootItem.fontName
            font.pixelSize: 22
            width: parent.width
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                setClickStatus(1);
            }
        }
    }
    Rectangle{
        z: 1
        width: parent.width
        height: 1
        anchors.top: oneSignal.bottom
        color: "#c0c0c0"
    }

    Rectangle{
        id: twoSignal
        width: parent.width
        height: preSignalsHeight
        anchors.top: parent.top
        anchors.topMargin: 150
        color: clickStatus2 == true ? "#00c767" : parent.color
        Text{
            id: preSignal2
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_SIGNAL_2")
            font.family: rootItem.fontName
            font.pixelSize: 22
            width: parent.width
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                setClickStatus(2);
            }
        }
    }
    Rectangle{
        z: 1
        width: parent.width
        height: 1
        anchors.top: twoSignal.bottom
        color: "#c0c0c0"
    }

    Rectangle{
        id: threeSignal
        width: parent.width
        height: preSignalsHeight
        anchors.top: parent.top
        anchors.topMargin: 200
        color: clickStatus3 == true ? "#00c767" : parent.color
        Text{
            id: preSignal3
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_SIGNAL_3")
            font.family: rootItem.fontName
            font.pixelSize: 22
            width: parent.width
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                setClickStatus(3);
            }
        }
    }
    Rectangle{
        z: 1
        width: parent.width
        height: 1
        anchors.top: threeSignal.bottom
        color: "#c0c0c0"
    }

    Rectangle{
        z: 1
        width: parent.width
        height: 1
        anchors.bottom: okBtn.top
        color: "#c0c0c0"
    }

    Rectangle{
        id: okBtn
        width: parent.width
        height: btnHeight
        anchors.bottom: parent.bottom
        color: okMouseArea.pressed ? "#DBE0E6" : "#ffffff"
        Text{
            id: okBtnText
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_OK")
            font.family: rootItem.fontName
            font.pixelSize: 22
            width: parent.width
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
        }
        MouseArea {
            id: okMouseArea
            anchors.fill: parent
            onClicked: {
                okSignal();
            }
        }
    }

    Component.onCompleted: {
        rootItem.updateLanguage.connect(updateLanguageSlot);
    }
}
