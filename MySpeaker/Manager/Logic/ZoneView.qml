import QtQuick 2.5
import QtQml.Models 2.2
import "../Components"
import "../Keyboard"

Rectangle {

    property int gridviewSpaceDistance: 18
    property int columns: 3
    property bool zoneViewSelectAllBtnClicked: false
    property bool nextEnable: false

    property int zoneViewTitleBarHeight: 50
    property int gridViewHeight: height - mainPage.g_titleBarHeight - zoneViewTitleBarHeight

    function updateLanguageSlot()
    {
        zoneViewName.text = langStrObject.GetLangString("ETCS_TARGET_ZONE");
        zoneViewSelectAllBtnName.text = langStrObject.GetLangString("ETCS_SELECT_ALL");
        zoneViewClearBtnName.text = langStrObject.GetLangString("ETCS_CLEAR_ALL");
    }

    function changeUserSlot(value)
    {
        if (value === 0)
        {
            initZoneListModel();
        }
        else if (value === 1)
        {
            initZoneListModel();
        }
        else if (value === 2)
        {
            initZoneListModel();
        }
    }

    function initStatus()
    {
        setVisibleValue(true);
        textEdit.setFocus(false);
        textEdit.clearSlot();
        keyBoard.quitStatus();
        updateSelectAllAndClearBtnStatus();
        updateNextBtnStatus();
        quickLiveBtn.updateCallBtnStatus();
        selectAndAllBtn.updateText();
    }

    function stopQuickLive()
    {
        if (quickLiveBtn.getCallId() !== 0)
        {
            pageObject.livePageStopCall(quickLiveBtn.getCallId());
            quickLiveBtn.setCallStatusValue(quickLiveBtn.dis_status);
            quickLiveBtn.updateCallBtnStatus();
            quickLiveBtn.setHaveGongValue(false);
            quickLiveBtn.setCallId(0);
            statusBar.setStatusBarMessageTextForTimer(langStrObject.GetLangString("ETCS_CALL_ERR_7"), "ETCS_CALL_ERR_7");
        }
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function updateSelectAllAndClearBtnStatus()
    {
        if (zoneManageObject.isZoneModelMoreThanOneZoneSelected() === true)
        {
            zoneViewSelectAllBtn.setVisibleValue(false);
            zoneViewClearBtn.setVisibleValue(true);
        }
        else
        {
            zoneViewSelectAllBtn.setVisibleValue(true);
            zoneViewClearBtn.setVisibleValue(false);
        }
    }

    function updateNextBtnStatus()
    {
        if (zoneManageObject.isZoneModelMoreThanOneZoneSelected() === true)
        {
            setNextEnableValue(true);
        }
        else
        {
            setNextEnableValue(false);
        }
    }

    function setNextEnableValue(value)
    {
        nextEnable = value;
    }

    function setNextBtnVisibleValue(value)
    {
        nextBtn.visible = value;
    }

    function initZoneListModel()
    {
        zoneManageObject.clearZoneModel();
        zoneManageObject.createZoneModel();
        updateSelectAllAndClearBtnStatus();
        updateNextBtnStatus();
        quickLiveBtn.updateCallBtnStatus();
        selectAndAllBtn.updateText();
    }

    function clearSearchBarText()
    {
        textEdit.clearSlot();
    }

    function inputStatus()
    {
        keyBoard.initStatus();
    }

    function normalStatus()
    {
        textEdit.setFocus(false);
        keyBoard.quitStatus();
    }

    Rectangle{
        id: titleBar
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width
        height: mainPage.g_titleBarHeight
        color: mainPage.g_titleColor

        Rectangle{
            id: searchBar
            width: 320
            height: 35
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 18
            color: "#505050"
            clip: true

            CommonImage{
                id: searchImage
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 5
                source: logicObject.getLocalImagePath() + "search.png"
                z: 2
            }

            Rectangle{
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                width: searchImage.width + 10
                height: parent.height
                color: parent.color
                z: 1
            }

            CommonTextInput{
                id: textEdit
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: searchImage.right
                anchors.leftMargin: 5
                width: parent.width - searchImage.width - 5 - 5 - 10
                height: parent.height
                color: "#ffffff"
                maximumLength: 30
                onFocusSignal: {
                    inputStatus();
                    mainPage.hideBusinessBar();
                }
                onTextChanged: {
                    if (selectAndAllBtn.getBtnStatus() === true)
                    {

                    }
                    else
                    {
                        zoneProxyModel.filterRole = 2;
                        zoneProxyModel.setFilterFixedString(textEdit.getText());
                    }
                }
            }
        }

        ImageButton3{

            property int callId: 0
            property bool haveGong: false
            property string wait_status: "wait"
            property string full_status: "full"
            property string part_status: "part"
            property string dis_status: "disconnect"
            property string callStatus: dis_status

            function updateCallBtnStatus()
            {
                if (getCallStatusValue() === wait_status)
                {
                    waitStatus();
                }
                else if (getCallStatusValue() === full_status)
                {
                    if (getHaveGongValue() === true)
                    {
                        gongStatus();
                    }
                    else
                    {
                        playingStatus();
                    }
                }
                else if (getCallStatusValue() === part_status)
                {
                    if (getHaveGongValue() === true)
                    {
                        gongStatus();
                    }
                    else
                    {
                        playingStatus();
                    }
                }
                else if (getCallStatusValue() === dis_status)
                {
                    if (zoneManageObject.isZoneModelMoreThanOneZoneSelected() === true)
                    {
                        initStatus();
                    }
                    else
                    {
                        disableStatus();
                    }
                }
            }

            function initStatus()
            {
                setButtonImageSource(logicObject.getLocalImagePath() + "quick_live_A.png");
                setImgPlayingValue(false);
                setEnabledValue(true);
                setVisibleValue(true);
            }

            function waitStatus()
            {
                setButtonImageSource(logicObject.getLocalImagePath() + "quick_live_B.png");
                setImgPlayingValue(false);
                setEnabledValue(true);
                setVisibleValue(true);
            }

            function gongStatus()
            {
                setButtonImageSource(logicObject.getLocalImagePath() + "quick_live_D.png");
                setImgPlayingValue(true);
                setEnabledValue(true);
                setVisibleValue(true);
            }

            function playingStatus()
            {
                setButtonImageSource(logicObject.getLocalImagePath() + "quick_live_B.png");
                setImgPlayingValue(false);
                setEnabledValue(true);
                setVisibleValue(true);
            }

            function disableStatus()
            {
                setButtonImageSource(logicObject.getLocalImagePath() + "quick_live_C.png");
                setImgPlayingValue(false);
                setEnabledValue(false);
                setVisibleValue(false);
            }

            function setHaveGongValue(value)
            {
                haveGong = value;
            }

            function getHaveGongValue()
            {
                return haveGong;
            }

            function setCallId(id)
            {
                callId = id;
            }

            function getCallId()
            {
                return callId;
            }

            function setCallStatusValue(value)
            {
                callStatus = value;
            }

            function getCallStatusValue()
            {
                return callStatus;
            }

            function setHaveGongTrueSlot(id)
            {
                if (id === getCallId())
                {
                    setHaveGongValue(true);
                    updateCallBtnStatus();
                }
            }

            function setHaveGongFalseSlot(id)
            {
                if (id === getCallId())
                {
                    setHaveGongValue(false);
                    updateCallBtnStatus();
                }
            }

            function activateStatusSlot(id, text)
            {
                if  (id !== getCallId())
                {
                    return;
                }

                if (text === dis_status)
                {
                    setCallStatusValue(dis_status);
                    updateCallBtnStatus();
                }
                else if (text === full_status)
                {
                    setCallStatusValue(full_status);
                    updateCallBtnStatus();
                }
                else if (text === part_status)
                {
                    setCallStatusValue(part_status);
                    updateCallBtnStatus();
                }
                else if (text === wait_status)
                {
                    setCallStatusValue(wait_status);
                    updateCallBtnStatus();
                }
            }

            id: quickLiveBtn
            width: parent.height
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: nextBtn.left
            visible: true
            onClickedSignal: {
                if (getCallStatusValue() === dis_status)
                {
                    var errorCode = pageObject.livePageStartCall(manualViewCall.getgPrority(),
                                                 manualViewCall.getVolumeSliderValue(),
                                                 manualViewCall.getgPreSignal(),
                                                 manualViewCall.getgReconnect(),
                                                 manualViewCall.getgPartlyConnect(),
                                                 41
                                                 );

                    var ret = rootItem.dealCallError(errorCode);
                    if (ret !== 0)
                    {
                        setCallStatusValue(dis_status);
                        updateCallBtnStatus();
                        return;
                    }

                    setCallId(pageObject.getCallId());
                    setCallStatusValue(wait_status);
                    updateCallBtnStatus();
                }
                else
                {
                    pageObject.livePageStopCall(getCallId());
                    setCallStatusValue(dis_status);
                    updateCallBtnStatus();
                    setHaveGongValue(false);
                    setCallId(0);
                }
            }
        }


        Rectangle{
            id: nextBtn
            width: parent.height
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            color: mouseArea.pressed ? "#505050" : parent.color
            Image{
                id: nextBtnImg
                source: nextEnable ? logicObject.getLocalImagePath() + "next.png" : ""
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }
            MouseArea{
                id: mouseArea
                anchors.fill: parent
                enabled: nextEnable
                onClicked: {
                    stopQuickLive();
                    changeView("manualViewCall");
                    normalStatus();
                }
            }
        }
    }

    Rectangle{
        id: zoneViewTitleBar
        width: parent.width
        height: zoneViewTitleBarHeight
        color: "#ffffff"
        anchors.top: titleBar.bottom
        anchors.left: parent.left

        Rectangle{
            id: zoneViewTitleRect
            width: 4
            height: 24
            color: "#4f4f4f"
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 18
        }

        Text{
            id: zoneViewName
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 28
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_TARGET_ZONE")
            font.family: rootItem.fontName
            font.pixelSize: 22
            width: parent.width - 28 - selectAndAllBtn.width- gridViewTypeBtn.width - zoneViewSelectAllBtn.width
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
            Component.onCompleted: {
            }
        }

        RectToggleButton{
            function updateText()
            {
                if (btnStatus == true)
                {
                    selectText.setText(zoneManageObject.getZoneModelSelectCount());
                    selectText.setTextColor("#00c767");
                    allText.setText(zoneManageObject.getZoneModelCount());
                    allText.setTextColor("#303030");
                }
                else
                {
                    selectText.setText(zoneManageObject.getZoneModelSelectCount());
                    selectText.setTextColor("#303030");
                    allText.setText(zoneManageObject.getZoneModelCount());
                    allText.setTextColor("#303030");
                }
            }

            id: selectAndAllBtn
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: gridViewTypeBtn.left
            width: 100
            height: parent.height
            border.width: 0
            bkSwitch: true
            CommonText{
                id: selectText
                anchors.left: parent.left
                anchors.leftMargin: (parent.width - selectText.contentWidth - slashText.contentWidth - 5 - allText.contentWidth - 5) / 2
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 22
                text: zoneManageObject.getZoneModelCount()
                color: "#303030"
            }
            CommonText{
                id: slashText
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: selectText.right
                anchors.leftMargin: 5
                font.pixelSize: 24
                text: "/"
                color: "#303030"
            }

            CommonText{
                id: allText
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: slashText.right
                anchors.leftMargin: 5
                font.pixelSize: 22
                text: zoneManageObject.getZoneModelCount()
                color: "#303030"
            }

            onSelectSignal: {
                zoneProxyModel.filterRole = 4;
                zoneProxyModel.setFilterFixedString(true);
                selectAndAllBtn.updateText();

                textEdit.setText("");
                normalStatus();
                mainPage.showBusinessBar();
            }
            onUndoSignal: {
                zoneProxyModel.filterRole = 2;
                zoneProxyModel.setFilterFixedString(textEdit.getText());
                selectAndAllBtn.updateText();
            }
        }

        Rectangle {
            property bool gridViewTypeBtnClicked: false

            id: gridViewTypeBtn
            width: 65
            height: 50
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: zoneViewSelectAllBtn.left
            color: typeBtnMouseArea.pressed ? "#DBE0E6" : parent.color
            Image{
                id: buttonImage
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                source: gridViewTypeBtn.gridViewTypeBtnClicked ? logicObject.getLocalImagePath() + "list_sort.png" : logicObject.getLocalImagePath() + "grid_sort.png"
            }

            MouseArea {
                id: typeBtnMouseArea
                anchors.fill: parent
                onClicked: {
                    gridViewTypeBtn.gridViewTypeBtnClicked = !gridViewTypeBtn.gridViewTypeBtnClicked;
                    if (gridViewTypeBtn.gridViewTypeBtnClicked == true)
                    {
                        zoneView.columns = 1;
                    }
                    else if(gridViewTypeBtn.gridViewTypeBtnClicked == false)
                    {
                        zoneView.columns = 3;
                    }
                }
            }
        }

        Rectangle {

            function setVisibleValue(value)
            {
                visible = value;
            }

            id: zoneViewSelectAllBtn
            width: 100
            height: 50
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            color: selectAllMouseArea.pressed ? "#DBE0E6" : parent.color
            visible: true

            Image{
                id: zoneViewSelectAllBtnImage
                anchors.left: parent.left
                anchors.leftMargin: (parent.width - zoneViewSelectAllBtnImage.width - zoneViewSelectAllBtnName.contentWidth) / 2
                anchors.verticalCenter: parent.verticalCenter
                source: zoneViewSelectAllBtnClicked ? logicObject.getLocalImagePath() + "ETCS_Target Zone_All_B.png" : logicObject.getLocalImagePath() + "select_all.png"
            }

            Text {
                id: zoneViewSelectAllBtnName
                font.family: rootItem.fontName
                font.pixelSize: 16
                anchors.left: zoneViewSelectAllBtnImage.right
                anchors.verticalCenter: parent.verticalCenter
                color: zoneViewSelectAllBtnClicked ? "#00c767" : "#303030"
                text: langStrObject.GetLangString("ETCS_SELECT_ALL")
                width: 110 - zoneViewSelectAllBtnImage.width
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
                Component.onCompleted: {
                }
            }

            MouseArea {
                id: selectAllMouseArea
                anchors.fill: parent
                onClicked: {
                    zoneManageObject.zoneModelSelectAll();
                    zoneManageObject.selectAllFromUserPermission();
                    //zoneManageObject.printAllSelectZones();
                    updateSelectAllAndClearBtnStatus();
                    updateNextBtnStatus();
                    quickLiveBtn.updateCallBtnStatus();
                    selectAndAllBtn.updateText();
                }
            }
        }

        Rectangle {

            function setVisibleValue(value)
            {
                visible = value;
            }

            id: zoneViewClearBtn
            width: 100
            height: 50
            anchors.verticalCenter: parent.verticalCenter
            anchors.right:  parent.right
            color: clearMouseArea.pressed ? "#DBE0E6" : parent.color
            visible: false

            Image{
                id: zoneViewClearBtnImage
                anchors.left: parent.left
                anchors.leftMargin: (parent.width - zoneViewClearBtnImage.width - zoneViewClearBtnName.contentWidth) / 2
                anchors.verticalCenter: parent.verticalCenter
                source: logicObject.getLocalImagePath() + "clear_all.png"
            }

            Text {
                id: zoneViewClearBtnName
                font.family: rootItem.fontName
                font.pixelSize: 16
                anchors.left: zoneViewClearBtnImage.right
                anchors.verticalCenter: parent.verticalCenter
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_CLEAR_ALL")
                width: 110 - zoneViewClearBtnImage.width
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
                Component.onCompleted: {
                }
            }

            MouseArea {
                id: clearMouseArea
                anchors.fill: parent
                onClicked: {
                        zoneManageObject.zoneModelUnselectAll();
                        zoneManageObject.unselectAllFromUserPermission();
                        //zoneManageObject.printAllSelectZones();
                        updateSelectAllAndClearBtnStatus();
                        updateNextBtnStatus();
                        quickLiveBtn.updateCallBtnStatus();
                        selectAndAllBtn.updateText();
                    }
            }
        }
    }

    Rectangle{
        z: 1
        width: parent.width
        height: 1
        anchors.top: zoneViewTitleBar.bottom
        color: "#c0c0c0"
    }

    ScrollBar{
        id: zoneViewVerticalScrollBar
        anchors.top: gridView.top
        anchors.right: parent.right
        anchors.rightMargin: 6
        width: 6
        height: gridView.height
        orientation: Qt.Vertical
        position: gridView.visibleArea.yPosition
        pageSize: gridView.visibleArea.heightRatio
        scrollBarVisible: gridView.visibleArea.heightRatio == 1 ? false : true
    }

    GridView{
        id: gridView
        width: parent.width - gridviewSpaceDistance
        height: gridViewHeight
        anchors.left: parent.left
        anchors.top: zoneViewTitleBar.bottom
        cellWidth: zoneView.columns == 1 ? width : (width / 3)
        cellHeight: 75
        clip: true
        boundsBehavior: Flickable.StopAtBounds
        snapMode: GridView.NoSnap
        footer: Rectangle{height: 8}
        model: visualModel
    }

    DelegateModel {
        id: visualModel

        property alias selectedItems: gp
        groups: [
            DelegateModelGroup{
                id: gp
                name: "selected"
            }
        ]

        model: zoneProxyModel
        delegate:Rectangle{

            id: zoneGridViewElement
            width: gridView.cellWidth
            height: gridView.cellHeight
            opacity: 1

            Rectangle{

                function setBorderColor(value)
                {
                    border.color = value;
                }

                function setBorderWidth(value)
                {
                    border.width = value;
                }

                id: item
                width: zoneView.columns == 1 ? (gridView.width - 18) :  (gridView.width - 3 * 18) / 3
                height: 60
                anchors.left: parent.left
                anchors.leftMargin: 18
                anchors.top: parent.top
                anchors.topMargin: 15
                border.color: e_select ? "#00c767" : "#bebebe"
                border.width: e_select ? 2 : 1

                Rectangle{

                    function setVisibleValue(value)
                    {
                        visible = value;
                    }

                    id: statusRect
                    width: 10
                    height: 10
                    anchors.top: parent.top
                    anchors.topMargin: 4
                    anchors.left: parent.left
                    anchors.leftMargin: 4
                    color: e_color
                }

                Image{

                    function setVisibleValue(value)
                    {
                        visible = value;
                    }

                    id: playImage
                    anchors.top: parent.top
                    anchors.topMargin: 4
                    anchors.right: parent.right
                    anchors.rightMargin: 4
                    source: logicObject.getLocalImagePath() + "playing.png"
                    visible: e_color === "#ffffff" ? true : false
                }

                Text{
                    function setTextColor(value)
                    {
                        color = value;
                    }

                    id: nameText
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: zoneView.columns == 1 ? parent.width - 36 : parent.width - 10
                    height: 20
                    text: e_name
                    color: e_select ? "#00c767" : "#303030"
                    font.family: rootItem.fontName
                    font.pixelSize: 18
                    wrapMode: Text.Wrap
                    horizontalAlignment: zoneView.columns == 1 ? Text.AlignLeft : Text.AlignHCenter
                    elide: Text.ElideRight
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {

                        /*
                        zoneGridViewElement.VisualDataModel.inSelected = !zoneGridViewElement.VisualDataModel.inSelected;
                        if (zoneGridViewElement.VisualDataModel.inSelected === true)
                        {
                            zoneManageObject.setZoneSelected(e_index, true);
                            e_select = true;
                        }
                        else if (zoneGridViewElement.VisualDataModel.inSelected === false)
                        {
                            zoneManageObject.setZoneSelected(e_index, false);
                            e_select = false;
                        }
                        */

                        e_select = !e_select
                        if (e_select === true)
                        {
                            zoneManageObject.setZoneSelected(e_index, true);
                        }
                        else
                        {
                            zoneManageObject.setZoneSelected(e_index, false);
                        }

                        //zoneManageObject.printAllSelectZones();

                        updateSelectAllAndClearBtnStatus();
                        updateNextBtnStatus();
                        quickLiveBtn.updateCallBtnStatus();
                        selectAndAllBtn.updateText();
                    }
                }
            }
        }
    }

    Keyboard{
        id: keyBoard
        anchors.bottom: parent.bottom
        anchors.bottomMargin: -mainPage.businessBarHeight
        anchors.left: parent.left
        width: 480
        height: 366
        visible: false
        onOkSignal: {
            normalStatus();
            mainPage.showBusinessBar();
        }
    }

    Component.onCompleted: {
        rootItem.updateLanguage.connect(updateLanguageSlot);
        rootItem.changeUserSignal.connect(changeUserSlot);
        keyBoard.addCharSignal.connect(textEdit.addCharSlot);
        keyBoard.delCharSignal.connect(textEdit.delCharSlot);
        keyBoard.clearSignal.connect(textEdit.clearSlot);
        logicObject.signalCallStatus.connect(quickLiveBtn.activateStatusSlot);
        logicObject.gongPlayingTrueSignal.connect(quickLiveBtn.setHaveGongTrueSlot);
        logicObject.gongPlayingFalseSignal.connect(quickLiveBtn.setHaveGongFalseSlot);
    }
}
