import QtQuick 2.5

Rectangle {

    property string btnName: ""
    property string infoText: ""
    property bool isClickBtn: true
    property bool line1Visible: true
    property bool line2Visible: true
    property int line1Width: width
    property int line2Width: width
    property alias infoFont: info.font.family

    property bool btnClicked: false
    property string btnValue: ""

    property alias btnNameWidth: buttonName.width
    property alias btnNameContentWidth: buttonName.contentWidth
    property alias buttonNameWidth: buttonName.width
    property alias imgSrc: buttonImage.source
    property alias clickImgSrc: clickImg.source
    property alias infoTextContentWidth: info.contentWidth
    property int infoTextWidth: width - (buttonImage.width + 16) - (buttonName.width + 15) - (clickImg.width + 10) - 10

    property alias buttonImageWidth: buttonImage.width
    property alias buttonImageHeight: buttonImage.height

    signal btnStatusSignal(var status);
    signal btnValueSignal(var value);

    function setBtnEnableValue(value)
    {
        isClickBtn = value;
    }

    function setBtnClickImgSrc(src)
    {
        clickImgSrc = src;
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function setHeight(value)
    {
        height = value;
    }

    width: 480
    height: 65

    color: mouseArea.pressed ? "#dbe0e6" : "#ffffff"

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
        width: 150
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignLeft
    }

    Image{
        id: clickImg
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 20
        visible: true
    }

    Text {
        id: info
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: buttonName.right
        anchors.right: clickImg.left
        anchors.rightMargin: 5
        font.family: rootItem.fontName
        font.pixelSize: 18
        color: "#858585"
        text: infoText
        width: infoTextWidth
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignRight
    }

    Rectangle{
        z:  1
        anchors.bottom : parent.bottom
        width: line2Width
        height: 1
        color: "#e0e0e0"
        visible: line2Visible
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        enabled: isClickBtn
        onClicked: {
            btnClicked = !btnClicked;
            btnStatusSignal(btnClicked);
            btnValueSignal(btnValue);
        }
    }

    Component.onCompleted: {

    }
}

