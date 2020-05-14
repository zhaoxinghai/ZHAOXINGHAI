import QtQuick 2.0
import QtQml.Models 2.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Slider{

    function setValue(data)
    {
        value = data;
    }

    function getValue()
    {
        return value;
    }

    function enableSlider()
    {
        enabled = true;
    }

    function disableSlider()
    {
        enabled = false;
    }

    function setMinimumValue(value)
    {
        minimumValue = value;
    }

    function setMaximumValue(value)
    {
        maximumValue = value;
    }

    function setMinAndMax(min, max)
    {
        setMinimumValue(min);
        setMaximumValue(max);
    }

    width: 360
    height: 60
    style: sliderStyle
    stepSize: 1.0
    maximumValue: 100
    minimumValue: 0
    //updateValueWhileDragging: false

    Component {
        id: sliderStyle
        SliderStyle {
            id: ss
            handle: Rectangle{
                        width: 22
                        height: 60
                        color: "#00000000"
                        Rectangle {
                            id: activateItem
                            width: 22
                            height: 22
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            color: control.pressed ? "#dbe0e6" : "#ffffff"
                            border.color: "#d1d0d0"
                            antialiasing: true
                            radius: height / 2
                        }
            }

            groove: Rectangle {
                        id: grooveB
                        width: 360
                        height: 2
                        antialiasing: true
                        color: "#d0d0d0"
                        radius: height / 2
                        Rectangle {
                            width: styleData.handlePosition
                            height: parent.height
                            antialiasing: true
                            color: control.enabled ? "#5f5f5f" : "#d0d0d0"
                        }

            }
        }
    }

    Rectangle{
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        height: parent.height
        width: ((parent.value - parent.minimumValue) / (parent.maximumValue - parent.minimumValue)) * parent.width - 22
        color: "#00000000"
        MouseArea{
            anchors.fill: parent
            onClicked: {

            }
        }
    }
    Rectangle{
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        height: parent.height
        width: ((1- ((parent.value - parent.minimumValue) / (parent.maximumValue - parent.minimumValue))) * parent.width) - 22
        color: "#00000000"
        MouseArea{
            anchors.fill: parent
            onClicked: {

            }
        }
    }
}
