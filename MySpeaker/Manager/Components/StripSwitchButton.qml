import QtQuick 2.0

Rectangle {

    property string imgSrc: ""
    property string btnName: ""
    property bool line1Visible: true
    property bool line2Visible: true
    property int line1Width: width
    property int line2Width: width

    property bool btnClicked: false
    property string btnValue: ""

    property alias btnNameWidth: buttonName.width
    property alias btnNameContentWidth: buttonName.contentWidth

    signal btnStatusSignal(var status);
    signal btnValueSignal(var value);

    function setBtnClickedStatus(value)
    {
        btnClicked = value;
    }

    width: 480
    height: 65

    Rectangle{
        z:  1
        anchors.top: parent.top
        width: line1Width
        height: 1
        color: "#e0e0e0"
        visible: line1Visible
    }

    Image{
        id: buttonImage
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 16
        source: imgSrc
    }

    Text {
        id: buttonName
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: buttonImage.right
        anchors.leftMargin: 15
        font.family: rootItem.fontName
        font.pixelSize: 18
        color: "#303030"
        text: btnName
        width: parent.width - buttonImage.width - 16 - switchRect.width - 36
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignLeft
    }

    Rectangle{
        z:  1
        anchors.bottom : parent.bottom
        width: line2Width
        height: 1
        color: "#e0e0e0"
        visible: line2Visible
    }

    Rectangle{
        id: switchRect
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 36
        width: 60
        height: 65
        color: parent.color

        Rectangle{
            id: groove
            width: 44
            height: 8
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: btnClicked  == true ? "#a1d6b9" : "#d6d6d6"

            Rectangle{
                id: handle
                x: btnClicked  == true ? 29 : -15
                y: -11
                width: 30
                height: 30
                radius: height / 2
                color: btnClicked  == true ? "#14974f": "#ffffff"
                border.width: 1
                border.color: btnClicked  == true ? "#14974f": "#c4c4c4"
            }
        }

        MouseArea{
            anchors.fill: parent
            onClicked: {
                btnClicked = !btnClicked;
                btnStatusSignal(btnClicked);
            }
        }
    }
}
