import QtQuick 2.0

Rectangle {

    signal keyPress(var op);
    signal okBtnRelease();

    function updateLanguageSlot()
    {
        okBtn.text = langStrObject.GetLangString("ETCS_OK");
    }

    function setOKBtnEnableValue(value)
    {
        okBtn.btnEnable = value;
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    width: 480
    height: 320

    Rectangle{
        z: 1
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width
        height: 1
        color: "#e0e0e0"
    }
    Rectangle{
        z: 1
        anchors.top: parent.top
        anchors.topMargin: 80
        anchors.left: parent.left
        width: parent.width - 120
        height: 1
        color: "#e0e0e0"
    }
    Rectangle{
        z: 1
        anchors.top: parent.top
        anchors.topMargin: 160
        anchors.left: parent.left
        width: parent.width
        height: 1
        color: "#e0e0e0"
    }
    Rectangle{
        z: 1
        anchors.top: parent.top
        anchors.topMargin: 240
        anchors.left: parent.left
        width: parent.width - 120
        height: 1
        color: "#e0e0e0"
    }
    Rectangle{
        z: 1
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 120
        width: 1
        height: parent.height
        color: "#e0e0e0"
    }
    Rectangle{
        z: 1
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 240
        width: 1
        height: parent.height
        color: "#e0e0e0"
    }
    Rectangle{
        z: 1
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 360
        width: 1
        height: parent.height
        color: "#e0e0e0"
    }


    KeyboardButton{id: number1; width: 120; height: 80; text: "1"; btnValue: "1";
                   anchors.top: parent.top; anchors.topMargin: 0; anchors.left: parent.left; anchors.leftMargin: 0;
    }
    KeyboardButton{id: number2; width: 120; height: 80; text: "2"; btnValue: "2";
                    anchors.top: parent.top; anchors.topMargin: 0; anchors.left: parent.left; anchors.leftMargin: 120;
    }
    KeyboardButton{id: number3; width: 120; height: 80; text: "3"; btnValue: "3";
                     anchors.top: parent.top; anchors.topMargin: 0; anchors.left: parent.left; anchors.leftMargin: 240;
    }
    KeyboardButton{id: number4; width: 120; height: 80; text: "4"; btnValue: "4";
                    anchors.top: parent.top; anchors.topMargin: 80; anchors.left: parent.left; anchors.leftMargin: 0;
    }
    KeyboardButton{id: number5; width: 120; height: 80; text: "5"; btnValue: "5";
                     anchors.top: parent.top; anchors.topMargin: 80; anchors.left: parent.left; anchors.leftMargin: 120;
    }
    KeyboardButton{id: number6; width: 120; height: 80; text: "6"; btnValue: "6";
                    anchors.top: parent.top; anchors.topMargin: 80; anchors.left: parent.left; anchors.leftMargin: 240;
    }
    KeyboardButton{id: number7; width: 120; height: 80; text: "7"; btnValue: "7";
                    anchors.top: parent.top; anchors.topMargin: 160; anchors.left: parent.left; anchors.leftMargin: 0;
    }
    KeyboardButton{id: number8; width: 120; height: 80; text: "8"; btnValue: "8";
                    anchors.top: parent.top; anchors.topMargin: 160; anchors.left: parent.left; anchors.leftMargin: 120;
    }
    KeyboardButton{id: number9; width: 120; height: 80; text: "9"; btnValue: "9";
                    anchors.top: parent.top; anchors.topMargin: 160; anchors.left: parent.left; anchors.leftMargin: 240;
    }
    KeyboardButton{id: numberX; width: 120; height: 80; text: "C"; btnValue: "C";
                    anchors.top: parent.top; anchors.topMargin: 240; anchors.left: parent.left; anchors.leftMargin: 0;
    }
    KeyboardButton{id: number0; width: 120; height: 80; text: "0"; btnValue: "0";
                    anchors.top: parent.top; anchors.topMargin: 240; anchors.left: parent.left; anchors.leftMargin: 120;
    }
    KeyboardButton{id: point; width: 120; height: 80; text: "."; btnValue: ".";
                    anchors.top: parent.top; anchors.topMargin: 240; anchors.left: parent.left; anchors.leftMargin: 240;
    }

    KeyboardButton{
        id: delBtn
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 360
        width: 120
        height: 160
        text: ""
        btnValue: "DEL"
        textVisible: false
        imageVisible: true
    }
    KeyboardButton{
        id: okBtn
        anchors.top: parent.top
        anchors.topMargin: 160
        anchors.left: parent.left
        anchors.leftMargin: 360
        width: 120
        height: 160
        text: langStrObject.GetLangString("ETCS_OK")
        btnValue: "OK"
        buttonPressBackgroundColor: "#bbbbbb"
        buttonBackgroundColor: "#3cb15f"
        textColor: "#f0f0f0"

        onButtonReleased: {
            okBtnRelease();
        }
    }

    Component.onCompleted: {
        rootItem.updateLanguage.connect(updateLanguageSlot);
        number1.buttonPressed.connect(keyPress);
        number2.buttonPressed.connect(keyPress);
        number3.buttonPressed.connect(keyPress);
        number4.buttonPressed.connect(keyPress);
        number5.buttonPressed.connect(keyPress);
        number6.buttonPressed.connect(keyPress);
        number7.buttonPressed.connect(keyPress);
        number8.buttonPressed.connect(keyPress);
        number9.buttonPressed.connect(keyPress);
        number0.buttonPressed.connect(keyPress);
        numberX.buttonPressed.connect(keyPress);
        point.buttonPressed.connect(keyPress);
        delBtn.buttonPressed.connect(keyPress);
        okBtn.buttonPressed.connect(keyPress);
    }
}
