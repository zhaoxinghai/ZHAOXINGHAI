import QtQuick 2.0
import "../Components"

Rectangle {

    function messageBarShow(text)
    {
        messageText.text = text;
        messageBarHeightSmooth.from = 0;
        messageBarHeightSmooth.to = 40;
        messageBarHeightSmooth.start();
        messageBarTimer.start();
    }

    function messageBarHide()
    {
        messageBarHeightSmooth.from = 40;
        messageBarHeightSmooth.to = 0;
        messageBarHeightSmooth.start();
    }

    Text{
        id: messageText
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 20
        font.family: rootItem.fontName
        color: "#ef574f"
        font.pointSize: 12
        text:""
        width: parent.width - 20
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
        Component.onCompleted: {

        }
    }

    SmoothedAnimation{
        id:messageBarHeightSmooth
        target: messageBar
        property: "height"
        velocity: 200
        onStopped: {

        }
    }

    Timer{
        id: messageBarTimer
        interval : 3000
        repeat : false
        triggeredOnStart : false
        onTriggered: {
            messageBarHeightSmooth.from = 40;
            messageBarHeightSmooth.to = 0;
            messageBarHeightSmooth.start();
        }
    }
}

