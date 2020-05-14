import QtQuick 2.0
import "../Components"
import "../Keyboard"

Rectangle {

    signal cancelSignal();
    signal saveSignal();

    function updateLanguageSlot()
    {
        saveBtn.setTextInfo(langStrObject.GetLangString("ETCS_SAVE"));
    }

    function addInitStatus(text)
    {
        setVisibleValue(true);
        title.setText(langStrObject.GetLangString("ETCS_PREDEFINE_SETTING"));
        prevBtn.setDataText(text);
        nameData.setText(langStrObject.GetLangString("ETCS_NAME"));
    }

    function updateInitStatus(text)
    {
        setVisibleValue(true);
        title.setText(langStrObject.GetLangString("ETCS_PREDEFINE_SETTING"));
        prevBtn.setDataText(text);
        nameData.setText(langStrObject.GetLangString("ETCS_NAME"));
    }

    function quitStatus()
    {
        nameData.setText(langStrObject.GetLangString("ETCS_NAME"));
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function getName()
    {
        return nameData.getTrimText();
    }

    color: "#e7e7e7"

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
            font.family: rootItem.fontName
            font.pixelSize: mainPage.g_titleTextFontSize
            text: langStrObject.GetLangString("ETCS_PREDEFINE_SETTING")
            width: 200
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
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
            imageSource: logicObject.getLocalImagePath() + "back.png"
            dataLeftMargin: -8
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
            nameWidth: 480 / 2 - title.contentWidth / 2
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

    Rectangle{
        id: nameTitle
        width: parent.width
        height: 70
        anchors.top: titleBar.bottom
        anchors.topMargin: 25
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
            text: langStrObject.GetLangString("ETCS_NAME")
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
               keyBoard.quitStatus();
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

