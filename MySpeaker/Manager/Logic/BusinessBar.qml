import QtQuick 2.0

import "../Components"

Item {

    property string index:""
    property int baseBtnWidth: 120
    property int baseBtnHeight: 86

    function updateLanguageSlot()
    {
        predefinedButton.textName = langStrObject.GetLangString("ETCS_PREDEFINE");
        manualButton.textName = langStrObject.GetLangString("ETCS_ZONE");
        scheduleButton.textName = langStrObject.GetLangString("ETCS_SCHEDULE");
        moreButton.textName = langStrObject.GetLangString("ETCS_MORE");
    }

    function setBusinessBarStatus(index)
    {
        if (index === "Predefined")
        {
            predefinedButton.setButtonClickedValue(true);
            manualButton.setButtonClickedValue(false);
            scheduleButton.setButtonClickedValue(false);
            moreButton.setButtonClickedValue(false);
        }
        else if(index === "Manual")
        {
            predefinedButton.setButtonClickedValue(false);
            manualButton.setButtonClickedValue(true);
            scheduleButton.setButtonClickedValue(false);
            moreButton.setButtonClickedValue(false);
        }
        else if(index === "Schedule")
        {
            predefinedButton.setButtonClickedValue(false);
            manualButton.setButtonClickedValue(false);
            scheduleButton.setButtonClickedValue(true);
            moreButton.setButtonClickedValue(false);
        }
        else if (index === "More")
        {
            predefinedButton.setButtonClickedValue(false);
            manualButton.setButtonClickedValue(false);
            scheduleButton.setButtonClickedValue(false);
            moreButton.setButtonClickedValue(true);
        }
    }

    function setClickedButton(name)
    {
        if (name === "Predefine")
        {
            predefinedButton.setButtonClickedValue(true);
            manualButton.setButtonClickedValue(false);
            scheduleButton.setButtonClickedValue(false);
            moreButton.setButtonClickedValue(false);
        }
        else if (name === "Zone")
        {
            predefinedButton.setButtonClickedValue(false);
            manualButton.setButtonClickedValue(true);
            scheduleButton.setButtonClickedValue(false);
            moreButton.setButtonClickedValue(false);
        }
        else if (name === "Schedule")
        {
            predefinedButton.setButtonClickedValue(false);
            manualButton.setButtonClickedValue(false);
            scheduleButton.setButtonClickedValue(true);
            moreButton.setButtonClickedValue(false);
        }
        else if (name === "More")
        {
            predefinedButton.setButtonClickedValue(false);
            manualButton.setButtonClickedValue(false);
            scheduleButton.setButtonClickedValue(false);
            moreButton.setButtonClickedValue(true);
        }
    }

    clip: true

    MouseArea{
        anchors.fill: parent
        onClicked: {

        }
    }

    Rectangle{
        z: 1
        width: parent.width
        height: 1
        anchors.top: parent.top
        color: "#c6c6c6"
    }

    Row{
        BaseButton{
            id: predefinedButton
            width: baseBtnWidth
            height: baseBtnHeight
            color: "#f5f5f5"
            margin1: 14
            margin2: 1
            textColor: predefinedButton.buttonClicked ? "#1792e5" : "#606060"
            textName: langStrObject.GetLangString("ETCS_PREDEFINE")
            imageSource: predefinedButton.buttonClicked ? logicObject.getLocalImagePath() + "predefine_B.png" : logicObject.getLocalImagePath() + "predefine_A.png"
            keyValue: "Predefined"
            Component.onCompleted: {
            }
        }
        BaseButton{
            id: manualButton
            width: baseBtnWidth
            height: baseBtnHeight
            color: "#f5f5f5"
            margin1: 14
            margin2: 1
            textName: langStrObject.GetLangString("ETCS_ZONE")
            textColor: manualButton.buttonClicked ? "#1792e5" : "#606060"
            imageSource: manualButton.buttonClicked ? logicObject.getLocalImagePath() + "manual_B.png" :  logicObject.getLocalImagePath() + "manual_A.png"
            keyValue: "Manual"
            Component.onCompleted: {
            }
        }
        BaseButton{
            id: scheduleButton
            width: baseBtnWidth
            height: baseBtnHeight
            color: "#f5f5f5"
            margin1: 14
            margin2: 1
            textName: langStrObject.GetLangString("ETCS_SCHEDULE")
            textColor: scheduleButton.buttonClicked ? "#1792e5" : "#606060"
            imageSource: scheduleButton.buttonClicked ? logicObject.getLocalImagePath() + "schedule_B.png" : logicObject.getLocalImagePath() + "schedule_A.png"
            buttonImageWidth: 39
            buttonImageHeight: 38
            keyValue: "Schedule"
            Component.onCompleted: {
            }
        }
        BaseButton{
            id: moreButton
            width: baseBtnWidth
            height: baseBtnHeight
            color: "#f5f5f5"
            margin1: 14
            margin2: 1
            textName: langStrObject.GetLangString("ETCS_MORE")
            textColor: moreButton.buttonClicked ? "#1792e5" : "#606060"
            imageSource: moreButton.buttonClicked ? logicObject.getLocalImagePath() + "more_B.png" : logicObject.getLocalImagePath() + "more_A.png"
            keyValue: "More"
            Component.onCompleted: {
            }
        }
    }

    Component.onCompleted: {
        predefinedButton.keyValueSignal.connect(mainPage.changeHomeView);
        manualButton.keyValueSignal.connect(mainPage.changeHomeView);
        scheduleButton.keyValueSignal.connect(mainPage.changeHomeView);
        moreButton.keyValueSignal.connect(mainPage.changeHomeView);
        predefinedButton.keyValueSignal.connect(setBusinessBarStatus);
        manualButton.keyValueSignal.connect(setBusinessBarStatus);
        scheduleButton.keyValueSignal.connect(setBusinessBarStatus);
        moreButton.keyValueSignal.connect(setBusinessBarStatus);
        setBusinessBarStatus("Predefined");

        rootItem.updateLanguage.connect(updateLanguageSlot)
    }
}

