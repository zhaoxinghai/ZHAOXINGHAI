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
    property int gridViewHeight: height - mainPage.g_titleBarHeight - zoneViewTitleBarHeight - funcArea.height

    property int id: 0
    property string wait_status: "wait"
    property string full_status: "full"
    property string part_status: "part"
    property string dis_status: "disconnect"

    signal updateStatusSignal(var color);
    signal lockSignal();
    signal quitSignal();

    function updateLanguageSlot()
    {
        zoneViewName.text = langStrObject.GetLangString("ETCS_TARGET_ZONE");
        zoneViewSelectAllBtnName.text = langStrObject.GetLangString("ETCS_SELECT_ALL");
        zoneViewClearBtnName.text = langStrObject.GetLangString("ETCS_CLEAR_ALL");
        liveBtn.setInfoValue(langStrObject.GetLangString("ETCS_VA_LIVE"));
        evacBtn.setInfoValue(langStrObject.GetLangString("ETCS_EVAC"));
        alertBtn.setInfoValue(langStrObject.GetLangString("ETCS_ALERT"));
    }

    function initStatus()
    {
        zoneManageObject.clearZoneModelVA();
        zoneManageObject.createZoneModelVA();
        zoneManageObject.unselectAllFromUserPermissionVA();
        updateSelectAllAndClearBtnStatus();
        selectAndAllBtn.updateText();
        checkEvacStatus();
        checkAlertStatus();
        setVisibleValue(true);
        logicObject.setButtonEnable("alarm", true);
    }

    function keepStatus()
    {
        setVisibleValue(true);
    }

    function quitStatus()
    {
        setVisibleValue(false);
    }

    function checkEvacStatus()
    {
        var ret = pageObject.isEVAExist();
        var errorCode = pageObject.GetEVAErrorCode();

        if (ret === true)
        {
            if (errorCode === 0)
            {
                evacBtn.initStatus();
            }
            else
            {
                evacBtn.errorStatus();
            }
        }
        else
        {
            if (errorCode === 0)
            {
                evacBtn.disableStatus();
            }
            else
            {
                evacBtn.errorStatus();
            }
        }
    }

    function checkAlertStatus()
    {
        var ret = pageObject.isAlertExist();
        var errorCode = pageObject.GetAlertErrorCode();

        if (ret === true)
        {
            if (errorCode === 0)
            {
                alertBtn.initStatus();
            }
            else
            {
                alertBtn.errorStatus();
            }
        }
        else
        {
            if (errorCode === 0)
            {
                alertBtn.disableStatus();
            }
            else
            {
                alertBtn.errorStatus();
            }
        }
    }

    function checkError(errorNum)
    {
        if (errorNum === 0)
        {
            return 0;
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

        return -1;
    }

    function changeUserSlot(value)
    {

    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function setZoneViewSelectAllBtnVisible(value)
    {
        zoneViewSelectAllBtn.visible = value;
    }

    function setZoneViewClearBtnVisible(value)
    {
        zoneViewClearBtn.visible = value;
    }

    function setId(value)
    {
        id = value;
    }

    function getId()
    {
       return id;
    }

    function updateSelectAllAndClearBtnStatus()
    {
        if (zoneManageObject.isZoneModelMoreThanOneZoneSelectedVA() === true)
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

    function checkZoneValid()
    {
        if (zoneManageObject.getZoneModelSelectCountVA() <= 0)
        {
            return false;
        }

        return true;
    }

    function activateUpdateSlot(id, text)
    {
        if (id === getId())
        {
            if (text === dis_status)
            {
                liveBtn.setInitStatus();
                liveBtn.setBtnStatus(false);
                setId(0);
            }
        }
    }

    MouseArea{
        anchors.fill: parent
        onClicked: {
            mouse.accepted = true;
        }
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
                    keyBoard.initStatus();
                    keyBoardMask.setVisibleValue(true);
                    mainPage.hideBusinessBar();
                }
                onTextChanged: {

                    if (selectAndAllBtn.getBtnStatus() === true)
                    {

                    }
                    else
                    {
                        zoneProxyModelVA.filterRole = 2;
                        zoneProxyModelVA.setFilterFixedString(textEdit.getText());
                    }
                }
            }
        }

        ImageButton{
            id: lockBtn
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: quitBtn.left
            width: parent.height
            height: parent.height
            buttonImageSource: logicObject.getLocalImagePath() + "lock.png"
            bkgPressedColor: "#505050"
            bkgReleasedColor: "#00ffffff"
            visible: false
            onClickedSignal: {
                //lockSignal();
            }
        }

        ImageButton{
            id: quitBtn
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            width: parent.height
            height: parent.height
            buttonImageSource: logicObject.getLocalImagePath() + "delete_3.png"
            bkgPressedColor: "#505050"
            bkgReleasedColor: "#00ffffff"
            onClickedSignal: { 
                quitSignal();
                logicObject.setButtonEnable("alarm", false);
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
            width: parent.width - 28 - selectAndAllBtn.width - gridViewTypeBtn.width - zoneViewSelectAllBtn.width
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
                    selectText.setText(zoneManageObject.getZoneModelSelectCountVA());
                    selectText.setTextColor("#00c767");
                    allText.setText(zoneManageObject.getZoneModelCountVA());
                    allText.setTextColor("#303030");
                }
                else
                {
                    selectText.setText(zoneManageObject.getZoneModelSelectCountVA());
                    selectText.setTextColor("#303030");
                    allText.setText(zoneManageObject.getZoneModelCountVA());
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
                text: zoneManageObject.getZoneModelSelectCountVA()
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
                text: zoneManageObject.getZoneModelCountVA()
                color: "#303030"
            }

            onSelectSignal: {

                zoneProxyModelVA.filterRole = 6;
                zoneProxyModelVA.setFilterFixedString(true);
                selectAndAllBtn.updateText();

                textEdit.setText("");
                textEdit.setFocus(false);
                keyBoard.quitStatus();
                keyBoardMask.setVisibleValue(false);
            }
            onUndoSignal: {

                zoneProxyModelVA.filterRole = 2;
                zoneProxyModelVA.setFilterFixedString(textEdit.getText());
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
                        columns = 1;
                    }
                    else if(gridViewTypeBtn.gridViewTypeBtnClicked == false)
                    {
                        columns = 3;
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
            color: selectAllMouseArea.pressed ? "#dbe0e6" : parent.color
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
                    zoneManageObject.zoneModelSelectAllVA();
                    zoneManageObject.selectAllFromUserPermissionVA();
                    //zoneManageObject.printAllSelectZonesVA();
                    updateSelectAllAndClearBtnStatus();
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
            color: clearMouseArea.pressed ? "#dbe0e6" : parent.color
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
                    zoneManageObject.zoneModelUnselectAllVA();
                    zoneManageObject.unselectAllFromUserPermissionVA();
                    //zoneManageObject.printAllSelectZonesVA();
                    updateSelectAllAndClearBtnStatus();
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
        cellWidth: columns == 1 ? width : (width / 3)
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

        model: zoneProxyModelVA
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

                function setColor(value)
                {
                    color = value;
                }

                id: item
                width: columns == 1 ? (gridView.width - 18) :  (gridView.width - 3 * 18) / 3
                height: 60
                anchors.left: parent.left
                anchors.leftMargin: 18
                anchors.top: parent.top
                anchors.topMargin: 15
                border.color: e_VASelect ? "#00c767" : "#bebebe"
                border.width: e_VASelect ? 2 : 1

                Rectangle{
                    function setVisible()
                    {
                        var protocol_ver = localConfigObject.getProtocolVersion();
                        //only visible in protocol 11
                        if ("11" !== protocol_ver)
                        {
                            return false;
                        }

                        return true;
                    }

                    id: vaStatusRect
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 5
                    width: 22
                    height: 8
                    color: e_VAColor
                    visible: setVisible()
                }

                Image{

                    function setVisible()
                    {
                        var protocol_ver = localConfigObject.getProtocolVersion();
                        //only visible in protocol 10
                        if ("10" !== protocol_ver)
                        {
                            return false;
                        }

                        var zone_color = e_color;
                        if (e_color === "#ffffff")
                        {
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                    }

                    id: playImage
                    anchors.top: parent.top
                    anchors.topMargin: 4
                    anchors.right: parent.right
                    anchors.rightMargin: 4
                    source: logicObject.getLocalImagePath() + "playing.png"
                    visible: setVisible()
                }

                Text{

                    function setTextColor(value)
                    {
                        color = value;
                    }

                    id: nameText
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: columns == 1 ? parent.width - 36 : parent.width - 10
                    height: 20
                    text: e_name
                    color: e_VASelect ? "#00c767" : "#303030"
                    font.family: rootItem.fontName
                    font.pixelSize: 18
                    wrapMode: Text.Wrap
                    horizontalAlignment: columns == 1 ? Text.AlignLeft : Text.AlignHCenter
                    elide: Text.ElideRight
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        e_VASelect = !e_VASelect;
                        if (e_VASelect === true)
                        {
                            zoneManageObject.setZoneSelectedVA(e_index, true);
                        }
                        else
                        {
                            zoneManageObject.setZoneSelectedVA(e_index, false);
                        }

                        //zoneManageObject.printAllSelectZonesVA();
                        updateSelectAllAndClearBtnStatus();
                        selectAndAllBtn.updateText();
                    }
                }

                Component.onCompleted: {
                   // zoneGridViewElement.VisualDataModel.inSelected = zoneManageObject.getZoneBoolValue(nIndex, "selectVA");
                }
            }
        }
    }

    Mask{
        id: keyBoardMask
        anchors.top: parent.top
        anchors.topMargin: -36
        anchors.left: parent.left
        width: logicObject.getRealWidth()
        height: logicObject.getRealHeight()
        color: "#00ffffff"
        visible: false
        z: 3
        passThrough: false

        Keyboard{
            id: keyBoard
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            visible: true
            onOkSignal: {
                textEdit.setFocus(false);
                keyBoard.quitStatus();
                keyBoardMask.setVisibleValue(false);
            }
        }

        onClickedSignal: {
            textEdit.setFocus(false);
            keyBoard.quitStatus();
            keyBoardMask.setVisibleValue(false);
        }
    }

    MessageBar{
        id: messageBar
        anchors.bottom: funcArea.top
        width: parent.width
        height: 0
        color: "#dddddd"
        clip: true
        border.width: 1
        border.color: "#f4f4f4"
        z: 2
    }

    Rectangle{

        id: funcArea
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width
        height: 86

        HorizontalLine{
            anchors.top: parent.top
            anchors.left: parent.left
        }

        ImageAndTextButton3{

            function setInitStatus()
            {
                imageSource = logicObject.getLocalImagePath() + "VA_live_A.png";
            }

            function setPlayingStatus()
            {
                imageSource = logicObject.getLocalImagePath() + "VA_live_B.png";
            }

            id: liveBtn
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            width: parent.width / 3
            height: parent.height
            imageSource: logicObject.getLocalImagePath() + "VA_live_A.png"
            info: langStrObject.GetLangString("ETCS_VA_LIVE")
            color: "#f3f3f3"
            onSelectSignal: {
                if (checkZoneValid() === false)
                {
                    messageBar.messageBarShow(langStrObject.GetLangString("ETCS_CALL_ERR_1"));
                    setBtnStatus(false);
                }
                else
                {
                    var errorCode = pageObject.live();
                    var ret = rootItem.dealCallError(errorCode);
                    if (ret !== 0)
                    {
                        liveBtn.setInitStatus();
                        liveBtn.setBtnStatus(false);
                        setId(0);
                        return;
                    }

                    setId(pageObject.getCallId());
                    setPlayingStatus();
                }
            }
            onUndoSignal: {
                pageObject.vaLiveStopCall(getId());
                setId(0);
                updateSelectAllAndClearBtnStatus();
                setInitStatus();
            }
            Component.onCompleted: {
            }
        }
        ImageAndTextButton2{

            function initStatus()
            {
                setImageSource(logicObject.getLocalImagePath() + "VA_EVAC_A.png");
                setDataColor("#5f5f5f");
                setBtnEnableValue(true);
                evacErrorImage.setVisibleValue(false);
            }

            function disableStatus()
            {
                setImageSource(logicObject.getLocalImagePath() + "VA_EVAC_B.png");
                setDataColor("#40666666");
                setBtnEnableValue(false);
                evacErrorImage.setVisibleValue(false);
            }

            function errorStatus()
            {
                setImageSource(logicObject.getLocalImagePath() + "VA_EVAC_A.png");
                setDataColor("#5f5f5f");
                setBtnEnableValue(true);
                evacErrorImage.setVisibleValue(true);
            }

            id: evacBtn
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: liveBtn.right
            width: parent.width / 3
            height: parent.height
            imageSource: logicObject.getLocalImagePath() + "VA_EVAC_A.png"
            info: langStrObject.GetLangString("ETCS_EVAC")
            bkgPressedColor: "#DBE0E6"
            bkgReleasedColor: "#f3f3f3"
            enable: pageObject.isAlertExist()
            onClickedSignal: {
                var errorNum = pageObject.GetEVAErrorCode();
                var ret = checkError(errorNum);
                if (ret !== 0)
                {
                    return;
                }

                if (checkZoneValid() === false)
                {
                    messageBar.messageBarShow(langStrObject.GetLangString("ETCS_CALL_ERR_1"));
                }
                else
                {
                    var errorCode = pageObject.evacuation();
                    ret = rootItem.dealCallError(errorCode);
                    if (ret !== 0)
                    {
                        return;
                    }
                }
            }

            CommonImage{
                id: evacErrorImage
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
                source: logicObject.getLocalImagePath() + "error.png"
                visible: false
            }
        }
        ImageAndTextButton2{

            function initStatus()
            {
                setImageSource(logicObject.getLocalImagePath() + "VA_Alert_A.png");
                setDataColor("#5f5f5f");
                setBtnEnableValue(true);
                alertErrorImage.setVisibleValue(false);
            }

            function disableStatus()
            {
                setImageSource(logicObject.getLocalImagePath() + "VA_Alert_B.png");
                setDataColor("#40666666");
                setBtnEnableValue(false);
                alertErrorImage.setVisibleValue(false);
            }

            function errorStatus()
            {
                setImageSource(logicObject.getLocalImagePath() + "VA_Alert_A.png");
                setDataColor("#5f5f5f");
                setBtnEnableValue(true);
                alertErrorImage.setVisibleValue(true);
            }

            id: alertBtn
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: evacBtn.right
            width: parent.width / 3
            height: parent.height
            imageSource: logicObject.getLocalImagePath() + "VA_Alert_A.png"
            info: langStrObject.GetLangString("ETCS_ALERT")
            bkgPressedColor: "#DBE0E6"
            bkgReleasedColor: "#f3f3f3"
            onClickedSignal: {

                var errorNum = pageObject.GetAlertErrorCode();
                var ret = checkError(errorNum);
                if (ret !== 0)
                {
                    return;
                }

                if (checkZoneValid() === false)
                {
                    messageBar.messageBarShow(langStrObject.GetLangString("ETCS_CALL_ERR_1"));
                }
                else
                {
                    var errorCode = pageObject.alert();
                    ret = rootItem.dealCallError(errorCode);
                    if (ret !== 0)
                    {
                        return;
                    }
                }
            }

            CommonImage{
                id: alertErrorImage
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
                source: logicObject.getLocalImagePath() + "error.png"
                visible: false
            }
        }
    }
    Component.onCompleted: {
        rootItem.updateLanguage.connect(updateLanguageSlot);
        rootItem.changeUserSignal.connect(changeUserSlot);
        keyBoard.addCharSignal.connect(textEdit.addCharSlot);
        keyBoard.delCharSignal.connect(textEdit.delCharSlot);
        keyBoard.clearSignal.connect(textEdit.clearSlot);
        logicObject.signalCallStatus.connect(activateUpdateSlot);
    }
}
