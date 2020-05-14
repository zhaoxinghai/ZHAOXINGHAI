import QtQuick 2.0

Item{

    signal addCharSignal(string ch);
    signal delCharSignal();
    signal okSignal();

    function updateLanguageSlot()
    {
        btnOk.text = langStrObject.GetLangString("ETCS_OK");
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    clip: true

    MouseArea{
        anchors.fill: parent
        onClicked: {

        }
    }

    Rectangle{
        z: 1
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width
        height: 1
        color: "#93989E"
    }
    Rectangle{
        z: 1
        anchors.top: parent.top
        anchors.topMargin: 74
        anchors.left: parent.left
        width: parent.width
        height: 1
        color: "#93989E"
    }
    Rectangle{
        z: 1
        anchors.top: parent.top
        anchors.topMargin: 148
        anchors.left: parent.left
        width: parent.width
        height: 1
        color: "#93989E"
    }
    Rectangle{
        z: 1
        anchors.top: parent.top
        anchors.topMargin: 222
        anchors.left: parent.left
        width: parent.width
        height: 1
        color: "#93989E"
    }
    Rectangle{
        z: 1
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 160
        width: 1
        height: parent.height
        color: "#93989E"
    }
    Rectangle{
        z: 1
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 320
        width: 1
        height: parent.height
        color: "#93989E"
    }

    PadButton2 {id: btn1;
                anchors.top: parent.top; anchors.topMargin: 0; anchors.left: parent.left; anchors.leftMargin: 0;
                topLineVisible: true; bottomLineVisible: false; leftLineVisible: false; rightLineVisible: false;
                text: "1"
                onClickSignal: {
                    addCharSignal(getText());
                }
    }
    PadButton2 {id: btn2;
                anchors.top: parent.top; anchors.topMargin: 0; anchors.left: parent.left; anchors.leftMargin: 160;
                topLineVisible: true; bottomLineVisible: false; leftLineVisible: true; rightLineVisible: false;
                text: "2"
                onClickSignal: {
                    addCharSignal(getText());
                }
    }
    PadButton2 {id: btn3;
        anchors.top: parent.top; anchors.topMargin: 0; anchors.left: parent.left; anchors.leftMargin: 320;
        topLineVisible: true; bottomLineVisible: false; leftLineVisible: true; rightLineVisible: false;
        text: "3"
        onClickSignal: {
            addCharSignal(getText());
        }
    }
    PadButton2 {id: btn4;
        anchors.top: parent.top; anchors.topMargin: 74; anchors.left: parent.left; anchors.leftMargin: 0;
        topLineVisible: true; bottomLineVisible: false; leftLineVisible: false; rightLineVisible: false;
        text: "4"
        onClickSignal: {
            addCharSignal(getText());
        }
    }
    PadButton2 {id: btn5;
        anchors.top: parent.top; anchors.topMargin: 74; anchors.left: parent.left; anchors.leftMargin: 160;
        topLineVisible: true; bottomLineVisible: false; leftLineVisible: true; rightLineVisible: false;
        text: "5"
        onClickSignal: {
            addCharSignal(getText());
        }
    }
    PadButton2 {id: btn6;
        anchors.top: parent.top; anchors.topMargin: 74; anchors.left: parent.left; anchors.leftMargin: 320;
        topLineVisible: true; bottomLineVisible: false; leftLineVisible: true; rightLineVisible: false;
        text: "6"
        onClickSignal: {
            addCharSignal(getText());
        }
    }
    PadButton2 {id: btn7;
        anchors.top: parent.top; anchors.topMargin: 148; anchors.left: parent.left; anchors.leftMargin: 0;
        topLineVisible: true; bottomLineVisible: false; leftLineVisible: false; rightLineVisible: false;
        text: "7"
        onClickSignal: {
            addCharSignal(getText());
        }
    }
    PadButton2 {id: btn8;
        anchors.top: parent.top; anchors.topMargin: 148; anchors.left: parent.left; anchors.leftMargin: 160;
        topLineVisible: true; bottomLineVisible: false; leftLineVisible: true; rightLineVisible: false;
        text: "8"
        onClickSignal: {
            addCharSignal(getText());
        }
    }
    PadButton2 {id: btn9;
        anchors.top: parent.top; anchors.topMargin: 148; anchors.left: parent.left; anchors.leftMargin: 320;
        topLineVisible: true; bottomLineVisible: false; leftLineVisible: true; rightLineVisible: false;
        text: "9"
        onClickSignal: {
            addCharSignal(getText());
        }
    }
    PadButton2 {id: btnOk;
        anchors.top: parent.top; anchors.topMargin: 222; anchors.left: parent.left; anchors.leftMargin: 0;
        topLineVisible: true; bottomLineVisible: false; leftLineVisible: false; rightLineVisible: false;
        text: langStrObject.GetLangString("ETCS_OK")
        onClickSignal: {
            okSignal();
        }
    }
    PadButton2 {id: btn0;
        anchors.top: parent.top; anchors.topMargin: 222; anchors.left: parent.left; anchors.leftMargin: 160;
        topLineVisible: true; bottomLineVisible: false; leftLineVisible: true; rightLineVisible: false;
        text: "0"
        onClickSignal: {
            addCharSignal(getText());
        }
    }
    PadButton2 {id: btnDel;
        anchors.top: parent.top; anchors.topMargin: 222; anchors.left: parent.left; anchors.leftMargin: 320;
        topLineVisible: true; bottomLineVisible: false; leftLineVisible: true; rightLineVisible: false;
        textVisible: false; imageVisible: true;
        text: ""
        onClickSignal: {
            delCharSignal();
        }
    }

    Component.onCompleted: {
        rootItem.updateLanguage.connect(updateLanguageSlot);
    }
}
