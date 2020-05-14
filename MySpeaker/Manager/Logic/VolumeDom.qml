import QtQuick 2.0
import "../Components"

Rectangle {

    property int g_node;
    property int nodeListViewHeight: height - mainPage.g_titleBarHeight

    function updateLanguageSlot()
    {
        title.text = langStrObject.GetLangString("ETCS_DOM") + getGNode();
        prevTitleName.text = langStrObject.GetLangString("ETCS_NODE");
        dalInputName.setText(langStrObject.GetLangString("ETCS_DAL_INPUT"));
        inchannel1_1Name.setText(langStrObject.GetLangString("ETCS_CHANNEL") + "1.1");
        inchannel1_2Name.setText(langStrObject.GetLangString("ETCS_CHANNEL") + "1.2");
        inchannel2_1Name.setText(langStrObject.GetLangString("ETCS_CHANNEL") + "2.1");
        inchannel2_2Name.setText(langStrObject.GetLangString("ETCS_CHANNEL") + "2.2");
        inchannel3_1Name.setText(langStrObject.GetLangString("ETCS_CHANNEL") + "3.1");
        inchannel3_2Name.setText(langStrObject.GetLangString("ETCS_CHANNEL") + "3.2");
        inchannel4_1Name.setText(langStrObject.GetLangString("ETCS_CHANNEL") + "4.1");
        inchannel4_2Name.setText(langStrObject.GetLangString("ETCS_CHANNEL") + "4.2");
        dalOutputName.setText(langStrObject.GetLangString("ETCS_DAL_OUTPUT"));
        outchannel1_1Name.setText(langStrObject.GetLangString("ETCS_CHANNEL") + "1.1");
        outchannel1_2Name.setText(langStrObject.GetLangString("ETCS_CHANNEL") + "1.2");
        outchannel2_1Name.setText(langStrObject.GetLangString("ETCS_CHANNEL") + "2.1");
        outchannel2_2Name.setText(langStrObject.GetLangString("ETCS_CHANNEL") + "2.2");
        outchannel3_1Name.setText(langStrObject.GetLangString("ETCS_CHANNEL") + "3.1");
        outchannel3_2Name.setText(langStrObject.GetLangString("ETCS_CHANNEL") + "3.2");
        outchannel4_1Name.setText(langStrObject.GetLangString("ETCS_CHANNEL") + "4.1");
        outchannel4_2Name.setText(langStrObject.GetLangString("ETCS_CHANNEL") + "4.2");
        amplifierName.setText(langStrObject.GetLangString("ETCS_AMPLIFIER"));
        ampChannel1Name.setText(langStrObject.GetLangString("ETCS_CHANNEL") + "1");
        ampChannel2Name.setText(langStrObject.GetLangString("ETCS_CHANNEL") + "2");
        ampChannel3Name.setText(langStrObject.GetLangString("ETCS_CHANNEL") + "3");
        ampChannel4Name.setText(langStrObject.GetLangString("ETCS_CHANNEL") + "4");
        miscellaneousName.setText(langStrObject.GetLangString("ETCS_MESCELLANEOUS"));
        signalGenerator1_1Name.setText(langStrObject.GetLangString("ETCS_SIGNAL_GENERATOR"));
        avcName.setText(langStrObject.GetLangString("ETCS_AVC_INPUT"));
        input1Name.setText(langStrObject.GetLangString("ETCS_AVC_INPUT") + "1");
        input2Name.setText(langStrObject.GetLangString("ETCS_AVC_INPUT") + "2");
        input3Name.setText(langStrObject.GetLangString("ETCS_AVC_INPUT") + "3");
        input4Name.setText(langStrObject.GetLangString("ETCS_AVC_INPUT") + "4");
    }

    function initStatus()
    {
        setVisibleValue(true);
        nodeListView.setContentY(0);
    }

    function quitStatus()
    {
        setVisibleValue(false);
        setGNode(0);
        nodeListView.setAllSliderDisable();
        nodeListView.setAllSliderDataInitStatus();
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function getGNode()
    {
        return g_node;
    }

    function setGNode(node)
    {
        g_node = node;
    }

    function updateVolumeResultSlot(node, type, number, channel, errorCode)
    {

        if (node !== getGNode())
        {
            return;
        }

        if (type === 0)
        {
            if ((number === 41) &&  (channel === 1))
            {
                if (errorCode === 0)
                {
                    inchannel1_1Data.setFontStrikeOutValue(false);
                    inchannel1_1Data.setTextColor("#303030");
                }
                else
                {
                    inchannel1_1Data.setFontStrikeOutValue(true);
                    inchannel1_1Data.setTextColor("red");
                }
            }
            else if ((number === 41) &&  (channel === 2))
            {
                if (errorCode === 0)
                {
                    inchannel1_2Data.setFontStrikeOutValue(false);
                    inchannel1_2Data.setTextColor("#303030");
                }
                else
                {
                    inchannel1_2Data.setFontStrikeOutValue(true);
                    inchannel1_2Data.setTextColor("red");
                }
            }
            else if ((number === 42) &&  (channel === 1))
            {
                if (errorCode === 0)
                {
                    inchannel2_1Data.setFontStrikeOutValue(false);
                    inchannel2_1Data.setTextColor("#303030");
                }
                else
                {
                    inchannel2_1Data.setFontStrikeOutValue(true);
                    inchannel2_1Data.setTextColor("red");
                }
            }
            else if ((number === 42) &&  (channel === 2))
            {
                if (errorCode === 0)
                {
                    inchannel2_2Data.setFontStrikeOutValue(false);
                    inchannel2_2Data.setTextColor("#303030");
                }
                else
                {
                    inchannel2_2Data.setFontStrikeOutValue(true);
                    inchannel2_2Data.setTextColor("red");
                }
            }
            else if ((number === 43) &&  (channel === 1))
            {
                if (errorCode === 0)
                {
                    inchannel3_1Data.setFontStrikeOutValue(false);
                    inchannel3_1Data.setTextColor("#303030");
                }
                else
                {
                    inchannel3_1Data.setFontStrikeOutValue(true);
                    inchannel3_1Data.setTextColor("red");
                }
            }
            else if ((number === 43) &&  (channel === 2))
            {
                if (errorCode === 0)
                {
                    inchannel3_2Data.setFontStrikeOutValue(false);
                    inchannel3_2Data.setTextColor("#303030");
                }
                else
                {
                    inchannel3_2Data.setFontStrikeOutValue(true);
                    inchannel3_2Data.setTextColor("red");
                }
            }
            else if ((number === 44) &&  (channel === 1))
            {
                if (errorCode === 0)
                {
                    inchannel4_1Data.setFontStrikeOutValue(false);
                    inchannel4_1Data.setTextColor("#303030");
                }
                else
                {
                    inchannel4_1Data.setFontStrikeOutValue(true);
                    inchannel4_1Data.setTextColor("red");
                }
            }
            else if ((number === 44) &&  (channel === 2))
            {
                if (errorCode === 0)
                {
                    inchannel4_2Data.setFontStrikeOutValue(false);
                    inchannel4_2Data.setTextColor("#303030");
                }
                else
                {
                    inchannel4_2Data.setFontStrikeOutValue(true);
                    inchannel4_2Data.setTextColor("red");
                }
            }
            else if ((number === 1) &&  (channel === 1))
            {
                if (errorCode === 0)
                {
                    input1Data.setFontStrikeOutValue(false);
                    input1Data.setTextColor("#303030");
                }
                else
                {
                    input1Data.setFontStrikeOutValue(true);
                    input1Data.setTextColor("red");
                }
            }
            else if ((number === 1) &&  (channel === 2))
            {
                if (errorCode === 0)
                {
                    input2Data.setFontStrikeOutValue(false);
                    input2Data.setTextColor("#303030");
                }
                else
                {
                    input2Data.setFontStrikeOutValue(true);
                    input2Data.setTextColor("red");
                }
            }
            else if ((number === 2) &&  (channel === 1))
            {
                if (errorCode === 0)
                {
                    input3Data.setFontStrikeOutValue(false);
                    input3Data.setTextColor("#303030");
                }
                else
                {
                    input3Data.setFontStrikeOutValue(true);
                    input3Data.setTextColor("red");
                }
            }
            else if ((number === 2) &&  (channel === 2))
            {
                if (errorCode === 0)
                {
                    input4Data.setFontStrikeOutValue(false);
                    input4Data.setTextColor("#303030");
                }
                else
                {
                    input4Data.setFontStrikeOutValue(true);
                    input4Data.setTextColor("red");
                }
            }
        }
        else if (type === 2)
        {
            if ((number === 41) &&  (channel === 1))
            {
                if (errorCode === 0)
                {
                    outchannel1_1Data.setFontStrikeOutValue(false);
                    outchannel1_1Data.setTextColor("#303030");
                }
                else
                {
                    outchannel1_1Data.setFontStrikeOutValue(true);
                    outchannel1_1Data.setTextColor("red");
                }
            }
            else if ((number === 42) &&  (channel === 1))
            {
                if (errorCode === 0)
                {
                    outchannel1_2Data.setFontStrikeOutValue(false);
                    outchannel1_2Data.setTextColor("#303030");
                }
                else
                {
                    outchannel1_2Data.setFontStrikeOutValue(true);
                    outchannel1_2Data.setTextColor("red");
                }
            }
            else if ((number === 43) &&  (channel === 1))
            {
                if (errorCode === 0)
                {
                    outchannel2_1Data.setFontStrikeOutValue(false);
                    outchannel2_1Data.setTextColor("#303030");
                }
                else
                {
                    outchannel2_1Data.setFontStrikeOutValue(true);
                    outchannel2_1Data.setTextColor("red");
                }
            }
            else if ((number === 44) &&  (channel === 1))
            {
                if (errorCode === 0)
                {
                    outchannel2_2Data.setFontStrikeOutValue(false);
                    outchannel2_2Data.setTextColor("#303030");
                }
                else
                {
                    outchannel2_2Data.setFontStrikeOutValue(true);
                    outchannel2_2Data.setTextColor("red");
                }
            }
            else if ((number === 45) &&  (channel === 1))
            {
                if (errorCode === 0)
                {
                    outchannel3_1Data.setFontStrikeOutValue(false);
                    outchannel3_1Data.setTextColor("#303030");
                }
                else
                {
                    outchannel3_1Data.setFontStrikeOutValue(true);
                    outchannel3_1Data.setTextColor("red");
                }
            }
            else if ((number === 46) &&  (channel === 1))
            {
                if (errorCode === 0)
                {
                    outchannel3_2Data.setFontStrikeOutValue(false);
                    outchannel3_2Data.setTextColor("#303030");
                }
                else
                {
                    outchannel3_2Data.setFontStrikeOutValue(true);
                    outchannel3_2Data.setTextColor("red");
                }
            }
            else if ((number === 47) &&  (channel === 1))
            {
                if (errorCode === 0)
                {
                    outchannel4_1Data.setFontStrikeOutValue(false);
                    outchannel4_1Data.setTextColor("#303030");
                }
                else
                {
                    outchannel4_1Data.setFontStrikeOutValue(true);
                    outchannel4_1Data.setTextColor("red");
                }
            }
            else if ((number === 48) &&  (channel === 1))
            {
                if (errorCode === 0)
                {
                    outchannel4_2Data.setFontStrikeOutValue(false);
                    outchannel4_2Data.setTextColor("#303030");
                }
                else
                {
                    outchannel4_2Data.setFontStrikeOutValue(true);
                    outchannel4_2Data.setTextColor("red");
                }
            }
        }
        else if (type === 4)
        {
            if ((number === 1) &&  (channel === 1))
            {
                if (errorCode === 0)
                {
                    signalGenerator1_1Data.setFontStrikeOutValue(false);
                    signalGenerator1_1Data.setTextColor("#303030");
                }
                else
                {
                    signalGenerator1_1Data.setFontStrikeOutValue(true);
                    signalGenerator1_1Data.setTextColor("red");
                }
            }
            /*
            else if ((number === 2) &&  (channel === 1))
            {
                if (errorCode === 0)
                {
                    signalGenerator2_1Data.setFontStrikeOutValue(false);
                    signalGenerator2_1Data.setTextColor("#303030");
                }
                else
                {
                    signalGenerator2_1Data.setFontStrikeOutValue(true);
                    signalGenerator2_1Data.setTextColor("red");
                }
            }
            else if ((number === 3) &&  (channel === 1))
            {
                if (errorCode === 0)
                {
                    signalGenerator3_1Data.setFontStrikeOutValue(false);
                    signalGenerator3_1Data.setTextColor("#303030");
                }
                else
                {
                    signalGenerator3_1Data.setFontStrikeOutValue(true);
                    signalGenerator3_1Data.setTextColor("red");
                }
            }
            else if ((number === 4) &&  (channel === 1))
            {
                if (errorCode === 0)
                {
                    signalGenerator4_1Data.setFontStrikeOutValue(false);
                    signalGenerator4_1Data.setTextColor("#303030");
                }
                else
                {
                    signalGenerator4_1Data.setFontStrikeOutValue(true);
                    signalGenerator4_1Data.setTextColor("red");
                }
            }
            */
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

        Text{
            id: title
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: mainPage.g_titleTextColor
            text: langStrObject.GetLangString("ETCS_DOM") + getGNode()
            font.family: rootItem.fontName
            font.pixelSize: mainPage.g_titleTextFontSize
            width: 200
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            z: 2
            Component.onCompleted: {

            }
        }

        Rectangle{
            id: prevBtnImgRect
            width: prevBtnImg.width + prevTitleName.contentWidth - 8 + 35
            height: parent.height
            anchors.top: parent.top
            anchors.left: parent.left
            color: mouseArea.pressed ? "#505050" : parent.color
            clip: true
            Image{
                id: prevBtnImg
                source: logicObject.getLocalImagePath() + "back.png"
                anchors.left: parent.left
                anchors.leftMargin: 14
                anchors.verticalCenter: parent.verticalCenter
            }
            Text{
                id: prevTitleName
                anchors.left: prevBtnImg.right
                anchors.leftMargin: -8
                anchors.verticalCenter: parent.verticalCenter
                color: mainPage.g_titleTextColor
                text: langStrObject.GetLangString("ETCS_NODE")
                font.family: rootItem.fontName
                font.pixelSize: mainPage.g_titleTextFontSize2
                width: 240 - (title.contentWidth / 2) - prevBtnImg.width - 14 + 8
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
                Component.onCompleted: {
                }
            }
            MouseArea{
                id: mouseArea
                anchors.fill: parent
                onClicked: {
                    quitStatus();
                }
            }
        }
    }

    Flickable{

        function updateVolListSlot()
        {
            var volDB;
            var min = 0.0;
            var max = 0.0;

            nodeListView.setAllSliderDisable();
            nodeListView.setAllSliderDataInitStatus();

            volDB = sourceManageObject.getVolumeFromNodeDev(getGNode(), 0, 41, 1);
            if (volDB !== -1000)
            {
                min = sourceManageObject.getMinVolumeFromNodeDev(getGNode(), 0, 41, 1);
                max = sourceManageObject.getMaxVolumeFromNodeDev(getGNode(), 0, 41, 1);
                inchannel1_1VolumeSlider.setMinimumValue(min);
                inchannel1_1VolumeSlider.setMaximumValue(max);
                inchannel1_1VolumeSlider.setValue(volDB);
                inchannel1_1VolumeSlider.enableSlider();
            }

            volDB = sourceManageObject.getVolumeFromNodeDev(getGNode(), 0, 41, 2);
            if (volDB !== -1000)
            {
                min = sourceManageObject.getMinVolumeFromNodeDev(getGNode(), 0, 41, 2);
                max = sourceManageObject.getMaxVolumeFromNodeDev(getGNode(), 0, 41, 2);
                inchannel1_2VolumeSlider.setMinimumValue(min);
                inchannel1_2VolumeSlider.setMaximumValue(max);
                inchannel1_2VolumeSlider.setValue(volDB);
                inchannel1_2VolumeSlider.enableSlider();
            }

            volDB = sourceManageObject.getVolumeFromNodeDev(getGNode(), 0, 42, 1);
            if (volDB !== -1000)
            {
                min = sourceManageObject.getMinVolumeFromNodeDev(getGNode(), 0, 42, 1);
                max = sourceManageObject.getMaxVolumeFromNodeDev(getGNode(), 0, 42, 1);
                inchannel2_1VolumeSlider.setMinimumValue(min);
                inchannel2_1VolumeSlider.setMaximumValue(max);
                inchannel2_1VolumeSlider.setValue(volDB);
                inchannel2_1VolumeSlider.enableSlider();
            }

            volDB = sourceManageObject.getVolumeFromNodeDev(getGNode(), 0, 42, 2);
            if (volDB !== -1000)
            {
                min = sourceManageObject.getMinVolumeFromNodeDev(getGNode(), 0, 42, 2);
                max = sourceManageObject.getMaxVolumeFromNodeDev(getGNode(), 0, 42, 2);
                inchannel2_2VolumeSlider.setMinimumValue(min);
                inchannel2_2VolumeSlider.setMaximumValue(max);
                inchannel2_2VolumeSlider.setValue(volDB);
                inchannel2_2VolumeSlider.enableSlider();
            }

            volDB = sourceManageObject.getVolumeFromNodeDev(getGNode(), 0, 43, 1);
            if (volDB !== -1000)
            {
                min = sourceManageObject.getMinVolumeFromNodeDev(getGNode(), 0, 43, 1);
                max = sourceManageObject.getMaxVolumeFromNodeDev(getGNode(), 0, 43, 1);
                inchannel3_1VolumeSlider.setMinimumValue(min);
                inchannel3_1VolumeSlider.setMaximumValue(max);
                inchannel3_1VolumeSlider.setValue(volDB);
                inchannel3_1VolumeSlider.enableSlider();
            }

            volDB = sourceManageObject.getVolumeFromNodeDev(getGNode(), 0, 43, 2);
            if (volDB !== -1000)
            {
                min = sourceManageObject.getMinVolumeFromNodeDev(getGNode(), 0, 43, 2);
                max = sourceManageObject.getMaxVolumeFromNodeDev(getGNode(), 0, 43, 2);
                inchannel3_2VolumeSlider.setMinimumValue(min);
                inchannel3_2VolumeSlider.setMaximumValue(max);
                inchannel3_2VolumeSlider.setValue(volDB);
                inchannel3_2VolumeSlider.enableSlider();
            }

            volDB = sourceManageObject.getVolumeFromNodeDev(getGNode(), 0, 44, 1);
            if (volDB !== -1000)
            {
                min = sourceManageObject.getMinVolumeFromNodeDev(getGNode(), 0, 44, 1);
                max = sourceManageObject.getMaxVolumeFromNodeDev(getGNode(), 0, 44, 1);
                inchannel4_1VolumeSlider.setMinimumValue(min);
                inchannel4_1VolumeSlider.setMaximumValue(max);
                inchannel4_1VolumeSlider.setValue(volDB);
                inchannel4_1VolumeSlider.enableSlider();
            }

            volDB = sourceManageObject.getVolumeFromNodeDev(getGNode(), 0, 44, 2);
            if (volDB !== -1000)
            {
                min = sourceManageObject.getMinVolumeFromNodeDev(getGNode(), 0, 44, 2);
                max = sourceManageObject.getMaxVolumeFromNodeDev(getGNode(), 0, 44, 2);
                inchannel4_2VolumeSlider.setMinimumValue(min);
                inchannel4_2VolumeSlider.setMaximumValue(max);
                inchannel4_2VolumeSlider.setValue(volDB);
                inchannel4_2VolumeSlider.enableSlider();
            }

            volDB = sourceManageObject.getVolumeFromNodeDev(getGNode(), 2, 41, 1);
            if (volDB !== -1000)
            {
                min = sourceManageObject.getMinVolumeFromNodeDev(getGNode(), 2, 41, 1);
                max = sourceManageObject.getMaxVolumeFromNodeDev(getGNode(), 2, 41, 1);
                outchannel1_1VolumeSlider.setMinimumValue(min);
                outchannel1_1VolumeSlider.setMaximumValue(max);
                outchannel1_1VolumeSlider.setValue(volDB);
                outchannel1_1VolumeSlider.enableSlider();
            }

            volDB = sourceManageObject.getVolumeFromNodeDev(getGNode(), 2, 42, 1);
            if (volDB !== -1000)
            {
                min = sourceManageObject.getMinVolumeFromNodeDev(getGNode(), 2, 42, 1);
                max = sourceManageObject.getMaxVolumeFromNodeDev(getGNode(), 2, 42, 1);
                outchannel1_2VolumeSlider.setMinimumValue(min);
                outchannel1_2VolumeSlider.setMaximumValue(max);
                outchannel1_2VolumeSlider.setValue(volDB);
                outchannel1_2VolumeSlider.enableSlider();
            }

            volDB = sourceManageObject.getVolumeFromNodeDev(getGNode(), 2, 43, 1);
            if (volDB !== -1000)
            {
                min = sourceManageObject.getMinVolumeFromNodeDev(getGNode(), 2, 43, 1);
                max = sourceManageObject.getMaxVolumeFromNodeDev(getGNode(), 2, 43, 1);
                outchannel2_1VolumeSlider.setMinimumValue(min);
                outchannel2_1VolumeSlider.setMaximumValue(max);
                outchannel2_1VolumeSlider.setValue(volDB);
                outchannel2_1VolumeSlider.enableSlider();
            }

            volDB = sourceManageObject.getVolumeFromNodeDev(getGNode(), 2, 44, 1);
            if (volDB !== -1000)
            {
                min = sourceManageObject.getMinVolumeFromNodeDev(getGNode(), 2, 44, 1);
                max = sourceManageObject.getMaxVolumeFromNodeDev(getGNode(), 2, 44, 1);
                outchannel2_2VolumeSlider.setMinimumValue(min);
                outchannel2_2VolumeSlider.setMaximumValue(max);
                outchannel2_2VolumeSlider.setValue(volDB);
                outchannel2_2VolumeSlider.enableSlider();
            }

            volDB = sourceManageObject.getVolumeFromNodeDev(getGNode(), 2, 45, 1);
            if (volDB !== -1000)
            {
                min = sourceManageObject.getMinVolumeFromNodeDev(getGNode(), 2, 45, 1);
                max = sourceManageObject.getMaxVolumeFromNodeDev(getGNode(), 2, 45, 1);
                outchannel3_1VolumeSlider.setMinimumValue(min);
                outchannel3_1VolumeSlider.setMaximumValue(max);
                outchannel3_1VolumeSlider.setValue(volDB);
                outchannel3_1VolumeSlider.enableSlider();
            }

            volDB = sourceManageObject.getVolumeFromNodeDev(getGNode(), 2, 46, 1);
            if (volDB !== -1000)
            {
                min = sourceManageObject.getMinVolumeFromNodeDev(getGNode(), 2, 46, 1);
                max = sourceManageObject.getMaxVolumeFromNodeDev(getGNode(), 2, 46, 1);
                outchannel3_2VolumeSlider.setMinimumValue(min);
                outchannel3_2VolumeSlider.setMaximumValue(max);
                outchannel3_2VolumeSlider.setValue(volDB);
                outchannel3_2VolumeSlider.enableSlider();
            }

            volDB = sourceManageObject.getVolumeFromNodeDev(getGNode(), 2, 47, 1);
            if (volDB !== -1000)
            {
                min = sourceManageObject.getMinVolumeFromNodeDev(getGNode(), 2, 47, 1);
                max = sourceManageObject.getMaxVolumeFromNodeDev(getGNode(), 2, 47, 1);
                outchannel4_1VolumeSlider.setMinimumValue(min);
                outchannel4_1VolumeSlider.setMaximumValue(max);
                outchannel4_1VolumeSlider.setValue(volDB);
                outchannel4_1VolumeSlider.enableSlider();
            }

            volDB = sourceManageObject.getVolumeFromNodeDev(getGNode(), 2, 48, 1);
            if (volDB !== -1000)
            {
                min = sourceManageObject.getMinVolumeFromNodeDev(getGNode(), 2, 48, 1);
                max = sourceManageObject.getMaxVolumeFromNodeDev(getGNode(), 2, 48, 1);
                outchannel4_2VolumeSlider.setMinimumValue(min);
                outchannel4_2VolumeSlider.setMaximumValue(max);
                outchannel4_2VolumeSlider.setValue(volDB);
                outchannel4_2VolumeSlider.enableSlider();
            }

            volDB = sourceManageObject.getVolumeFromNodeDev(getGNode(), 2, 1, 1);
            if (volDB !== -1000)
            {
                min = sourceManageObject.getMinVolumeFromNodeDev(getGNode(), 2, 1, 1);
                max = sourceManageObject.getMaxVolumeFromNodeDev(getGNode(), 2, 1, 1);
                ampChannel1VolumeSlider.setMinimumValue(min);
                ampChannel1VolumeSlider.setMaximumValue(max);
                ampChannel1VolumeSlider.setValue(volDB);
                ampChannel1VolumeSlider.enableSlider();
            }

            volDB = sourceManageObject.getVolumeFromNodeDev(getGNode(), 2, 2, 1);
            if (volDB !== -1000)
            {
                min = sourceManageObject.getMinVolumeFromNodeDev(getGNode(), 2, 2, 1);
                max = sourceManageObject.getMaxVolumeFromNodeDev(getGNode(), 2, 2, 1);
                ampChannel2VolumeSlider.setMinimumValue(min);
                ampChannel2VolumeSlider.setMaximumValue(max);
                ampChannel2VolumeSlider.setValue(volDB);
                ampChannel2VolumeSlider.enableSlider();
            }

            volDB = sourceManageObject.getVolumeFromNodeDev(getGNode(), 2, 3, 1);
            if (volDB !== -1000)
            {
                min = sourceManageObject.getMinVolumeFromNodeDev(getGNode(), 2, 3, 1);
                max = sourceManageObject.getMaxVolumeFromNodeDev(getGNode(), 2, 3, 1);
                ampChannel3VolumeSlider.setMinimumValue(min);
                ampChannel3VolumeSlider.setMaximumValue(max);
                ampChannel3VolumeSlider.setValue(volDB);
                ampChannel3VolumeSlider.enableSlider();
            }

            volDB = sourceManageObject.getVolumeFromNodeDev(getGNode(), 2, 4, 1);
            if (volDB !== -1000)
            {
                min = sourceManageObject.getMinVolumeFromNodeDev(getGNode(), 2, 4, 1);
                max = sourceManageObject.getMaxVolumeFromNodeDev(getGNode(), 2, 4, 1);
                ampChannel4VolumeSlider.setMinimumValue(min);
                ampChannel4VolumeSlider.setMaximumValue(max);
                ampChannel4VolumeSlider.setValue(volDB);
                ampChannel4VolumeSlider.enableSlider();
            }

            volDB = sourceManageObject.getVolumeFromNodeDev(getGNode(), 4, 1, 1);
            if (volDB !== -1000)
            {
                min = sourceManageObject.getMinVolumeFromNodeDev(getGNode(), 4, 1, 1);
                max = sourceManageObject.getMaxVolumeFromNodeDev(getGNode(), 4, 1, 1);
                signalGenerator1_1VolumeSlider.setMinimumValue(min);
                signalGenerator1_1VolumeSlider.setMaximumValue(max);
                signalGenerator1_1VolumeSlider.setValue(volDB);
                signalGenerator1_1VolumeSlider.enableSlider();
            }

            /*
            volDB = sourceManageObject.getVolumeFromNodeDev(getGNode(), 4, 2, 1);
            if (volDB !== -1000)
            {
                min = sourceManageObject.getMinVolumeFromNodeDev(getGNode(), 4, 2, 1);
                max = sourceManageObject.getMaxVolumeFromNodeDev(getGNode(), 4, 2, 1);
                signalGenerator2_1VolumeSlider.setMinimumValue(min);
                signalGenerator2_1VolumeSlider.setMaximumValue(max);
                signalGenerator2_1VolumeSlider.setValue(volDB);
                signalGenerator2_1VolumeSlider.enableSlider();
            }

            volDB = sourceManageObject.getVolumeFromNodeDev(getGNode(), 4, 3, 1);
            if (volDB !== -1000)
            {
                min = sourceManageObject.getMinVolumeFromNodeDev(getGNode(), 4, 3, 1);
                max = sourceManageObject.getMaxVolumeFromNodeDev(getGNode(), 4, 3, 1);
                signalGenerator3_1VolumeSlider.setMinimumValue(min);
                signalGenerator3_1VolumeSlider.setMaximumValue(max);
                signalGenerator3_1VolumeSlider.setValue(volDB);
                signalGenerator3_1VolumeSlider.enableSlider();
            }

            volDB = sourceManageObject.getVolumeFromNodeDev(getGNode(), 4, 4, 1);
            if (volDB !== -1000)
            {
                min = sourceManageObject.getMinVolumeFromNodeDev(getGNode(), 4, 4, 1);
                max = sourceManageObject.getMaxVolumeFromNodeDev(getGNode(), 4, 4, 1);
                signalGenerator4_1VolumeSlider.setMinimumValue(min);
                signalGenerator4_1VolumeSlider.setMaximumValue(max);
                signalGenerator4_1VolumeSlider.setValue(volDB);
                signalGenerator4_1VolumeSlider.enableSlider();
            }
            */

            volDB = sourceManageObject.getVolumeFromNodeDev(getGNode(), 0, 1, 1);
            if (volDB !== -1000)
            {
                min = sourceManageObject.getMinVolumeFromNodeDev(getGNode(), 0, 1, 1);
                max = sourceManageObject.getMaxVolumeFromNodeDev(getGNode(), 0, 1, 1);
                input1VolumeSlider.setMinimumValue(min);
                input1VolumeSlider.setMaximumValue(max);
                input1VolumeSlider.setValue(volDB);
                input1VolumeSlider.enableSlider();
            }

            volDB = sourceManageObject.getVolumeFromNodeDev(getGNode(), 0, 1, 2);
            if (volDB !== -1000)
            {
                min = sourceManageObject.getMinVolumeFromNodeDev(getGNode(), 0, 1, 2);
                max = sourceManageObject.getMaxVolumeFromNodeDev(getGNode(), 0, 1, 2);
                input2VolumeSlider.setMinimumValue(min);
                input2VolumeSlider.setMaximumValue(max);
                input2VolumeSlider.setValue(volDB);
                input2VolumeSlider.enableSlider();
            }

            volDB = sourceManageObject.getVolumeFromNodeDev(getGNode(), 0, 2, 1);
            if (volDB !== -1000)
            {
                min = sourceManageObject.getMinVolumeFromNodeDev(getGNode(), 0, 2, 1);
                max = sourceManageObject.getMaxVolumeFromNodeDev(getGNode(), 0, 2, 1);
                input3VolumeSlider.setMinimumValue(min);
                input3VolumeSlider.setMaximumValue(max);
                input3VolumeSlider.setValue(volDB);
                input3VolumeSlider.enableSlider();
            }

            volDB = sourceManageObject.getVolumeFromNodeDev(getGNode(), 0, 2, 2);
            if (volDB !== -1000)
            {
                min = sourceManageObject.getMinVolumeFromNodeDev(getGNode(), 0, 2, 2);
                max = sourceManageObject.getMaxVolumeFromNodeDev(getGNode(), 0, 2, 2);
                input4VolumeSlider.setMinimumValue(min);
                input4VolumeSlider.setMaximumValue(max);
                input4VolumeSlider.setValue(volDB);
                input4VolumeSlider.enableSlider();
            }
        }

        function setAllSliderDisable()
        {
            inchannel1_1VolumeSlider.disableSlider();
            inchannel1_2VolumeSlider.disableSlider();
            inchannel2_1VolumeSlider.disableSlider();
            inchannel2_2VolumeSlider.disableSlider();
            inchannel3_1VolumeSlider.disableSlider();
            inchannel3_2VolumeSlider.disableSlider();
            inchannel4_1VolumeSlider.disableSlider();
            inchannel4_2VolumeSlider.disableSlider();
            outchannel1_1VolumeSlider.disableSlider();
            outchannel1_2VolumeSlider.disableSlider();
            outchannel2_1VolumeSlider.disableSlider();
            outchannel2_2VolumeSlider.disableSlider();
            outchannel3_1VolumeSlider.disableSlider();
            outchannel3_2VolumeSlider.disableSlider();
            outchannel4_1VolumeSlider.disableSlider();
            outchannel4_2VolumeSlider.disableSlider();
            ampChannel1VolumeSlider.disableSlider();
            ampChannel2VolumeSlider.disableSlider();
            ampChannel3VolumeSlider.disableSlider();
            ampChannel4VolumeSlider.disableSlider();
            signalGenerator1_1VolumeSlider.disableSlider();
            //signalGenerator2_1VolumeSlider.disableSlider();
            //signalGenerator3_1VolumeSlider.disableSlider();
            //signalGenerator4_1VolumeSlider.disableSlider();
            input1VolumeSlider.disableSlider();
            input2VolumeSlider.disableSlider();
            input3VolumeSlider.disableSlider();
            input4VolumeSlider.disableSlider();
        }

        function setAllSliderDataInitStatus()
        {
            inchannel1_1Data.setFontStrikeOutValue(false);
            inchannel1_1Data.setTextColor("#303030");
            inchannel1_2Data.setFontStrikeOutValue(false);
            inchannel1_2Data.setTextColor("#303030");
            inchannel2_1Data.setFontStrikeOutValue(false);
            inchannel2_1Data.setTextColor("#303030");
            inchannel2_2Data.setFontStrikeOutValue(false);
            inchannel2_2Data.setTextColor("#303030");
            inchannel3_1Data.setFontStrikeOutValue(false);
            inchannel3_1Data.setTextColor("#303030");
            inchannel3_2Data.setFontStrikeOutValue(false);
            inchannel3_2Data.setTextColor("#303030");
            inchannel4_1Data.setFontStrikeOutValue(false);
            inchannel4_1Data.setTextColor("#303030");
            inchannel4_2Data.setFontStrikeOutValue(false);
            inchannel4_2Data.setTextColor("#303030");
            input1Data.setFontStrikeOutValue(false);
            input1Data.setTextColor("#303030");
            input2Data.setFontStrikeOutValue(false);
            input2Data.setTextColor("#303030");
            input3Data.setFontStrikeOutValue(false);
            input3Data.setTextColor("#303030");
            input4Data.setFontStrikeOutValue(false);
            input4Data.setTextColor("#303030");
            outchannel1_1Data.setFontStrikeOutValue(false);
            outchannel1_1Data.setTextColor("#303030");
            outchannel1_2Data.setFontStrikeOutValue(false);
            outchannel1_2Data.setTextColor("#303030");
            outchannel2_1Data.setFontStrikeOutValue(false);
            outchannel2_1Data.setTextColor("#303030");
            outchannel2_2Data.setFontStrikeOutValue(false);
            outchannel2_2Data.setTextColor("#303030");
            outchannel3_1Data.setFontStrikeOutValue(false);
            outchannel3_1Data.setTextColor("#303030");
            outchannel3_2Data.setFontStrikeOutValue(false);
            outchannel3_2Data.setTextColor("#303030");
            outchannel4_1Data.setFontStrikeOutValue(false);
            outchannel4_1Data.setTextColor("#303030");
            outchannel4_2Data.setFontStrikeOutValue(false);
            outchannel4_2Data.setTextColor("#303030");
            signalGenerator1_1Data.setFontStrikeOutValue(false);
            signalGenerator1_1Data.setTextColor("#303030");
        }

        function setContentY(value)
        {
            contentY = value;
        }

        id: nodeListView
        anchors.top: titleBar.bottom
        anchors.left: parent.left
        width: parent.width
        height: nodeListViewHeight
        contentWidth: width
        contentHeight: 30 * 65 + 25 * 5 + 20
        flickableDirection: Flickable.VerticalFlick
        boundsBehavior: Flickable.StopAtBounds
        clip: true

        Rectangle{
            anchors.fill: parent
            color: "#e7e7e7"
        }

        StripFrame{
            id: dalInput
            anchors.top: parent.top
            anchors.topMargin: 25
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: true
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonText{
                id: dalInputName
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 25
                width: 400
                font.pixelSize: 18
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_DAL_INPUT")
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
        }

        StripFrame{
            id: inchannel1_1
            anchors.top: dalInput.bottom
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: true
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonSlider{
                id: inchannel1_1VolumeSlider
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 10
                width: 400
                height: 60
                stepSize: 1
                maximumValue: 6
                minimumValue: -80
                enabled: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        inchannel1_1Timer.setTmpValue(inchannel1_1VolumeSlider.getValue());
                        inchannel1_1Timer.start();
                    }
                    else
                    {
                        inchannel1_1Timer.stop();
                        sourceManageObject.volumeControl(getGNode(), 0, 41, 1, inchannel1_1VolumeSlider.getValue());
                    }
                }

                Timer{

                    property real tmpValue: 0
                    function setTmpValue(tmp)
                    {
                        tmpValue = tmp;
                    }

                    id: inchannel1_1Timer
                    interval: 200
                    repeat: true
                    triggeredOnStart: true
                    onTriggered: {
                        if (tmpValue !== inchannel1_1VolumeSlider.getValue())
                        {
                            sourceManageObject.volumeControl(getGNode(), 0, 41, 1, inchannel1_1VolumeSlider.getValue());
                            tmpValue = inchannel1_1VolumeSlider.getValue();
                        }
                    }
                }
            }
            CommonText{
                id: inchannel1_1Name
                anchors.bottom: inchannel1_1VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.left: parent.left
                anchors.leftMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_CHANNEL") + "1.1"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
            CommonText{
                id: inchannel1_1Data
                anchors.bottom: inchannel1_1VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.right: parent.right
                anchors.rightMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: inchannel1_1VolumeSlider.getValue() + "dB"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
            }
        }

        StripFrame{
            id: inchannel1_2
            anchors.top: inchannel1_1.bottom
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: true
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonSlider{
                id: inchannel1_2VolumeSlider
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 10
                width: 400
                height: 60
                stepSize: 1
                maximumValue: 6
                minimumValue: -80
                enabled: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        inchannel1_2Timer.setTmpValue(inchannel1_2VolumeSlider.getValue());
                        inchannel1_2Timer.start();
                    }
                    else
                    {
                        inchannel1_2Timer.stop();
                        sourceManageObject.volumeControl(getGNode(), 0, 41, 2, inchannel1_2VolumeSlider.getValue());
                    }
                }

                Timer{

                    property real tmpValue: 0
                    function setTmpValue(tmp)
                    {
                        tmpValue = tmp;
                    }

                    id: inchannel1_2Timer
                    interval: 200
                    repeat: true
                    triggeredOnStart: true
                    onTriggered: {
                        if (tmpValue !== inchannel1_2VolumeSlider.getValue())
                        {
                            sourceManageObject.volumeControl(getGNode(), 0, 41, 2, inchannel1_2VolumeSlider.getValue());
                            tmpValue = inchannel1_2VolumeSlider.getValue();
                        }
                    }
                }

            }
            CommonText{
                id: inchannel1_2Name
                anchors.bottom: inchannel1_2VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.left: parent.left
                anchors.leftMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_CHANNEL") + "1.2"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
            CommonText{
                id: inchannel1_2Data
                anchors.bottom: inchannel1_2VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.right: parent.right
                anchors.rightMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: inchannel1_2VolumeSlider.getValue() + "dB"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
            }
        }

        StripFrame{
            id: inchannel2_1
            anchors.top: inchannel1_2.bottom
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: true
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonSlider{
                id: inchannel2_1VolumeSlider
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 10
                width: 400
                height: 60
                stepSize: 1
                maximumValue: 6
                minimumValue: -80
                enabled: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        inchannel2_1Timer.setTmpValue(inchannel2_1VolumeSlider.getValue());
                        inchannel2_1Timer.start();
                    }
                    else
                    {
                        inchannel2_1Timer.stop();
                        sourceManageObject.volumeControl(getGNode(), 0, 42, 1, inchannel2_1VolumeSlider.getValue());
                    }
                }

                Timer{

                    property real tmpValue: 0
                    function setTmpValue(tmp)
                    {
                        tmpValue = tmp;
                    }

                    id: inchannel2_1Timer
                    interval: 200
                    repeat: true
                    triggeredOnStart: true
                    onTriggered: {
                        if (tmpValue !== inchannel2_1VolumeSlider.getValue())
                        {
                            sourceManageObject.volumeControl(getGNode(), 0, 42, 1, inchannel2_1VolumeSlider.getValue());
                            tmpValue = inchannel2_1VolumeSlider.getValue();
                        }
                    }
                }
            }

            CommonText{
                id: inchannel2_1Name
                anchors.bottom: inchannel2_1VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.left: parent.left
                anchors.leftMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_CHANNEL") + "2.1"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
            CommonText{
                id: inchannel2_1Data
                anchors.bottom: inchannel2_1VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.right: parent.right
                anchors.rightMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: inchannel2_1VolumeSlider.getValue() + "dB"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
            }
        }

        StripFrame{
            id: inchannel2_2
            anchors.top: inchannel2_1.bottom
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: true
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonSlider{
                id: inchannel2_2VolumeSlider
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 10
                width: 400
                height: 60
                stepSize: 1
                maximumValue: 6
                minimumValue: -80
                enabled: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        inchannel2_2Timer.setTmpValue(inchannel2_2VolumeSlider.getValue());
                        inchannel2_2Timer.start();
                    }
                    else
                    {
                        inchannel2_2Timer.stop();
                        sourceManageObject.volumeControl(getGNode(), 0, 42, 2, inchannel2_2VolumeSlider.getValue());
                    }
                }

                Timer{

                    property real tmpValue: 0
                    function setTmpValue(tmp)
                    {
                        tmpValue = tmp;
                    }

                    id: inchannel2_2Timer
                    interval: 200
                    repeat: true
                    triggeredOnStart: true
                    onTriggered: {
                        if (tmpValue !== inchannel2_2VolumeSlider.getValue())
                        {
                            sourceManageObject.volumeControl(getGNode(), 0, 42, 2, inchannel2_2VolumeSlider.getValue());
                            tmpValue = inchannel2_2VolumeSlider.getValue();
                        }
                    }
                }
            }
            CommonText{
                id: inchannel2_2Name
                anchors.bottom: inchannel2_2VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.left: parent.left
                anchors.leftMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_CHANNEL") + "2.2"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
            CommonText{
                id: inchannel2_2Data
                anchors.bottom: inchannel2_2VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.right: parent.right
                anchors.rightMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: inchannel2_2VolumeSlider.getValue() + "dB"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
            }
        }

        StripFrame{
            id: inchannel3_1
            anchors.top: inchannel2_2.bottom
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: true
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonSlider{
                id: inchannel3_1VolumeSlider
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 10
                width: 400
                height: 60
                stepSize: 1
                maximumValue: 6
                minimumValue: -80
                enabled: false

                onPressedChanged: {
                    if (pressed == true)
                    {
                        inchannel3_1Timer.setTmpValue(inchannel3_1VolumeSlider.getValue());
                        inchannel3_1Timer.start();
                    }
                    else
                    {
                        inchannel3_1Timer.stop();
                        sourceManageObject.volumeControl(getGNode(), 0, 43, 1, inchannel3_1VolumeSlider.getValue());
                    }
                }

                Timer{

                    property real tmpValue: 0
                    function setTmpValue(tmp)
                    {
                        tmpValue = tmp;
                    }

                    id: inchannel3_1Timer
                    interval: 200
                    repeat: true
                    triggeredOnStart: true
                    onTriggered: {
                        if (tmpValue !== inchannel3_1VolumeSlider.getValue())
                        {
                            sourceManageObject.volumeControl(getGNode(), 0, 43, 1, inchannel3_1VolumeSlider.getValue());
                            tmpValue = inchannel3_1VolumeSlider.getValue();
                        }
                    }
                }
            }
            CommonText{
                id: inchannel3_1Name
                anchors.bottom: inchannel3_1VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.left: parent.left
                anchors.leftMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_CHANNEL") + "3.1"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
            CommonText{
                id: inchannel3_1Data
                anchors.bottom: inchannel3_1VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.right: parent.right
                anchors.rightMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: inchannel3_1VolumeSlider.getValue() + "dB"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
            }
        }

        StripFrame{
            id: inchannel3_2
            anchors.top: inchannel3_1.bottom
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: true
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonSlider{
                id: inchannel3_2VolumeSlider
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 10
                width: 400
                height: 60
                stepSize: 1
                maximumValue: 6
                minimumValue: -80
                enabled: false

                onPressedChanged: {
                    if (pressed == true)
                    {
                        inchannel3_2Timer.setTmpValue(inchannel3_2VolumeSlider.getValue());
                        inchannel3_2Timer.start();
                    }
                    else
                    {
                        inchannel3_2Timer.stop();
                        sourceManageObject.volumeControl(getGNode(), 0, 43, 2, inchannel3_2VolumeSlider.getValue());
                    }
                }

                Timer{

                    property real tmpValue: 0
                    function setTmpValue(tmp)
                    {
                        tmpValue = tmp;
                    }

                    id: inchannel3_2Timer
                    interval: 200
                    repeat: true
                    triggeredOnStart: true
                    onTriggered: {
                        if (tmpValue !== inchannel3_2VolumeSlider.getValue())
                        {
                            sourceManageObject.volumeControl(getGNode(), 0, 43, 2, inchannel3_2VolumeSlider.getValue());
                            tmpValue = inchannel3_2VolumeSlider.getValue();
                        }
                    }
                }
            }
            CommonText{
                id: inchannel3_2Name
                anchors.bottom: inchannel3_2VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.left: parent.left
                anchors.leftMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_CHANNEL") + "3.2"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
            CommonText{
                id: inchannel3_2Data
                anchors.bottom: inchannel3_2VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.right: parent.right
                anchors.rightMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: inchannel3_2VolumeSlider.getValue() + "dB"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
            }
        }

        StripFrame{
            id: inchannel4_1
            anchors.top: inchannel3_2.bottom
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: true
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonSlider{
                id: inchannel4_1VolumeSlider
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 10
                width: 400
                height: 60
                stepSize: 1
                maximumValue: 6
                minimumValue: -80
                enabled: false

                onPressedChanged: {
                    if (pressed == true)
                    {
                        inchannel4_1Timer.setTmpValue(inchannel4_1VolumeSlider.getValue());
                        inchannel4_1Timer.start();
                    }
                    else
                    {
                        inchannel4_1Timer.stop();
                        sourceManageObject.volumeControl(getGNode(), 0, 44, 1, inchannel4_1VolumeSlider.getValue());
                    }
                }

                Timer{

                    property real tmpValue: 0
                    function setTmpValue(tmp)
                    {
                        tmpValue = tmp;
                    }

                    id: inchannel4_1Timer
                    interval: 200
                    repeat: true
                    triggeredOnStart: true
                    onTriggered: {
                        if (tmpValue !== inchannel4_1VolumeSlider.getValue())
                        {
                            sourceManageObject.volumeControl(getGNode(), 0, 44, 1, inchannel4_1VolumeSlider.getValue());
                            tmpValue = inchannel4_1VolumeSlider.getValue();
                        }
                    }
                }
            }
            CommonText{
                id: inchannel4_1Name
                anchors.bottom: inchannel4_1VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.left: parent.left
                anchors.leftMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_CHANNEL") + "4.1"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
            CommonText{
                id: inchannel4_1Data
                anchors.bottom: inchannel4_1VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.right: parent.right
                anchors.rightMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: inchannel4_1VolumeSlider.getValue() + "dB"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
            }
        }

        StripFrame{
            id: inchannel4_2
            anchors.top: inchannel4_1.bottom
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: false
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonSlider{
                id: inchannel4_2VolumeSlider
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 10
                width: 400
                height: 60
                stepSize: 1
                maximumValue: 6
                minimumValue: -80
                enabled: false

                onPressedChanged: {
                    if (pressed == true)
                    {
                        inchannel4_2Timer.setTmpValue(inchannel4_2VolumeSlider.getValue());
                        inchannel4_2Timer.start();
                    }
                    else
                    {
                        inchannel4_2Timer.stop();
                        sourceManageObject.volumeControl(getGNode(), 0, 44, 2, inchannel4_2VolumeSlider.getValue());
                    }
                }

                Timer{

                    property real tmpValue: 0
                    function setTmpValue(tmp)
                    {
                        tmpValue = tmp;
                    }

                    id: inchannel4_2Timer
                    interval: 200
                    repeat: true
                    triggeredOnStart: true
                    onTriggered: {
                        if (tmpValue !== inchannel4_2VolumeSlider.getValue())
                        {
                            sourceManageObject.volumeControl(getGNode(), 0, 44, 2, inchannel4_2VolumeSlider.getValue());
                            tmpValue = inchannel4_2VolumeSlider.getValue();
                        }
                    }
                }
            }
            CommonText{
                id: inchannel4_2Name
                anchors.bottom: inchannel4_2VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.left: parent.left
                anchors.leftMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_CHANNEL") + "4.2"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
            CommonText{
                id: inchannel4_2Data
                anchors.bottom: inchannel4_2VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.right: parent.right
                anchors.rightMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: inchannel4_2VolumeSlider.getValue() + "dB"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
            }
        }

        StripFrame{
            id: dalOutput
            anchors.top: inchannel4_2.bottom
            anchors.topMargin: 25
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: true
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonText{
                id: dalOutputName
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 25
                width: 400
                font.pixelSize: 18
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_DAL_OUTPUT")
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
        }

        StripFrame{
            id: outchannel1_1
            anchors.top: dalOutput.bottom
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: true
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonSlider{
                id: outchannel1_1VolumeSlider
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 10
                width: 400
                height: 60
                stepSize: 1
                maximumValue: 6
                minimumValue: -80
                enabled: false

                onPressedChanged: {
                    if (pressed == true)
                    {
                        outchannel1_1Timer.setTmpValue(outchannel1_1VolumeSlider.getValue());
                        outchannel1_1Timer.start();
                    }
                    else
                    {
                        outchannel1_1Timer.stop();
                        sourceManageObject.volumeControl(getGNode(), 2, 41, 1, outchannel1_1VolumeSlider.getValue());
                    }
                }

                Timer{

                    property real tmpValue: 0
                    function setTmpValue(tmp)
                    {
                        tmpValue = tmp;
                    }

                    id: outchannel1_1Timer
                    interval: 200
                    repeat: true
                    triggeredOnStart: true
                    onTriggered: {
                        if (tmpValue !== outchannel1_1VolumeSlider.getValue())
                        {
                            sourceManageObject.volumeControl(getGNode(), 2, 41, 1, outchannel1_1VolumeSlider.getValue());
                            tmpValue = outchannel1_1VolumeSlider.getValue();
                        }
                    }
                }
            }
            CommonText{
                id: outchannel1_1Name
                anchors.bottom: outchannel1_1VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.left: parent.left
                anchors.leftMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_CHANNEL") + "1.1"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
            CommonText{
                id: outchannel1_1Data
                anchors.bottom: outchannel1_1VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.right: parent.right
                anchors.rightMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: outchannel1_1VolumeSlider.getValue() + "dB"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
            }
        }

        StripFrame{
            id: outchannel1_2
            anchors.top: outchannel1_1.bottom
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: true
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonSlider{
                id: outchannel1_2VolumeSlider
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 10
                width: 400
                height: 60
                stepSize: 1
                maximumValue: 6
                minimumValue: -80
                enabled: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        outchannel1_2Timer.setTmpValue(outchannel1_2VolumeSlider.getValue());
                        outchannel1_2Timer.start();
                    }
                    else
                    {
                        outchannel1_2Timer.stop();
                        sourceManageObject.volumeControl(getGNode(), 2, 42, 1, outchannel1_2VolumeSlider.getValue());
                    }
                }

                Timer{

                    property real tmpValue: 0
                    function setTmpValue(tmp)
                    {
                        tmpValue = tmp;
                    }

                    id: outchannel1_2Timer
                    interval: 200
                    repeat: true
                    triggeredOnStart: true
                    onTriggered: {
                        if (tmpValue !== outchannel1_2VolumeSlider.getValue())
                        {
                            sourceManageObject.volumeControl(getGNode(), 2, 42, 1, outchannel1_2VolumeSlider.getValue());
                            tmpValue = outchannel1_2VolumeSlider.getValue();
                        }
                    }
                }
            }
            CommonText{
                id: outchannel1_2Name
                anchors.bottom: outchannel1_2VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.left: parent.left
                anchors.leftMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_CHANNEL") + "1.2"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
            CommonText{
                id: outchannel1_2Data
                anchors.bottom: outchannel1_2VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.right: parent.right
                anchors.rightMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: outchannel1_2VolumeSlider.getValue() + "dB"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
            }
        }

        StripFrame{
            id: outchannel2_1
            anchors.top: outchannel1_2.bottom
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: true
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonSlider{
                id: outchannel2_1VolumeSlider
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 10
                width: 400
                height: 60
                stepSize: 1
                maximumValue: 6
                minimumValue: -80
                enabled: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        outchannel2_1Timer.setTmpValue(outchannel2_1VolumeSlider.getValue());
                        outchannel2_1Timer.start();
                    }
                    else
                    {
                        outchannel2_1Timer.stop();
                        sourceManageObject.volumeControl(getGNode(), 2, 43, 1, outchannel2_1VolumeSlider.getValue());
                    }
                }

                Timer{

                    property real tmpValue: 0
                    function setTmpValue(tmp)
                    {
                        tmpValue = tmp;
                    }

                    id: outchannel2_1Timer
                    interval: 200
                    repeat: true
                    triggeredOnStart: true
                    onTriggered: {
                        if (tmpValue !== outchannel2_1VolumeSlider.getValue())
                        {
                            sourceManageObject.volumeControl(getGNode(), 2, 43, 1, outchannel2_1VolumeSlider.getValue());
                            tmpValue = outchannel2_1VolumeSlider.getValue();
                        }
                    }
                }
            }
            CommonText{
                id: outchannel2_1Name
                anchors.bottom: outchannel2_1VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.left: parent.left
                anchors.leftMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_CHANNEL") + "2.1"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
            CommonText{
                id: outchannel2_1Data
                anchors.bottom: outchannel2_1VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.right: parent.right
                anchors.rightMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: outchannel2_1VolumeSlider.getValue() + "dB"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
            }
        }

        StripFrame{
            id: outchannel2_2
            anchors.top: outchannel2_1.bottom
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: true
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonSlider{
                id: outchannel2_2VolumeSlider
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 10
                width: 400
                height: 60
                stepSize: 1
                maximumValue: 6
                minimumValue: -80
                enabled: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        outchannel2_2Timer.setTmpValue(outchannel2_2VolumeSlider.getValue());
                        outchannel2_2Timer.start();
                    }
                    else
                    {
                        outchannel2_2Timer.stop();
                        sourceManageObject.volumeControl(getGNode(), 2, 44, 1, outchannel2_2VolumeSlider.getValue());
                    }
                }

                Timer{

                    property real tmpValue: 0
                    function setTmpValue(tmp)
                    {
                        tmpValue = tmp;
                    }

                    id: outchannel2_2Timer
                    interval: 200
                    repeat: true
                    triggeredOnStart: true
                    onTriggered: {
                        if (tmpValue !== outchannel2_2VolumeSlider.getValue())
                        {
                            sourceManageObject.volumeControl(getGNode(), 2, 44, 1, outchannel2_2VolumeSlider.getValue());
                            tmpValue = outchannel2_2VolumeSlider.getValue();
                        }
                    }
                }
            }
            CommonText{
                id: outchannel2_2Name
                anchors.bottom: outchannel2_2VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.left: parent.left
                anchors.leftMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_CHANNEL") + "2.2"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
            CommonText{
                id: outchannel2_2Data
                anchors.bottom: outchannel2_2VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.right: parent.right
                anchors.rightMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: outchannel2_2VolumeSlider.getValue() + "dB"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
            }
        }

        StripFrame{
            id: outchannel3_1
            anchors.top: outchannel2_2.bottom
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: true
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonSlider{
                id: outchannel3_1VolumeSlider
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 10
                width: 400
                height: 60
                stepSize: 1
                maximumValue: 6
                minimumValue: -80
                enabled: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        outchannel3_1Timer.setTmpValue(outchannel3_1VolumeSlider.getValue());
                        outchannel3_1Timer.start();
                    }
                    else
                    {
                        outchannel3_1Timer.stop();
                        sourceManageObject.volumeControl(getGNode(), 2, 45, 1, outchannel3_1VolumeSlider.getValue());
                    }
                }

                Timer{

                    property real tmpValue: 0
                    function setTmpValue(tmp)
                    {
                        tmpValue = tmp;
                    }

                    id: outchannel3_1Timer
                    interval: 200
                    repeat: true
                    triggeredOnStart: true
                    onTriggered: {
                        if (tmpValue !== outchannel3_1VolumeSlider.getValue())
                        {
                            sourceManageObject.volumeControl(getGNode(), 2, 45, 1, outchannel3_1VolumeSlider.getValue());
                            tmpValue = outchannel3_1VolumeSlider.getValue();
                        }
                    }
                }
            }
            CommonText{
                id: outchannel3_1Name
                anchors.bottom: outchannel3_1VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.left: parent.left
                anchors.leftMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_CHANNEL") + "3.1"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
            CommonText{
                id: outchannel3_1Data
                anchors.bottom: outchannel3_1VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.right: parent.right
                anchors.rightMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: outchannel3_1VolumeSlider.getValue() + "dB"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
            }
        }

        StripFrame{
            id: outchannel3_2
            anchors.top: outchannel3_1.bottom
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: true
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonSlider{
                id: outchannel3_2VolumeSlider
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 10
                width: 400
                height: 60
                stepSize: 1
                maximumValue: 6
                minimumValue: -80
                enabled: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        outchannel3_2Timer.setTmpValue(outchannel3_2VolumeSlider.getValue());
                        outchannel3_2Timer.start();
                    }
                    else
                    {
                        outchannel3_2Timer.stop();
                        sourceManageObject.volumeControl(getGNode(), 2, 46, 1, outchannel3_2VolumeSlider.getValue());
                    }
                }

                Timer{

                    property real tmpValue: 0
                    function setTmpValue(tmp)
                    {
                        tmpValue = tmp;
                    }

                    id: outchannel3_2Timer
                    interval: 200
                    repeat: true
                    triggeredOnStart: true
                    onTriggered: {
                        if (tmpValue !== outchannel3_2VolumeSlider.getValue())
                        {
                            sourceManageObject.volumeControl(getGNode(), 2, 46, 1, outchannel3_2VolumeSlider.getValue());
                            tmpValue = outchannel3_2VolumeSlider.getValue();
                        }
                    }
                }
            }
            CommonText{
                id: outchannel3_2Name
                anchors.bottom: outchannel3_2VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.left: parent.left
                anchors.leftMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_CHANNEL") + "3.2"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
            CommonText{
                id: outchannel3_2Data
                anchors.bottom: outchannel3_2VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.right: parent.right
                anchors.rightMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: outchannel3_2VolumeSlider.getValue() + "dB"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
            }
        }

        StripFrame{
            id: outchannel4_1
            anchors.top: outchannel3_2.bottom
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: true
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonSlider{
                id: outchannel4_1VolumeSlider
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 10
                width: 400
                height: 60
                stepSize: 1
                maximumValue: 6
                minimumValue: -80
                enabled: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        outchannel4_1Timer.setTmpValue(outchannel4_1VolumeSlider.getValue());
                        outchannel4_1Timer.start();
                    }
                    else
                    {
                        outchannel4_1Timer.stop();
                        sourceManageObject.volumeControl(getGNode(), 2, 47, 1, outchannel4_1VolumeSlider.getValue());
                    }
                }

                Timer{

                    property real tmpValue: 0
                    function setTmpValue(tmp)
                    {
                        tmpValue = tmp;
                    }

                    id: outchannel4_1Timer
                    interval: 200
                    repeat: true
                    triggeredOnStart: true
                    onTriggered: {
                        if (tmpValue !== outchannel4_1VolumeSlider.getValue())
                        {
                            sourceManageObject.volumeControl(getGNode(), 2, 47, 1, outchannel4_1VolumeSlider.getValue());
                            tmpValue = outchannel4_1VolumeSlider.getValue();
                        }
                    }
                }
            }
            CommonText{
                id: outchannel4_1Name
                anchors.bottom: outchannel4_1VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.left: parent.left
                anchors.leftMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_CHANNEL") + "4.1"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
            CommonText{
                id: outchannel4_1Data
                anchors.bottom: outchannel4_1VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.right: parent.right
                anchors.rightMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: outchannel4_1VolumeSlider.getValue() + "dB"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
            }
        }

        StripFrame{
            id: outchannel4_2
            anchors.top: outchannel4_1.bottom
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: false
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonSlider{
                id: outchannel4_2VolumeSlider
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 10
                width: 400
                height: 60
                stepSize: 1
                maximumValue: 6
                minimumValue: -80
                enabled: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        outchannel4_2Timer.setTmpValue(outchannel4_2VolumeSlider.getValue());
                        outchannel4_2Timer.start();
                    }
                    else
                    {
                        outchannel4_2Timer.stop();
                        sourceManageObject.volumeControl(getGNode(), 2, 48, 1, outchannel4_2VolumeSlider.getValue());
                    }
                }

                Timer{

                    property real tmpValue: 0
                    function setTmpValue(tmp)
                    {
                        tmpValue = tmp;
                    }

                    id: outchannel4_2Timer
                    interval: 200
                    repeat: true
                    triggeredOnStart: true
                    onTriggered: {
                        if (tmpValue !== outchannel4_2VolumeSlider.getValue())
                        {
                            sourceManageObject.volumeControl(getGNode(), 2, 48, 1, outchannel4_2VolumeSlider.getValue());
                            tmpValue = outchannel4_2VolumeSlider.getValue();
                        }
                    }
                }
            }
            CommonText{
                id: outchannel4_2Name
                anchors.bottom: outchannel4_2VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.left: parent.left
                anchors.leftMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_CHANNEL") + "4.2"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
            CommonText{
                id: outchannel4_2Data
                anchors.bottom: outchannel4_2VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.right: parent.right
                anchors.rightMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: outchannel4_2VolumeSlider.getValue() + "dB"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
            }
        }

        StripFrame{
            id: amplifier
            anchors.top: outchannel4_2.bottom
            anchors.topMargin: 25
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: true
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonText{
                id: amplifierName
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 25
                width: 400
                font.pixelSize: 18
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_AMPLIFIER")
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
        }

        StripFrame{
            id: ampChannel1
            anchors.top: amplifier.bottom
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: true
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonSlider{
                id: ampChannel1VolumeSlider
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 10
                width: 400
                height: 60
                stepSize: 1
                maximumValue: 6
                minimumValue: -80
                enabled: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        ampChannel1Timer.setTmpValue(ampChannel1VolumeSlider.getValue());
                        ampChannel1Timer.start();
                    }
                    else
                    {
                        ampChannel1Timer.stop();
                        sourceManageObject.volumeControl(getGNode(), 2, 1, 1, ampChannel1VolumeSlider.getValue());
                    }
                }

                Timer{

                    property real tmpValue: 0
                    function setTmpValue(tmp)
                    {
                        tmpValue = tmp;
                    }

                    id: ampChannel1Timer
                    interval: 200
                    repeat: true
                    triggeredOnStart: true
                    onTriggered: {
                        if (tmpValue !== ampChannel1VolumeSlider.getValue())
                        {
                            sourceManageObject.volumeControl(getGNode(), 2, 1, 1, ampChannel1VolumeSlider.getValue());
                            tmpValue = ampChannel1VolumeSlider.getValue();
                        }
                    }
                }
            }
            CommonText{
                id: ampChannel1Name
                anchors.bottom: ampChannel1VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.left: parent.left
                anchors.leftMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_CHANNEL") + "1"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
            CommonText{
                id: ampChannel1Data
                anchors.bottom: ampChannel1VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.right: parent.right
                anchors.rightMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: ampChannel1VolumeSlider.getValue() + "dB"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
            }
        }

        StripFrame{
            id: ampChannel2
            anchors.top: ampChannel1.bottom
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: true
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonSlider{
                id: ampChannel2VolumeSlider
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 10
                width: 400
                height: 60
                stepSize: 1
                maximumValue: 6
                minimumValue: -80
                enabled: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        ampChannel2Timer.setTmpValue(ampChannel2VolumeSlider.getValue());
                        ampChannel2Timer.start();
                    }
                    else
                    {
                        ampChannel2Timer.stop();
                        sourceManageObject.volumeControl(getGNode(), 2, 2, 1, ampChannel2VolumeSlider.getValue());
                    }
                }

                Timer{

                    property real tmpValue: 0
                    function setTmpValue(tmp)
                    {
                        tmpValue = tmp;
                    }

                    id: ampChannel2Timer
                    interval: 200
                    repeat: true
                    triggeredOnStart: true
                    onTriggered: {
                        if (tmpValue !== ampChannel2VolumeSlider.getValue())
                        {
                            sourceManageObject.volumeControl(getGNode(), 2, 2, 1, ampChannel2VolumeSlider.getValue());
                            tmpValue = ampChannel2VolumeSlider.getValue();
                        }
                    }
                }
            }
            CommonText{
                id: ampChannel2Name
                anchors.bottom: ampChannel2VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.left: parent.left
                anchors.leftMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_CHANNEL") + "2"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
            CommonText{
                id: ampChannel2Data
                anchors.bottom: ampChannel2VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.right: parent.right
                anchors.rightMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: ampChannel2VolumeSlider.getValue() + "dB"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
            }
        }

        StripFrame{
            id: ampChannel3
            anchors.top: ampChannel2.bottom
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: true
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonSlider{
                id: ampChannel3VolumeSlider
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 10
                width: 400
                height: 60
                stepSize: 1
                maximumValue: 6
                minimumValue: -80
                enabled: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        ampChannel3Timer.setTmpValue(ampChannel3VolumeSlider.getValue());
                        ampChannel3Timer.start();
                    }
                    else
                    {
                        ampChannel3Timer.stop();
                        sourceManageObject.volumeControl(getGNode(), 2, 3, 1, ampChannel3VolumeSlider.getValue());
                    }
                }

                Timer{

                    property real tmpValue: 0
                    function setTmpValue(tmp)
                    {
                        tmpValue = tmp;
                    }

                    id: ampChannel3Timer
                    interval: 200
                    repeat: true
                    triggeredOnStart: true
                    onTriggered: {
                        if (tmpValue !== ampChannel3VolumeSlider.getValue())
                        {
                            sourceManageObject.volumeControl(getGNode(), 2, 3, 1, ampChannel3VolumeSlider.getValue());
                            tmpValue = ampChannel3VolumeSlider.getValue();
                        }
                    }
                }
            }

            CommonText{
                id: ampChannel3Name
                anchors.bottom: ampChannel3VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.left: parent.left
                anchors.leftMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_CHANNEL") + "3"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
            CommonText{
                id: ampChannel3Data
                anchors.bottom: ampChannel3VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.right: parent.right
                anchors.rightMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: ampChannel3VolumeSlider.getValue() + "dB"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
            }
        }

        StripFrame{
            id: ampChannel4
            anchors.top: ampChannel3.bottom
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: true
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonSlider{
                id: ampChannel4VolumeSlider
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 10
                width: 400
                height: 60
                stepSize: 1
                maximumValue: 6
                minimumValue: -80
                enabled: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        ampChannel4Timer.setTmpValue(ampChannel4VolumeSlider.getValue());
                        ampChannel4Timer.start();
                    }
                    else
                    {
                        ampChannel4Timer.stop();
                        sourceManageObject.volumeControl(getGNode(), 2, 4, 1, ampChannel4VolumeSlider.getValue());
                    }
                }

                Timer{

                    property real tmpValue: 0
                    function setTmpValue(tmp)
                    {
                        tmpValue = tmp;
                    }

                    id: ampChannel4Timer
                    interval: 200
                    repeat: true
                    triggeredOnStart: true
                    onTriggered: {
                        if (tmpValue !== ampChannel4VolumeSlider.getValue())
                        {
                            sourceManageObject.volumeControl(getGNode(), 2, 4, 1, ampChannel4VolumeSlider.getValue());
                            tmpValue = ampChannel4VolumeSlider.getValue();
                        }
                    }
                }
            }
            CommonText{
                id: ampChannel4Name
                anchors.bottom: ampChannel4VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.left: parent.left
                anchors.leftMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_CHANNEL") + "4"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
            CommonText{
                id: ampChannel4Data
                anchors.bottom: ampChannel4VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.right: parent.right
                anchors.rightMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: ampChannel4VolumeSlider.getValue() + "dB"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
            }
        }

        StripFrame{
            id: miscellaneous
            anchors.top: ampChannel4.bottom
            anchors.topMargin: 25
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: true
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonText{
                id: miscellaneousName
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 25
                width: 400
                font.pixelSize: 18
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_MESCELLANEOUS")
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
        }

        StripFrame{
            id: signalGenerator1_1
            anchors.top: miscellaneous.bottom
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: true
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonSlider{
                id: signalGenerator1_1VolumeSlider
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 10
                width: 400
                height: 60
                stepSize: 1
                maximumValue: 6
                minimumValue: -80
                enabled: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        signalGenerator1_1Timer.setTmpValue(signalGenerator1_1VolumeSlider.getValue());
                        signalGenerator1_1Timer.start();
                    }
                    else
                    {
                        signalGenerator1_1Timer.stop();
                        sourceManageObject.volumeControl(getGNode(), 4, 1, 1, signalGenerator1_1VolumeSlider.getValue());
                    }
                }

                Timer{

                    property real tmpValue: 0
                    function setTmpValue(tmp)
                    {
                        tmpValue = tmp;
                    }

                    id: signalGenerator1_1Timer
                    interval: 200
                    repeat: true
                    triggeredOnStart: true
                    onTriggered: {
                        if (tmpValue !== signalGenerator1_1VolumeSlider.getValue())
                        {
                            sourceManageObject.volumeControl(getGNode(), 4, 1, 1, signalGenerator1_1VolumeSlider.getValue());
                            tmpValue = signalGenerator1_1VolumeSlider.getValue();
                        }
                    }
                }
            }
            CommonText{
                id: signalGenerator1_1Name
                anchors.bottom: signalGenerator1_1VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.left: parent.left
                anchors.leftMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_SIGNAL_GENERATOR")
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
            CommonText{
                id: signalGenerator1_1Data
                anchors.bottom: signalGenerator1_1VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.right: parent.right
                anchors.rightMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: signalGenerator1_1VolumeSlider.getValue() + "dB"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
            }
        }

        /*
        StripFrame{
            id: signalGenerator2_1
            anchors.top: signalGenerator1_1.bottom
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: true
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonSlider{
                id: signalGenerator2_1VolumeSlider
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 10
                width: 400
                height: 60
                stepSize: 1
                maximumValue: 6
                minimumValue: -80
                enabled: false
                onValueChanged: {
                    sourceManageObject.volumeControl(getGNode(), 4, 2, 1, signalGenerator2_1VolumeSlider.getValue());
                }
            }
            CommonText{
                id: signalGenerator2_1Name
                anchors.bottom: signalGenerator2_1VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.left: parent.left
                anchors.leftMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: langStrObject.GetLangString("Signal generator 2.1")
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
            CommonText{
                id: signalGenerator2_1Data
                anchors.bottom: signalGenerator2_1VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.right: parent.right
                anchors.rightMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: signalGenerator2_1VolumeSlider.getValue() + "dB"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
            }
        }

        StripFrame{
            id: signalGenerator3_1
            anchors.top: signalGenerator2_1.bottom
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: true
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonSlider{
                id: signalGenerator3_1VolumeSlider
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 10
                width: 400
                height: 60
                stepSize: 1
                maximumValue: 6
                minimumValue: -80
                enabled: false
                onValueChanged: {
                    sourceManageObject.volumeControl(getGNode(), 4, 3, 1, signalGenerator3_1VolumeSlider.getValue());
                }
            }
            CommonText{
                id: signalGenerator3_1Name
                anchors.bottom: signalGenerator3_1VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.left: parent.left
                anchors.leftMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: langStrObject.GetLangString("Signal generator 3.1")
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
            CommonText{
                id: signalGenerator3_1Data
                anchors.bottom: signalGenerator3_1VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.right: parent.right
                anchors.rightMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: signalGenerator3_1VolumeSlider.getValue() + "dB"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
            }
        }

        StripFrame{
            id: signalGenerator4_1
            anchors.top: signalGenerator3_1.bottom
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: true
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonSlider{
                id: signalGenerator4_1VolumeSlider
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 10
                width: 400
                height: 60
                stepSize: 1
                maximumValue: 6
                minimumValue: -80
                enabled: false
                onValueChanged: {
                    sourceManageObject.volumeControl(getGNode(), 4, 4, 1, signalGenerator4_1VolumeSlider.getValue());
                }
            }
            CommonText{
                id: signalGenerator4_1Name
                anchors.bottom: signalGenerator4_1VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.left: parent.left
                anchors.leftMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: langStrObject.GetLangString("Signal generator 4.1")
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
            CommonText{
                id: signalGenerator4_1Data
                anchors.bottom: signalGenerator4_1VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.right: parent.right
                anchors.rightMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: signalGenerator4_1VolumeSlider.getValue() + "dB"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
            }
        }
        */

        StripFrame{
            id: avcInput
            anchors.top: signalGenerator1_1.bottom
            anchors.topMargin: 25
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: true
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonText{
                id: avcName
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 25
                width: 400
                font.pixelSize: 18
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_AVC_INPUT")
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
        }

        StripFrame{
            id: input1
            anchors.top: avcInput.bottom
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: true
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonSlider{
                id: input1VolumeSlider
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 10
                width: 400
                height: 60
                stepSize: 1
                maximumValue: 6
                minimumValue: -80
                enabled: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        input1Timer.setTmpValue(input1VolumeSlider.getValue());
                        input1Timer.start();
                    }
                    else
                    {
                        input1Timer.stop();
                        sourceManageObject.volumeControl(getGNode(), 0, 1, 1, input1VolumeSlider.getValue());
                    }
                }

                Timer{

                    property real tmpValue: 0
                    function setTmpValue(tmp)
                    {
                        tmpValue = tmp;
                    }

                    id: input1Timer
                    interval: 200
                    repeat: true
                    triggeredOnStart: true
                    onTriggered: {
                        if (tmpValue !== input1VolumeSlider.getValue())
                        {
                            sourceManageObject.volumeControl(getGNode(), 0, 1, 1, input1VolumeSlider.getValue());
                            tmpValue = input1VolumeSlider.getValue();
                        }
                    }
                }
            }
            CommonText{
                id: input1Name
                anchors.bottom: input1VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.left: parent.left
                anchors.leftMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_AVC_INPUT") + "1"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
            CommonText{
                id: input1Data
                anchors.bottom: input1VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.right: parent.right
                anchors.rightMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: input1VolumeSlider.getValue() + "dB"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
            }
        }

        StripFrame{
            id: input2
            anchors.top: input1.bottom
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: true
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonSlider{
                id: input2VolumeSlider
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 10
                width: 400
                height: 60
                stepSize: 1
                maximumValue: 6
                minimumValue: -80
                enabled: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        input2Timer.setTmpValue(input2VolumeSlider.getValue());
                        input2Timer.start();
                    }
                    else
                    {
                        input2Timer.stop();
                        sourceManageObject.volumeControl(getGNode(), 0, 1, 2, input2VolumeSlider.getValue());
                    }
                }

                Timer{

                    property real tmpValue: 0
                    function setTmpValue(tmp)
                    {
                        tmpValue = tmp;
                    }

                    id: input2Timer
                    interval: 200
                    repeat: true
                    triggeredOnStart: true
                    onTriggered: {
                        if (tmpValue !== input2VolumeSlider.getValue())
                        {
                            sourceManageObject.volumeControl(getGNode(), 0, 1, 2, input2VolumeSlider.getValue());
                            tmpValue = input2VolumeSlider.getValue();
                        }
                    }
                }
            }
            CommonText{
                id: input2Name
                anchors.bottom: input2VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.left: parent.left
                anchors.leftMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_AVC_INPUT") + "2"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
            CommonText{
                id: input2Data
                anchors.bottom: input2VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.right: parent.right
                anchors.rightMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: input2VolumeSlider.getValue() + "dB"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
            }
        }

        StripFrame{
            id: input3
            anchors.top: input2.bottom
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: true
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonSlider{
                id: input3VolumeSlider
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 10
                width: 400
                height: 60
                stepSize: 1
                maximumValue: 6
                minimumValue: -80
                enabled: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        input3Timer.setTmpValue(input3VolumeSlider.getValue());
                        input3Timer.start();
                    }
                    else
                    {
                        input3Timer.stop();
                        sourceManageObject.volumeControl(getGNode(), 0, 2, 1, input3VolumeSlider.getValue());
                    }
                }

                Timer{

                    property real tmpValue: 0
                    function setTmpValue(tmp)
                    {
                        tmpValue = tmp;
                    }

                    id: input3Timer
                    interval: 200
                    repeat: true
                    triggeredOnStart: true
                    onTriggered: {
                        if (tmpValue !== input3VolumeSlider.getValue())
                        {
                            sourceManageObject.volumeControl(getGNode(), 0, 2, 1, input3VolumeSlider.getValue());
                            tmpValue = input3VolumeSlider.getValue();
                        }
                    }
                }
            }
            CommonText{
                id: input3Name
                anchors.bottom: input3VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.left: parent.left
                anchors.leftMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_AVC_INPUT") + "3"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
            CommonText{
                id: input3Data
                anchors.bottom: input3VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.right: parent.right
                anchors.rightMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: input3VolumeSlider.getValue() + "dB"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
            }
        }

        StripFrame{
            id: input4
            anchors.top: input3.bottom
            anchors.left: parent.left
            width: parent.width
            height: 65
            line1Visible: false
            line2Visible: true
            bkSwitch: false
            bkgPressedColor: "#dbe0e6"
            bkgReleasedColor: "#ffffff"
            CommonSlider{
                id: input4VolumeSlider
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 10
                width: 400
                height: 60
                stepSize: 1
                maximumValue: 6
                minimumValue: -80
                enabled: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        input4Timer.setTmpValue(input4VolumeSlider.getValue());
                        input4Timer.start();
                    }
                    else
                    {
                        input4Timer.stop();
                        sourceManageObject.volumeControl(getGNode(), 0, 2, 2, input4VolumeSlider.getValue());
                    }
                }

                Timer{

                    property real tmpValue: 0
                    function setTmpValue(tmp)
                    {
                        tmpValue = tmp;
                    }

                    id: input4Timer
                    interval: 200
                    repeat: true
                    triggeredOnStart: true
                    onTriggered: {
                        if (tmpValue !== input4VolumeSlider.getValue())
                        {
                            sourceManageObject.volumeControl(getGNode(), 0, 2, 2, input4VolumeSlider.getValue());
                            tmpValue = input4VolumeSlider.getValue();
                        }
                    }
                }
            }
            CommonText{
                id: input4Name
                anchors.bottom: input4VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.left: parent.left
                anchors.leftMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: langStrObject.GetLangString("ETCS_AVC_INPUT") + "4"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
            }
            CommonText{
                id: input4Data
                anchors.bottom: input4VolumeSlider.top
                anchors.bottomMargin: -15
                anchors.right: parent.right
                anchors.rightMargin: 40
                width: 400
                font.pixelSize: 14
                color: "#303030"
                text: input4VolumeSlider.getValue() + "dB"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
            }
        }
    }

    ScrollBar{
        id: scrollBar
        anchors.top: nodeListView.top
        anchors.right: nodeListView.right
        anchors.rightMargin: 2
        height: nodeListView.height
        position: nodeListView.visibleArea.yPosition
        pageSize: nodeListView.visibleArea.heightRatio
        scrollBarVisible: nodeListView.visibleArea.heightRatio == 1 ? false : true
        z: 3
    }

    Component.onCompleted: {
        rootItem.updateLanguage.connect(updateLanguageSlot);
        logicObject.setNodeVolListSignal.connect(nodeListView.updateVolListSlot);
        logicObject.updateVolumeResultSignal.connect(updateVolumeResultSlot);
    }
}
