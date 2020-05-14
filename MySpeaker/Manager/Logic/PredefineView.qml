import QtQuick 2.0
import QtQml.Models 2.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

import "../Components"
import "../Keyboard"

Rectangle {

    property int predefineIndex: 0
    property int gridViewTitleBarHeight: 50
    property int gridViewHeight: height - mainPage.g_titleBarHeight - gridViewTitleBarHeight
    property int predefinedViewCallHeight: height + mainPage.businessBarHeight
    property int gridviewSpaceDistance: 18
    property string predefineItemStatus: "Normal"  // Normal, Edit
    property string viewType: "list"   //list or grid

    property string wait_status: "wait"
    property string full_status: "full"
    property string part_status: "part"
    property string dis_status: "disconnect"

    property int delPredefineIndex: -1

    function updateLanguageSlot()
    {
        gridViewTitleData.text = langStrObject.GetLangString("ETCS_PREDEFINE");
        editBtn.setTextInfo(langStrObject.GetLangString("ETCS_EDIT"));
        saveBtn.setTextInfo(langStrObject.GetLangString("ETCS_OK"));
        updatePredefineStatusLang();
    }

    function changeUserSlot(value)
    {
        if (value === 0)
        {
            editBtn.setVisibleValue(true);
            saveBtn.setVisibleValue(false);
            setPredefineItemStatus("Normal");
        }
        else if (value === 1)
        {
            editBtn.setVisibleValue(true);
            saveBtn.setVisibleValue(false);
            setPredefineItemStatus("Normal");
        }
        else if (value === 2)
        {
            editBtn.setVisibleValue(false);
            saveBtn.setVisibleValue(false);
            setPredefineItemStatus("Normal");
        }
    }

    function initStatus()
    {
        setVisibleValue(true);
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function addPredefineSlot()
    {
        updateEditAndOKStatus();
        updateViewTypeBtnStatus();
    }

    function clearAndSync()
    {
        gridViewUnselectAll();
    }

    function clearVisualModel()
    {
        visualModel.selectedItems.remove(0, visualModel.selectedItems.count);
    }

    function gridViewUnselectAll()
    {
        var count = visualModel.model.rowCount();
        for (var i = 0; i < count; i++)
        {
            visualModel.model.setData(visualModel.model.index(i, 0), 0, 3);
        }
        return 0;
    }

    function selectOne(index, value, role)
    {
        visualModel.model.setData(visualModel.model.index(index, 0), 1, role);
    }

    function unselectOne(index, value, role)
    {
        visualModel.model.setData(visualModel.model.index(index, 0), 0, role);
    }

    function getPredefineSelectedIndex()
    {
        var count = predefineModel.rowCount();
        var select;
        var index;
        for (var i = 0; i < count; i++)
        {
            select = predefineModel.data(predefineModel.index(i, 0), 3);
            if (Number(select) === 1)
            {
                index = predefineModel.data(predefineModel.index(i, 0), 1);
                break;
            }
        }
        return index;
    }

    function updatePredefineStatusLang()
    {
        var count = predefineModel.rowCount();
        var status;
        var cur_index = gridView.currentIndex
        for (var i = 0; i < count; i++)
        {
            gridView.currentIndex = i
            gridView.currentItem.children[0].updateCallBtnStatus();
        }
        gridView.currentIndex = cur_index
    }

    function setPredefineListSelect()
    {
        var count = predefineModel.rowCount();
        var select;
        var index;

        logicObject.predefineUnselectAll();
        for (var i = 0; i < count; i++)
        {
            select = predefineModel.data(predefineModel.index(i, 0), 3);
            if (Number(select) === 1)
            {
                index = predefineModel.data(predefineModel.index(i, 0), 1);
                logicObject.setPredefineSelected(Number(index), 1);
            }
        }
        return 0;
    }

    function setPredefineViewCallStatus(id)
    {
        predefinedViewCall.setStatus(id);
    }

    function setPredefineItemStatus(value)
    {
        predefineItemStatus = value;
    }

    function getPredefineItemStatus()
    {
        return predefineItemStatus;
    }

    function setViewType(type)
    {
        viewType = type;
    }

    function getViewType()
    {
        return viewType;
    }

    function updateEditAndOKStatus()
    {
        var vt;
        var pis;
        var count;

        count = predefineModel.rowCount();
        vt = getViewType();
        pis = getPredefineItemStatus();

        if (count <= 0 || vt === "grid")
        {
            editBtn.setVisibleValue(false);
            saveBtn.setVisibleValue(false);
            return;
        }

        if (pis === "Edit")
        {
            editBtn.setVisibleValue(false);
            saveBtn.setVisibleValue(true);
        }
        else
        {
            editBtn.setVisibleValue(true);
            saveBtn.setVisibleValue(false);
        }
    }

    function updateViewTypeBtnStatus()
    {
        var vt;
        var pis;
        var count;

        vt = getViewType();
        count = predefineModel.rowCount();
        pis = getPredefineItemStatus();

        if (count <= 0 || pis === "Edit")
        {
            listBtn.setVisibleValue(false);
            gridBtn.setVisibleValue(false);
            return;
        }

        if (vt === "list")
        {
            listBtn.setVisibleValue(true);
            gridBtn.setVisibleValue(false);
        }
        else
        {
            listBtn.setVisibleValue(false);
            gridBtn.setVisibleValue(true);
        }
    }

    function getSearchBarText()
    {
        return textEdit.getText();
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

    function clearSearchBarText()
    {
        textEdit.clearSlot();
    }

    function setDelPredefineIndex(value)
    {
        delPredefineIndex = value;
    }

    function getDelPredefineIndex()
    {
        return delPredefineIndex;
    }

    function confirmMaskOKSlot()
    {
        if (getDelPredefineIndex() === -1)
        {
            return ;
        }

        logicObject.delPredefineDeep(getDelPredefineIndex());
        updateEditAndOKStatus();
        updateViewTypeBtnStatus();
        setDelPredefineIndex(-1);
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
                width: parent.width - searchImage.width - 5 - 5 -10
                height: parent.height
                color: "#ffffff"
                maximumLength: 30
                onFocusSignal: {
                    inputStatus();
                    mainPage.hideBusinessBar();
                }
                onTextChanged: {
                    predefineModel.setFilterFixedString(textEdit.getText());
                    updateEditAndOKStatus();
                    updateViewTypeBtnStatus();
                }
            }
        }

        TextButton2{
            id: editBtn
            width: nameContentWidth < parent.height ? parent.height : nameContentWidth
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 5
            nameWidth: parent.width - searchBar.width - 18 - 5
            fontSize: 18
            info: langStrObject.GetLangString("ETCS_EDIT")
            textColor: "#ffffff"
            visible: true
            bkgPressedColor: "#505050"
            onClickedSignal: {
                setPredefineItemStatus("Edit");
                updateEditAndOKStatus();
                updateViewTypeBtnStatus();
            }
        }

        TextButton2{
            id: saveBtn
            width: nameContentWidth < parent.height ? parent.height : nameContentWidth
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 5
            nameWidth: parent.width - searchBar.width - 18 - 5
            fontSize: 18
            info: langStrObject.GetLangString("ETCS_OK")
            textColor: "#ffffff"
            visible: false
            bkgPressedColor: "#505050"
            onClickedSignal: {
                setPredefineItemStatus("Normal");
                updateEditAndOKStatus();
                updateViewTypeBtnStatus();
            }
        }
    }

    Rectangle{
        id: gridViewTitleBar
        anchors.top: titleBar.bottom
        anchors.left: parent.left
        width: parent.width
        height: gridViewTitleBarHeight

        Rectangle{
            width: 4
            height: 24
            color: "#4f4f4f"
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 18
        }

        CommonText{
            id: gridViewTitleData
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 28
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_PREDEFINE")
            font.family: rootItem.fontName
            font.pixelSize: 22
            width: parent.width - listBtn.width - 22 - 22
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
            Component.onCompleted: {
            }
        }

        ImageButton{
            id: listBtn
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 22
            width: parent.height
            height: parent.height
            buttonImageSource: logicObject.getLocalImagePath() + "list_sort.png"
            visible: true
            onClickedSignal: {
                setViewType("grid");
                updateEditAndOKStatus();
                updateViewTypeBtnStatus();
            }
        }
        ImageButton{
            id: gridBtn
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 22
            width: parent.height
            height: parent.height
            buttonImageSource: logicObject.getLocalImagePath() + "grid_sort.png"
            visible: false
            onClickedSignal: {
                setViewType("list");
                updateEditAndOKStatus();
                updateViewTypeBtnStatus();
            }
        }
    }

    HorizontalLine{
        anchors.bottom: gridViewTitleBar.bottom
        z: 1
    }

    ScrollBar{
        id: scrollBar
        height: gridView.height
        z: 1
        anchors.top: gridView.top
        anchors.right: parent.right
        anchors.rightMargin: 6
        position: gridView.visibleArea.yPosition
        pageSize: gridView.visibleArea.heightRatio
        scrollBarVisible: gridView.visibleArea.heightRatio == 1 ? false : true
    }

    GridView{

        function activateUpdateSlot(id, text)
        {
            var count = predefineModel.rowCount();
            var tmpid;
            for (var i = 0; i < count; i++)
            {
                tmpid = predefineModel.data(predefineModel.index(i, 0), 13);
                if (tmpid === id)
                {
                    gridView.currentIndex = i;
                    gridView.currentItem.children[0].activateUpdateSlot(id, text);
                    break;
                }
            }
        }

        function setGongTrueStatus(id)
        {
            var count = predefineModel.rowCount();
            var tmpid;
            for (var i = 0; i < count; i++)
            {
                tmpid = predefineModel.data(predefineModel.index(i, 0), 13);
                if (tmpid === id)
                {
                    gridView.currentIndex = i;
                    gridView.currentItem.children[0].setGongTrueStatus(id);
                    break;
                }
            }
        }

        function setGongFalseStatus(id)
        {
            var count = predefineModel.rowCount();
            var tmpid;
            for (var i = 0; i < count; i++)
            {
                tmpid = predefineModel.data(predefineModel.index(i, 0), 13);
                if (tmpid === id)
                {
                    gridView.currentIndex = i;
                    gridView.currentItem.children[0].setGongFalseStatus(id);
                    break;
                }
            }
        }

        id: gridView
        width: parent.width - gridviewSpaceDistance
        height: gridViewHeight
        anchors.top: gridViewTitleBar.bottom
        boundsBehavior: Flickable.StopAtBounds
        snapMode: GridView.NoSnap
        cellWidth: viewType == "list" ? width : (width / 3)
        cellHeight: 105
        clip: true
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

        model: predefineModel
        delegate:Rectangle{
            id: gridViewElement
            width: gridView.cellWidth
            height: gridView.cellHeight

            Rectangle{

                function setColor(value)
                {
                    e_color = value;
                }

                function setText(value)
                {
                    e_text = value;
                }

                function setBorderColor(value)
                {
                    e_borderColor = value;
                }

                function setBorderWidth(value)
                {
                    e_borderWidth = value;
                }

                function setCallStatusValue(value)
                {
                    e_callStatus = value;
                }

                function getCallStatusValue()
                {
                    return e_callStatus;
                }

                function setHaveGongValue(value)
                {
                    e_haveGong = value;
                }

                function getHaveGongValue()
                {
                    return e_haveGong;
                }

                function activateUpdateSlot(tmpid, text)
                {
                    if  (e_callId === 0)
                    {
                        return ;
                    }

                    if (tmpid === e_callId)
                    {
                        if (text === dis_status)
                        {            
                            setCallStatusValue(dis_status);
                            updateCallBtnStatus();
                            e_callId = 0;
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
                }

                function setGongTrueStatus(tmpid)
                {
                    if (tmpid === e_callId)
                    {
                        setHaveGongValue(true);
                        updateCallBtnStatus();
                    }
                }

                function setGongFalseStatus(tmpid)
                {
                    if (tmpid === e_callId)
                    {
                        setHaveGongValue(false);
                        updateCallBtnStatus();
                    }
                }

                function callBtnWaitStatus()
                {
                    statusTimer.stop();
                    setColor("#ffffff");
                    setText(langStrObject.GetLangString("ETCS_WAIT"));
                    setBorderWidth(2);
                    setBorderColor("#00c767");
                }

                function callBtnFullGongStatus()
                {
                    e_timerMode = 1;
                    e_timerCount = 0;
                    statusTimer.start();
                    setText(langStrObject.GetLangString("ETCS_FULL_CONNECT"));
                    setBorderWidth(1);
                    setBorderColor("#c0c0c0");
                }

                function callBtnFullStatus()
                {
                    statusTimer.stop();
                    setColor("#00c767");
                    setText(langStrObject.GetLangString("ETCS_FULL_CONNECT"));
                    setBorderWidth(1);
                    setBorderColor("#c0c0c0");
                }

                function callBtnPartGongStatus()
                {
                    e_timerMode = 1;
                    e_timerCount = 0;
                    statusTimer.start();
                    setText(langStrObject.GetLangString("ETCS_PARTLY_CONNECT"));
                    setBorderWidth(1);
                    setBorderColor("#c0c0c0");
                }

                function callBtnPartStatus()
                {
                    statusTimer.stop();
                    setColor("#00c767");
                    setText(langStrObject.GetLangString("ETCS_PARTLY_CONNECT"));
                    setBorderWidth(1);
                    setBorderColor("#c0c0c0");
                }

                function callBtnDisconnectStatus()
                {
                    statusTimer.stop();
                    setColor("#ffffff");
                    setBorderWidth(1);
                    setBorderColor("#c0c0c0");
                    setText("");
                    gridViewElement.VisualDataModel.inSelected = false;
                }

                function updateCallBtnStatus()
                {
                    if (getCallStatusValue() === wait_status)
                    {
                        callBtnWaitStatus();
                    }
                    else if (getCallStatusValue() === full_status)
                    {
                        if (getHaveGongValue() === true)
                        {
                            callBtnFullGongStatus();
                        }
                        else
                        {
                            callBtnFullStatus();
                        }
                    }
                    else if (getCallStatusValue() === part_status)
                    {
                        if (getHaveGongValue() === true)
                        {
                            callBtnPartGongStatus();
                        }
                        else
                        {
                            callBtnPartStatus();
                        }
                    }
                    else if (getCallStatusValue() === dis_status)
                    {
                        callBtnDisconnectStatus();
                    }
                }

                id: item
                width: getViewType() === "list" ? (gridView.width - 18) :  (gridView.width - 3*18) / 3
                height: 90
                anchors.left: parent.left
                anchors.leftMargin: 18
                anchors.top: parent.top
                anchors.topMargin: 15
                color: e_color
                border.width: e_borderWidth
                border.color: e_borderColor

                Timer{
                    id: statusTimer
                    interval: 200
                    repeat: true
                    triggeredOnStart: true
                    onTriggered: {
                        if (e_timerMode === 0)
                        {
                            if (e_timerTmp === false)
                            {
                                e_timerTmp = true;
                                parent.setColor("#00c767");
                            }
                            else
                            {
                                e_timerTmp = false;
                                parent.setColor("#ffffff");
                            }
                        }
                        else if (e_timerMode === 1)
                        {
                            e_timerCount++;
                            if (e_timerCount === 1)
                            {
                                parent.setColor("#00c767");
                            }
                            else if (e_timerCount === 2)
                            {
                                parent.setColor("#ffffff");
                            }
                            else if (e_timerCount === 3)
                            {
                                parent.setColor("#00c767");
                            }
                            else if (e_timerCount === 4)
                            {
                                parent.setColor("#ffffff");
                            }
                            else if (e_timerCount > 8)
                            {
                                e_timerCount = 0;
                            }
                        }
                    }
                }

                CommonImage{
                    id: localErrorImage
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: getViewType() === "list" ? 18 : 5
                    source: logicObject.getLocalImagePath() + "error.png"
                    visible: e_isError
                }

                Text{
                    id: predefineName;
                    text: e_name
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: getNameAnchorsLeft()
                    anchors.leftMargin: getNameAnchorsLeftMargin()
                    anchors.rightMargin: 5
                    width: getNameWidth()
                    height: 20
                    color: "#303030"
                    font.family: rootItem.fontName
                    font.bold: false
                    font.pixelSize: 18
                    elide: Text.ElideRight
                    wrapMode: Text.Wrap
                    horizontalAlignment: getViewType() === "list" ? Text.AlignLeft : (e_isError === true ? Text.AlignLeft :Text.AlignHCenter)
                    function getNameAnchorsLeft()
                    {
                        if (e_isError === true)
                        {
                            return localErrorImage.right;
                        }
                        return parent.left;
                    }

                    function getNameAnchorsLeftMargin()
                    {
                        if (e_isError === true)
                        {
                            return 0;
                        }

                        if (getViewType() === "list")
                        {
                            return 18;
                        }

                        return 5;
                    }

                    function getNameWidth()
                    {
                        var name_width = parent.width;

                        // - lefeMargin
                        var lefeMargin = 5;
                        if (getViewType() === "list")
                        {
                            lefeMargin = 18
                        }
                        name_width = name_width - lefeMargin;

                        // - ErrorImage
                        if (e_isError === true)
                        {
                            name_width = name_width - localErrorImage.width;
                        }

                        // - detailsBtn
                        if (getViewType() === "list")
                        {
                            name_width = name_width - detailsBtn.width;
                        }

                        // - rightMargin
                        name_width = name_width - 5;

                        return name_width;
                    }
                }

                CommonText{
                    id: status
                    text: e_text
                    anchors.top: parent.top
                    anchors.topMargin: 60
                    anchors.left: predefineName.anchors.left
                    anchors.leftMargin: predefineName.anchors.leftMargin
                    anchors.rightMargin: 5
                    width: predefineName.width
                    height: 20
                    color: "#303030"
                    font.family: rootItem.fontName
                    font.bold: false
                    font.pixelSize: 18
                    elide: Text.ElideRight
                    wrapMode: Text.Wrap
                    horizontalAlignment: getViewType() === "list" ? Text.AlignLeft : (e_isError === true ? Text.AlignLeft :Text.AlignHCenter)
                }

                ImageButton{
                    id: detailsBtn
                    width: 60
                    height: 60
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                    buttonImageSource: logicObject.getLocalImagePath() + "more_1.png"
                    visible: ((e_predefineType !== 1) && (getViewType() === "list") &&
                              (getPredefineItemStatus() === "Normal") && (e_isError === false)) ? true : false
                    z: 1
                    bkgPressedColor: "#40505050"
                    onClickedSignal: {       
                        mainPage.hideBusinessBar();
                        clearAndSync();
                        selectOne(index, 1 ,3);
                        setPredefineListSelect();
                        setPredefineViewCallStatus(getPredefineSelectedIndex());
                        predefinedViewCall.updateCallStatus(e_callId, e_callStatus, e_haveGong);
                        normalStatus();
                    }
                }

                ImageButton{
                    id: deleteBtn
                    width: 60
                    height: parent.height - 2
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                    buttonImageSource: logicObject.getLocalImagePath() + "recycle.png"
                    visible: ((getViewType() === "list") &&
                              (e_myConfig == true) &&
                              (getPredefineItemStatus() === "Edit")) ? true : false
                    z: 1
                    onClickedSignal: {
                        setDelPredefineIndex(e_index);
                        confirmMask.setVisibleValue(true);
                    }
                }

                SequentialAnimation{
                    id: clickEffect
                    running: false
                    ColorAnimation{
                        target: item
                        property: "color"
                        from: "#ffffff"
                        to: "#00c767"
                        duration: 0
                    }
                    ColorAnimation {
                        target: item
                        property: "color"
                        from: "#00c767"
                        to: "#ffffff"
                        duration: 30
                    }
                }

                MouseArea{
                    id: mouseArea
                    anchors.fill: parent
                    enabled: true
                    onClicked:{

                        if (getPredefineItemStatus() === "Edit")
                        {
                            noticeEditMode.setVisibleValue(true);
                            return;
                        }

                        if (e_isError === true)
                        {
                            var errorNum;
                            errorNum = logicObject.getPredefineIntValue(e_index, "errorNum");
                            if (errorNum === 2)
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
                        }
                        else
                        {
                            if (e_type !== 3)
                            {
                                gridViewElement.VisualDataModel.inSelected = !gridViewElement.VisualDataModel.inSelected;
                                var curCallStatus = parent.getCallStatusValue();
                                if ("" === curCallStatus || dis_status === curCallStatus)
                                {
                                    clearAndSync();
                                    selectOne(index, 1 ,3);
                                    setPredefineListSelect();
                                    var errorCode = pageObject.predefineCall(logicObject.getPredefineFloatValue(getPredefineSelectedIndex(), "Volume"));
                                    var ret = rootItem.dealCallError(errorCode);
                                    if (ret !== 0)
                                    {
                                        parent.setCallStatusValue(dis_status);
                                        parent.updateCallBtnStatus();
                                        e_callId = 0;
                                        return;
                                    }

                                    e_callId = pageObject.getCallId();
                                    parent.setCallStatusValue(wait_status);
                                    parent.updateCallBtnStatus();
                                }
                                else
                                {
                                    pageObject.predefineStopCallFromId(e_callId);
                                    parent.setCallStatusValue(dis_status);
                                    parent.updateCallBtnStatus();
                                    e_callId = 0;
                                }
                            }
                            else
                            {
                                clickEffect.start();
                                clearAndSync();
                                selectOne(index, 1 ,3);
                                setPredefineListSelect();
                                pageObject.predefineVolumeControl();
                            }
                        }
                    }
                }

                Component.onCompleted: {

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
        z: 2
        onOkSignal: {
            normalStatus();
            mainPage.showBusinessBar();
        }
    }

    PredefinedViewCall{
        id: predefinedViewCall
        z: 3
        width: parent.width
        height: predefinedViewCallHeight
        visible: false
    }

    Component.onCompleted: {
        rootItem.updateLanguage.connect(updateLanguageSlot);
        rootItem.changeUserSignal.connect(changeUserSlot);
        logicObject.addPredefineSignal.connect(addPredefineSlot);
        confirmMask.okSignal.connect(confirmMaskOKSlot);
        keyBoard.addCharSignal.connect(textEdit.addCharSlot);
        keyBoard.delCharSignal.connect(textEdit.delCharSlot);
        keyBoard.clearSignal.connect(textEdit.clearSlot);
        logicObject.signalCallStatus.connect(gridView.activateUpdateSlot);
        logicObject.gongPlayingTrueSignal.connect(gridView.setGongTrueStatus);
        logicObject.gongPlayingFalseSignal.connect(gridView.setGongFalseStatus);
    }
}
