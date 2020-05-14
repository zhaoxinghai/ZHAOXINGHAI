import QtQuick 2.0

Rectangle {

    signal timerStop();

    function setMessage(text)
    {
        message.setText(text);
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function messageShow(text)
    {
        message.setText(text);
        setVisibleValue(true);
        showTimer.start();
    }

    Text{
        function setText(value)
        {
            text = value;
        }

        id: message
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        font.family: rootItem.fontName
        color: "#ef574f"
        font.pointSize: 12
        text: ""
        width: parent.width
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        Component.onCompleted: {

        }
    }

    Timer{
        id: showTimer
        interval : 2000
        repeat : false
        triggeredOnStart : false
        onTriggered: {
            setVisibleValue(false);
            timerStop();
        }
    }
}
