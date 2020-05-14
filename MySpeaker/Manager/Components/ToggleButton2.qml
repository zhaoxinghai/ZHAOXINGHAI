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
    border.width:1
    border.color: borderUndoColor

    function setSelectStatus()
    {
        border.width = 2;
        border.color = borderSelectColor;
        data.color = dataSelectColor;
    }

    function setUndoStatus()
    {
        border.width = 1;
        border.color = borderUndoColor;
        data.color = dataUndoColor;
    }

    function setDataText(value)
    {
        data.text = value;
    }

    Text{
        id: data
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        color: dataUndoColor
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
