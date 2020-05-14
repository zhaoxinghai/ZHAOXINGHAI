import QtQuick 2.0
import "../Components"

Rectangle{

    property int restoreY: 0
    property int errorScope: 1
    property int spaceFromYAndHeight: 50
    property int targetWindowHeight: 800
    signal dragUnitIsvalid(var status)
    signal dragUnitStatusSignal(var status);
    signal clearStatusBarText()

    function setDragUnitEnableValue(value)
    {
        mouseArea.enabled = value;
    }

    function initTargetPageStatus()
    {
        pullDownPage.height = 0;
        dragUnit.y = 0;
    }

    x: 0
    y: -spaceFromYAndHeight

    MouseArea{
        id: mouseArea
        enabled: false
        anchors.fill: parent
        drag.target: dragUnit
        drag.axis : Drag.YAxis
        drag.threshold: 30

        onPressed: {    
            pullDownPage.setPullBtnStatus("Pressed");
        }

        onPositionChanged: {
            if (dragUnit.y < -spaceFromYAndHeight)
            {

            }
            else if (dragUnit.y > targetWindowHeight - spaceFromYAndHeight)
            {

            }
            else if ((dragUnit.y >= -spaceFromYAndHeight) && (dragUnit.y <= targetWindowHeight - spaceFromYAndHeight))
            {
                restoreY = pullDownPage.height - spaceFromYAndHeight;
                pullDownPage.height = dragUnit.y + spaceFromYAndHeight;
            }
        }
        onReleased: {
            if (dragUnit.y >= restoreY - errorScope && dragUnit.y  <= restoreY + errorScope)
            {
                    if (pullDownPage.height > 0 && pullDownPage.height < targetWindowHeight/2)
                    {
                        smoothHeight.from = pullDownPage.height;
                        smoothHeight.to = 0;
                        smoothHeight.start();

                        smoothY.from = dragUnit.y;
                        smoothY.to = 0 - spaceFromYAndHeight;
                        smoothY.start();
                        pullDownPage.setIsActiveUpdateValue(false);
                        pullDownPage.setIsRecentUpdateValue(false);
                    }
                    else if (pullDownPage.height >= (targetWindowHeight/2) && pullDownPage.height <= targetWindowHeight)
                    {
                        smoothHeight1.from = pullDownPage.height;
                        smoothHeight1.to = targetWindowHeight;
                        smoothHeight1.start();

                        smoothY.from = dragUnit.y;
                        smoothY.to = targetWindowHeight - spaceFromYAndHeight;
                        smoothY.start();

                        statusBar.clearBarMessageText();
                    }
            }
            else if ( dragUnit.y < restoreY - errorScope)
            {
                smoothHeight.from = pullDownPage.height;
                smoothHeight.to = 0;
                smoothHeight.start();

                smoothY.from = dragUnit.y;
                smoothY.to = 0 - spaceFromYAndHeight;
                smoothY.start();
                pullDownPage.setIsActiveUpdateValue(false);
                pullDownPage.setIsRecentUpdateValue(false);
            }
            else if (dragUnit.y > errorScope + restoreY)
            {
                smoothHeight1.from = pullDownPage.height;
                smoothHeight1.to = targetWindowHeight;
                smoothHeight1.start();

                smoothY.from = dragUnit.y;
                smoothY.to = targetWindowHeight - spaceFromYAndHeight;
                smoothY.start();

                statusBar.clearBarMessageText();
            }
            pullDownPage.setPullBtnStatus("Released");
        }
        onClicked: {
            smoothHeight.from = pullDownPage.height;
            smoothHeight.to = 0;
            smoothHeight.start();

            smoothY.from = dragUnit.y;
            smoothY.to = 0 - spaceFromYAndHeight;
            smoothY.start();
            pullDownPage.setIsActiveUpdateValue(false);
            pullDownPage.setIsRecentUpdateValue(false);
        }
    }

    SmoothedAnimation{
        id:smoothHeight
        target: pullDownPage
        property: "height"
        velocity: 1000
        duration: 300
        onStopped: {
            if (pullDownPage.height == 0)
            {
                pullDownPage.setPullBtnImgStatus("up");
            }
            else
            {
                pullDownPage.setPullBtnImgStatus("down");
            }
        }
    }

    SmoothedAnimation{
        id:smoothHeight1
        target: pullDownPage
        property: "height"
        velocity: 1000
        duration: 300
        onStopped: {
            if (pullDownPage.height == 0)
            {
                pullDownPage.setPullBtnImgStatus("up");
            }
            else
            {
                pullDownPage.setPullBtnImgStatus("down");
            }
        }
    }

    SmoothedAnimation{
        id:smoothY
        target: dragUnit
        property: "y"
        velocity: 1000
        duration: 300
        onStopped: {

        }
    }

    Component.onCompleted: {
        setDragUnitEnableValue(false);
    }
}
