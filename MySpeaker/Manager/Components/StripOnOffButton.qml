import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.4

Rectangle {

    property bool isEnable: false
    property alias btnNameWidth: buttonName.width
    property alias btnNameContentWidth: buttonName.contentWidth

    signal switchChange(var value);

    function setButtonNameText(text)
    {
        buttonName.text = text;
    }

    function getButtonNameText()
    {
        return buttonName.text;
    }

    function setLine1VisibleValue(value)
    {
        line1.visible = value;
    }

    function setLine1Width(value)
    {
        line1.width = value;
    }

    function setLine2VisibleValue(value)
    {
        line2.visible = value;
    }

    function setLine2Width(value)
    {
        line2.width = value;
    }

    width: 480
    height: 65
    color: "#ffffff"

    Rectangle{
        id: line1
        z:  1
        anchors.top: parent.top
        height: 1
        color: "#e0e0e0"
    }

    Text {
        id: buttonName
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 31
        font.family: rootItem.fontName
        font.pixelSize: 18
        color: "#303030"
        width: 200
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignLeft
        Component.onCompleted: {

        }
    }

    Rectangle{
        id: imageSwitch
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 30
        width: 60
        height: 63

        Image{
            id: switchImage
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            source: isEnable ? logicObject.getLocalImagePath() + "switch_on.png" : logicObject.getLocalImagePath() + "switch_off.png"
        }

        MouseArea{
            anchors.fill: parent
            onClicked: {
                if (isEnable === false)
                {
                    isEnable = true;
                }
                else
                {
                    isEnable = false;
                }
                switchChange(isEnable);
            }
        }
    }

    Rectangle{
        id: line2
        z:  2
        anchors.bottom : parent.bottom
        height: 1
        color: "#e0e0e0"
    }
}
