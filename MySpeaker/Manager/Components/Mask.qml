import QtQuick 2.0

Rectangle{

    property bool passThrough: true

    function setVisibleValue(value)
    {
        visible = value;
    }

    signal clickedSignal();

    MouseArea{
        propagateComposedEvents: true

        anchors.fill: parent
        onClicked: {
            clickedSignal();
            mouse.accepted = passThrough;
        }
    }

    color: "#80cccccc"
}
