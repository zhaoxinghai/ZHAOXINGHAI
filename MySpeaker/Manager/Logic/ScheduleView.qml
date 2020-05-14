import QtQuick 2.0
import "../Components"

Rectangle {

    property int listViewHeight: height - mainPage.g_titleBarHeight
    property int scheduleSettingViewHeight: height + mainPage.businessBarHeight
    property int tmpCurrentIndex: 0
    property string scheduleMode: "Normal" // Normal Edit

    property int delScheduleModelIndex: -1
    property int delScheduleLogicIndex: -1

    function updateLanguageSlot()
    {
        title.text = langStrObject.GetLangString("ETCS_SCHEDULE");      
        editBtn.setTextInfo(langStrObject.GetLangString("ETCS_EDIT"));
        saveBtn.setTextInfo(langStrObject.GetLangString("ETCS_OK"));
        allOnBtn.setTextInfo(langStrObject.GetLangString("ETCS_ALL_ON"));
        allOffBtn.setTextInfo(langStrObject.GetLangString("ETCS_ALL_OFF"));

        updateScheduleAndKeepScrollBarSlot();
    }

    function changeUserSlot(value)
    {
        if (value === 0)
        {
            initSchedueSlot();
        }
        else if (value === 1)
        {
            initSchedueSlot();
        }
        else if (value === 2)
        {
            initSchedueSlot();
            modeBtnHideStatus();
        }
    }

    function initSchedueSlot()
    {
        setScheduleMode("Normal");
        listModel.clearListModel();
        listModel.createListModel(getScheduleMode());
        if (listModel.getCount() === 0)
        {
            modeBtnHideStatus();
            allBtnHideStatus();
        }
        else
        {
            normalModeStatus();
            checkAndSetAllBtnStatus();
        }
    }

    function updateScheduleAndKeepScrollBarSlot()
    {
        listModel.clearListModel();
        listModel.createListModel(getScheduleMode());

        editModeStatus();
        allBtnHideStatus();

        scheduleListView.positionViewAtIndex(tmpCurrentIndex, ListView.Center);
    }

    function addScheduleSlot(index)
    {
        var contentY;
        var oldContentHeight = scheduleListView.getContentHeight();
        var newContentHeight = oldContentHeight + 65;

        listModel.insertIndex(index);
        if (listModel.getCount() === 0)
        {
            modeBtnHideStatus();
            allBtnHideStatus();
        }
        else
        {
            if (getScheduleMode() === "Normal")
            {
                normalModeStatus();
                checkAndSetAllBtnStatus();
            }
            else if (getScheduleMode() === "Edit")
            {
                editModeStatus();
                allBtnHideStatus();
            }
        }

        if (newContentHeight > scheduleListView.getHeight())
        {
           contentY = newContentHeight - scheduleListView.getHeight();
           scheduleListView.setContentY(contentY);
        }
        else
        {
            ;
        }
    }

    function updateScheduleSlot(index, value)
    {
        listModel.updateListModelItem(index, value);
    }

    function initStatus()
    {
        setVisibleValue(true);
    }

    function checkAndSetAllBtnStatus()
    {
        if (scheduleManage.isAllScheduleError() === true)
        {
            allBtnHideStatus();
            return;
        }

        if (scheduleManage.isAllScheduleOff() === true)
        {
            allOnShowStatus();
        }
        else
        {
            allOffShowStatus();
        }
    }

    function allBtnHideStatus()
    {
        allOnBtn.setVisibleValue(false);
        allOffBtn.setVisibleValue(false);
    }

    function allOnShowStatus()
    {
        allOnBtn.setVisibleValue(true);
        allOffBtn.setVisibleValue(false);
    }

    function allOffShowStatus()
    {
        allOnBtn.setVisibleValue(false);
        allOffBtn.setVisibleValue(true);
    }

    function modeBtnHideStatus()
    {
        editBtn.setVisibleValue(false);
        saveBtn.setVisibleValue(false);
    }

    function editModeStatus()
    {
        editBtn.setVisibleValue(false);
        saveBtn.setVisibleValue(true);
    }

    function normalModeStatus()
    {
        editBtn.setVisibleValue(true);
        saveBtn.setVisibleValue(false);
    }

    function setScheduleMode(value)
    {
        scheduleMode = value;
    }

    function getScheduleMode()
    {
        return scheduleMode;
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function setDelScheduleModelIndex(value)
    {
        delScheduleModelIndex = value;
    }

    function getDelScheduleModelIndex()
    {
        return delScheduleModelIndex
    }

    function setDelScheduleLogicIndex(value)
    {
        delScheduleLogicIndex = value;
    }

    function getDelScheduleLogicIndex()
    {
        return delScheduleLogicIndex
    }

    function confirmMaskOKSlot()
    {
        if ((getDelScheduleLogicIndex() === -1) || (getDelScheduleModelIndex() === -1))
        {
            return ;
        }

        scheduleManage.delSchedule(getDelScheduleLogicIndex());
        listModel.delIndex(getDelScheduleModelIndex());
        if (listModel.getCount() === 0)
        {
            setScheduleMode("Normal");
            if (listModel.getCount() === 0)
            {
                modeBtnHideStatus();
                allBtnHideStatus();
            }
            else
            {
                normalModeStatus();
                checkAndSetAllBtnStatus();
            }
        }
        setDelScheduleLogicIndex(-1);
        setDelScheduleModelIndex(-1);
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
            text: langStrObject.GetLangString("ETCS_SCHEDULE")
            font.family: rootItem.fontName
            font.pixelSize: mainPage.g_titleTextFontSize
            width: 240
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            Component.onCompleted: {
            }
        }

        TextButton2{
            id: allOnBtn
            width: (240 - title.contentWidth / 2) / 2
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: editBtn.left
            nameWidth: width
            fontSize: 18
            info: langStrObject.GetLangString("ETCS_ALL_ON")
            textColor: "#ffffff"
            visible: false
            bkgPressedColor: "#505050"
            onClickedSignal: {
                scheduleManage.AllScheduleOn(true);
                listModel.updateListModel(true);
                allOffShowStatus();
            }
        }

        TextButton2{
            id: allOffBtn
            width: (240 - title.contentWidth / 2) / 2
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: editBtn.left
            nameWidth: width
            fontSize: 18
            info: langStrObject.GetLangString("ETCS_ALL_OFF")
            textColor: "#ffffff"
            visible: false
            bkgPressedColor: "#505050"
            onClickedSignal: {
                scheduleManage.AllScheduleOn(false);
                listModel.updateListModel(false);
                allOnShowStatus();
            }
        }

        TextButton2{
            id: editBtn
            width: (240 - title.contentWidth / 2) / 2
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            nameWidth: width
            fontSize: 18
            info: langStrObject.GetLangString("ETCS_EDIT")
            textColor: "#ffffff"
            visible: false
            bkgPressedColor: "#505050"
            onClickedSignal: {
                setScheduleMode("Edit");
                editModeStatus();
                allBtnHideStatus();
            }
        }

        TextButton2{
            id: saveBtn
            width: (240 - title.contentWidth / 2) / 2
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            nameWidth: width
            fontSize: 18
            info: langStrObject.GetLangString("ETCS_OK")
            textColor: "#ffffff"
            visible: false
            bkgPressedColor: "#505050"
            onClickedSignal: {
                setScheduleMode("Normal");
                normalModeStatus();
                checkAndSetAllBtnStatus();
            }
        }
    }

    ScrollBar{
        id: scrollBar
        anchors.top: scheduleListView.top
        anchors.right: parent.right
        anchors.rightMargin: 6
        height: scheduleListView.height
        position: scheduleListView.visibleArea.yPosition
        pageSize: scheduleListView.visibleArea.heightRatio
        scrollBarVisible: scheduleListView.visibleArea.heightRatio == 1 ? false : true
        z: 1
    }

    ListModel {

        function createListModel(scheduleModel)
        {
            var count = scheduleManage.getScheduleListCount();
            var index;
            var myConfig;
            var ret;
            var switchStatus;
            for (var i = 0; i < count; i++)
            {
                index = scheduleManage.getScheduleIndex(i);
                ret = scheduleManage.isIndexValid(index);
                if (ret === true)
                {
                    switchStatus = scheduleManage.getScheduleBoolValue(index, "switchStatus");
                    listModel.append({"nIndex": index, "switchStatus": switchStatus});
                }
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
            insert(listModel.count, {"nIndex": index, "switchStatus": true});
        }

        function delIndex(index)
        {
            if (listModel.count > 0)
            {
                remove(index, 1);
            }
        }

        function updateListModelItem(index, value)
        {
            var count = scheduleManage.getScheduleListCount();
            var tmpIndex;
            for (var i = 0; i < count; i++)
            {
                tmpIndex = scheduleManage.getScheduleIndex(i);
                if (tmpIndex === index)
                {
                    listModel.get(i).switchStatus = value;
                    break;
                }
            }
        }

        function updateListModel(value)
        {
            var count = scheduleManage.getScheduleListCount();
            for (var i = 0; i < count; i++)
            {
                listModel.get(i).switchStatus = value;
            }
        }

        function getCount()
        {
            return listModel.count;
        }

        id: listModel
    }

    ListView {

        function setContentY(value)
        {
            contentY = value;
        }

        function getContentY()
        {
            return contentY;
        }

        function getHeight()
        {
            return height;
        }

        function getContentHeight()
        {
            return contentHeight;
        }

        id: scheduleListView
        anchors.top: titleBar.bottom
        width: parent.width
        height: listViewHeight
        visible: true
        boundsBehavior: Flickable.StopAtBounds
        snapMode: ListView.NoSnap
        footer: Rectangle{height: 8}
        clip: true
        model: listModel

        delegate: StripFrame2{
                    function getTopInfo()
                    {
                        return scheduleManage.getStrTime(scheduleManage.getScheduleValue(nIndex, "startHour"),
                                                         scheduleManage.getScheduleValue(nIndex, "startMin"));

                    }

                    function getName()
                    {
                        return scheduleManage.getScheduleStrValue(nIndex, "name");
                    }

                    function getReat()
                    {
                        var repeat = "";
                        var mon;
                        var tue;
                        var wed;
                        var thur;
                        var fri;
                        var sat;
                        var sun;

                        mon = scheduleManage.getDay(nIndex, 0);
                        tue = scheduleManage.getDay(nIndex, 1);
                        wed = scheduleManage.getDay(nIndex, 2);
                        thur = scheduleManage.getDay(nIndex, 3);
                        fri = scheduleManage.getDay(nIndex, 4);
                        sat = scheduleManage.getDay(nIndex, 5);
                        sun = scheduleManage.getDay(nIndex, 6);

                        if (mon === false && tue === false && wed === false &&
                                thur === false && fri === false && sat === false && sun === false)
                        {
                            repeat = langStrObject.GetLangString("ETCS_ONLY_ONCE");
                        }
                        else if (mon === true && tue === true && wed === true &&
                                thur === true && fri === true && sat === true && sun === true)
                        {
                            repeat = langStrObject.GetLangString("ETCS_ERVERYDAY");
                        }
                        else if (mon === true && tue === true && wed ===true && thur === true && fri === true
                                 && sat === false && sun === false)
                        {
                            repeat = langStrObject.GetLangString("ETCS_MON_TO_FRI");
                        }
                        else if ((mon && tue && wed && thur && fri) === false ||
                                 ((mon && tue && wed && thur && fri) === true && (sat !== sun)))
                        {
                            if (mon === true)
                            {
                                repeat += langStrObject.GetLangString("ETCS_MONDAY") + " ";
                            }
                            if (tue === true)
                            {
                                repeat += langStrObject.GetLangString("ETCS_TUESDAY") + " ";
                            }
                            if (wed === true)
                            {
                                repeat += langStrObject.GetLangString("ETCS_WEDNESDAY") + " ";
                            }
                            if (thur === true)
                            {
                                repeat += langStrObject.GetLangString("ETCS_THURSDAY") + " ";
                            }
                            if (fri === true)
                            {
                                repeat += langStrObject.GetLangString("ETCS_FRIDAY") + " ";
                            }
                            if (sat === true)
                            {
                                repeat += langStrObject.GetLangString("ETCS_SATURDAY") + " ";
                            }
                            if (sun === true)
                            {
                                repeat += langStrObject.GetLangString("ETCS_SUNDAY");
                            }
                        }
                        else
                        {
                            return "";
                        }

                        return repeat;
                    }

                    id: item
                    topTextLeftMargin: 40
                    bottomTextLeftMargin : 40
                    topTextWidth: 300
                    bottomTextWidth: 300
                    topInfo: getTopInfo()
                    bottomInfo: getName() + ", " + getReat()
                    line1Visible: false
                    line2Visible: true
                    visible: true
                    clip: true

                    CommonImage{
                        id: localErrorImage
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        source: logicObject.getLocalImagePath() + "error.png"
                        visible: scheduleManage.getScheduleBoolValue(nIndex, "isError")
                    }

                    Rectangle{
                        id: imageSwitch
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: 30
                        width: 60
                        height: 63
                        visible: ((scheduleManage.getScheduleBoolValue(nIndex, "isError") === false) &&
                                 (getScheduleMode() === "Normal")) ? true : false
                        z: 1
                        Image{

                            function setSource(value)
                            {
                                source = value;
                            }

                            id: switchImage
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            source: switchStatus ? logicObject.getLocalImagePath() + "switch_on.png" : logicObject.getLocalImagePath() + "switch_off.png"
                        }

                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                if (switchStatus === false)
                                {
                                    scheduleManage.openSchedule(nIndex, scheduleManage.getScheduleBoolValue(nIndex, "MyConfig"));
                                    checkAndSetAllBtnStatus();
                                    switchStatus = true;
                                }
                                else
                                {
                                    scheduleManage.closeSchedule(nIndex, scheduleManage.getScheduleBoolValue(nIndex, "MyConfig"));
                                    checkAndSetAllBtnStatus();
                                    switchStatus = false;
                                }
                            }
                        }
                    }

                    ImageButton{
                        id: editImgBtn
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: delImgBtn.left
                        width: 50
                        height: parent.height
                        buttonImageSource: logicObject.getLocalImagePath() + "edit.png"
                        visible:((scheduleManage.getScheduleBoolValue(nIndex, "isError") === false) &&
                                (getScheduleMode() === "Edit") &&
                                 scheduleManage.getScheduleBoolValue(nIndex, "MyConfig")) ? true : false
                        z: 1
                        onClickedSignal: {

                            var hour;
                            var min;
                            var name;
                            var cycle;
                            var gapTime;

                            var mon;
                            var tue;
                            var wed;
                            var thur;
                            var fri;
                            var sat;
                            var sun;

                            hour = scheduleManage.getScheduleValue(nIndex, "startHour");
                            min = scheduleManage.getScheduleValue(nIndex, "startMin");
                            name = scheduleManage.getScheduleStrValue(nIndex, "name");
                            cycle = scheduleManage.getScheduleValue(nIndex, "cycle");
                            gapTime = scheduleManage.getScheduleValue(nIndex, "gapTime");

                            mon = scheduleManage.getDay(nIndex, 0);
                            tue = scheduleManage.getDay(nIndex, 1);
                            wed = scheduleManage.getDay(nIndex, 2);
                            thur = scheduleManage.getDay(nIndex, 3);
                            fri = scheduleManage.getDay(nIndex, 4);
                            sat = scheduleManage.getDay(nIndex, 5);
                            sun = scheduleManage.getDay(nIndex, 6);

                            scheduleSettingView.setDay(mon, tue, wed, thur, fri, sat, sun);

                            var ret = scheduleManage.scheduleIsLive(nIndex);
                            if (ret === true)
                            {
                                scheduleSettingView.updateLiveInitStatus(nIndex,
                                                                     hour,
                                                                     min,
                                                                     name,
                                                                     cycle,
                                                                     gapTime,
                                                                     title.getText());
                            }
                            else
                            {
                                scheduleSettingView.updateInitStatus(nIndex,
                                                                     hour,
                                                                     min,
                                                                     name,
                                                                     cycle,
                                                                     gapTime,
                                                                     title.getText());
                            }

                            tmpCurrentIndex = index;
                            mainPage.hideBusinessBar();
                        }
                    }

                    ImageButton{
                        id: delImgBtn
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: 30
                        width: 50
                        height: parent.height
                        visible: (getScheduleMode() === "Edit") && scheduleManage.getScheduleBoolValue(nIndex, "MyConfig") ? true : false
                        buttonImageSource: logicObject.getLocalImagePath() + "recycle.png"
                        z: 1
                        onClickedSignal: {
                            setDelScheduleLogicIndex(nIndex);
                            setDelScheduleModelIndex(index);
                            confirmMask.setVisibleValue(true);
                        }
                    }

                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            var isError =  scheduleManage.getScheduleBoolValue(nIndex, "isError");
                            var errorNum;
                            if (isError === true)
                            {
                                errorNum = scheduleManage.getScheduleIntValue(nIndex, "errorNum");
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
                        }
                    }
                }
    }

    ScheduleSettingView{
        id: scheduleSettingView
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width
        height: scheduleSettingViewHeight
        visible: false
        z: 1
        onCancelSignal: {
            scheduleSettingView.setVisibleValue(false);
            mainPage.showBusinessBar();
        }
        onSaveSignal: {
            scheduleSettingView.updateSchedule();
            scheduleManage.updateSchedule(scheduleSettingView.getIndex(), scheduleManage.getScheduleBoolValue(scheduleSettingView.getIndex(), "MyConfig"));
            updateScheduleAndKeepScrollBarSlot();
            scheduleSettingView.setVisibleValue(false);
            mainPage.showBusinessBar();
        }
    }

    Component.onCompleted: {
        initSchedueSlot();
        logicObject.addScheduleSignal.connect(addScheduleSlot);
        logicObject.updateScheduleSignal.connect(updateScheduleSlot);
        rootItem.updateLanguage.connect(updateLanguageSlot);
        rootItem.changeUserSignal.connect(changeUserSlot);
        confirmMask.okSignal.connect(confirmMaskOKSlot);
    }
}
