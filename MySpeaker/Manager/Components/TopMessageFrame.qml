import QtQuick 2.0

Rectangle {

    property alias imageSource: mesImage.source
    property alias mesText: message.text

    signal messageHideSignal();

    function setText(value)
    {
        mesText = value;
    }

    function setImageSource(value)
    {
        imageSource = value;
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function messagePlay()
    {
        if (messageShowAnimation.running === false
                 && timer.running === false
                &&messageShowAnimation.running === false)
        {
            messageShowAnimation.start();
        }
    }

    width: 480
    height: 70
    border.width: 2
    border.color: "#1792e5"
    clip: true

    MouseArea{
        anchors.fill: parent
    }

    Rectangle{
        id: mesHeadRect
        width: 7
        height: 70
        color: "#1792e5"
    }

    Image {
        id: mesImage
        anchors.left: mesHeadRect.right
        anchors.leftMargin: 21
        anchors.verticalCenter: parent.verticalCenter
        width: 28
        height: 27
    }

    Text{
        id: message
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: mesImage.right
        anchors.leftMargin: 20
        color: "#303030"
        font.family: rootItem.fontName
        font.pixelSize: 14
        width: 350
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.Wrap
        maximumLineCount: 2
    }

    PropertyAnimation on height{
        id: messageShowAnimation
        from: 0
        to: 70
        duration: 0
        easing.type: Easing.InOutElastic
        easing.amplitude: 2.0
        easing.period: 1.5
        running: false
        onStopped: {
            timer.start();
        }
    }

    PropertyAnimation  on height{
        id: messageHideAnimation
        from: 70
        to: 0
        duration: 0
        easing.type: Easing.Linear
        easing.amplitude: 2.0
        easing.period: 1.5
        running: false
        onStopped: {
            messageHideSignal();
        }
    }

    Timer{
        id: timer
        interval: 3000
        repeat: false
        triggeredOnStart: false
        onTriggered: {
            messageHideAnimation.start();
        }
    }
}
