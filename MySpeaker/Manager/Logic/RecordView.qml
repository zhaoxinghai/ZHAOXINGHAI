import QtQuick 2.0
import QtQml.Models 2.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

import "../Components"
import "../Keyboard"

Rectangle {

    property int recordTitleBarHeight: 116
    property int funcBtnBarHeight: 150
    property int areaHeight: height - mainPage.g_titleBarHeight
                            - recordTitleBarHeight - funcBtnBarHeight

    property string wait_status: "wait"
    property string full_status: "full"
    property string part_status: "part"
    property string dis_status: "disconnect"

    function updateLanguageSlot()
    {
        title.text = langStrObject.GetLangString("ETCS_RECORD");
        prevBtn.dataText = langStrObject.GetLangString("ETCS_MORE");
        prelistenBtn.disableStatus();
        recordBtn.initStatus();
        okBtn.disableStatus();
        saveBtn.setTextInfo(langStrObject.GetLangString("ETCS_OK"));
        deleteBtn.setTextInfo(langStrObject.GetLangString("ETCS_DELETE"));
        cancelBtn.setTextInfo(langStrObject.GetLangString("ETCS_CANCEL"));
        editSaveBtn.setTextInfo(langStrObject.GetLangString("ETCS_SAVE"));
    }

    function localRecordStatusSlot(errorNum)
    {
        if (errorNum === 7)
        {
            stopStatus();
            localErrorMask.initStatus(langStrObject.GetLangString("ETCS_LOCAL_MICROPHONE_BUSY"));
        }
        else if (errorNum === 0)
        {

        }
        else
        {
            stopStatus();
            localErrorMask.initStatus(langStrObject.GetLangString("ETCS_LOCAL_RECORD_ERROR"));
        }
    }

    function initStatus()
    {
        setVisibleValue(true);
        recordObject.removeAllCacheRecord();
        recordObject.updateAutosaveToRecordList();
        listModel.clearListModel();
        listModel.createListModel();
        recordTimer.setMaxCount(recordObject.getRecordMaxTime() * 60);
    }

    function quitStatus()
    {
        setVisibleValue(false);
        recordTimer.stop();
        recordTimer.resetCount();
        recordTime.setText(recordObject.getFormatTime(0));
        if (recordObject.getCachePrelistenId() !== 0)
        {
            recordObject.stopPrelistenRecord();
            recordObject.setCachePrelistenId(0);
        }

        if (recordObject.getRecordId() !== 0)
        {
            recordObject.stopRecord();
            recordObject.setRecordId(0);
        }

        if (recordObject.getPrelistenId() !== 0)
        {
            recordObject.stopPrelistenRecord();
            recordObject.setPrelistenId(0);
        }
        if (visualModel.selectedItems.count > 0)
        {
            visualModel.selectedItems.remove(0, visualModel.selectedItems.count);
        }

        recordObject.removeAllCacheRecord();
        prelistenBtn.disableStatus();
        recordBtn.initStatus();
        okBtn.disableStatus();
        mainPage.showBusinessBar();
        recordListMask.setVisibleValue(false);
    }

    function stopStatus()
    {
        recordTimer.stop();
        recordTimer.resetCount();
        recordTime.setText(recordObject.getFormatTime(0));
        if (recordObject.getCachePrelistenId() !== 0)
        {
            recordObject.stopCachePrelistenRecord();
            recordObject.setCachePrelistenId();
        }

        if (recordObject.getRecordId() !== 0)
        {
            recordObject.stopRecord();
            recordObject.setRecordId(0);
        }

        if (recordObject.getPrelistenId() !== 0)
        {
            recordObject.stopPrelistenRecord();
            recordObject.setPrelistenId(0);
        }

        recordObject.removeAllCacheRecord();
        prelistenBtn.disableStatus();
        recordBtn.initStatus();
        okBtn.disableStatus();
        recordListMask.setVisibleValue(false);
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    MouseArea{
        anchors.fill: parent
        onClicked: {

        }
    }

    Rectangle{
        id: titleBar
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width
        height: mainPage.g_titleBarHeight
        color: mainPage.g_titleColor

        CommonText{
            id: title
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: mainPage.g_titleTextColor
            text: langStrObject.GetLangString("ETCS_RECORD")
            font.family: rootItem.fontName
            font.pixelSize: mainPage.g_titleTextFontSize
            width: 200
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            Component.onCompleted: {
            }
        }

        ImageAndTextButton{
            id: prevBtn
            width: imageWidth + dataContentWidth - 8 + 35
            height: 60
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            imgLeftMargin: 14
            dataLeftMargin: -8
            imageSource: logicObject.getLocalImagePath() + "back.png"
            visible: true
            bkgPressedColor: "#505050"
            dataText: langStrObject.GetLangString("ETCS_MORE");
            dataWidth: 240 - (title.contentWidth/2) - imageWidth - 14 + 8
            onClickedSignal: {
                quitStatus();
            }
            Component.onCompleted: {
            }
        }  
    }

    Rectangle{
        id: recordTitleBar
        width: 480
        height: recordTitleBarHeight
        anchors.top: titleBar.bottom
        color: "#202020"
        z: 3
        CommonText{
            id: recordTime
            anchors.top: parent.top
            anchors.topMargin: 35
            anchors.left: parent.left
            anchors.leftMargin: 22   
            font.pixelSize: 38
            color: "#ffffff"
            text: recordObject.getFormatTime(0);
            width: 400
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
        }
    }

    Rectangle{
        id: funcBtnBar
        width: 480
        height: funcBtnBarHeight
        anchors.top: titleBar.bottom
        anchors.topMargin: 116
        color: "#202020"
        ImageAndTextButton3{

            property int cachePrelistenId: 0

            function disableStatus()
            {
                setBtnEnableValue(false);
                setImageSource(logicObject.getLocalImagePath() + "record_prelisten_1_C.png");
                setInfoValue(langStrObject.GetLangString("ETCS_PRELISTEN"));
                setDataColor("#d8d8d8");
            }

            function preListenStatus()
            {
                setBtnEnableValue(true);
                setImageSource(logicObject.getLocalImagePath() + "record_prelisten_1_B.png");
                setInfoValue(langStrObject.GetLangString("ETCS_STOP"));
                setDataColor("#ffffff");
            }

            function initStatus()
            {
                setBtnEnableValue(true);
                setImageSource(logicObject.getLocalImagePath() + "record_prelisten_1_A.png");
                setInfoValue(langStrObject.GetLangString("ETCS_PRELISTEN"));
                setDataColor("#ffffff");
                setBtnStatus(false);
            }

            function updatePrelistenPlayingStatusSlot(id, text)
            {
                if (cachePrelistenId !== id)
                {
                    return;
                }

                if (text === dis_status)
                {
                    prelistenBtn.initStatus();
                    cachePrelistenId = 0;
                }
                else if (text === full_status)
                {
                    prelistenBtn.preListenStatus();
                }
            }

            id: prelistenBtn
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            width: 160
            height: 100
            topMargin1: 20
            topMargin2: 20
            imgWidth: 36
            imgHeight: 35
            color: parent.color
            imageSource: logicObject.getLocalImagePath() + "record_prelisten_1_A.png"
            info: langStrObject.GetLangString("ETCS_PRELISTEN")

            onSelectSignal: {
                var errorCode = recordObject.prelistenCacheRecord();
                var ret = rootItem.dealCallError(errorCode);
                if (ret !== 0)
                {
                    prelistenBtn.initStatus();
                    cachePrelistenId = 0;
                    return;
                }
                cachePrelistenId = recordObject.getCachePrelistenId();
            }

            onUndoSignal: {
                recordObject.stopPrelistenCacheRecord();
                recordObject.setCachePrelistenId(0);
            }

            Component.onCompleted: {
                logicObject.signalCallStatus.connect(updatePrelistenPlayingStatusSlot);
            }
        }
        ImageAndTextButton3{
            function disableStatus()
            {
                setBtnEnableValue(false);
                setImageSource(logicObject.getLocalImagePath() + "record_start_C.png");
                setInfoValue(langStrObject.GetLangString("ETCS_CALL"));
                setDataColor("#d8d8d8");
            }

            function recordStatus()
            {
                setBtnEnableValue(true);
                setImageSource(logicObject.getLocalImagePath() + "record_start_B.png");
                setInfoValue(langStrObject.GetLangString("ETCS_PAUSE"));
                setDataColor("#ffffff");
            }

            function initStatus()
            {
                setBtnEnableValue(true);
                setImageSource(logicObject.getLocalImagePath() + "record_start_A.png");
                setInfoValue(langStrObject.GetLangString("ETCS_CALL"));
                setDataColor("#ffffff");
                setBtnStatus(false);
            }

            function initRecordEnv()
            {
                var ret;
                ret = recordObject.isOverRecordStorage();
                if (true === ret)
                {
                    promptMask.messageShow(langStrObject.GetLangString("ETCS_RECORD_STORAGE_FULL"));
                    return false;
                }

                ret = recordObject.isOverRecordMaxCount();
                if (true === ret)
                {
                    promptMask.messageShow(langStrObject.GetLangString("ETCS_RECORD_FULL"));
                    return false;
                }

                var remain_snd = recordObject.getRemainRecordSecond();
                if (remain_snd < 1)
                {
                    promptMask.messageShow(langStrObject.GetLangString("ETCS_RECORD_STORAGE_FULL"));
                    return false;
                }

                recordTimer.setMaxCount(remain_snd);
                return true;
            }

            id: recordBtn
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: prelistenBtn.right
            width: 160
            height: 100
            imgWidth: 56
            imgHeight: 54
            topMargin1: 10
            topMargin2: 12
            color: parent.color
            imageSource: logicObject.getLocalImagePath() + "record_start_A.png"
            info: langStrObject.GetLangString("ETCS_CALL")

            onSelectSignal: {

                var ret;
                ret = initRecordEnv();
                if (false === ret)
                {
                    timeOutTimer.setInterval(3000);
                    timeOutTimer.start();
                    recordBtn.initStatus();
                    return;
                }

                prelistenBtn.disableStatus();
                recordBtn.recordStatus();
                okBtn.disableStatus();
                recordListMask.setVisibleValue(true);

                if (recordObject.getCachePrelistenId() !== 0)
                {
                    recordObject.stopPrelistenCacheRecord();
                    recordObject.setCachePrelistenId(0);
                }
                if (recordObject.getPrelistenId() !== 0)
                {
                    recordObject.stopPrelistenRecord();
                    recordObject.setPrelistenId(0);
                }
                recordObject.startRecord();
                recordTimer.start();
                prelistenBtn.disableStatus();
            }

            onUndoSignal: {
                recordBtn.initStatus();
                prelistenBtn.initStatus();
                okBtn.initStatus();
                recordObject.stopRecord();
                recordObject.setRecordId(0);
                recordTimer.stop();
            }
            Component.onCompleted: {

            }
        }
        ImageAndTextButton3{

            function disableStatus()
            {
                setBtnEnableValue(false);
                setImageSource(logicObject.getLocalImagePath() + "record_OK_B.png");
                setInfoValue(langStrObject.GetLangString("ETCS_SAVE"));
                setDataColor("#d8d8d8");
            }

            function initStatus()
            {
                setBtnEnableValue(true);
                setImageSource(logicObject.getLocalImagePath() + "record_OK_A.png");
                setInfoValue(langStrObject.GetLangString("ETCS_SAVE"));
                setDataColor("#ffffff");
                setBtnStatus(false);
            }

            id: okBtn
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: recordBtn.right
            width: 160
            height: 100
            topMargin1: 20
            topMargin2: 20
            imgWidth: 36
            imgHeight: 35
            color: parent.color
            imageSource: logicObject.getLocalImagePath() + "record_OK_A.png"
            info: langStrObject.GetLangString("ETCS_SAVE")

            onSelectSignal: {
                if (recordObject.getCachePrelistenId() !== 0)
                {
                    recordObject.stopPrelistenCacheRecord();
                    recordObject.setCachePrelistenId(0);
                }
                prelistenBtn.disableStatus();
                recordBtn.initStatus();
                okBtn.disableStatus();

                keyBoard.initStatus();
                recordTimer.resetCount();
                recordTime.setText(recordObject.getFormatTime(0));
                nameData.setText(recordObject.getAutoName());
                nameData.setFocus(true);

                recordConfirmMask.addModelStatus();
                recordConfirmMask.setVisibleValue(true);
            }

            Component.onCompleted: {
            }
        }
    }

    HorizontalLine{
        anchors.top: funcBtnBar.bottom
        z: 1
    }

    ListModel{
        function createListModel()
        {
            var index;
            var count;

            count = recordObject.getRecordListCount();
            var end_itr = 0
            var isCCCFVersion = recordObject.isCCCFVersion();
            if (false === isCCCFVersion)
            {
                end_itr = 1;
            }

            for (var i = count - 1; i >= end_itr; i--)
            {
                index = recordObject.getItemIndex(i);
                listModel.append({"nIndex": index});
            }
        }

        function clearListModel()
        {
           if (listModel.count > 0)
           {
                remove(0, listModel.count);
           }
        }

        function insertIndex(index)
        {
            insert(0, {"nIndex": index});
        }

        function delIndex(index)
        {
            if (listModel.count > 0)
            {
                remove(index, 1);
            }
        }

        function getCount()
        {
            return listModel.count;
        }

        id: listModel
    }

    ScrollBar{
        id: scrollBar
        anchors.top: listView.top
        anchors.right: parent.right
        anchors.rightMargin: 6
        height: listView.height
        position: listView.visibleArea.yPosition
        pageSize: listView.visibleArea.heightRatio
        scrollBarVisible: listView.visibleArea.heightRatio == 1 ? false : true
        z: 1
    }

    Rectangle{

        function setVisibleValue(value)
        {
            visible = value;
        }

        id: recordListMask
        anchors.top: listView.top
        anchors.left: listView.left
        width: listView.width
        height: listView.height
        color: "#70666666"
        z: 3
        visible: false
        MouseArea{
            anchors.fill: parent
        }
    }

    ListView{

        property int oldContentHeight: 0
        property int oldClickedIndex: -1

        function setOldContentHeight(value)
        {
            oldContentHeight = value;
        }

        function getOldContentHeight()
        {
            return oldContentHeight;
        }

        function setContentY(value)
        {
            contentY = value;
        }

        function getContentY()
        {
            return contentY;
        }

        function setOldClickedIndex(value)
        {
            oldClickedIndex = value;
        }

        function getOldClickedIndex()
        {
            return oldClickedIndex;
        }

        id: listView
        width: 480
        height: areaHeight
        anchors.top: funcBtnBar.bottom
        clip: true
        boundsBehavior: Flickable.StopAtBounds
        snapMode: ListView.SnapOneItem
        footer: Rectangle{height: 8}
        model: visualModel
        onMovementEnded: {

        }
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

        model: listModel

        delegate: RectButton2{
            function canEdit()
            {
                var basename = recordObject.getItemStrValue(nIndex, "baseName");
                var autosave = recordObject.getAutoSaveName();
                if (autosave === basename)
                {
                    return false;
                }
                return true;
            }

            function isSelected()
            {
                if (listViewElement.VisualDataModel.inSelected === true)
                {
                    return true;
                }
                return false;
            }

            id: listViewElement
            width: 480
            height: isSelected() ? 105 : 65
            bkgReleasedColor: "#ffffff"
            color: bkgReleasedColor
            line1Color: "#e0e0e0"
            line2Color: "#e0e0e0"

            CommonText{
                id: name
                anchors.top: parent.top
                anchors.topMargin: 15
                anchors.left: parent.left
                anchors.leftMargin: 22
                width: 290
                color: "#3d3d3d"
                text: recordObject.getItemStrValue(nIndex, "baseName");
            }

            CommonText{
                id: time
                anchors.top: name.bottom
                anchors.left: parent.left
                anchors.leftMargin: 22
                width: 290
                font.pixelSize: 15
                color: "#a7a7a7"
                text: recordObject.getItemStrValue(nIndex, "time") +
                      "  " + recordObject.getItemStrValue(nIndex, "size")
            }

            CommonRect{
                function setVisibleValue(value)
                {
                    visible = value;
                }

                id: moreFuncRect
                width: parent.width
                height: listViewElement.isSelected() ? 40 : 0
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                clip: true

                ImageButton{
                    id: editBtn
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 30
                    width: parent.height
                    height: parent.height
                    buttonImageWidth: 36
                    buttonImageHeight: 35
                    visible: listViewElement.canEdit();
                    buttonImageSource: logicObject.getLocalImagePath() + "record_edit.png"
                    onClickedSignal: {
                        if (recordObject.getPrelistenId() !== 0)
                        {
                            recordObject.stopPrelistenRecord();
                            recordObject.setPrelistenId(0);
                        }
                        keyBoard.initStatus();
                        nameData.setText(recordObject.getItemStrValue(nIndex, "baseName"));
                        nameData.setFocus(true);
                        recordConfirmMask.setOldBaseName(recordObject.getItemStrValue(nIndex, "baseName"));
                        recordConfirmMask.editModelStatus();
                        recordConfirmMask.setVisibleValue(true);
                    }
                }

                ImageButon2{

                    property int prelistenId: 0

                    function initStatus()
                    {
                        setImageSource(logicObject.getLocalImagePath() + "record_play.png");
                        setBtnStatus(false);
                    }

                    function isPlayingStatus()
                    {
                        setImageSource(logicObject.getLocalImagePath() + "record_stop.png");
                    }

                    function updateRecordPlayingStatusSlot(id, text)
                    {
                        if (prelistenId !== id)
                        {
                            return;
                        }

                        if (text === dis_status)
                        {
                            recordPlayBtn.initStatus();
                            prelistenId = 0;
                        }
                        else if (text === full_status)
                        {
                            recordPlayBtn.isPlayingStatus();
                        }
                    }

                    id: recordPlayBtn
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: parent.height
                    height: parent.height
                    buttonImageWidth: 36
                    buttonImageHeight: 35
                    visible: true
                    imageSource: logicObject.getLocalImagePath() + "record_play.png"
                    onSelectSignal: {

                        if (recordObject.getPrelistenId() !== 0)
                        {
                            recordObject.stopPrelistenRecord();
                            recordObject.setPrelistenId(0);
                        }

                        var errorCode = recordObject.prelistenRecord(nIndex);
                        var ret = rootItem.dealCallError(errorCode);
                        if (ret !== 0)
                        {
                            recordPlayBtn.initStatus();
                            prelistenId = 0;
                            return;
                        }

                        prelistenId = recordObject.getPrelistenId();
                    }

                    onUndoSignal: {
                        recordObject.stopPrelistenRecord();
                        recordObject.setPrelistenId(0);
                    }

                    Component.onCompleted: {
                        logicObject.signalCallStatus.connect(updateRecordPlayingStatusSlot);
                    }
                }

                ImageButton{
                    id: delImgBtn
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 30
                    width: parent.height
                    height: parent.height
                    buttonImageWidth: 36
                    buttonImageHeight: 35
                    visible: listViewElement.canEdit();
                    buttonImageSource: logicObject.getLocalImagePath() + "record_delete.png"
                    onClickedSignal: {
                        var filename = recordObject.getItemStrValue(nIndex, "name");
                        if (recordObject.getPrelistenId() !== 0)
                        {
                            recordObject.stopPrelistenRecord();
                            recordObject.setPrelistenId(0);
                        }
                        recordObject.removeRecordFromRecordList(nIndex);
                        recordObject.removeItemFromRecordGroup(filename);

                        listViewElement.VisualDataModel.inSelected = false; //if not set inSelected, delIndex will crash!!!
                        listModel.delIndex(index);

                        if (recordObject.getRecordListCount() <= 0)
                        {
                            iomapObject.removeRecordGroup();
                        }

                    }
                }
            }

            onClickedSignal: {
                if (listViewElement.VisualDataModel.inSelected === true)
                {
                    if (visualModel.selectedItems.count > 0)
                    {
                        visualModel.selectedItems.remove(0, visualModel.selectedItems.count);
                    }
                }
                else
                {
                    if (visualModel.selectedItems.count > 0)
                    {
                        visualModel.selectedItems.remove(0, visualModel.selectedItems.count);
                    }
                    listViewElement.VisualDataModel.inSelected = true;
                }
            }
        }
    }

    Timer{

        property int maxCount: 1800
        property int count: 0

        function resetCount()
        {
            count = 0;
        }

        function setMaxCount(value)
        {
            maxCount = value;
        }

        id: recordTimer
        interval: 1000
        repeat: true
        triggeredOnStart: false
        onTriggered: {
            if (count >= maxCount)
            {
                prelistenBtn.initStatus();
                recordBtn.disableStatus();
                okBtn.initStatus();
                if (recordObject.getRecordId() !== 0)
                {
                    recordObject.stopRecord();
                    recordObject.setRecordId(0);
                }
                recordTimer.stop();
                recordTimer.resetCount();
                return;
            }
            count++;
            recordTime.setText(recordObject.getFormatTime(count));
        }
    }

    Rectangle{

        property string oldBaseName: ""

        function setOldBaseName(value)
        {
            oldBaseName = value;
        }

        function getOldBaseName()
        {
            return oldBaseName;
        }

        function addModelStatus()
        {
            deleteBtn.setVisibleValue(true);
            cancelBtn.setVisibleValue(false);
            saveBtn.setVisibleValue(true);
            editSaveBtn.setVisibleValue(false);
            recordSaveTitle.setText(langStrObject.GetLangString("ETCS_ADD_RECORD"))
        }

        function editModelStatus()
        {
            deleteBtn.setVisibleValue(false);
            cancelBtn.setVisibleValue(true);
            saveBtn.setVisibleValue(false);
            editSaveBtn.setVisibleValue(true);
            recordSaveTitle.setText(langStrObject.GetLangString("ETCS_RENAME_RECORD"))
        }

        function setVisibleValue(value)
        {
            visible = value;
        }

        id: recordConfirmMask
        anchors.top: parent.top
        anchors.left: parent.left
        z: 4
        width: parent.width
        height: parent.height
        color: "#70666666"
        visible: false

        MouseArea{
            anchors.fill: parent
            onClicked: {
            }
        }

        Rectangle{
            id: recordConfirmRect
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: -100
            anchors.horizontalCenter: parent.horizontalCenter
            width: 420
            height: 200
            color: "#f0f0f0"
            border.width: 1
            border.color: "#c0c0c0"

            CommonText{
                id: recordSaveTitle
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 20
                font.pixelSize: 22
                color: "#303030"
            }

            CommonRect{
                id: nameDataRect
                width: 380
                height: 45
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 60
                border.width: 1
                border.color: "#c0c0c0"
                z: 2

                CommonTextInput{
                    id: nameData
                    width: 365
                    height: 43
                    anchors.left: parent.left
                    anchors.leftMargin: 15
                    anchors.top: parent.top
                    anchors.topMargin: 1
                    font.family: rootItem.fontName
                    font.pixelSize: 24
                    color: "#303030"
                    horizontalAlignment: Text.AlignLeft
                    maximumLength: 30
                    validator: RegExpValidator
                    {
                        regExp: /^[^\/\,\.\?\:\*\<\>\|\\\"]*$/
                    }

                    onFocusSignal: {
                        keyBoard.initStatus();
                    }
                    onTextChanged: {
                        if (nameData.getTrimText() === "")
                        {
                            keyBoard.setOKBtnDisableStatus();
                            saveBtn.disableStatus();
                            editSaveBtn.disableStatus();
                        }
                        else
                        {
                            keyBoard.setOKBtnEnableStatus();
                            saveBtn.initStatus();
                            editSaveBtn.initStatus();
                        }
                    }
                }
            }

            Rectangle{
                width: parent.width
                height: 1
                anchors.bottom: deleteBtn.top
                color: "#c0c0c0"
                z: 2
            }

            Rectangle{
                width: 1
                height: deleteBtn.height
                anchors.bottom: deleteBtn.bottom
                anchors.left: deleteBtn.right
                color: "#c0c0c0"
                z: 2
            }

            TextButon{
                id: deleteBtn
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                width: 210
                height: 53
                info: langStrObject.GetLangString("ETCS_DELETE")
                textColor:"#57a0fe"
                z: 1
                bkgReleasedColor: "#f0f0f0"
                bkgPressedColor: "#dbe0e6"
                onClickedSignal: {
                    recordObject.removeAllCacheRecord();
                    nameData.setText("");
                    nameData.setFocus(false);
                    recordConfirmMask.setVisibleValue(false);
                    recordListMask.setVisibleValue(false);
                }
            }

            TextButon{
                id: cancelBtn
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                width: 210
                height: 53
                info: langStrObject.GetLangString("ETCS_CANCEL")
                textColor:"#57a0fe"
                z: 1
                bkgReleasedColor: "#f0f0f0"
                bkgPressedColor: "#dbe0e6"
                onClickedSignal: {
                    recordConfirmMask.setOldBaseName("");
                    nameData.setText("");
                    nameData.setFocus(false);
                    recordConfirmMask.setVisibleValue(false);
                }
            }

            TextButon{

                function initStatus()
                {
                    color = "#f0f0f0";
                    textColor = "#57a0fe";
                    enabled = true;
                }

                function disableStatus()
                {
                    color = "#f0f0f0";
                    textColor = "#40666666";
                    enabled = false;
                }

                id: saveBtn
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                width: 210
                height: 53
                info: langStrObject.GetLangString("ETCS_SAVE");
                textColor: "#57a0fe"
                z: 1
                bkgReleasedColor: "#f0f0f0"
                bkgPressedColor: "#dbe0e6"
                onClickedSignal: {
                    var ret;
                    var is_reserved = recordObject.isReservedName(nameData.getTrimText());
                    if (true === is_reserved)
                    {
                        confirmPrompt.messageShow(langStrObject.GetLangString("ETCS_NAME_RESERVED"));
                        return;
                    }

                    ret = recordObject.doubleCheck(nameData.getTrimText());
                    if (ret === true)
                    {
                        confirmPrompt.messageShow(langStrObject.GetLangString("ETCS_NAME_REPEAT"));
                        return;
                    }

                    recordObject.saveRecord(nameData.getTrimText());
                    recordObject.insertRecordToRecordList(nameData.getTrimText());
                    recordObject.insertRecordToIOmap(nameData.getTrimText());

                    if (visualModel.selectedItems.count > 0)
                    {
                        visualModel.selectedItems.remove(0, visualModel.selectedItems.count);
                    }

                    listModel.clearListModel();
                    listModel.createListModel();

                    //sourceManageObject.clearLocalSourceList();
                    //sourceManageObject.getLocalSourceList();
                    //sourceManageObject.setFileMapToLogic();

                    recordObject.removeAllCacheRecord();

                    nameData.setText("");
                    nameData.setFocus(false);
                    recordConfirmMask.setVisibleValue(false);
                    recordListMask.setVisibleValue(false);

                }
            }

            TextButon{

                function initStatus()
                {
                    color = "#f0f0f0";
                    textColor = "#57a0fe";
                    enabled = true;
                }

                function disableStatus()
                {
                    color = "#f0f0f0";
                    textColor = "#40666666";
                    enabled = false;
                }

                id: editSaveBtn
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                width: 210
                height: 53
                info: langStrObject.GetLangString("ETCS_SAVE");
                textColor: "#57a0fe"
                z: 1
                bkgReleasedColor: "#f0f0f0"
                bkgPressedColor: "#dbe0e6"
                onClickedSignal: {
                    var is_reserved = recordObject.isReservedName(nameData.getTrimText());
                    if (true === is_reserved)
                    {
                        confirmPrompt.messageShow(langStrObject.GetLangString("ETCS_NAME_RESERVED"));
                        return;
                    }

                    var ret;
                    ret = recordObject.doubleCheck(nameData.getTrimText());
                    if (ret === true)
                    {
                        confirmPrompt.messageShow(langStrObject.GetLangString("ETCS_NAME_REPEAT"));
                        return;
                    }

                    recordObject.updateRecord(recordConfirmMask.getOldBaseName(), nameData.getTrimText());
                    recordObject.updateRecordToRecordList(recordConfirmMask.getOldBaseName(), nameData.getTrimText());
                    recordObject.updateRecordToIOmap(recordConfirmMask.getOldBaseName(), nameData.getTrimText());

                    if (visualModel.selectedItems.count > 0)
                    {
                        visualModel.selectedItems.remove(0, visualModel.selectedItems.count);
                    }
                    listModel.clearListModel();
                    listModel.createListModel();


                    nameData.setText("");
                    nameData.setFocus(false);
                    recordConfirmMask.setOldBaseName("");
                    recordConfirmMask.setVisibleValue(false);

                }
            }

            CommonPrompt{
                id: confirmPrompt
                width: parent.width
                height: 40
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 60
                color: parent.color
                visible: false
            }
        }

        Keyboard{
            id: keyBoard
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            visible: true
            onOkSignal: {
                nameData.setFocus(false);
                quitStatus();
                keyBoard.setVisibleValue(false);
            }
        }
    }

    Component.onCompleted: {
        rootItem.updateLanguage.connect(updateLanguageSlot);
        keyBoard.addCharSignal.connect(nameData.addCharSlot);
        keyBoard.delCharSignal.connect(nameData.delCharSlot);
        keyBoard.clearSignal.connect(nameData.clearSlot);
        logicObject.localRecordStatusSignal.connect(localRecordStatusSlot);
        listModel.clearListModel();
        listModel.createListModel();
        prelistenBtn.disableStatus();
        recordBtn.initStatus();
        okBtn.disableStatus();
    }
}
