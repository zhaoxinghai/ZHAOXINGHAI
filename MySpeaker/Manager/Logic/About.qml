import QtQuick 2.0
import "../Components"

Rectangle {

    function updateLanguageSlot()
    {
        systemVersion.setText(langStrObject.GetLangString("ETCS_SYSTEM_VERSION"));
        imageFitName.setText(langStrObject.GetLangString("ETCS_IMAGE_VERSION"));
        fsName.setText(langStrObject.GetLangString("ETCS_FILESYSTEM_VERSION"))
        softwareVersionBtn.btnName = langStrObject.GetLangString("ETCS_SOFTWARE_VERSION");
        protocolVersionBtn.btnName = langStrObject.GetLangString("ECTS_PROTOCOL_VERSION");
        buildTimeBtn.btnName = langStrObject.GetLangString("ETCS_COMPILE_TIME");
    }

    function initStatus()
    {

    }

    color: "#e7e7e7"


    Rectangle{
        id: rect1
        width: parent.width
        height: 25
        anchors.top: parent.top
        anchors.left: parent.left
        color: "#e7e7e7"
    }

    StripFrame{
        id: systemFrame
        anchors.top: rect1.bottom
        anchors.left: parent.left
        width: parent.width
        height: 65
        line1Visible: false
        line2Visible: true
        bkSwitch: false
        bkgPressedColor: "#dbe0e6"
        bkgReleasedColor: "#ffffff"
        CommonText{
            id: systemVersion
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 34
            width: parent.width - 34
            font.pixelSize: 18
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_SYSTEM_VERSION")
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
            Component.onCompleted: {

            }
        }
    }

    StripFrame{
        id: imageFitFrame
        anchors.top: systemFrame.bottom
        anchors.left: parent.left
        width: parent.width
        height: 65
        line1Visible: false
        line2Visible: true
        bkSwitch: false
        bkgPressedColor: "#dbe0e6"
        bkgReleasedColor: "#ffffff"
        CommonText{
            id: imageFitName
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 50
            width: 200
            font.pixelSize: 18
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_IMAGE_VERSION")
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
        }
        CommonText{
            id: imageFitVersion
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 25
            width: parent.width - imageFitName.contentWidth - 40 - 25
            font.pixelSize: 18
            color: "#858585"
            text: gpioObject.getImageFitVersion()
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignRight
        }
    }

    StripFrame{
        id: fsFrame
        anchors.top: imageFitFrame.bottom
        anchors.left: parent.left
        width: parent.width
        height: 65
        line1Visible: false
        line2Visible: false
        bkSwitch: false
        bkgPressedColor: "#dbe0e6"
        bkgReleasedColor: "#ffffff"
        CommonText{
            id: fsName
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 50
            anchors.left: parent.left
            width: 200
            font.pixelSize: 18
            color: "#303030"
            text: langStrObject.GetLangString("ETCS_FILESYSTEM_VERSION")
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
        }
        CommonText{
            id: fsVersion
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 25
            width: parent.width - fsName.contentWidth - 40 - 25
            font.pixelSize: 18
            color: "#858585"
            text: gpioObject.getFilesystemVersion()
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignRight
        }
    }
    StripButton{
        id: softwareVersionBtn
        anchors.top: fsFrame.bottom
        anchors.topMargin: 25
        anchors.left: parent.left
        width: parent.width
        imgSrc: ""
        btnName: langStrObject.GetLangString("ETCS_SOFTWARE_VERSION")
        infoText: logicObject.getVersion();
        isClickBtn: false
        clickImgSrc: ""
        btnValue: "aboutBtn"
        line1Visible: true
        line2Visible: false
        buttonNameWidth: 250
        Component.onCompleted: {

        }
    }

    StripButton{
        id: protocolVersionBtn
        anchors.top: softwareVersionBtn.bottom
        anchors.left: parent.left
        width: parent.width
        imgSrc: ""
        btnName: langStrObject.GetLangString("ECTS_PROTOCOL_VERSION")
        infoText: localConfigObject.getProtocolVersion()
        isClickBtn: false
        clickImgSrc: ""
        btnValue: "protocolVersionBtn"
        line1Visible: true
        line2Visible: false
        buttonNameWidth: 250
        Component.onCompleted: {

        }
    }
    StripButton{
        id: buildTimeBtn
        anchors.top: protocolVersionBtn.bottom
        anchors.left: parent.left
        width: parent.width
        imgSrc: ""
        btnName: langStrObject.GetLangString("ETCS_COMPILE_TIME")
        infoText: logicObject.getCompileTime()
        isClickBtn: false
        clickImgSrc: ""
        btnValue: "buildTimeBtn"
        line1Visible: true
        line2Visible: true
        buttonNameWidth: 250
        Component.onCompleted: {
        }
    }

	/*
    Rectangle{
        id: languageRect
        anchors.top: parent.top
        anchors.left: parent.left
        width: 480
        height: 800
        Text{
            id: one
            anchors.top: parent.top
            anchors.left: parent.left
            width: 480
            height: 80
            text: "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
            font.family: rootItem.fontName
            font.pixelSize: 16
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.Wrap
            color: "red"
            visible: true
            z: 10
        }
        Text{
            id: two
            anchors.top: one.bottom
            anchors.left: parent.left
            width: 480
            height: 80
            text: "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZäöüÄÖÜß"
            font.family: rootItem.fontName
            font.pixelSize: 16
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.Wrap
            color: "red"
            visible: true
            z: 10
        }
        Text{
            id: three
            anchors.top: two.bottom
            anchors.left: parent.left
            width: 480
            height: 80
            text: "AaÁáBbCcČčDdĎďEeÉéĚěFfGgHhChchIiÍíJjKkLlMmNnŇňOoÓóPpQqRrŘřSsŠšTtŤťUuÚúŮůVvWwXxYyÝýZzŽž"
            font.family: rootItem.fontName
            font.pixelSize: 16
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.Wrap
            color: "red"
            visible: true
            z: 10
        }
        Text{
            id: four
            anchors.top: three.bottom
            anchors.left: parent.left
            width: 480
            height: 80
            text: "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя"
            font.family: rootItem.fontName
            font.pixelSize: 16
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.Wrap
            color: "red"
            visible: true
            z: 10
        }
        Text{
            id: five
            anchors.top: four.bottom
            anchors.left: parent.left
            width: 480
            height: 80
            text: "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
            font.family: rootItem.fontName
            font.pixelSize: 16
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.Wrap
            color: "red"
            visible: true
            z: 10
        }
        Text{
            id: six
            anchors.top: five.bottom
            anchors.left: parent.left
            width: 480
            height: 80
            text: "AaĂăÂâBbCcDdEeFfGgHhIiÎîJjKkLlMmNnOoPpQqRrSsȘșTtȚțUuVvWwXxYyZz"
            font.family: rootItem.fontName
            font.pixelSize: 16
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.Wrap
            color: "red"
            visible: true
            z: 10
        }
        Text{
            id: seven
            anchors.top: six.bottom
            anchors.left: parent.left
            width: 480
            height: 80
            text: "AaBbCcÇçDdEeFfGgĞğHhIıİiJjKkLlMmNnOoÖöPpRrSsŞşTtUuÜüVvYyZz"
            font.family: rootItem.fontName
            font.pixelSize: 16
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.Wrap
            color: "red"
            visible: true
            z: 10
        }
        Text{
            id: eight
            anchors.top: seven.bottom
            anchors.left: parent.left
            width: 480
            height: 80
            text: "AaĄąBbCcĆćDdEeĘęFfGgHhIiJjKkLlŁłMmNnŃńOoóPpQqRrSsŚśTtUuVvWwXxYyZzŹźŻż"
            font.family: rootItem.fontName
            font.pixelSize: 16
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.Wrap
            color: "red"
            visible: true
            z: 10
        }
    }
	*/

    Component.onCompleted: {
        rootItem.updateLanguage.connect(updateLanguageSlot);
    }
}
