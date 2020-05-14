import QtQuick 2.0

Rectangle {

    property int gapTimeBtnHeight: 50
    property int viewHeight: 200 + 50
    property int okBtnHeight: 60

    property int tmpMin: 0
    property int tmpSec: 0

    signal cancelSignal();
    signal okSignal();

    function updateLanguageSlot()
    {
        gaptimeName.text = langStrObject.GetLangString("ETCS_GAPTIME");
        cancelBtnText.text = langStrObject.GetLangString("ETCS_CANCEL");
        okBtnText.text = langStrObject.GetLangString("ETCS_OK");
        minute.text = langStrObject.GetLangString("ETCS_MIN");
        second.text = langStrObject.GetLangString("ETCS_SECOND");
    }

    function initStatus(min, sec, type)
    {
        tmpMin = min;
        tmpSec = sec;
        setCurrentMinIndex(tmpMin);
        setCurrentSecondIndex(tmpSec);
        setTimeType(type);
        setVisibleValue(true);
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function setCurrentMinIndex(index)
    {
        minPathView.currentIndex = Number(index);
    }

    function getCurrentMinIndex()
    {
        return minPathView.currentIndex;
    }

    function setCurrentSecondIndex(index)
    {
        secondPathView.currentIndex = Number(index);
    }

    function getCurrentSecondIndex()
    {
        return secondPathView.currentIndex;
    }

    function getGapTimeFormMinAndSec()
    {
        return getCurrentMinIndex() * 60 + getCurrentSecondIndex();
    }

    function setTimeType(type)
    {
        /*
        if (type == true)
        {
            gapTimeBtn.setBtnValidStatus();
            intervalTimeBtn.resetBtnStatus();
        }
        else
        {
            gapTimeBtn.resetBtnStatus();
            intervalTimeBtn.setBtnValidStatus();
        }
        */
    }

    function getTimeType()
    {

        return true;
        /*
        var gapTimeStatus;
        var intervalTimeStatus;

        gapTimeStatus = gapTimeBtn.getClickStatus();
        //intervalTimeStatus = intervalTimeBtn.getClickStatus();
        if (gapTimeStatus == 1 && intervalTimeStatus == 0)
        {
            return true;
        }
        else if (gapTimeStatus == 0 && intervalTimeStatus == 1)
        {
            return false;
        }
        */
    }

    color: "#ffffff"

    width: 480
    height: 360

    MouseArea{
        anchors.fill: parent
    }

    RectButton2{
        id: gapTimeBtn
        width: parent.width
        height: gapTimeBtnHeight
        anchors.top: parent.top
        bkgPressedColor: "#00c767"
        bkgReleasedColor: "#ffffff"
        CommonText{
            id: gaptimeName
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_GAPTIME")
            font.family: rootItem.fontName
            font.pixelSize: 22
            width: parent.width
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
        }
        onClickedSignal: {

        }
    }

    HorizontalLine{
        z: 1
        anchors.top: minPathView.top
        anchors.topMargin: 100
        color: "#c0c0c0"
    }

    Text
    {
        id: minute
        z: 2
        anchors.verticalCenter: minPathView.verticalCenter
        anchors.horizontalCenter: minPathView.horizontalCenter
        anchors.horizontalCenterOffset: 50
        font.family: rootItem.fontName
        font.bold: false
        font.pixelSize: 18
        color: "#1792e5"
        text: langStrObject.GetLangString("ETCS_MIN")
        width: 80
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
    }

    Text
    {
        id: second
        z: 2
        anchors.verticalCenter: secondPathView.verticalCenter
        anchors.horizontalCenter: secondPathView.horizontalCenter
        anchors.horizontalCenterOffset: 50
        font.family: rootItem.fontName
        font.bold: false
        font.pixelSize: 18
        color: "#1792e5"
        text: langStrObject.GetLangString("ETCS_SECOND")
        width: 80
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
    }

    HorizontalLine{
        z: 1
        anchors.top: minPathView.top
        anchors.topMargin: 150
        color: "#c0c0c0"
    }

    Component{
        id: minPathDelegate
        Rectangle{
           property real minTextScale: PathView.itemScale
           property real minTextOpacity: PathView.itemOpacity

           width: 240
           height: minPathView.height / minPathView.pathItemCount
           color: "#ffffff"
           visible: PathView.onPath
           Text{
               text: index
               anchors.verticalCenter: parent.verticalCenter
               anchors.horizontalCenter: parent.horizontalCenter
               color: minTextScale == 1.0 ? "#1792e5" : "#303030"
               font.family: rootItem.fontName
               font.bold: false
               font.pixelSize: 30
               scale: minTextScale
               opacity: minTextOpacity
               width: parent.width
               elide: Text.ElideRight
               horizontalAlignment: Text.AlignHCenter
           }
       }
    }

    PathView{
        id: minPathView
        width: parent.width / 2
        height: viewHeight
        anchors.left: parent.left
        anchors.top: gapTimeBtn.bottom
        pathItemCount: 5
        preferredHighlightBegin: 0.5
        preferredHighlightEnd: 0.5
        highlightRangeMode: PathView.StrictlyEnforceRange
        maximumFlickVelocity: 1000
        flickDeceleration: 800
        highlightMoveDuration: 0
        clip: true
        model: 60
        delegate: minPathDelegate

        path: Path{

            startX: 120
            startY: 25
            PathAttribute{name: "itemScale"; value: 0.2}
            PathAttribute{name: "itemOpacity"; value: 0.2}

            PathLine{x: 120; y: 75}
            PathAttribute{name: "itemScale"; value: 0.5}
            PathAttribute{name: "itemOpacity"; value: 0.5}

            PathLine{x: 120; y: 125}
            PathAttribute{name: "itemScale"; value: 1.0}
            PathAttribute{name: "itemOpacity"; value: 1.0}

            PathLine{x: 120; y: 175}
            PathAttribute{name: "itemScale"; value: 0.5}
            PathAttribute{name: "itemOpacity"; value: 0.5}

            PathLine{x: 120; y: 225}
            PathAttribute{name: "itemScale"; value: 0.2}
            PathAttribute{name: "itemOpacity"; value: 0.2}

        }
    }

    Component{
        id: secondPathDelegate
        Rectangle{
           property real secondTextScale: PathView.itemScale
           property real secondTextOpacity: PathView.itemOpacity
           width: 240
           height: secondPathView.height / secondPathView.pathItemCount
           color: "#ffffff"
           visible: PathView.onPath
           Text{
               text: index
               anchors.verticalCenter: parent.verticalCenter
               anchors.horizontalCenter: parent.horizontalCenter
               color: secondTextScale == 1.0 ? "#1792e5" : "#303030"
               font.family: rootItem.fontName
               font.bold: false
               font.pixelSize: 30
               scale: secondTextScale
               opacity: secondTextOpacity
               width: parent.width
               elide: Text.ElideRight
               horizontalAlignment: Text.AlignHCenter
           }
       }
    }

    PathView{
        id: secondPathView
        width: parent.width/2
        height: viewHeight
        anchors.left: parent.left
        anchors.leftMargin: 240
        anchors.top: gapTimeBtn.bottom
        pathItemCount: 5
        preferredHighlightBegin: 0.5
        preferredHighlightEnd: 0.5
        highlightRangeMode: PathView.StrictlyEnforceRange
        maximumFlickVelocity: 1000
        flickDeceleration: 800
        highlightMoveDuration: 0
        clip: true
        model: 60
        delegate: secondPathDelegate

        path: Path{
            startX: 120
            startY: 25
            PathAttribute{name: "itemScale"; value: 0.2}
            PathAttribute{name: "itemOpacity"; value: 0.2}

            PathLine{x: 120; y: 75}
            PathAttribute{name: "itemScale"; value: 0.5}
            PathAttribute{name: "itemOpacity"; value: 0.5}

            PathLine{x: 120; y: 125}
            PathAttribute{name: "itemScale"; value: 1.0}
            PathAttribute{name: "itemOpacity"; value: 1.0}

            PathLine{x: 120; y: 175}
            PathAttribute{name: "itemScale"; value: 0.5}
            PathAttribute{name: "itemOpacity"; value: 0.5}

            PathLine{x: 120; y: 225}
            PathAttribute{name: "itemScale"; value: 0.2}
            PathAttribute{name: "itemOpacity"; value: 0.2}
        }
    }

    Rectangle{
        z: 1
        width: parent.width
        height: 1
        anchors.top: minPathView.bottom
        color: "#c0c0c0"
    }

    Rectangle{
        id: gapTimeCancelBtn
        width: parent.width/2
        height: okBtnHeight
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        color: cancelMouseArea.pressed ? "#DBE0E6" : "#ffffff"
        Text{
            id: cancelBtnText
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_CANCEL")
            font.family: rootItem.fontName
            font.pixelSize: 22
            width: parent.width
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
        }
        MouseArea {
            id: cancelMouseArea
            anchors.fill: parent
            onClicked: {
                setCurrentMinIndex(tmpMin);
                setCurrentSecondIndex(tmpSec);
                cancelSignal();
            }
        }
    }

    Rectangle{
        id: gapTimeOkBtn
        width: parent.width/2
        height: okBtnHeight
        anchors.left: parent.left
        anchors.leftMargin: 240
        anchors.bottom: parent.bottom
        color: okMouseArea.pressed ? "#DBE0E6" : "#ffffff"
        Text{
            id: okBtnText
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_OK")
            font.family: rootItem.fontName
            font.pixelSize: 22
            width: parent.width
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
        }
        MouseArea {
            id: okMouseArea
            anchors.fill: parent
            onClicked: {
                okSignal();
            }
        }
    }

    Component.onCompleted: {
        rootItem.updateLanguage.connect(updateLanguageSlot);
    }
}
