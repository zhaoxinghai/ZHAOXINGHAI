import QtQuick 2.0
import QtQml.Models 2.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

import "../Components"

Rectangle {

    id: rect

    function initStatus()
    {
        logicObject.gpioTest(true);
        gpioObject.openBeep();
        colorTimer.stop();
        colorTimer.start();
        colorTimer.setIndex(0);
        selfTestTimer.start();
        setVisibleValue(true);
    }

    function quitStatus()
    {
        logicObject.gpioTest(false);
        gpioObject.closeBeep();
        colorTimer.stop();
        colorTimer.setIndex(0);
        selfTestTimer.stop();
        setVisibleValue(false);
    }

    function selfTestSlot(isActived)
    {
        if (true === isActived)
        {
            initStatus();
        }
        else
        {
            quitStatus();
        }
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function setColor(value)
    {
        color = value;
    }

    Timer{

        property int index: 0

        function setIndex(value)
        {
            index = value;
        }

        id: colorTimer
        interval: 800
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            if (index === 0)
            {
                setColor("#ffffff");
            }
            else if (index === 1)
            {
                setColor("#ff0000");
            }
            else if (index === 2)
            {
                setColor("#00ff00");
            }
            else if (index === 3)
            {
                setColor("#0000ff");
            }
            else if (index === 4)
            {
                setColor("#000000");
                index = 0;
                return ;
            }

            index++;
        }
    }

    Timer{
        id: selfTestTimer
        interval: 60 * 1000
        repeat: false
        triggeredOnStart: false
        onTriggered: {
            quitStatus();
        }
    }

    MouseArea{
        anchors.fill: parent
        onClicked: {
            quitStatus();
            logicObject.lampTest(false);
        }
    }
    Component.onCompleted: {
        logicObject.selfTestSignal.connect(selfTestSlot);
    }
}
