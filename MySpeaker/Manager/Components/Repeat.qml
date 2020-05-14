import QtQuick 2.0

Rectangle {

    property bool mon: false
    property bool tue: false
    property bool wed: false
    property bool thur: false
    property bool fri: false
    property bool sat: false
    property bool sun: false

    signal cancelSignal();

    function updateLanguageSlot()
    {
        title.setText(langStrObject.GetLangString("ETCS_REPEAT"));
        allBtn.setTextInfo(langStrObject.GetLangString("ETCS_SELECT_ALL"));
        clearBtn.setTextInfo(langStrObject.GetLangString("ETCS_CLEAR_ALL"));
        monName.setText(langStrObject.GetLangString("ETCS_MONDAY"));
        tueName.setText(langStrObject.GetLangString("ETCS_TUESDAY"));
        wedName.setText(langStrObject.GetLangString("ETCS_WEDNESDAY"));
        thurName.setText(langStrObject.GetLangString("ETCS_THURSDAY"));
        friName.setText(langStrObject.GetLangString("ETCS_FRIDAY"));
        satName.setText(langStrObject.GetLangString("ETCS_SATURDAY"));
        sunName.setText(langStrObject.GetLangString("ETCS_SUNDAY"));
    }

    function initStatus(text)
    {
        setVisibleValue(true);
        updateBtnStatus();
        isAllorClear();   
        prevBtn.setDataText(text);
    }

    function setAllBtnValue(val0, val1, val2, val3, val4, val5, val6)
    {
        mon = val0;
        tue = val1;
        wed = val2;
        thur = val3;
        fri = val4;
        sat = val5;
        sun = val6;
    }

    function updateBtnStatus()
    {
        monBtn.btnStatus = mon;
        tueBtn.btnStatus = tue;
        wedBtn.btnStatus = wed;
        thurBtn.btnStatus = thur;
        friBtn.btnStatus = fri;
        satBtn.btnStatus = sat;
        sunBtn.btnStatus = sun;
    }

    function selectAll()
    {
        mon = true;
        tue = true;
        wed = true;
        thur = true;
        fri = true;
        sat = true;
        sun = true;
    }

    function clearAll()
    {
        mon = false;
        tue = false;
        wed = false;
        thur = false;
        fri = false;
        sat = false;
        sun = false;
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function setAllBtnValid()
    {
        allBtn.setVisibleValue(true);
        clearBtn.setVisibleValue(false);
    }

    function setClearBtnValid()
    {
        allBtn.setVisibleValue(false);
        clearBtn.setVisibleValue(true);
    }

    function isAllorClear()
    {
        if ((mon && tue  && wed  && thur && fri && sat && sun) === false)
        {
            setAllBtnValid();
        }
        if ((mon || tue  || wed  || thur || fri || sat || sun) === true)
        {
            setClearBtnValid();
        }
    }

    color: "#e7e7e7"

    MouseArea{
        anchors.fill: parent
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
            text: langStrObject.GetLangString("ETCS_REPEAT")
            font.family: rootItem.fontName
            font.pixelSize: mainPage.g_titleTextFontSize
            width: 200
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
        }

        ImageAndTextButton{
            id: prevBtn
            width: imageWidth + dataContentWidth - 8 + 35
            height: 60
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            imgLeftMargin: 14
            dataLeftMargin: -8
            fontSize: mainPage.g_titleTextFontSize2
            imageSource: logicObject.getLocalImagePath() + "back.png"
            visible: true
            bkgPressedColor: "#505050"
            dataWidth: 240 - (title.contentWidth/2) - imageWidth - 14 + 8
            onClickedSignal: {
                cancelSignal();
            }
        }

        TextButon{
            id: allBtn
            width: parent.height
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            fontSize: 18
            info: langStrObject.GetLangString("ETCS_SELECT_ALL")
            textColor: "#ffffff"
            visible: true
            bkgPressedColor: "#505050"
            onClickedSignal: {
                selectAll();
                updateBtnStatus();
                setClearBtnValid();
            }
        }

        TextButon{
            id: clearBtn
            width: parent.height
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            fontSize: 18
            info: langStrObject.GetLangString("ETCS_CLEAR_ALL")
            textColor: "#ffffff"
            visible: false
            bkgPressedColor: "#505050"
            onClickedSignal: {
                clearAll();
                updateBtnStatus();
                setAllBtnValid();
            }
        }
    }

    StripFrame3{
        id: monBtn
        anchors.top: titleBar.bottom
        anchors.topMargin: 25
        anchors.left: parent.left
        width: 480
        height: 65
        btnStatus: mon
        line1Visible: false
        line2Visible: true
        CommonText{
            id: monName
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 22
            text: langStrObject.GetLangString("ETCS_MONDAY")
            color: "#303030"
            width: parent.width - 22 - monImage.width - 22
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
        }

        CommonImage{
            id: monImage
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 22
            visible: mon
            source: logicObject.getLocalImagePath() + "repeat.png"
        }

        onSelectSignal: {
            mon = true;
            isAllorClear();
        }
        onUndoSignal: {
            mon = false;
            isAllorClear();
        }
    }
    StripFrame3{
        id: tueBtn
        anchors.top: monBtn.bottom
        anchors.left: parent.left
        btnStatus: tue
        line1Visible: false
        line2Visible: true
        CommonText{
            id: tueName
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 22
            text: langStrObject.GetLangString("ETCS_TUESDAY")
            color: "#303030"
            width: parent.width - 22 - tueImage.width - 22
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
        }

        CommonImage{
            id: tueImage
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 22
            visible: tue
            source: logicObject.getLocalImagePath() + "repeat.png"
        }

        onSelectSignal: {
            tue = true;
            isAllorClear();
        }
        onUndoSignal: {
            tue = false;
            isAllorClear();
        }
    }
    StripFrame3{
        id: wedBtn
        anchors.top: tueBtn.bottom
        anchors.left: parent.left
        btnStatus: wed
        line1Visible: false
        line2Visible: true
        CommonText{
            id: wedName
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 22
            text: langStrObject.GetLangString("ETCS_WEDNESDAY")
            color: "#303030"
            width: parent.width - 22 - wedImage.width - 22
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
        }

        CommonImage{
            id: wedImage
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 22
            visible: wed
            source: logicObject.getLocalImagePath() + "repeat.png"
        }
        onSelectSignal: {
            wed = true;
            isAllorClear();
        }
        onUndoSignal: {
            wed = false;
            isAllorClear();
        }
    }
    StripFrame3{
        id: thurBtn
        anchors.top: wedBtn.bottom
        anchors.left: parent.left
        btnStatus: thur
        line1Visible: false
        line2Visible: true
        CommonText{
            id: thurName
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 22
            text: langStrObject.GetLangString("ETCS_THURSDAY")
            color: "#303030"
            width: parent.width - 22 - thurImage.width - 22
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
        }

        CommonImage{
            id: thurImage
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 22
            visible: thur
            source: logicObject.getLocalImagePath() + "repeat.png"
        }
        onSelectSignal: {
            thur = true;
            isAllorClear();
        }
        onUndoSignal: {
            thur = false;
            isAllorClear();
        }
    }
    StripFrame3{
        id: friBtn
        anchors.top: thurBtn.bottom
        anchors.left: parent.left
        btnStatus: fri
        line1Visible: false
        line2Visible: true
        CommonText{
            id: friName
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 22
            text: langStrObject.GetLangString("ETCS_FRIDAY")
            color: "#303030"
            width: parent.width - 22 - friImage.width - 22
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
        }

        CommonImage{
            id: friImage
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 22
            visible: fri
            source: logicObject.getLocalImagePath() + "repeat.png"
        }
        onSelectSignal: {
            fri = true;
            isAllorClear();
        }
        onUndoSignal: {
            fri = false;
            isAllorClear();
        }
    }
    StripFrame3{
        id: satBtn
        anchors.top: friBtn.bottom
        anchors.left: parent.left
        btnStatus: sat
        line1Visible: false
        line2Visible: true
        CommonText{
            id: satName
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 22
            text: langStrObject.GetLangString("ETCS_SATURDAY")
            color: "#303030"
            width: parent.width - 22 - satImage.width - 22
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
        }

        CommonImage{
            id: satImage
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 22
            visible: sat
            source: logicObject.getLocalImagePath() + "repeat.png"
        }
        onSelectSignal: {
            sat = true;
            isAllorClear();
        }
        onUndoSignal: {
            sat = false;
            isAllorClear();
        }
    }
    StripFrame3{
        id: sunBtn
        anchors.top: satBtn.bottom
        anchors.left: parent.left
        btnStatus: sun
        line1Visible: false
        line2Visible: false
        CommonText{
            id: sunName
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 22
            text: langStrObject.GetLangString("ETCS_SUNDAY")
            color: "#303030"
            width: parent.width - 22 - sunImage.width - 22
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
        }

        CommonImage{
            id: sunImage
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 22
            visible: sun
            source: logicObject.getLocalImagePath() + "repeat.png"
        }
        onSelectSignal: {
            sun = true;
            isAllorClear();
        }
        onUndoSignal: {
            sun = false;
            isAllorClear();
        }
    }

    Component.onCompleted: {
        rootItem.updateLanguage.connect(updateLanguageSlot);
    }
}
