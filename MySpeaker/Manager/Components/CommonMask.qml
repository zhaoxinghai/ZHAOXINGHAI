import QtQuick 2.0

Rectangle{

    property bool imageVisible:image.visible

    signal clickedSignal();

    function setVisibleValue(value)
    {
        visible = value;
    }

    function setImageVisibleValue(value)
    {
        imageVisible = value;
    }

    width: 480
    height: 800
    color: "#70666666"

    AnimatedImage{
        id: image
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        source: logicObject.getLocalImagePath() + "waiting_1.gif"
        playing: true
    }

    MouseArea{
        anchors.fill: parent
        onClicked: {
            clickedSignal();
        }
    }
}
