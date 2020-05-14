import QtQuick 2.0

Rectangle{

    property bool btnStatus: false
    property string borderSelectColor: "#00c767"
    property string borderUndoColor: "#c0c0c0"
    property string dataSelectColor: "#00c767"
    property string dataUndoColor: "#303030"
    property alias dataText: data.text

    signal selectSignal();
    signal undoSignal();

    width: 140
    height: 60
    border.width: btnStatus ? 2 : 1
    border.color: btnStatus ? borderSelectColor : borderUndoColor

    Text{
        id: data
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        color: btnStatus ? dataSelectColor : dataUndoColor
        font.family: rootItem.fontName
        font.pixelSize: 18
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
    }

    MouseArea{
        anchors.fill: parent
        onClicked: {
            btnStatus = !btnStatus;
            if (btnStatus == true)
            {
                selectSignal();
            }
            else
            {
                undoSignal();
            }
        }
    }
}
