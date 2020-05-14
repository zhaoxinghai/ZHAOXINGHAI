import QtQuick 2.0

Rectangle {

    signal cancelSignal();
    signal okSignal();

    property int cycleTitleHeight: 50
    property int cyclePathViewHeight: 250
    property int btnHeight: 60
    property int tmpIndex: 0
    property var forever: langStrObject.GetLangString("ETCS_FOREVER")

    function updateLanguageSlot()
    {
        cycleTitleText.text = langStrObject.GetLangString("ETCS_CYCLE");
        cancelBtnText.text = langStrObject.GetLangString("ETCS_CANCEL");
        okBtnText.text = langStrObject.GetLangString("ETCS_OK");
        setForever(langStrObject.GetLangString("ETCS_FOREVER"));
        cyclePathView.setModel(251);
    }

    function initStatus(index)
    {
        tmpIndex = index;
        setCurrentIndex(tmpIndex);
        setVisibleValue(true);
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function setCurrentIndex(index)
    {
        cyclePathView.currentIndex = index;
    }

    function getCurrentIndex()
    {
        return cyclePathView.currentIndex;
    }

    function getCycleValueFromCurrentIndex()
    {
        return getCurrentIndex();
    }

    function setForever(value)
    {
        forever = value;
    }

    function getForever()
    {
        return forever;
    }

    width: 480
    height: 360

    MouseArea{
        anchors.fill: parent
    }

    Rectangle{
        id: cycleTitle
        width: parent.width
        height: cycleTitleHeight
        color: parent.color
        Text{
            id: cycleTitleText
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_CYCLE")
            font.family: rootItem.fontName
            font.pixelSize: 22
            width: parent.width
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
        }
    }

    Rectangle{
        width: parent.width
        height: 1
        anchors.top: cycleTitle.bottom
        z: 1
        color: "#c0c0c0"
    }

    Rectangle{
        width: parent.width
        height: 1
        anchors.top: cyclePathView.top
        z: 1
        anchors.topMargin: 100
        color: "#c0c0c0"
    }

    Rectangle{
        width: parent.width
        height: 1
        anchors.top: cyclePathView.top
        anchors.topMargin: 150
        z: 1
        color: "#c0c0c0"
    }

    Component{
        id: cyclePathDelegate
        Rectangle{
           property var textScale: PathView.itemScale
           property var textOpacity: PathView.itemOpacity
           width: 480
           height: 50
           color: "#ffffff"
           visible: PathView.onPath

           Text{
               id: cyclePathItemText
               text: (index == 0) ? forever : index
               anchors.horizontalCenter: parent.horizontalCenter
               anchors.verticalCenter: parent.verticalCenter
               color: textScale == 1.0 ? "#1792e5" : "#303030"
               font.family: rootItem.fontName
               font.bold: false
               font.pixelSize: 30
               scale: textScale
               opacity: textOpacity
               width: parent.width
               elide: Text.ElideRight
               horizontalAlignment: Text.AlignHCenter
           }
       }
    }

    PathView{

        function setModel(value)
        {
            model = value;
        }

        id: cyclePathView
        width: parent.width
        height: cyclePathViewHeight
        anchors.left: parent.left
        anchors.top: cycleTitle.bottom
        pathItemCount: 5
        preferredHighlightBegin: 0.5
        preferredHighlightEnd: 0.5
        highlightRangeMode: PathView.StrictlyEnforceRange
        maximumFlickVelocity: 1200
        flickDeceleration: 800
        highlightMoveDuration: 0
        clip: true
        model: 251
        delegate: cyclePathDelegate

        path: Path{
            startX: 240
            startY: 25
            PathAttribute{name: "itemScale"; value: 0.2}
            PathAttribute{name: "itemOpacity"; value: 0.2}

            PathLine{x: 240; y: 75}
            PathAttribute{name: "itemScale"; value: 0.5}
            PathAttribute{name: "itemOpacity"; value: 0.5}

            PathLine{x: 240; y: 125}
            PathAttribute{name: "itemScale"; value: 1.0}
            PathAttribute{name: "itemOpacity"; value: 1.0}

            PathLine{x: 240; y: 175}
            PathAttribute{name: "itemScale"; value: 0.5}
            PathAttribute{name: "itemOpacity"; value: 0.5}

            PathLine{x: 240; y: 225}
            PathAttribute{name: "itemScale"; value: 0.2}
            PathAttribute{name: "itemOpacity"; value: 0.2}

        }

        onDragEnded: {

        }
    }

    Rectangle{
        width: parent.width
        height: 1
        anchors.top: cyclePathView.bottom
        z: 1
        color: "#c0c0c0"
    }

    Rectangle{
        width: 240
        height: btnHeight
        anchors.top: cyclePathView.bottom
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
                cancelSignal();
                setCurrentIndex(tmpIndex);
            }
        }
    }

    Rectangle{
        width: 240
        height: btnHeight
        anchors.top: cyclePathView.bottom
        anchors.left: parent.left
        anchors.leftMargin: 240
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
        setCurrentIndex(1);
        rootItem.updateLanguage.connect(updateLanguageSlot);
    }
}

