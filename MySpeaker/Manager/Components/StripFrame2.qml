import QtQuick 2.0

Rectangle {

    property int topTextLeftMargin: 22
    property int bottomTextLeftMargin: 22
    property alias topInfo: topText.text
    property alias bottomInfo: bottomText.text
    property alias topTextColor: topText.color
    property alias bottomTextColor: bottomText.color
    property alias topTextWidth: topText.width
    property alias bottomTextWidth: bottomText.width
    property alias topTextStrikeOut: topText.font.strikeout
    property alias line1Visible: line1.visible
    property alias line2Visible: line2.visible

    width: 480
    height: 65
    color: "#ffffff"

    HorizontalLine{
        id: line1
        z: 1
        anchors.top: parent.top
        anchors.left: parent.left
    }

    HorizontalLine{
        id: line2
        z: 1
        anchors.bottom: parent.bottom
        anchors.left: parent.left
    }

    CommonText{
        id: topText
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: topTextLeftMargin
        font.pixelSize: 26
        font.strikeout: false
        color: "#303030"
        width: 200
    }

    CommonText{
        id: bottomText
        anchors.top: topText.bottom
        anchors.topMargin: -5
        anchors.left: parent.left
        anchors.leftMargin: bottomTextLeftMargin
        width: 380
    }
}
