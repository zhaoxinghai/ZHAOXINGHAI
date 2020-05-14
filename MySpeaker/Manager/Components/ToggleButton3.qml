import QtQuick 2.0

Rectangle{

    property bool btnStatus: false
    property string borderSelectColor: "#00c767"
    property string borderUndoColor: "#c0c0c0"
    property string dataSelectColor: "#00c767"
    property string dataUndoColor: "#303030"
    property alias dataText: data.text
    property alias dataColor: data.color

    signal selectSignal();
    signal undoSignal();

    width: 140
    height: 60

    function setText(value)
    {
        dataText = value;
    }

    Text{
        id: data
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        font.family: rootItem.fontName
        font.pixelSize: 18
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
    }

    MouseArea{
        anchors.fill: parent
        onClicked: {
            selectSignal();
        }
    }
}
