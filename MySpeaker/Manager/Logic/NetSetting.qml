import QtQuick 2.0
import "../Components"

Rectangle {

    function updateLanguageSlot()
    {
        ipInputBtn.setButtonNameText(langStrObject.GetLangString("ETCS_IP_ADDRESS"));
        netmaskInputBtn.setButtonNameText(langStrObject.GetLangString("ETCS_SUBNET_MASK"));
        gatewayInputBtn.setButtonNameText(langStrObject.GetLangString("ETCS_DEFAULT_GATEWAY"));
    }

    function initStatus()
    {
        setVisibleValue(true);

        ipInputBtn.setButtonNameText(langStrObject.GetLangString("ETCS_IP_ADDRESS"));
        ipInputBtn.setInputText(logicObject.getRealIp())
        ipInputBtn.setInputTextFieldFocus(false);
        ipInputBtn.setLine1VisibleValue(false);
        ipInputBtn.setLine1Width(width);
        ipInputBtn.setLine2VisibleValue(true);
        ipInputBtn.setLine2Width(width);
        ipInputBtn.setMaxLength();

        netmaskInputBtn.setButtonNameText(langStrObject.GetLangString("ETCS_SUBNET_MASK"));
        netmaskInputBtn.setInputText(logicObject.getRealNetmask())
        netmaskInputBtn.setInputTextFieldFocus(false);
        netmaskInputBtn.setLine1VisibleValue(false);
        netmaskInputBtn.setLine1Width(width);
        netmaskInputBtn.setLine2VisibleValue(true);
        netmaskInputBtn.setLine2Width(width);
        netmaskInputBtn.setMaxLength();

        gatewayInputBtn.setButtonNameText(langStrObject.GetLangString("ETCS_DEFAULT_GATEWAY"));
        gatewayInputBtn.setInputText(logicObject.getRealGateway())
        gatewayInputBtn.setInputTextFieldFocus(false);
        gatewayInputBtn.setLine1VisibleValue(false);
        gatewayInputBtn.setLine1Width(width);
        gatewayInputBtn.setLine2VisibleValue(false);
        gatewayInputBtn.setLine2Width(width);
        gatewayInputBtn.setMaxLength();

        updateOKButtonStatus();
        systemSettingKeyboard.setVisibleValue(false);
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function getVisibleValue()
    {
        return visible;
    }

    function updateOKButtonStatus()
    {
        var ipOK;
        var netmaskOK;
        var gatewayOK;
        var ipAndGatewaySameNum;
        var ip;
        var netmask;
        var gateway;

        ip = ipInputBtn.getInputText();
        netmask = netmaskInputBtn.getInputText();
        gateway = gatewayInputBtn.getInputText();
        ipOK = logicObject.isIPAddressValid(ip, netmask);
        netmaskOK = logicObject.isSubnetMaskIsValid(netmask);
        gatewayOK =  logicObject.isGatewayIsValid(gateway, netmask);
        ipAndGatewaySameNum = logicObject.isIpAndGatewaySameNetNumber(ip, netmask, gateway);

        if ((ipOK === true) && (netmaskOK === true) && (gatewayOK === true) && (ipAndGatewaySameNum === 0))
        {
            systemSettingKeyboard.setOKBtnEnableValue(true);
        }
        else
        {
            systemSettingKeyboard.setOKBtnEnableValue(false);
        }
    }

    function keyPressed(op)
    {
        if (ipInputBtn.getInputTextFieldFocus() === true)
        {
            if (op === "DEL")
            {
                ipInputBtn.removeData(op);
            }
            else if ((op >= "0" && op <= "9") || (op === "."))
            {
                ipInputBtn.insertData(op);
            }
            else if (op === "C")
            {
                ipInputBtn.setInputText("");
            }
        }
        else if (netmaskInputBtn.getInputTextFieldFocus() === true)
        {
            if (op === "DEL")
            {
                netmaskInputBtn.removeData(op);
            }
            else if ((op >= "0" && op <= "9") || (op === "."))
            {
                netmaskInputBtn.insertData(op);
            }
            else if (op === "C")
            {
                netmaskInputBtn.setInputText("");
            }
        }
        else if (gatewayInputBtn.getInputTextFieldFocus() === true)
        {
            if (op === "DEL")
            {
                gatewayInputBtn.removeData(op);
            }
            else if ((op >= "0" && op <= "9") || (op === "."))
            {
                gatewayInputBtn.insertData(op);
            }
            else if (op === "C")
            {
                gatewayInputBtn.setInputText("");
            }
        }

        updateOKButtonStatus();
    }

    function okBtnReleaseSlot()
    {
        var ret;
        ret = updateNetData();
        if (ret < 0)
        {
            messageBar.messageBarShow(langStrObject.GetLangString("ETCS_NETWORK_SET_ERROR"));
            return;
        }

        ipInputBtn.setInputTextFieldFocus(false);
        netmaskInputBtn.setInputTextFieldFocus(false);
        gatewayInputBtn.setInputTextFieldFocus(false);
        systemSettingKeyboard.setVisibleValue(false);
        updateOKButtonStatus();
    }

    function updateNetData()
    {
        var ret;
        var ip;
        var netmask;
        var gateway;

        var tmpIp;
        var tmpNetmask;
        var tmpGateway;

        ip = ipInputBtn.getInputText();
        netmask = netmaskInputBtn.getInputText();
        gateway = gatewayInputBtn.getInputText();

        tmpIp = logicObject.getRealIp();
        tmpNetmask = logicObject.getRealNetmask();
        tmpGateway = logicObject.getRealGateway();

        /*
        console.log("ip ========= " + ip);
        console.log("netmask ========= " + netmask);
        console.log("gateway ========= " + gateway);
        console.log("tmpIp ========= " + tmpIp);
        console.log("tmpNetmask ========= " + tmpNetmask);
        console.log("tmpGateway ========= " + tmpGateway);
        */

        if ((ip === tmpIp) && (netmask === tmpNetmask) && (gateway === tmpGateway))
        {
            return 0;
        }

        if (ip !== tmpIp)
        {
            ret = logicObject.syncIpAddress(ip);
            if (ret < 0)
            {
                return -1;
            }
            else
            {
                logicObject.setIpAddressAndNetmaskToFile(ip, tmpNetmask);
            }
        }

        if (netmask !== tmpNetmask)
        {
            ret = logicObject.syncNetMask(netmask);
            if (ret < 0)
            {
                return -1;
            }
            else
            {
                logicObject.setIpAddressAndNetmaskToFile(ip, netmask);
            }
        }

        tmpGateway = logicObject.getRealGateway();
        if (tmpGateway === "")
        {
            ret = logicObject.syncGateway(gateway);
            if (ret < 0)
            {
                return -1;
            }
            else
            {
                logicObject.setGatewayToFile(gateway);
            }
        }
        else if ((tmpGateway !== "") && (gateway !== tmpGateway))
        {
            //console.log("gateway !== tmpGateway");
            ret = logicObject.syncGateway(gateway);
            if (ret < 0)
            {
                return -1;
            }
            else
            {
                logicObject.deleteGateway(tmpGateway);
                logicObject.setGatewayToFile(gateway);
            }
        }

        return 0;
    }

    color: "#e7e7e7"

    StripInputButton
    {
        id: ipInputBtn
        anchors.top: parent.top
        anchors.topMargin: 22
        anchors.left: parent.left
        width: parent.width
        visible: true
    }

    StripInputButton
    {
        id: netmaskInputBtn
        anchors.top: ipInputBtn.bottom
        anchors.left: parent.left
        width: parent.width
        visible: true
    }

    StripInputButton
    {
        id: gatewayInputBtn
        anchors.top: netmaskInputBtn.bottom
        anchors.left: parent.left
        width: parent.width
        visible: true
    }

    MessageBar{
        id: messageBar
        anchors.bottom: systemSettingKeyboard.top
        width: parent.width
        height: 0
        color: "#dddddd"
        clip: true
        z: 2
    }

    SystemSettingKeyboard{
        id: systemSettingKeyboard
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        visible: false
    }

    Component.onCompleted: {
        rootItem.updateLanguage.connect(updateLanguageSlot);
        systemSettingKeyboard.keyPress.connect(keyPressed);
        systemSettingKeyboard.okBtnRelease.connect(okBtnReleaseSlot);
        ipInputBtn.focusChange.connect(systemSettingKeyboard.setVisibleValue);
        netmaskInputBtn.focusChange.connect(systemSettingKeyboard.setVisibleValue);
        gatewayInputBtn.focusChange.connect(systemSettingKeyboard.setVisibleValue);
    }
}
