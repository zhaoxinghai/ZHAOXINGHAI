import QtQuick 2.0
import "../Components"

Rectangle {

    property int dayAndTimeTitleHeight: 66
    property int pathViewWidth: 480 / 5
    property int pathViewHeight: 220
    property int tailBackgroundHeight: height - dayAndTimeTitleHeight - pathViewWidth
    property int pathViewX: 480 / 10

    function updateLanguageSlot()
    {

    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function getVisibleValue()
    {
        return visible;
    }

    function getDayModel()
    {
        var ret;
        var month;
        var tmp;

        month = monthView.currentIndex;
        if (month === 0 ||
            month === 2 ||
            month === 4 ||
            month === 6 ||
            month === 7 ||
            month === 9 ||
            month === 11)
        {
            tmp = dayListModel.getListModelCount() - 31;

            if (tmp > 0)
            {
                dayListModel.remove(dayListModel.getListModelCount() - tmp , tmp);
            }
            else if (tmp < 0)
            {
                dayListModel.insertListModel(-tmp);
            }
        }
        else if (month === 3 || month === 5 || month === 8 || month === 10)
        {
            tmp = dayListModel.getListModelCount() - 30;

            if (tmp > 0)
            {
                dayListModel.remove(dayListModel.getListModelCount() - tmp, tmp);
            }
            else if (tmp < 0)
            {
                dayListModel.insertListModel(-tmp);
            }
        }
        else if (month === 1)
        {
            ret = isLeapYear();
            if (ret === 0)
            {
                tmp = dayListModel.getListModelCount() - 29;

                if (tmp > 0)
                {
                    dayListModel.remove(dayListModel.getListModelCount() - tmp, tmp);
                }
                else if (tmp < 0)
                {
                    dayListModel.insertListModel(-tmp);
                }
            }
            else
            {
                tmp = dayListModel.getListModelCount() - 28;

                if (tmp > 0)
                {
                    dayListModel.remove(dayListModel.getListModelCount() - tmp, tmp);
                }
                else if (tmp < 0)
                {
                    dayListModel.insertListModel(-tmp);
                }
            }
        } 
    }

    function createDayModel(month)
    {
        var ret;

        if (month === 0 ||
            month === 2 ||
            month === 4 ||
            month === 6 ||
            month === 7 ||
            month === 9 ||
            month === 11)
        {
            dayListModel.createListModel(31);
        }
        else if (month === 3 || month === 5 || month === 8 || month === 10)
        {
            dayListModel.createListModel(30);
        }
        else if (month === 1)
        {
            ret = isLeapYear();
            if (ret === 0)
            {
                dayListModel.createListModel(29);
            }
            else
            {
                dayListModel.createListModel(28);
            }
        }
    }

    function isLeapYear()
    {
        var year = yearView.currentIndex + 2018;
        if (((year % 4 == 0)&&(year % 100 != 0)) || (year % 100 == 0))
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }

    function getDayAndTime()
    {
        return logicObject.getDateAndTime(yearView.currentIndex + 2018,
                                   monthView.currentIndex + 1,
                                   dayView.currentIndex + 1,
                                   hourView.currentIndex,
                                   minView.currentIndex);
    }

    function updateSystemTime()
    {
        logicObject.setSystemTime(Number(yearView.currentIndex + 2018),
                                  Number(monthView.currentIndex + 1),
                                  Number(dayView.currentIndex + 1),
                                  Number(hourView.currentIndex),
                                  Number(minView.currentIndex));
    }

    function setTitleDayAndTime()
    {
        dayAndTime.text = getDayAndTime();
    }

    function setCurrentIndexs()
    {
        monthView.currentIndex = logicObject.getValueFromSystemTime("Month") - 1;
        getDayModel();
        dayView.currentIndex = logicObject.getValueFromSystemTime("Day") - 1;
        yearView.currentIndex = logicObject.getValueFromSystemTime("Year")  - 2018;
        hourView.currentIndex = logicObject.getValueFromSystemTime("Hour");
        minView.currentIndex = logicObject.getValueFromSystemTime("Min");
        setTitleDayAndTime();
    }

    color: "#ffffff"

    MouseArea{
        anchors.fill: parent
        onClicked: {

        }
    }

    Path{
        id: gPath
        startX: pathViewX
        startY: 22
        PathAttribute{name: "itemScale"; value: 0.4}
        PathAttribute{name: "itemOpacity"; value: 0.00}

        PathLine{x: pathViewX; y: 66}
        PathAttribute{name: "itemScale"; value: 0.6}
        PathAttribute{name: "itemOpacity"; value: 0.4}

        PathLine{x: pathViewX; y: 110}
        PathAttribute{name: "itemScale"; value: 1.0}
        PathAttribute{name: "itemOpacity"; value: 1.0}

        PathLine{x: pathViewX; y: 154}
        PathAttribute{name: "itemScale"; value: 0.6}
        PathAttribute{name: "itemOpacity"; value: 0.4}

        PathLine{x: pathViewX; y: 198}
        PathAttribute{name: "itemScale"; value: 0.4}
        PathAttribute{name: "itemOpacity"; value: 0.00}
    }

    Rectangle{
        id: dayAndTimeTitle
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width
        height: dayAndTimeTitleHeight
        color: "#d7d7d7"
        Text{
            id: dayAndTime
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 30
            color: "#00c767"
            text: getDayAndTime();
            font.family: rootItem.fontName
            font.pixelSize: 18
        }
    }

    Rectangle{
        z:  1
        anchors.top: monthView.top
        anchors.topMargin: 88
        width: parent.width
        height: 1
        color: "#e0e0e0"
        visible: true
    }

    Rectangle{
        z:  1
        anchors.top: monthView.top
        anchors.topMargin: 132
        width: parent.width
        height: 1
        color: "#e0e0e0"
        visible: true
    }

    Component{
        id: monthDelegate
        Rectangle{
           property var textScale: PathView.itemScale
           property var textOpacity: PathView.itemOpacity

           width: pathViewWidth
           height: 220 / 5
           color: "#00ffffff"
           visible: PathView.onPath

           Text{
               text: index + 1
               anchors.horizontalCenter: parent.horizontalCenter
               anchors.verticalCenter: parent.verticalCenter
               color: "#303030"
               font.family: rootItem.fontName
               font.bold: false
               font.pixelSize: 20
               scale: textScale
               opacity: textOpacity
           }
       }
    }

    PathView{
        id: monthView
        width: pathViewWidth
        height: pathViewHeight
        anchors.top: dayAndTimeTitle.bottom
        anchors.left: parent.left
        pathItemCount: 5
        preferredHighlightBegin: 0.5
        preferredHighlightEnd: 0.5
        highlightRangeMode: PathView.StrictlyEnforceRange
        maximumFlickVelocity: 1200
        flickDeceleration: 800
        clip: true
        model: 12
        delegate: monthDelegate
        path: gPath
        onMovementEnded: {
            getDayModel();
            setTitleDayAndTime();
            updateSystemTime();
        }
    }

    Component{
        id: dayDelegate
        Rectangle{
           property var textScale: PathView.itemScale
           property var textOpacity: PathView.itemOpacity

           width: pathViewWidth
           height: 220 / 5
           color: "#ffffff"
           visible: PathView.onPath

           Text{
               text: nIndex + 1
               anchors.horizontalCenter: parent.horizontalCenter
               anchors.verticalCenter: parent.verticalCenter
               color: "#303030"
               font.family: rootItem.fontName
               font.bold: false
               font.pixelSize: 20
               scale: textScale
               opacity: textOpacity
           }
       }
    }

    ListModel{

        function createListModel(count)
        {
            for (var i = 0; i < count; i++)
            {
                dayListModel.append({"nIndex": i});
            }
        }

        function insertListModel(count)
        {
            var i = 0;
            if (dayListModel.count <= 0)
            {
                dayListModel.index(0, 0);
                for (i = 1; i < count - 1; i++)
                {
                    dayListModel.insert(dayListModel.count, {"nIndex": dayListModel.count});
                }
            }
            else
            {
                for (i = 0; i < count; i++)
                {
                    dayListModel.insert(dayListModel.count, {"nIndex": dayListModel.count});
                }
            }
        }

        function removeListModel(pos, count)
        {
            if(pos < 0)
            {
                return;
            }

            dayListModel.remove(pos, count);
        }

        function clearListModel()
        {
           if (dayListModel.count > 0)
           {
                remove(0, dayListModel.count);
           }
        }

        function getListModelCount()
        {
            return dayListModel.count;
        }

        id: dayListModel
    }

    PathView{
        id: dayView
        anchors.top: monthView.top
        anchors.left: monthView.right
        width: pathViewWidth
        height: pathViewHeight
        pathItemCount: 5
        preferredHighlightBegin: 0.5
        preferredHighlightEnd: 0.5
        highlightRangeMode: PathView.StrictlyEnforceRange
        maximumFlickVelocity: 1200
        flickDeceleration: 800
        clip: true
        model: dayListModel
        delegate: dayDelegate
        path:gPath
        onMovementEnded: {
            setTitleDayAndTime();
            updateSystemTime();
        }
    }

    Component{
        id: yearDelegate
        Rectangle{
           property var textScale: PathView.itemScale
           property var textOpacity: PathView.itemOpacity

           width: pathViewWidth
           height: 220 / 5
           color: "#ffffff"
           visible: PathView.onPath

           Text{
               text: index + 2018
               anchors.horizontalCenter: parent.horizontalCenter
               anchors.verticalCenter: parent.verticalCenter
               color: "#303030"
               font.family: rootItem.fontName
               font.bold: false
               font.pixelSize: 20
               scale: textScale
               opacity: textOpacity
           }
       }
    }

    PathView{
        id: yearView
        anchors.top: monthView.top
        anchors.left: dayView.right
        width: pathViewWidth
        height: pathViewHeight

        pathItemCount: 5
        preferredHighlightBegin: 0.5
        preferredHighlightEnd: 0.5
        highlightRangeMode: PathView.StrictlyEnforceRange
        maximumFlickVelocity: 1200
        flickDeceleration: 800
        clip: true
        model: 19
        delegate: yearDelegate
        path: gPath
        onMovementEnded:  {
            getDayModel();
            setTitleDayAndTime();
            updateSystemTime();
        }
    }

    Component{
        id: hourDelegate
        Rectangle{
           property var textScale: PathView.itemScale
           property var textOpacity: PathView.itemOpacity

           width: pathViewWidth
           height: 220 / 5
           color: "#ffffff"
           visible: PathView.onPath

           Text{
               text: index
               anchors.horizontalCenter: parent.horizontalCenter
               anchors.verticalCenter: parent.verticalCenter
               color: "#303030"
               font.family: rootItem.fontName
               font.bold: false
               font.pixelSize: 20
               scale: textScale
               opacity: textOpacity
           }
       }
    }

    PathView{
        id: hourView
        anchors.top: monthView.top
        anchors.left: yearView.right
        width: pathViewWidth
        height: pathViewHeight
        pathItemCount: 5
        preferredHighlightBegin: 0.5
        preferredHighlightEnd: 0.5
        highlightRangeMode: PathView.StrictlyEnforceRange
        maximumFlickVelocity: 1200
        flickDeceleration: 800
        clip: true
        model: 24
        delegate: hourDelegate
        path: gPath
        onMovementEnded:  {
            setTitleDayAndTime();
            updateSystemTime();
        }
    }

    Component{
        id: minDelegate
        Rectangle{
           property var textScale: PathView.itemScale
           property var textOpacity: PathView.itemOpacity

           width: pathViewWidth
           height: 220 / 5
           color: "#ffffff"
           visible: PathView.onPath

           Text{
               text: index
               anchors.horizontalCenter: parent.horizontalCenter
               anchors.verticalCenter: parent.verticalCenter
               color: "#303030"
               font.family: rootItem.fontName
               font.bold: false
               font.pixelSize: 20
               scale: textScale
               opacity: textOpacity
           }
       }
    }

    PathView{
        id: minView
        anchors.top: monthView.top
        anchors.left: hourView.right
        width: pathViewWidth
        height: pathViewHeight
        pathItemCount: 5
        preferredHighlightBegin: 0.5
        preferredHighlightEnd: 0.5
        highlightRangeMode: PathView.StrictlyEnforceRange
        maximumFlickVelocity: 1200
        flickDeceleration: 800
        clip: true
        model: 60
        delegate: minDelegate
        path: gPath
        onMovementEnded: {
            setTitleDayAndTime();
            updateSystemTime();
        }
    }

    Rectangle{
        id: rect1
        width: parent.width
        height: tailBackgroundHeight
        anchors.top: monthView.bottom
        anchors.left: parent.left
        color: "#e7e7e7"
    }

    Timer{
        id: syncTimePathViewTimer
        interval: 1 * 1000
        repeat: true
        onTriggered: {          
            if (dayAndTimeSettingRect.visible == false)
            {
                setCurrentIndexs();
            }
        }
    }

    Component.onCompleted: {
        createDayModel(logicObject.getValueFromSystemTime("Month") - 1);
        setCurrentIndexs();
        syncTimePathViewTimer.start();
        rootItem.updateLanguage.connect(updateLanguageSlot);
    }
}
