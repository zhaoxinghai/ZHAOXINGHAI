import QtQuick 2.0
import "../Components"

Rectangle {

    function updateLanguageSlot()
    {
        title.text = langStrObject.GetLangString("ETCS_LOCAL_VOLUME");
        prevTitleName.text = langStrObject.GetLangString("ETCS_NODE");
        inputName.setText(langStrObject.GetLangString("ETCS_INPUT"));
        microphoneName.setText(langStrObject.GetLangString("ETCS_MIC"));
        lineinName.setText(langStrObject.GetLangString("ETCS_LINEIN"));
        outputName.setText(langStrObject.GetLangString("ETCS_OUTPUT"));
        speakerName.setText(langStrObject.GetLangString("ETCS_SPEAKER"));
        lineoutName.setText(langStrObject.GetLangString("ETCS_LINEOUT"));
        miscellaneousName.setText(langStrObject.GetLangString("ETCS_MESCELLANEOUS"));
        signalGeneratorName.setText(langStrObject.GetLangString("ETCS_SIGNAL_GENERATOR"));
    }

    function initStatus()
    {
        setVisibleValue(true);
    }

    function quitStatus()
    {
        setVisibleValue(false);
        setAllSliderDiable();
        setAllSliderInitStatus();
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function setAllSliderDiable()
    {
        microphoneVolumeSlider.disableSlider();
        lineinVolumeSlider.disableSlider();
        speakerVolumeSlider.disableSlider();
        lineoutVolumeSlider.disableSlider();
        signalGeneratorVolumeSlider.disableSlider();
    }

    function setAllSliderInitStatus()
    {
        microphoneData.setFontStrikeOutValue(false);
        microphoneData.setTextColor("#303030");
        lineinData.setFontStrikeOutValue(false);
        lineinData.setTextColor("#303030");
        speakerData.setFontStrikeOutValue(false);
        speakerData.setTextColor("#303030");
        lineoutData.setFontStrikeOutValue(false);
        lineoutData.setTextColor("#303030");
        signalGeneratorData.setFontStrikeOutValue(false);
        signalGeneratorData.setTextColor("#303030");
    }

    function updateVolListSlot()
    {
        var volDB;
        var min = 0.0;
        var max = 0.0;

        setAllSliderDiable();
        setAllSliderInitStatus();

        volDB = sourceManageObject.getVolumeFromNodeDev(localConfigObject.getLocalNode(), 0, 41, 1);
        if (volDB !== -1000)
        {
            min = sourceManageObject.getMinVolumeFromNodeDev(localConfigObject.getLocalNode(), 0, 41, 1);
            max = sourceManageObject.getMaxVolumeFromNodeDev(localConfigObject.getLocalNode(), 0, 41, 1);
            microphoneVolumeSlider.setMinimumValue(min);
            microphoneVolumeSlider.setMaximumValue(max);
            microphoneVolumeSlider.setValue(volDB);
            microphoneVolumeSlider.enableSlider();
        }

        volDB = sourceManageObject.getVolumeFromNodeDev(localConfigObject.getLocalNode(), 0, 41, 2);
        if (volDB !== -1000)
        {
            min = sourceManageObject.getMinVolumeFromNodeDev(localConfigObject.getLocalNode(), 0, 41, 2);
            max = sourceManageObject.getMaxVolumeFromNodeDev(localConfigObject.getLocalNode(), 0, 41, 2);
            lineinVolumeSlider.setMinimumValue(min);
            lineinVolumeSlider.setMaximumValue(max);
            lineinVolumeSlider.setValue(volDB);
            lineinVolumeSlider.enableSlider();
        }

        volDB = sourceManageObject.getVolumeFromNodeDev(localConfigObject.getLocalNode(), 2, 41, 1);
        if (volDB !== -1000)
        {
            min = sourceManageObject.getMinVolumeFromNodeDev(localConfigObject.getLocalNode(), 2, 41, 1);
            max = sourceManageObject.getMaxVolumeFromNodeDev(localConfigObject.getLocalNode(), 2, 41, 1);
            speakerVolumeSlider.setMinimumValue(min);
            speakerVolumeSlider.setMaximumValue(max);
            speakerVolumeSlider.setValue(volDB);
            speakerVolumeSlider.enableSlider();
        }

        volDB = sourceManageObject.getVolumeFromNodeDev(localConfigObject.getLocalNode(), 2, 42, 1);
        if (volDB !== -1000)
        {
            min = sourceManageObject.getMinVolumeFromNodeDev(localConfigObject.getLocalNode(), 2, 42, 1);
            max = sourceManageObject.getMaxVolumeFromNodeDev(localConfigObject.getLocalNode(), 2, 42, 1);
            lineoutVolumeSlider.setMinimumValue(min);
            lineoutVolumeSlider.setMaximumValue(max);
            lineoutVolumeSlider.setValue(volDB);
            lineoutVolumeSlider.enableSlider();
        }

        volDB = sourceManageObject.getVolumeFromNodeDev(localConfigObject.getLocalNode(), 4, 1, 1);
        if (volDB !== -1000)
        {
            min = sourceManageObject.getMinVolumeFromNodeDev(localConfigObject.getLocalNode(), 4, 1, 1);
            max = sourceManageObject.getMaxVolumeFromNodeDev(localConfigObject.getLocalNode(), 4, 1, 1);
            signalGeneratorVolumeSlider.setMinimumValue(min);
            signalGeneratorVolumeSlider.setMaximumValue(max);
            signalGeneratorVolumeSlider.setValue(volDB);
            signalGeneratorVolumeSlider.enableSlider();
        }
    }

    function updateVolumeResultSlot(node, type, number, channel, errorCode)
    {
        if (node !== localConfigObject.getLocalNode())
        {
            return;
        }

        if (type === 0)
        {
            if ((number === 41) &&  (channel === 1))
            {
                if (errorCode === 0)
                {
                    microphoneData.setFontStrikeOutValue(false);
                    microphoneData.setTextColor("#303030");
                }
                else
                {
                    microphoneData.setFontStrikeOutValue(true);
                    microphoneData.setTextColor("red");
                }
            }
            else if ((number === 41) &&  (channel === 2))
            {
                if (errorCode === 0)
                {
                    lineinData.setFontStrikeOutValue(false);
                    lineinData.setTextColor("#303030");
                }
                else
                {
                    lineinData.setFontStrikeOutValue(true);
                    lineinData.setTextColor("red");
                }
            }
        }
        else if (type === 2)
        {
            if ((number === 41) &&  (channel === 1))
            {
                if (errorCode === 0)
                {
                    speakerData.setFontStrikeOutValue(false);
                    speakerData.setTextColor("#303030");
                }
                else
                {
                    speakerData.setFontStrikeOutValue(true);
                    speakerData.setTextColor("red");
                }
            }
            else if ((number === 42) &&  (channel === 1))
            {
                if (errorCode === 0)
                {
                    lineoutData.setFontStrikeOutValue(false);
                    lineoutData.setTextColor("#303030");
                }
                else
                {
                    lineoutData.setFontStrikeOutValue(true);
                    lineoutData.setTextColor("red");
                }
            }
        }
        else if (type === 4)
        {
            if ((number === 1) &&  (channel === 1))
            {
                if (errorCode === 0)
                {
                    signalGeneratorData.setFontStrikeOutValue(false);
                    signalGeneratorData.setTextColor("#303030");
                }
                else
                {
                    signalGeneratorData.setFontStrikeOutValue(true);
                    signalGeneratorData.setTextColor("red");
                }
            }
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
            text: langStrObject.GetLangString("ETCS_LOCAL_VOLUME")
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

    StripFrame{
        id: input
        anchors.top: titleBar.bottom
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
            id: inputName
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 25
            width: 400
            font.pixelSize: 18
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_INPUT")
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
        }
    }

    StripFrame{
        id: microphone
        anchors.top: input.bottom
        anchors.left: parent.left
        width: parent.width
        height: 65
        line1Visible: false
        line2Visible: true
        bkSwitch: false
        bkgPressedColor: "#dbe0e6"
        bkgReleasedColor: "#ffffff"
        CommonSlider{
            id: microphoneVolumeSlider
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
                sourceManageObject.volumeAjust(true, 41, microphoneVolumeSlider.getValue());
            }
        }
        CommonText{
            id: microphoneName
            anchors.bottom: microphoneVolumeSlider.top
            anchors.bottomMargin: -15
            anchors.left: parent.left
            anchors.leftMargin: 40
            width: 400
            font.pixelSize: 14
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_MIC")
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
        }
        CommonText{
            id: microphoneData
            anchors.bottom: microphoneVolumeSlider.top
            anchors.bottomMargin: -15
            anchors.right: parent.right
            anchors.rightMargin: 40
            width: 400
            font.pixelSize: 14
            color: "#303030"
            text: microphoneVolumeSlider.getValue() + "dB"
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignRight
        }
    }

    StripFrame{
        id: linein
        anchors.top: microphone.bottom
        anchors.left: parent.left
        width: parent.width
        height: 65
        line1Visible: false
        line2Visible: false
        bkSwitch: false
        bkgPressedColor: "#dbe0e6"
        bkgReleasedColor: "#ffffff"
        CommonSlider{
            id: lineinVolumeSlider
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
                sourceManageObject.volumeAjust(true, 42, lineinVolumeSlider.getValue());
            }
        }
        CommonText{
            id: lineinName
            anchors.bottom: lineinVolumeSlider.top
            anchors.bottomMargin: -15
            anchors.left: parent.left
            anchors.leftMargin: 40
            width: 400
            font.pixelSize: 14
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_LINEIN")
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
        }
        CommonText{
            id: lineinData
            anchors.bottom: lineinVolumeSlider.top
            anchors.bottomMargin: -15
            anchors.right: parent.right
            anchors.rightMargin: 40
            width: 400
            font.pixelSize: 14
            color: "#303030"
            text: lineinVolumeSlider.getValue() + "dB"
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignRight
        }
    }

    StripFrame{
        id: output
        anchors.top: linein.bottom
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
            id: outputName
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 25
            width: 400
            font.pixelSize: 18
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_OUTPUT")
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
        }
    }

    StripFrame{
        id: speaker
        anchors.top: output.bottom
        anchors.left: parent.left
        width: parent.width
        height: 65
        line1Visible: false
        line2Visible: true
        bkSwitch: false
        bkgPressedColor: "#dbe0e6"
        bkgReleasedColor: "#ffffff"
        CommonSlider{
            id: speakerVolumeSlider
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
                sourceManageObject.volumeAjust(false, 41, speakerVolumeSlider.getValue());
            }
        }
        CommonText{
            id: speakerName
            anchors.bottom: speakerVolumeSlider.top
            anchors.bottomMargin: -15
            anchors.left: parent.left
            anchors.leftMargin: 40
            width: 400
            font.pixelSize: 14
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_SPEAKER")
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
        }
        CommonText{
            id: speakerData
            anchors.bottom: speakerVolumeSlider.top
            anchors.bottomMargin: -15
            anchors.right: parent.right
            anchors.rightMargin: 40
            width: 400
            font.pixelSize: 14
            color: "#303030"
            text: speakerVolumeSlider.getValue() + "dB"
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignRight
        }
    }

    StripFrame{
        id: lineout
        anchors.top: speaker.bottom
        anchors.left: parent.left
        width: parent.width
        height: 65
        line1Visible: false
        line2Visible: false
        bkSwitch: false
        bkgPressedColor: "#dbe0e6"
        bkgReleasedColor: "#ffffff"
        CommonSlider{
            id: lineoutVolumeSlider
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
                sourceManageObject.volumeAjust(false, 42, lineoutVolumeSlider.getValue());
            }
        }
        CommonText{
            id: lineoutName
            anchors.bottom: lineoutVolumeSlider.top
            anchors.bottomMargin: -15
            anchors.left: parent.left
            anchors.leftMargin: 40
            width: 400
            font.pixelSize: 14
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_LINEOUT")
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
        }
        CommonText{
            id: lineoutData
            anchors.bottom: lineoutVolumeSlider.top
            anchors.bottomMargin: -15
            anchors.right: parent.right
            anchors.rightMargin: 40
            width: 400
            font.pixelSize: 14
            color: "#303030"
            text: lineoutVolumeSlider.getValue() + "dB"
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignRight
        }
    }

    StripFrame{
        id: miscellaneous
        anchors.top: lineout.bottom
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
        id: signalGenerator
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
            id: signalGeneratorVolumeSlider
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
                sourceManageObject.volumeSignalGenerator(signalGeneratorVolumeSlider.getValue());
            }
        }
        CommonText{
            id: signalGeneratorName
            anchors.bottom: signalGeneratorVolumeSlider.top
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
            id: signalGeneratorData
            anchors.bottom: signalGeneratorVolumeSlider.top
            anchors.bottomMargin: -15
            anchors.right: parent.right
            anchors.rightMargin: 40
            width: 400
            font.pixelSize: 14
            color: "#303030"
            text: signalGeneratorVolumeSlider.getValue() + "dB"
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignRight
        }
    }

    Component.onCompleted: {
        rootItem.updateLanguage.connect(updateLanguageSlot);
        logicObject.setNodeVolListSignal.connect(updateVolListSlot);
        logicObject.updateVolumeResultSignal.connect(updateVolumeResultSlot);
    }
}
