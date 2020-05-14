import QtQuick 2.0

Rectangle {

    function initStatus()
    {
        if (gpioObject.isScreenProtectorValid() === false)
        {
            return;
        }
        colorChangeTimer.start();
        setVisibleValue(true);
    }

    function quitStatus()
    {
        if (gpioObject.isScreenProtectorValid() === false)
        {
            return;
        }
        setVisibleValue(false);
        setColor("#000000")
        colorChangeTimer.stop();
    }

    function setColor(value)
    {
        color = value;
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    Timer{

        property int colorFlag: 0

        id: colorChangeTimer
        interval: 10 * 1000
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            if (colorFlag == 0)
            {
                setColor("#000000");
                colorFlag = 1;
            }
            else
            {
                setColor("#ffffff");
                colorFlag = 0;
            }
        }
    }
}
