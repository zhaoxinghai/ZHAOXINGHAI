import QtQuick 2.0

Rectangle {

    property int prorityTitleHeight: 50
    property int prorityPathViewHeight: 250
    property int btnHeight: 60
    property int tmpIndex: 0

    signal okSignal();
    signal cancelSignal();

    function initStatus(index)
    {
        tmpIndex = index;
        setCurrentIndex(tmpIndex - 1);
        setVisibleValue(true);
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function setCurrentIndex(index)
    {
        prorityPathView.currentIndex = index;
    }

    function getCurrentIndex()
    {
        return prorityPathView.currentIndex;
    }

    function getProrityFromCurrentIndex()
    {
        return getCurrentIndex() + 1;
    }

    function updateLanguageSlot()
    {
        priorityTitle.text = langStrObject.GetLangString("ETCS_PRIORITY");
        cancelBtnText.text = langStrObject.GetLangString("ETCS_CANCEL");
        okBtnText.text = langStrObject.GetLangString("ETCS_OK");
    }

    Rectangle{
        id: prorityTitle
        width: parent.width
        height: prorityTitleHeight
        color: parent.color
        Text{
            id: priorityTitle
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_PRIORITY")
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
        anchors.top: prorityTitle.bottom
        color: "#c0c0c0"
        z: 1
    }

    Rectangle{
        z: 1
        width: parent.width
        height: 1
        anchors.top: prorityPathView.top
        anchors.topMargin: 100
        color: "#c0c0c0"
    }

    Rectangle{
        z: 1
        width: parent.width
        height: 1
        anchors.top: prorityPathView.top
        anchors.topMargin: 150
        color: "#c0c0c0"
    }

    Component{
        id: prorityPathDelegate
        Rectangle{
           property var textScale: PathView.itemScale
           property var textOpacity: PathView.itemOpacity

           width: 480
           height: 50
           color: "#ffffff"
           visible: PathView.onPath

           Text{
               text: index + 1
               anchors.horizontalCenter: parent.horizontalCenter
               anchors.verticalCenter: parent.verticalCenter
               color: textScale == 1.0 ? "#1792e5" :"#303030"
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
        id: prorityPathView
        width: parent.width
        height: prorityPathViewHeight
        anchors.left: parent.left
        anchors.top: prorityTitle.bottom
        pathItemCount: 5
        preferredHighlightBegin: 0.5
        preferredHighlightEnd: 0.5
        highlightRangeMode: PathView.StrictlyEnforceRange
        maximumFlickVelocity: 1200
        flickDeceleration: 800
        highlightMoveDuration: 0
        clip: true
        currentIndex: 249
        model: 250
        delegate: prorityPathDelegate

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

    }

    Rectangle{
        z: 1
        width: parent.width
        height: 1
        anchors.top: prorityPathView.bottom
        color: "#c0c0c0"
    }

    Rectangle{
        id: cancleBtn
        width: parent.width / 2
        height: btnHeight
        anchors.bottom: parent.bottom
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
                setCurrentIndex(tmpIndex - 1);
                cancelSignal();
            }
        }
    }

    Rectangle{
        id: okBtn
        width: parent.width / 2
        height: btnHeight
        anchors.bottom: parent.bottom
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
        rootItem.updateLanguage.connect(updateLanguageSlot);
    }
}
