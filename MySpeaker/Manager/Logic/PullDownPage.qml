import QtQuick 2.0
import QtQml.Models 2.2
import "../Components"

Rectangle {

    property int gid: 0
    property int baseHeight: 0
    property int btnBackgroundHeight: 80
    property int btnBackgroundTopMargin: 26
    property int viewHeight: logicObject.getRealHeight() - btnBackgroundHeight - btnBackgroundTopMargin - pullBtnHeight
    property int pullBtnHeight: 50
    property int clearBtnHeight: 50
    property bool isActiveUpdate: false
    property bool isRecentUpdate: false

    function updateLanguageSlot()
    {
        activateBtnName.text = langStrObject.GetLangString("ETCS_ACTIVATE");
        faultBtnName.text = langStrObject.GetLangString("ETCS_FAULT");
        historyBtnName.text = langStrObject.GetLangString("ETCS_RECENT");
        activateTimeTitle.text = langStrObject.GetLangString("ETCS_TIME");
        activateNameTitle.text = langStrObject.GetLangString("ETCS_NAME");
        activateStatusTitle.text = langStrObject.GetLangString("ETCS_STATUS");
        activateZoneTitle.text = langStrObject.GetLangString("ETCS_ZONE");
        faultTimeTitle.text = langStrObject.GetLangString("ETCS_TIME");
        faultDeviceTitle.text = langStrObject.GetLangString("ETCS_DEVICE");
        faultDescriptionTitle.text = langStrObject.GetLangString("ETCS_DESCRIPTION");
        beepOffBtnName.setText(langStrObject.GetLangString("ETCS_ACKNOWLEDGE"));
        historyTimeTitle.text = langStrObject.GetLangString("ETCS_TIME");
        historyNameTitle.text = langStrObject.GetLangString("ETCS_SOURCE");
        historyZoneTitle.text = langStrObject.GetLangString("ETCS_ZONE");
        updateActivate();
        updateHistorySlot();
        updateFault();
    }

    function changeUserSlot(value)
    {
        if (value === 0)
        {
            updateHistorySlot();
        }
        else if (value === 1)
        {
            updateHistorySlot();
        }
        else if (value === 2)
        {
            updateHistorySlot();
        }
    }

    function okSlot()
    {
        if (activateView.visible === true)
        {
            logicObject.clearActivateListModel();
            logicObject.clearAllActivate();
            pullDownPage.setClearBtnVisibleValue(false);
            pullDownPage.setClearBtnTopLineVisibleValue(false);
        }
        else if (historyView.visible === true)
        {
            historyObject.CleanRecent();
            updateHistorySlot();
        }
    }

    function updateHistorySlot()
    {
        historyListModel.clearListModel();
        historyListModel.createListModel();

        if (historyListModel.getListModelCount() > 0)
        {
            historyClearBtnTopLine.setVisibleValue(true);
            historyClearBtn.setVisibleValue(true);
        }
        else
        {
            historyClearBtnTopLine.setVisibleValue(false);
            historyClearBtn.setVisibleValue(false);
        }
    }

    function updateActivate()
    {
        logicObject.clearActivateListModel();
        logicObject.createActivateListModel();

        if (logicObject.getActivateCount() > 0)
        {
            pullDownPage.setClearBtnVisibleValue(true);
            clearBtnTopLine.setVisibleValue(true);
        }
        else
        {
            pullDownPage.setClearBtnVisibleValue(false);
            clearBtnTopLine.setVisibleValue(false);
        }
    }

    function updateFault()
    {
        logicObject.clearFaultListModel();
        logicObject.createFaultListModel();
    }

    function setPullBtnStatus(status)
    {
        if (status === "Pressed")
        {
            pullBtn.color = "#606060"
        }
        else if (status === "Released")
        {
            pullBtn.color = "#303030"
        }
    }

    function setPullBtnImgStatus(value)
    {
        if (value === "up")
        {
            pullBtnImg.rotation = 0;
        }
        else if (value === "down")
        {
           pullBtnImg.rotation = 180;
        }
    }

    function changeView(name)
    {
        if (name === "Activate")
        {
            activateBtn.color = "#d8d8d8";
            activateBtnName.color = "#303030";
            faultBtn.color = "#595959";
            faultBtnName.color = "#bbbbbb";
            historyBtn.color = "#595959";
            historyBtnName.color = "#bbbbbb";
            activateView.visible = true;
            faultView.visible = false;
            historyView.visible = false;  
        }
        else if(name === "Fault")
        {
            activateBtn.color = "#595959";
            activateBtnName.color = "#bbbbbb";
            faultBtn.color = "#d8d8d8";
            faultBtnName.color = "#303030";
            historyBtn.color = "#595959";
            historyBtnName.color = "#bbbbbb";
            activateView.visible = false;
            faultView.visible = true;
            historyView.visible = false;
        }
        else if(name === "History")
        {
            activateBtn.color = "#595959";
            activateBtnName.color = "#bbbbbb";
            faultBtn.color = "#595959";
            faultBtnName.color = "#bbbbbb";
            historyBtn.color = "#d8d8d8";
            historyBtnName.color = "#303030";
            activateView.visible = false;
            faultView.visible = false;
            historyView.visible = true;
        }
    }

    function setClearBtnVisibleValue(value)
    {
        clearBtn.setVisibleValue(value);
    }

    function setClearBtnTopLineVisibleValue(value)
    {
        clearBtnTopLine.setVisibleValue(value);
    }

    function setIsActiveUpdateValue(value)
    {
        isActiveUpdate = value;
    }

    function setIsRecentUpdateValue(value)
    {
        isRecentUpdate = value;
    }

    function activateAddSlot()
    {
        activateUpdateTimer.restart();
    }

    function addHistorySlot(index)
    {
        histroyUpdateTimer.restart();
    }

    function noActivateSlot()
    {
        clearBtnTopLine.setVisibleValue(false);
        clearBtn.setVisibleValue(false);
    }

    function setBeepOffBtnStatusSlot(value)
    {
        if (value === true)
        {
            beepOffBtn.setVisibleValue(true)
            beepOffTopLine.setVisibleValue(true);
        }
        else
        {
            beepOffBtn.setVisibleValue(false)
            beepOffTopLine.setVisibleValue(false);
        }
    }

    function mapStatusToStatusName(eStatus)
    {
        if (eStatus === "wait")
        {
            return langStrObject.GetLangString("ETCS_WAIT")
        }
        else if (eStatus === "full")
        {
            return langStrObject.GetLangString("ETCS_FULL_CONNECT")
        }
        else if (eStatus === "part")
        {
            return langStrObject.GetLangString("ETCS_PARTLY_CONNECT")
        }
        else if (eStatus === "disconnect")
        {
            return langStrObject.GetLangString("ETCS_DISCONNECT")
        }
        else
        {
            return "";
        }
    }

    function mapErrorToErrorDesc(e_key,e_num,e_ch)
    {
        return langStrObject.GetLangString(e_key,e_num,e_ch);
    }
    function mapNameToNameText(e_name,e_local)
    {
        if(e_local)
        {
            return langStrObject.GetLangString("ETCS_LO");
        }
        else
        {
            return e_name;
        }
    }
    color: "#303030"

    MouseArea{
        anchors.fill: parent
        onClicked: {

        }
    }

    Rectangle{
        id: activateBtnBackground 
        anchors.top: parent.top
        anchors.topMargin: btnBackgroundTopMargin
        anchors.left: parent.left
        anchors.leftMargin: 30
        width: 140
        height: btnBackgroundHeight
        color: "#00000000"
        Rectangle{
            id: activateBtn
            width: parent.width
            height: 35
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#d8d8d8"
            Text {
                id: activateBtnName
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                font.family: rootItem.fontName
                font.bold: false
                font.pixelSize: 22
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_ACTIVATE")
                width: parent.width
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignHCenter
                Component.onCompleted: {
                }
            }
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                changeView("Activate");
            }
        }
    }
    Rectangle{
        id: faultBtnBackground
        width: 140
        height: btnBackgroundHeight
        anchors.top: parent.top
        anchors.topMargin: btnBackgroundTopMargin
        anchors.left: activateBtnBackground.right
        color: parent.color
        Rectangle{
            id: faultBtn
            width: parent.width
            height: 35
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#595959"

            Text {
                id: faultBtnName
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                font.family: rootItem.fontName
                font.bold: false
                font.pixelSize: 22
                color: "#d8d8d8"
                text: langStrObject.GetLangString("ETCS_FAULT")
                width: parent.width
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignHCenter
                Component.onCompleted: {
                }
            }
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                changeView("Fault");
            }
        }
    }
    Rectangle{
        id: historyBtnBackground
        width: 140
        height: btnBackgroundHeight
        anchors.top: parent.top
        anchors.topMargin: btnBackgroundTopMargin
        anchors.left: faultBtnBackground.right
        color: parent.color
        Rectangle{
            id: historyBtn
            width: parent.width
            height: 35
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#595959"

            Text {
                id: historyBtnName
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                font.family: rootItem.fontName
                font.bold: false
                font.pixelSize: 22
                color: "#d8d8d8"
                text: langStrObject.GetLangString("ETCS_RECENT")
                width: parent.width
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignHCenter
                Component.onCompleted: {
                }
            }
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                changeView("History");
            }
        }
    }

    Rectangle{

        function setVisibleValue()
        {
            visible = value;
        }

        function getVisibleValue()
        {
            return visible;
        }

        id: activateView
        anchors.top: parent.top
        anchors.topMargin: 106
        anchors.left: parent.left
        width: parent.width
        height: viewHeight
        visible: true
        color: "#303030"
        Rectangle{
            id: activateViewTitle
            width: parent.width
            height: 40
            color: "#232323"
            Rectangle{
                id: activateTimeTitleRect
                width: 80
                height: parent.height - 2
                color: parent.color
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 25
                clip: true
                Text{
                    id: activateTimeTitle
                    text: langStrObject.GetLangString("ETCS_TIME")
                    width: parent.width
                    height: parent.height
                    anchors.verticalCenter:parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: rootItem.fontName
                    font.bold: false
                    font.pixelSize: 20
                    color: "#dedede"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    Component.onCompleted: {
                    }
                }
            }
            Rectangle{
                id: activateNameTitleRect
                width: 90
                height: parent.height - 2
                color: parent.color
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: activateTimeTitleRect.right
                anchors.leftMargin: 5
                clip: true
                Text{
                    id: activateNameTitle
                    text: langStrObject.GetLangString("ETCS_NAME")
                    width: parent.width
                    height: parent.height
                    anchors.verticalCenter:parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: rootItem.fontName
                    font.bold: false
                    font.pixelSize: 20
                    color: "#dedede"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    Component.onCompleted: {
                    }
                }
            }
            Rectangle{
                id: activateStatusTitleRect
                width: 80
                height: parent.height - 2
                color: parent.color
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: activateNameTitleRect.right
                anchors.leftMargin: 5
                clip: true
                Text{
                    id: activateStatusTitle
                    text: langStrObject.GetLangString("ETCS_STATUS")
                    width: parent.width
                    height: parent.height
                    anchors.verticalCenter:parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: rootItem.fontName
                    font.bold: false
                    font.pixelSize: 20
                    color: "#dedede"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    Component.onCompleted: {
                    }
                }
            }
            Rectangle{
                id: activateZoneTitleRect
                width: 130
                height: parent.height - 2
                color: parent.color
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: activateStatusTitleRect.right
                anchors.leftMargin: 5
                clip: true
                Text{
                    id: activateZoneTitle
                    text: langStrObject.GetLangString("ETCS_ZONE")
                    width: parent.width
                    height: parent.height
                    anchors.verticalCenter:parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: rootItem.fontName
                    font.bold: false
                    font.pixelSize: 20
                    color: "#dedede"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    Component.onCompleted: {
                    }
                }
            }
        }

        ListView{
            id: listView
            width: parent.width
            height: parent.height - activateViewTitle.height - clearBtn.height - 10
            anchors.top: activateViewTitle.bottom
            focus:true
            clip:true
            boundsBehavior: Flickable.StopAtBounds
            snapMode: ListView.NoSnap
            model: activateViewVisualModel
        }

        Rectangle{

            function setVisibleValue(value)
            {
                visible = value;
            }

            id: clearBtnTopLine
            width: parent.width
            height: 1
            anchors.bottom: listView.bottom
            color: "#595959"
            z: 1
        }

        ImageButton{
            id: clearBtn
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            width: 63
            height: 60
            buttonImageSource: logicObject.getLocalImagePath() + "recycle_1.png"
            bkgPressedColor: "#a0505050"
            bkgReleasedColor: "#a0606060"
            radius: height / 2
            visible: false
            onClickedSignal: {
                clearConfirmMask.setVisibleValue(true);
            }
        }

        DelegateModel {
            id: activateViewVisualModel

            property alias selectedItems: activateViewGp
            groups: [
                DelegateModelGroup{
                    id: activateViewGp
                    name: "selected"
                }
            ]

            model: activateModel
            delegate:Rectangle{
                    id: activateItem
                    width: 480
                    height: 60
                    color: "#303030"

                    SequentialAnimation{

                        id: activateClickEffect
                        running: false
                        ColorAnimation{
                            target: activateItem
                            property: "color"
                            from: "#303030"
                            to: "#505050"
                            duration: 50
                        }
                        ColorAnimation {
                            target: activateItem
                            property: "color"
                            from: "#505050"
                            to: "#303030"
                            duration: 100
                        }

                        onStopped: {
                            detailedInfo.show(e_id);
                        }
                    }

                    Rectangle{
                        anchors.top: parent.top
                        anchors.left: parent.left
                        width: parent.width
                        height: 1
                        color: "#3a3a3a"
                        z: 2
                        visible: true
                    }
                    Rectangle{
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        width: parent.width
                        height: 1
                        color: "#3a3a3a"
                        z: 2
                        visible: index == activateViewVisualModel.count - 1 ? true : false
                    }

                    Image{
                        id: isPlayingImage
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 5
                        source:logicObject.getLocalImagePath() + "playing.png"
                        visible: (e_status == "full" || e_status == "part") ? true : false
                    }
                    Rectangle{
                        id: timeTextRect
                        width: 80
                        height: parent.height
                        color: parent.color
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: isPlayingImage.right
                        anchors.leftMargin: 5
                        clip: true
                        Text{
                            text: e_time
                            width: parent.width
                            height: parent.height
                            anchors.verticalCenter:parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            color: e_emergency == true ? "#ee3124" : "#dfdfdf"
                            font.family: rootItem.fontName
                            font.bold: e_emergency == true ? true : false
                            font.pixelSize:18
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.Wrap
                            maximumLineCount: 2
                            elide: Text.ElideRight
                        }
                    }
                    Rectangle{
                        id: nameTextRect
                        width: 90
                        height: parent.height
                        color: parent.color
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: timeTextRect.right
                        anchors.leftMargin: 5
                        clip: true
                        Text{
                            text: e_name === "Prelisten" ? langStrObject.GetLangString("ETCS_PRELISTEN") : e_name
                            width: parent.width
                            height: parent.height
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            color: e_emergency == true ? "#ee3124" : "#dfdfdf"
                            font.family: rootItem.fontName
                            font.bold: e_emergency == true ? true : false
                            font.pixelSize: 18
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.Wrap
                            maximumLineCount: 2
                            elide: Text.ElideRight
                        }
                    }
                    Rectangle{
                        id: statusTextRect
                        width: 90
                        height: parent.height
                        color: parent.color
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: nameTextRect.right
                        anchors.leftMargin: 5
                        clip: true
                        Text{
                            text: mapStatusToStatusName(e_status)
                            width: parent.width
                            height: parent.height
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            color: e_emergency == true ? "#ee3124" : "#dfdfdf"
                            font.family: rootItem.fontName
                            font.bold: e_emergency == true ? true : false
                            font.pixelSize:18
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.Wrap
                            maximumLineCount: 2
                            elide: Text.ElideRight
                        }
                    }
                    Rectangle{
                        id: zoneTextRect
                        width: 120
                        height: parent.height
                        color: parent.color
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: statusTextRect.right
                        anchors.leftMargin: 5
                        z: 1
                        clip: true
                        Text{
                            text: e_zones === "Speaker" ? langStrObject.GetLangString("ETCS_SPEAKER") : e_zones
                            width: parent.width
                            height: parent.height
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            color: e_emergency == true ? "#ee3124" : "#dfdfdf"
                            font.family: rootItem.fontName
                            font.bold: e_emergency == true ? true : false
                            font.pixelSize: 18
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.Wrap
                            maximumLineCount: 2
                            elide: Text.ElideRight
                        }
                    }

                    ImageButton{
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        width: parent.height
                        height: parent.height
                        bkgPressedColor: "#505050"
                        bkgReleasedColor: "#00ffffff"
                        buttonImageWidth: 28
                        buttonImageHeight: 27
                        buttonImageSource: logicObject.getLocalImagePath() + "delete.png"

                        onClickedSignal: {
                            logicObject.DeleteActivate(e_id);
                            logicObject.delActivate(e_id);
                            if (logicObject.getActivateCount() > 0)
                            {
                                pullDownPage.setClearBtnVisibleValue(true);
                                clearBtnTopLine.setVisibleValue(true);
                            }
                            else
                            {
                                pullDownPage.setClearBtnVisibleValue(false);
                                clearBtnTopLine.setVisibleValue(false);
                            }
                        }
                    }

                    MouseArea{
                        id: mouseArea
                        width: 400
                        height: parent.height
                        anchors.top: parent.top
                        anchors.left: parent.left
                        onClicked: {
                            activateClickEffect.start();
                        }
                    }
                }
            }

    }

    Rectangle{

        function setVisibleValue()
        {
            visible = value;
        }

        function getVisibleValue()
        {
            return visible;
        }

        id: faultView
        width: parent.width
        height: viewHeight
        anchors.top: parent.top
        anchors.topMargin: 106
        color: "#303030"
        visible: false
        Rectangle{
            id: faultViewTitle
            width: parent.width
            height: 40
            color: "#232323"
            Rectangle{
                id: faultTimeTitleRect
                width: 80
                height: parent.height - 2
                color: parent.color
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 25
                clip: true
                Text{
                    id: faultTimeTitle
                    text: langStrObject.GetLangString("ETCS_TIME")
                    width: parent.width
                    height: parent.height
                    anchors.verticalCenter:parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: rootItem.fontName
                    font.bold: false
                    font.pixelSize: 20
                    color: "#dedede"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    Component.onCompleted: {
                    }
                }
            }
            Rectangle{
                id: faultDeviceTitleRect
                width: 150
                height: parent.height - 2
                color: parent.color
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: faultTimeTitleRect.right
                anchors.leftMargin: 5
                clip: true
                Text{
                    id: faultDeviceTitle
                    text: langStrObject.GetLangString("ETCS_DEVICE")
                    width: parent.width
                    height: parent.height
                    anchors.verticalCenter:parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: rootItem.fontName
                    font.bold: false
                    font.pixelSize: 20
                    color: "#dedede"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    Component.onCompleted: {
                    }
                }
            }
            Rectangle{
                id: faultDescriptionTitleRect
                width: 210
                height: parent.height - 2
                color: parent.color
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: faultDeviceTitleRect.right
                anchors.leftMargin: 5
                clip: true
                Text{
                    id: faultDescriptionTitle
                    text: langStrObject.GetLangString("ETCS_DESCRIPTION")
                    width: parent.width
                    height: parent.height
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: rootItem.fontName
                    font.bold: false
                    font.pixelSize: 20
                    color: "#dedede"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    Component.onCompleted: {
                    }
                }
            }
        }

        Rectangle{

            function setVisibleValue(value)
            {
                visible = value;
            }

            id: beepOffTopLine
            anchors.top: beepOffBtn.top
            width: parent.width
            height: 1
            color: "#595959"
            visible: true
            z: 1
        }

        RectButton{

            function setVisibleValue(value)
            {
                visible = value;
            }

            id: beepOffBtn
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: false
            bkgPressedColor: "#606060"
            bkgReleasedColor: "#303030"
            clip: true
            visible: true
            onClickedSignal: {
                logicObject.resetError();
            }

            CommonText{
                id: beepOffBtnName
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                font.family: rootItem.fontName
                font.pixelSize: 18
                color: "#ffffff"
                text: langStrObject.GetLangString("ETCS_ACKNOWLEDGE")
                width: parent.width
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignHCenter
                Component.onCompleted: {
                }
            }
        }

        ListView{

            id: faultViewListView
            width: parent.width
            height: parent.height - faultViewTitle.height - beepOffBtn.height - 10
            anchors.top: faultViewTitle.bottom
            focus: true
            clip: true
            boundsBehavior: Flickable.StopAtBounds //OvershootBounds DragAndOvershootBounds StopAtBounds DragOverBounds
            snapMode: ListView.NoSnap
            model: faultViewVisualModel
        }

        DelegateModel {
            id: faultViewVisualModel

            property alias selectedItems: faultGp
            groups: [
                DelegateModelGroup{
                    id: faultGp
                    name: "selected"
                }
            ]

            model: faultModel
            delegate:Rectangle{
                    width: 480
                    height: 60
                    color: "#303030"

                    Rectangle{
                        anchors.top: parent.top
                        anchors.left: parent.left
                        width: parent.width
                        height: 1
                        color: "#3a3a3a"
                        z: 2
                        visible: true
                    }
                    Rectangle{
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        width: parent.width
                        height: 1
                        color: "#3a3a3a"
                        z: 2
                        visible: index == faultViewVisualModel.count - 1 ? true : false
                    }

                    Rectangle{
                        id: faultTimeTextRect
                        width: 80
                        height: parent.height
                        color: parent.color
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 25
                        clip: true
                        Text{
                            text: e_time
                            width: parent.width
                            height: parent.height
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            color: "#dfdfdf"
                            font.family: rootItem.fontName
                            font.bold: false
                            font.pixelSize:18
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.Wrap
                            maximumLineCount: 2
                            elide: Text.ElideRight
                        }
                    }
                    Rectangle{
                        id: faultNameTextRect
                        width: 150
                        height: parent.height
                        color: parent.color
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: faultTimeTextRect.right
                        anchors.leftMargin: 5
                        clip: true
                        Text{
                            text: mapNameToNameText(e_name,e_local)
                            width: parent.width
                            height: parent.height
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            color: "#dfdfdf"
                            font.family: rootItem.fontName
                            font.bold: false
                            font.pixelSize:18
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.Wrap
                            maximumLineCount: 2
                            elide: Text.ElideRight
                        }
                    }
                    Rectangle{
                        id: descTextRect
                        width: 200
                        height: parent.height
                        color: parent.color
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: faultNameTextRect.right
                        anchors.leftMargin: 5
                        clip: true
                        Text{
                            text: mapErrorToErrorDesc(e_key,e_num,e_ch)
                            width: parent.width
                            height: parent.height
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            color: "#dfdfdf"
                            font.family: rootItem.fontName
                            font.bold: false
                            font.pixelSize: 18
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.Wrap
                            maximumLineCount: 2
                            elide: Text.ElideRight
                        }
                    }
                }
            }

    }

    Rectangle{

        function setVisibleValue()
        {
            visible = value;
        }

        function getVisibleValue()
        {
            return visible;
        }

        id: historyView
        width: parent.width
        height: viewHeight
        anchors.top: parent.top
        anchors.topMargin: 106
        color: "#303030"
        visible: false
        Rectangle{
            id: historyViewTitle
            width: parent.width
            height: 40
            color: "#232323"
            Rectangle{
                id: historyTimeTitleRect
                width: 80
                height: parent.height - 2
                color: parent.color
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 25
                clip: true
                Text{
                    id: historyTimeTitle
                    text: langStrObject.GetLangString("ETCS_TIME")
                    width: parent.width
                    height: parent.height
                    anchors.verticalCenter:parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: rootItem.fontName
                    font.bold: false
                    font.pixelSize: 20
                    color: "#dedede"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    Component.onCompleted: {
                    }
                }
            }
            Rectangle{
                id: historyNameTitleRect
                width: 160
                height: parent.height - 2
                color: parent.color
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: historyTimeTitleRect.right
                anchors.leftMargin: 5
                clip: true
                Text{
                    id: historyNameTitle
                    text: langStrObject.GetLangString("ETCS_SOURCE")
                    width: parent.width
                    height: parent.height
                    anchors.verticalCenter:parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: rootItem.fontName
                    font.bold: false
                    font.pixelSize: 20
                    color: "#dedede"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    Component.onCompleted: {
                    }
                }
            }
            Rectangle{
                id: historyZoneTitleRect
                width: 155
                height: parent.height - 2
                color: parent.color
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: historyNameTitleRect.right
                anchors.leftMargin: 5
                clip: true
                Text{
                    id: historyZoneTitle
                    text: langStrObject.GetLangString("ETCS_ZONE")
                    width: parent.width
                    height: parent.height
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: rootItem.fontName
                    font.bold: false
                    font.pixelSize: 20
                    color: "#dedede"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    Component.onCompleted: {
                    }
                }
            }
        }

        ListModel{

            function createListModel()
            {
                var index;
                var count;
                var ret;

                count = historyObject.getHistoryListCount()
                for (var i = count - 1; i >= 0; i--)
                {
                    ret = historyObject.isHistoryValid(i);
                    if (ret === true)
                    {
                        index = historyObject.getHistoryIndex(i)
                        historyListModel.append({"nIndex": index});
                    }
                }
            }

            function clearListModel()
            {
               if (historyListModel.count > 0)
               {
                    remove(0, historyListModel.count);
               }
            }

            function insertIndex(index)
            {
                insert(0, {"nIndex": index});
            }

            function delIndex(index)
            {
                if (historyListModel.count > 0)
                {
                    remove(index, 1);
                }
            }

            function getListModelCount()
            {
                return historyListModel.count;
            }

            id: historyListModel
        }

        ListView{
            id: historyListView
            width: parent.width
            height: parent.height - historyViewTitle.height - historyClearBtn.height - 10
            anchors.top: historyViewTitle.bottom
            focus: true
            clip: true
            boundsBehavior: Flickable.StopAtBounds
            snapMode: ListView.NoSnap
            model: historyVisualModel
        }

        Rectangle{

            function setVisibleValue(value)
            {
                visible = value;
            }

            id: historyClearBtnTopLine
            width: parent.width
            height: 1
            anchors.bottom: historyListView.bottom
            color: "#595959"
            visible: true
            z: 1
        }

        ImageButton{
            id: historyClearBtn
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            width: 63
            height: 60
            buttonImageSource: logicObject.getLocalImagePath() + "recycle_1.png"
            bkgPressedColor: "#a0505050"
            bkgReleasedColor: "#a0606060"
            radius: height / 2
            visible: true
            onClickedSignal: {
                clearConfirmMask.setVisibleValue(true);
            }
        }

        DelegateModel {
            id: historyVisualModel

            property alias selectedItems: historyGp
            groups: [
                DelegateModelGroup{
                    id: historyGp
                    name: "selected"
                }
            ]

            model: historyListModel
            delegate:Rectangle{
                    id:historyItem
                    width: 480
                    height: 60
                    color: "#303030"

                    SequentialAnimation{

                        id: historyClickEffect
                        running: false
                        ColorAnimation{
                            target: historyItem
                            property: "color"
                            from: "#303030"
                            to: "#505050"
                            duration: 50
                        }
                        ColorAnimation {
                            target: historyItem
                            property: "color"
                            from: "#505050"
                            to: "#303030"
                            duration: 100
                        }

                        onStopped: {
                            var errorNum;
                            errorNum = historyObject.getHistoryIntValue(nIndex, "errorNum");
                            if (errorNum === 0)
                            {
                                 historyDetailInfo.show(nIndex);
                            }
                            else if (errorNum === 2)
                            {
                                localErrorMask.initStatus(langStrObject.GetLangString("ETCS_CONFIG_ERROR"));
                            }
                            else if (errorNum === 3)
                            {
                                localErrorMask.initStatus(langStrObject.GetLangString("ETCS_DATABASE_ERROR"));
                            }
                            else if (errorNum === 4)
                            {
                                localErrorMask.initStatus(langStrObject.GetLangString("ETCS_FILE_NOT_EXIST_ERROR"));
                            }
                            else if (errorNum === 5)
                            {
                                localErrorMask.initStatus(langStrObject.GetLangString("ETCS_FILE_FORMAT_ERROR"));
                            }
                            else
                            {
                                ;
                            }

                        }
                    }

                    Rectangle{
                        anchors.top: parent.top
                        anchors.left: parent.left
                        width: parent.width
                        height: 1
                        color: "#3a3a3a"
                        z: 2
                        visible: true
                    }
                    Rectangle{
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        width: parent.width
                        height: 1
                        color: "#3a3a3a"
                        z: 2
                        visible: index == historyVisualModel.count - 1 ? true : false
                    }

                    Rectangle{
                        id: historyTimeTextRect
                        width: 110
                        height: parent.height
                        color: parent.color
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 15
                        clip: true
                        Text{
                            text: historyObject.getHistoryStrValue(nIndex, "Time");
                            width: parent.width
                            height: parent.height
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            color: "#dfdfdf"
                            font.family: rootItem.fontName
                            font.bold: false
                            font.pixelSize:18
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.Wrap
                            maximumLineCount: 2
                            elide: Text.ElideRight
                        }
                    }
                    Rectangle{
                        id: historyNameTextRect
                        width: 135
                        height: parent.height
                        color: parent.color
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: historyTimeTextRect.right
                        anchors.leftMargin: 5
                        clip: true
                        Text{
                            text: historyObject.getHistoryStrValue(nIndex, "SourceList");
                            width: parent.width
                            height: parent.height
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            color: "#dfdfdf"
                            font.family: rootItem.fontName
                            font.bold: false
                            font.pixelSize:18
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.Wrap
                            maximumLineCount: 2
                            elide: Text.ElideRight
                        }
                    }
                    Rectangle{
                        id: historyZoneTextRect
                        width: 150
                        height: parent.height
                        color: parent.color
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: historyNameTextRect.right
                        anchors.leftMargin: 5
                        clip: true
                        Text{
                            text: historyObject.getHistoryStrValue(nIndex, "Zone");
                            width: parent.width
                            height: parent.height
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            color: "#dfdfdf"
                            font.family: rootItem.fontName
                            font.bold: false
                            font.pixelSize: 18
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.Wrap
                            maximumLineCount: 2
                            elide: Text.ElideRight
                        }
                    }
                    ImageButton{
                        anchors.top: parent.top
                        anchors.right: parent.right
                        width: parent.height
                        height: parent.height
                        z: 1
                        bkgPressedColor: "#505050"
                        buttonImageSource: logicObject.getLocalImagePath() + "send_A.png"
                        visible: (historyObject.getHistoryBoolValue(nIndex, "isLive") === true) ||
                                 (historyObject.getHistoryBoolValue(nIndex, "isError") === true)
                                 ? false : true
                        onClickedSignal: {

                        }
                        onReleasedSignal: {
                            var errorCode = historyObject.historyCall(nIndex);
                            var ret = rootItem.dealCallError(errorCode);
                            if (ret !== 0)
                            {
                                return;
                            }
                        }
                    }

                    CommonImage{
                        id: localErrorImage
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: 20
                        source: logicObject.getLocalImagePath() + "error.png"
                        visible: historyObject.getHistoryBoolValue(nIndex, "isError") === true ? true : false
                    }

                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            historyClickEffect.start()
                        }
                    }
                }
            }

    }

    Rectangle{
        width: parent.width
        height: 1
        anchors.bottom: pullBtn.top
        color: "#595959"
    }

    Rectangle{
        id: pullBtn
        width: parent.width
        height: pullBtnHeight
        anchors.bottom: parent.bottom
        color: "#303030"
        Image{
            id: pullBtnImg
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            source: logicObject.getLocalImagePath() + "arrow_down.png"
        }
    }

    DetailedInfo{
        id: detailedInfo
        width: parent.width
        height: parent.baseHeight
        visible: false
    }

    HistoryDetailInfo{
        id: historyDetailInfo
        width: parent.width
        height: parent.baseHeight
        visible: false
    }

    Timer{
        id: activateUpdateTimer
        interval: 800
        repeat: false
        onTriggered: {
            updateActivate();
        }
    }

    Timer{
        id: histroyUpdateTimer
        interval: 3 * 1000
        repeat: false
        onTriggered: {
            updateHistorySlot();
        }
    }

    Component.onCompleted: {
        rootItem.updateLanguage.connect(updateLanguageSlot);
        logicObject.activateAddSignal.connect(activateAddSlot);
        logicObject.addHistorySignal.connect(addHistorySlot);
        logicObject.noActivateSignal.connect(noActivateSlot);
        rootItem.changeUserSignal.connect(changeUserSlot);
        clearConfirmMask.okSignal.connect(okSlot);
        updateActivate();
        updateHistorySlot();
    }
}
