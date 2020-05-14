import QtQuick 2.0

Item{

    function updateLanguageSlot()
    {
        btnOk.text = langStrObject.GetLangString("ETCS_OK");
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

    PadButton {id: btn1;
                anchors.top: parent.top; anchors.topMargin: 0; anchors.left: parent.left; anchors.leftMargin: 0;
                topLineVisible: true; bottomLineVisible: false; leftLineVisible: false; rightLineVisible: false;
                text: "1"; key: "1"}
    PadButton {id: btn2;
                anchors.top: parent.top; anchors.topMargin: 0; anchors.left: parent.left; anchors.leftMargin: 160;
                topLineVisible: true; bottomLineVisible: false; leftLineVisible: true; rightLineVisible: false;
                text: "2"; key: "2"}
    PadButton {id: btn3;
        anchors.top: parent.top; anchors.topMargin: 0; anchors.left: parent.left; anchors.leftMargin: 320;
        topLineVisible: true; bottomLineVisible: false; leftLineVisible: true; rightLineVisible: false;
        text: "3"; key: "3"}
    PadButton {id: btn4;
        anchors.top: parent.top; anchors.topMargin: 74; anchors.left: parent.left; anchors.leftMargin: 0;
        topLineVisible: true; bottomLineVisible: false; leftLineVisible: false; rightLineVisible: false;
        text: "4"; key: "4" }
    PadButton {id: btn5;
        anchors.top: parent.top; anchors.topMargin: 74; anchors.left: parent.left; anchors.leftMargin: 160;
        topLineVisible: true; bottomLineVisible: false; leftLineVisible: true; rightLineVisible: false;
        text: "5"; key: "5" }
    PadButton {id: btn6;
        anchors.top: parent.top; anchors.topMargin: 74; anchors.left: parent.left; anchors.leftMargin: 320;
        topLineVisible: true; bottomLineVisible: false; leftLineVisible: true; rightLineVisible: false;
        text: "6"; key: "6" }
    PadButton {id: btn7;
        anchors.top: parent.top; anchors.topMargin: 148; anchors.left: parent.left; anchors.leftMargin: 0;
        topLineVisible: true; bottomLineVisible: false; leftLineVisible: false; rightLineVisible: false;
        text: "7"; key: "7" }
    PadButton {id: btn8;
        anchors.top: parent.top; anchors.topMargin: 148; anchors.left: parent.left; anchors.leftMargin: 160;
        topLineVisible: true; bottomLineVisible: false; leftLineVisible: true; rightLineVisible: false;
        text: "8"; key: "8" }
    PadButton {id: btn9;
        anchors.top: parent.top; anchors.topMargin: 148; anchors.left: parent.left; anchors.leftMargin: 320;
        topLineVisible: true; bottomLineVisible: false; leftLineVisible: true; rightLineVisible: false;
        text: "9"; key: "9" }
    PadButton {id: btnOk;
        anchors.top: parent.top; anchors.topMargin: 222; anchors.left: parent.left; anchors.leftMargin: 0;
        topLineVisible: true; bottomLineVisible: false; leftLineVisible: false; rightLineVisible: false;
        text: langStrObject.GetLangString("ETCS_OK")
        key: "OK"}
    PadButton {id: btn0;
        anchors.top: parent.top; anchors.topMargin: 222; anchors.left: parent.left; anchors.leftMargin: 160;
        topLineVisible: true; bottomLineVisible: false; leftLineVisible: true; rightLineVisible: false;
        text: "0"
        key: "0"}
    PadButton {id: btnDel;
        anchors.top: parent.top; anchors.topMargin: 222; anchors.left: parent.left; anchors.leftMargin: 320;
        topLineVisible: true; bottomLineVisible: false; leftLineVisible: true; rightLineVisible: false;
        textVisible: false; imageVisible: true;
        text: ""
        key: "DEL"}

    Component.onCompleted: {
        rootItem.updateLanguage.connect(updateLanguageSlot);
    }
}
