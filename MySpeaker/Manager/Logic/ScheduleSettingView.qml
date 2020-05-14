import QtQuick 2.0
import "../Components"
import "../Keyboard"

Rectangle {

    property int m_index;
    property int pathViewHeight: 260
    property int pathViewX: 120

    signal cancelSignal();
    signal saveSignal();

    function updateLanguageSlot()
    {
        saveBtn.setTextInfo(langStrObject.GetLangString("ETCS_SAVE"));
        hourText.setText(langStrObject.GetLangString("ETCS_HOUR"));
        minText.setText(langStrObject.GetLangString("ETCS_MIN"));
        repeatBtnName.setText(langStrObject.GetLangString("ETCS_REPEAT"));
        cycleBtnName.setText(langStrObject.GetLangString("ETCS_CYCLE"));
        cycleBtn.setCycleBtnData(cycle.getCurrentIndex());
        gapTimeName.setText(langStrObject.GetLangString("ETCS_GAPTIME"));
    }

    function addInitStatus(cy, gap, text)
    {
        setVisibleValue(true);
        hourView.setCurrentIndex(0);
        minView.setCurrentIndex(0);
        nameData.setText(langStrObject.GetLangString("ETCS_NAME"));
        cycleBtn.setCycleBtnData(cy);
        cycleBtn.setVisibleValue(true);
        cycle.setCurrentIndex(cy);
        if (Number(cycleBtnData.text) === 1)
        {
            gapTimeBtn.disableStatus();
        }
        else
        {
            gapTimeBtn.enableStatus();
        }
        gapTime.setCurrentMinIndex(gap/60);
        gapTime.setCurrentSecondIndex(gap%60);
        gapTimeBtn.setGapTimeData(scheduleManage.getStrTime(gapTime.getCurrentMinIndex(), gapTime.getCurrentSecondIndex()));
        gapTimeBtn.setVisibleValue(true);
        setDay(false, false, false, false, false, false, false);
        repeatBtn.setRepeatData(getRepeatInfo(repeat.mon,
                                                repeat.tue,
                                                repeat.wed,
                                                repeat.thur,
                                                repeat.fri,
                                                repeat.sat,
                                                repeat.sun));
        title.setText(langStrObject.GetLangString("ETCS_ADD_SCHEDULE"));
        prevBtn.setDataText(text);
    }

    function addLiveInitStatus(text)
    {
        setVisibleValue(true);
        hourView.setCurrentIndex(0);
        minView.setCurrentIndex(0);
        nameData.setText(langStrObject.GetLangString("ETCS_NAME"));
        cycleBtn.setCycleBtnData(0);
        cycleBtn.setVisibleValue(false);
        cycle.setCurrentIndex(0);
        if (Number(cycleBtnData.text) === 1)
        {
            gapTimeBtn.disableStatus();
        }
        else
        {
            gapTimeBtn.enableStatus();
        }
        gapTime.setCurrentMinIndex(0/60);
        gapTime.setCurrentSecondIndex(0%60);
        gapTimeBtn.setGapTimeData(scheduleManage.getStrTime(gapTime.getCurrentMinIndex(), gapTime.getCurrentSecondIndex()));
        gapTimeBtn.setVisibleValue(false);
        setDay(false, false, false, false, false, false, false);
        repeatBtn.setRepeatData(getRepeatInfo(repeat.mon,
                                                repeat.tue,
                                                repeat.wed,
                                                repeat.thur,
                                                repeat.fri,
                                                repeat.sat,
                                                repeat.sun));
        title.setText(langStrObject.GetLangString("ETCS_ADD_SCHEDULE"));
        prevBtn.setDataText(text);
    }

    function updateInitStatus(index, hour, min, name, cy, gap, text)
    {
        setVisibleValue(true);
        setIndex(index);
        hourView.setCurrentIndex(hour);
        minView.setCurrentIndex(min);
        nameData.setText(name);
        cycleBtn.setCycleBtnData(cy);
        cycleBtn.setVisibleValue(true);
        cycle.setCurrentIndex(cy);
        if (Number(cycleBtnData.text) === 1)
        {
            gapTimeBtn.disableStatus();
        }
        else
        {
            gapTimeBtn.enableStatus();
        }
        gapTime.setCurrentMinIndex(gap/60);
        gapTime.setCurrentSecondIndex(gap%60);
        gapTimeBtn.setGapTimeData(scheduleManage.getStrTime(gapTime.getCurrentMinIndex(), gapTime.getCurrentSecondIndex()));
        gapTimeBtn.setVisibleValue(true);
        repeatBtn.setRepeatData(getRepeatInfo(repeat.mon,
                                            repeat.tue,
                                            repeat.wed,
                                            repeat.thur,
                                            repeat.fri,
                                            repeat.sat,
                                            repeat.sun));
        title.setText(langStrObject.GetLangString("ETCS_EDIT_SCHEDULE"));
        prevBtn.setDataText(text);
    }

    function updateLiveInitStatus(index, hour, min, name, cy, gap, text)
    {
        setVisibleValue(true);
        setIndex(index);
        hourView.setCurrentIndex(hour);
        minView.setCurrentIndex(min);
        nameData.setText(name);
        cycleBtn.setCycleBtnData(0);
        cycleBtn.setVisibleValue(false);
        cycle.setCurrentIndex(0);
        if (Number(cycleBtnData.text) === 1)
        {
            gapTimeBtn.disableStatus();
        }
        else
        {
            gapTimeBtn.enableStatus();
        }
        gapTime.setCurrentMinIndex(0 / 60);
        gapTime.setCurrentSecondIndex(0 % 60);
        gapTimeBtn.setGapTimeData(scheduleManage.getStrTime(gapTime.getCurrentMinIndex(), gapTime.getCurrentSecondIndex()));
        gapTimeBtn.setVisibleValue(false);
        repeatBtn.setRepeatData(getRepeatInfo(repeat.mon,
                                            repeat.tue,
                                            repeat.wed,
                                            repeat.thur,
                                            repeat.fri,
                                            repeat.sat,
                                            repeat.sun));
        title.setText(langStrObject.GetLangString("ETCS_EDIT_SCHEDULE"));
        prevBtn.setDataText(text);
    }

    function quitStatus()
    {
        hourView.setCurrentIndex(0);
        minView.setCurrentIndex(0);
        cycleBtn.setCycleBtnData(0);
        cycle.setCurrentIndex(0);
        gapTime.setCurrentMinIndex(0);
        gapTime.setCurrentSecondIndex(0);
    }

    function setDay(mon, tue, wed, thur, fri, sat, sun)
    {
        repeat.mon = mon;
        repeat.tue = tue;
        repeat.wed = wed;
        repeat.thur = thur;
        repeat.fri = fri;
        repeat.sat = sat;
        repeat.sun = sun;
    }

    function addSchedule()
    {
        pageObject.setMday(0, repeat.mon);
        pageObject.setMday(1, repeat.tue);
        pageObject.setMday(2, repeat.wed);
        pageObject.setMday(3, repeat.thur);
        pageObject.setMday(4, repeat.fri);
        pageObject.setMday(5, repeat.sat);
        pageObject.setMday(6, repeat.sun);
        pageObject.setScheduleParam(hourView.getCurrentIndex(),
                                    minView.getCurrentIndex(),
                                    nameData.getTrimText(),
                                    getOnlyOnce(repeat.mon,
                                                repeat.tue,
                                                repeat.wed,
                                                repeat.thur,
                                                repeat.fri,
                                                repeat.sat,
                                                repeat.sun),
                                    cycle.getCurrentIndex(),
                                    gapTime.getGapTimeFormMinAndSec());

    }

    function updateSchedule()
    {
        scheduleManage.setMday(0, repeat.mon);
        scheduleManage.setMday(1, repeat.tue);
        scheduleManage.setMday(2, repeat.wed);
        scheduleManage.setMday(3, repeat.thur);
        scheduleManage.setMday(4, repeat.fri);
        scheduleManage.setMday(5, repeat.sat);
        scheduleManage.setMday(6, repeat.sun);
        scheduleManage.setScheduleParam(hourView.getCurrentIndex(),
                                        minView.getCurrentIndex(),
                                        nameData.getTrimText(),
                                        getOnlyOnce(repeat.mon,
                                                    repeat.tue,
                                                    repeat.wed,
                                                    repeat.thur,
                                                    repeat.fri,
                                                    repeat.sat,
                                                    repeat.sun),
                                        cycle.getCurrentIndex(),
                                        gapTime.getGapTimeFormMinAndSec());

    }

    function setIndex(nIndex)
    {
        m_index = nIndex;
    }

    function getIndex()
    {
        return m_index;
    }

    function setEditType(value)
    {
        editType = value
    }

    function getEditType()
    {
        return editType;
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function getOnlyOnce(mon, tue, wed, thur, fri, sat, sun)
    {
        if (mon === false && tue === false && wed === false &&
                thur === false && fri === false && sat === false && sun === false)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    function getRepeatInfo(mon, tue, wed, thur, fri, sat, sun)
    {
        var repeat = "";
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
        else if (mon === true && tue === true && wed === true && thur === true && fri === true
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

    color: "#e7e7e7"

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

        CommonText{
            id: title
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: mainPage.g_titleTextColor
            font.family: rootItem.fontName
            font.pixelSize: mainPage.g_titleTextFontSize
            width: 200
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            text: langStrObject.GetLangString("ETCS_ADD_SCHEDULE")
            z: 1
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
            fontSize: mainPage.g_titleTextFontSize2
            visible: true
            bkgPressedColor: "#505050"
            dataWidth: 240 - (title.contentWidth/2) - imageWidth - 14 + 8
            onClickedSignal: {
                cancelSignal();
            }
        }

        TextButton2{
            id: saveBtn
            width: nameContentWidth < parent.height ? parent.height : nameContentWidth
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 5
            nameWidth: 480 / 2 - title.contentWidth / 2 - 5
            fontSize: 18
            info: langStrObject.GetLangString("ETCS_SAVE")
            textColor: "#ffffff"
            visible: true
            bkgPressedColor: "#505050"
            onClickedSignal: {
                saveSignal();
            }
        }
    }

    HorizontalLine{
        anchors.top: titleBar.bottom
        anchors.topMargin: 2 * (hourView.height / hourView.pathItemCount)
        anchors.left: parent.left
        color: "#d7d7d7"
        z: 1
    }

    HorizontalLine{
        anchors.top: titleBar.bottom
        anchors.topMargin: 3 * (hourView.height / hourView.pathItemCount)
        anchors.left: parent.left
        color: "#d7d7d7"
        z: 1
    }

    PathView{

        function setCurrentIndex(index)
        {
            currentIndex = index;
        }

        function getCurrentIndex()
        {
            return currentIndex;
        }

        id: hourView
        width: 240
        height: pathViewHeight
        anchors.top: titleBar.bottom
        anchors.left: parent.left
        pathItemCount: 5
        preferredHighlightBegin: 0.5
        preferredHighlightEnd: 0.5
        highlightRangeMode: PathView.StrictlyEnforceRange
        highlightMoveDuration: 0
        maximumFlickVelocity: 1200
        flickDeceleration: 800
        clip: true
        model: 24
        delegate: RollerComponent{
            width: 240
            height: hourView.height / hourView.pathItemCount
            color: "#e7e7e7"
            visible: PathView.onPath
            info: index
            textScale: PathView.itemScale
            textOpacity: PathView.itemOpacity
            dataColor: PathView.itemScale === 1.0 ? "#1792e5" : "#303030"
        }

        path: gPath
        onMovementEnded: {

        }
    }

    CommonText{
        id: hourText
        z: 1
        anchors.verticalCenter: hourView.verticalCenter
        anchors.horizontalCenter: hourView.horizontalCenter
        anchors.horizontalCenterOffset: 50
        font.family: rootItem.fontName
        font.bold: false
        font.pixelSize: 18
        color: "#1792e5"
        text: langStrObject.GetLangString("ETCS_HOUR")
        width: 80
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
    }


    PathView{

        function setCurrentIndex(index)
        {
            currentIndex = index;
        }

        function getCurrentIndex()
        {
            return currentIndex;
        }

        id: minView
        width: 240
        height: pathViewHeight
        anchors.top: titleBar.bottom
        anchors.left: hourView.right
        pathItemCount: 5
        preferredHighlightBegin: 0.5
        preferredHighlightEnd: 0.5
        highlightRangeMode: PathView.StrictlyEnforceRange
        maximumFlickVelocity: 1200
        highlightMoveDuration: 0
        flickDeceleration: 800
        clip: true
        model: 60
        delegate: RollerComponent{
            width: 240
            height: minView.height / minView.pathItemCount
            color: "#e7e7e7"
            visible: PathView.onPath
            info: index
            textScale: PathView.itemScale
            textOpacity: PathView.itemOpacity
            dataColor: PathView.itemScale === 1.0 ? "#1792e5" : "#303030"
        }
        path: gPath
        onMovementEnded: {

        }
    }

    CommonText{
        id: minText
        anchors.verticalCenter: minView.verticalCenter
        anchors.horizontalCenter: minView.horizontalCenter
        anchors.horizontalCenterOffset: 50
        font.family: rootItem.fontName
        font.bold: false
        font.pixelSize: 18
        color: "#1792e5"
        text: langStrObject.GetLangString("ETCS_MIN")
        width: 80
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
        z: 1
    }

    Path{
        id: gPath
        startX: pathViewX
        startY: 26
        PathAttribute{name: "itemScale"; value: 0.4}
        PathAttribute{name: "itemOpacity"; value: 0.00}

        PathLine{x: pathViewX; y: 78}
        PathAttribute{name: "itemScale"; value: 0.6}
        PathAttribute{name: "itemOpacity"; value: 0.4}

        PathLine{x: pathViewX; y: 130}
        PathAttribute{name: "itemScale"; value: 1.0}
        PathAttribute{name: "itemOpacity"; value: 1.0}  

        PathLine{x: pathViewX; y: 182}
        PathAttribute{name: "itemScale"; value: 0.6}
        PathAttribute{name: "itemOpacity"; value: 0.4}

        PathLine{x: pathViewX; y: 234}
        PathAttribute{name: "itemScale"; value: 0.4}
        PathAttribute{name: "itemOpacity"; value: 0.00}
    }

    Rectangle{

        id: nameTitle
        width: parent.width
        height: 70
        anchors.top: minView.bottom
        anchors.left: parent.left
        color: "#ffffff"
        z: 3
        HorizontalLine{
            anchors.top: parent.top
            anchors.left: parent.left
        }

        HorizontalLine{
            anchors.bottom: parent.bottom
            anchors.left: parent.left
        }

        CommonTextInput{

            id: nameData
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            text: scheduleManage.getScheduleStrValue(getIndex(), "name");
            color: "#303030"
            font.pixelSize: 20
            font.family: rootItem.fontName
            width: parent.width - 40
            height: parent.height
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            maximumLength: 30

            onFocusSignal: {
                keyBoard.initStatus();
                keyBoardMask.setVisibleValue(true);
            }

            onTextChanged: {
                if (nameData.getTrimText() === "")
                {
                    keyBoard.setOKBtnDisableStatus();
                }
                else
                {
                    keyBoard.setOKBtnEnableStatus();
                }
            }
        }
    }

    HorizontalLine{
        anchors.top: repeatBtn.top
    }

    RectButton{

        function setRepeatData(text)
        {
            repeatBtnData.text = text;
        }

        id: repeatBtn
        anchors.top: nameTitle.bottom
        anchors.topMargin: 16
        anchors.left: parent.left
        bkgPressedColor: "#DBE0E6"
        bkgReleasedColor: "#ffffff"
        CommonText{
            id: repeatBtnName
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 18
            width: parent.width - 18 - repeatBtnData.contentWidth - 18 - repeatBtnImage.width
            text: langStrObject.GetLangString("ETCS_REPEAT");
            color: "#303030"
            Component.onCompleted: {
            }
        }
        CommonText{
            id: repeatBtnData
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: repeatBtnImage.left
            anchors.leftMargin: 18
            width: 240
            horizontalAlignment: Text.AlignRight
        }
        CommonImage {
            id: repeatBtnImage
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 18
            source: logicObject.getLocalImagePath() + "next_1.png"
        }
        onClickedSignal: {
            repeat.initStatus(title.getText());
        }
    }

    Repeat{
        id: repeat
        anchors.top: parent.top
        width: parent.width
        height: parent.height
        visible: false
        z: 3

        onCancelSignal: {
            setVisibleValue(false);
            repeatBtn.setRepeatData(getRepeatInfo(repeat.mon,
                                                    repeat.tue,
                                                    repeat.wed,
                                                    repeat.thur,
                                                    repeat.fri,
                                                    repeat.sat,
                                                    repeat.sun));
        }
    }

    RectButton{

        function setCycleBtnData(text)
        {
            if (text === 0)
            {
                cycleBtnData.text = langStrObject.GetLangString("ETCS_FOREVER");
            }
            else
            {
                cycleBtnData.text = text;
            }
        }

        id: cycleBtn
        anchors.top: repeatBtn.bottom
        anchors.left: parent.left
        bkgPressedColor: "#DBE0E6"
        bkgReleasedColor: "#ffffff"
        CommonText{
            id: cycleBtnName
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 18
            width: parent.width - 18 - cycleBtnData.contentWidth - 18 - cycleBtnImage.width - 18
            text: langStrObject.GetLangString("ETCS_CYCLE");
            color: "#303030"
            Component.onCompleted: {
            }
        }
        CommonText{
            id: cycleBtnData
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: cycleBtnImage.left
            anchors.leftMargin: 18
            width: 240
            horizontalAlignment: Text.AlignRight
        }
        CommonImage {
            id: cycleBtnImage
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 18
            source: logicObject.getLocalImagePath() + "next_1.png"
        }

        onClickedSignal: {
            var index;
            mask.setVisibleValue(true);
            if (cycleBtnData.text === langStrObject.GetLangString("ETCS_FOREVER"))
            {
                cycle.initStatus(0);
            }
            else
            {
                cycle.initStatus(Number(cycleBtnData.text));
            }

            if (Number(cycleBtnData.text) === 1)
            {
                gapTimeBtn.disableStatus();
            }
            else
            {
                gapTimeBtn.enableStatus();
            }
        }
    }

    RectButton{

        function enableStatus()
        {
            gapTimeBtn.setBtnEnable(true);
            gapTimeBtnImage.setVisibleValue(true);
        }

        function disableStatus()
        {
            gapTimeBtn.setBtnEnable(false);
            gapTimeBtnImage.setVisibleValue(false);
        }

        function setGapTimeData(text)
        {
            gapTimeData.text = text;
        }

        id: gapTimeBtn
        anchors.top: cycleBtn.bottom
        anchors.left: parent.left
        bkgPressedColor: "#DBE0E6"
        bkgReleasedColor: "#ffffff"
        CommonText{
            id: gapTimeName
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 18
            text: langStrObject.GetLangString("ETCS_GAPTIME");
            width: parent.width - 18 - gapTimeData.contentWidth - 18 - gapTimeBtnImage.width - 18
            color: "#303030"
            Component.onCompleted: {

            }
        }
        CommonText{
            id: gapTimeData
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: gapTimeBtnImage.left
            anchors.leftMargin: 18
            width: 80
            horizontalAlignment: Text.AlignRight
        }
        CommonImage {
            id: gapTimeBtnImage
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 18
            source: logicObject.getLocalImagePath() + "next_1.png"
        }

        onClickedSignal: {
            mask.setVisibleValue(true);
            gapTime.initStatus(gapTime.getCurrentMinIndex(), gapTime.getCurrentSecondIndex());
        }
    }

    Mask{
       id: mask
       anchors.top: parent.top
       width: parent.width
       height: parent.height - cycle.height
       visible: false
       z: 3
       passThrough: true
    }

   Cycle{
       id: cycle
       anchors.top: mask.bottom
       anchors.left: parent.left
       visible: false
       z: 3
       onCancelSignal: {
           cycle.setVisibleValue(false);
           mask.setVisibleValue(false);
       }
       onOkSignal: {
           cycle.setVisibleValue(false);
           mask.setVisibleValue(false);
           cycleBtn.setCycleBtnData(getCurrentIndex());
           if (Number(cycleBtnData.text) === 1)
           {
               gapTimeBtn.disableStatus();
           }
           else
           {
               gapTimeBtn.enableStatus();
           }
       }
   }

   GapTime{
       id: gapTime
       anchors.top: mask.bottom
       anchors.left: parent.left
       visible: false
       z: 3
       onCancelSignal: {
           gapTime.setVisibleValue(false);
           mask.setVisibleValue(false);
       }
       onOkSignal: {
           gapTime.setVisibleValue(false);
           mask.setVisibleValue(false);
           gapTimeBtn.setGapTimeData(scheduleManage.getStrTime(gapTime.getCurrentMinIndex(), gapTime.getCurrentSecondIndex()));
       }
   }

   Mask{
       id: keyBoardMask
       anchors.top: parent.top
       anchors.left: parent.left
       width: parent.width
       height: parent.height
       color: "#70666666"
       z: 2
       visible: false
       passThrough: true

       Keyboard{
           id: keyBoard
           anchors.bottom: keyBoardMask.bottom
           anchors.left: parent.left
           visible: true
           onOkSignal: {
               nameData.setFocus(false);
               quitStatus();
               parent.setVisibleValue(false);
           }
       }
   }

    Component.onCompleted: {
        rootItem.updateLanguage.connect(updateLanguageSlot);
        keyBoard.addCharSignal.connect(nameData.addCharSlot);
        keyBoard.delCharSignal.connect(nameData.delCharSlot);
        keyBoard.clearSignal.connect(nameData.clearSlot);
    }
}

