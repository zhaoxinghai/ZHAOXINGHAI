import QtQuick 2.0
import "../Components"

Rectangle {

    property string textKey: ""
    property string backgroudColor: "#a0000000"
    property string textColor: "#ffffff"

    function setVisibleValue(value)
    {
        visible = value;
    }

    function updateLanguageSlot()
    {
        if (textKey != "")
        {
            setStatusBarMessageText(langStrObject.GetLangString(textKey), textKey);
        }
    }

    function setStatusBarStatus(index)
    {

    }

    function insertImg(img)
    {
        var flag = 0;

        for (var i = 0; i < listModel.count; i++)
        {
            if (listModel.get(i).imgURL === img)
            {
               flag = 1;
               break;
            }
        }

        if (flag == 0)
        {
            listModel.append({"imgURL": img});
        }
        else
        {

        }
    }

    function removeImg(img)
    {
        var ret;

        for (var i = 0; i < listModel.count; i++)
        {
            if (listModel.get(i).imgURL === img)
            {
               listModel.remove(i, 1);
               break;
            }
        }
    }

    function setStatusBarMessageText(text, key)
    {
        statusBarMessageText.text = text;
        logicObject.resetFaultIndex();
        textKey = key;
        textFlashingStart();
    }

    function setStatusBarMessageTextForTimer(text, key)
    {
        statusBarMessageText.text = text;
        logicObject.resetFaultIndex();
        textKey = key;
        textFlashingStart();
        clearTextTimer.start();
    }

    function setStatusBarMessageTextByFault(text, key)
    {
        statusBarMessageText.text = text;
        textKey = key;
        textFlashingStart();
    }

    function clearBarMessageTextFromTextKey(key)
    {
        if (textKey === key)
        {
            clearBarMessageText();
        }
    }

    function clearBarMessageText()
    {
        statusBarMessageText.text = "";
        logicObject.resetFaultIndex();
        textKey = "";
    }

    function textFlashingStart()
    {
        textFlashing.start();
    }

    function getTextKey()
    {
        return textKey;
    }

    color: backgroudColor

    Rectangle{
        id: timeTextRect
        width: 45
        height: parent.height
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        color: "#00000000"
        clip: true
        Text{
            id: timeText
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 3
            text: ""
            color: textColor
            font.family: rootItem.fontName
            font.bold: false
            font.pixelSize: 16
            width: parent.width
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
        }
    }

    Timer{
        id: timeTextTimer
        interval: 1000
        repeat: true
        onTriggered: {
            timeText.text = logicObject.getHourAndMinFromSystemTime();
        }
    }

    Timer{
        id: clearTextTimer
        interval: 5 * 1000
        repeat: false
        triggeredOnStart: false
        onTriggered: {
            clearBarMessageText();
        }
    }

    Rectangle{
        width: 480 - timeTextRect.width - listView.contentWidth - 10
        height: parent.height
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        color: "#00000000"
        Text{
            id: statusBarMessageText
            anchors.top: parent.top
            anchors.topMargin: 7
            anchors.left: parent.left
            anchors.leftMargin: 5
            width: parent.width
            height: parent.height
            text: ""
            color: textColor
            font.family: rootItem.fontName
            font.bold: false
            font.pixelSize: 16
            elide: Text.ElideRight
            Component.onCompleted: {

            }
        }
    }

    ListModel {
        id: listModel
    }

    ListView{
        id: listView
        width: 195
        height: parent.height
        anchors.right: timeTextRect.left
        anchors.rightMargin: 5
        anchors.verticalCenter: parent.verticalCenter
        boundsBehavior: Flickable.StopAtBounds
        clip: true
        orientation: ListView.Horizontal
        layoutDirection: Qt.RightToLeft

        add: Transition {
            SequentialAnimation{
                loops: 3
                NumberAnimation
                {
                    property: "opacity"
                    to: 0.0
                    duration: 500
                }
                NumberAnimation
                {
                    property: "opacity"
                    to: 1.0
                    duration: 500
                }
            }
        }

        model:listModel
        delegate:Rectangle{
                width: 26
                height: parent.height
                color: "#00000000"
                clip: true
                Image{
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: 25
                    height: 24
                    source: imgURL
                }
            }
    }

    SequentialAnimation{
        id: textFlashing
        running:false
        loops: 1
        NumberAnimation
        {
            target: statusBarMessageText
            property: "opacity"
            to: 0.0
            duration: 500
        }
        NumberAnimation
        {
            target: statusBarMessageText
            property: "opacity"
            to: 1.0
            duration: 500
        }
    }

    Component.onCompleted: {
        timeTextTimer.start();
        rootItem.updateLanguage.connect(updateLanguageSlot);
    }
}
