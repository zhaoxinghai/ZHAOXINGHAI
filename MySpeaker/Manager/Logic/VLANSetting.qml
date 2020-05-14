import QtQuick 2.0
import "../Components"

Rectangle {

    function updateLanguageSlot()
    {
        vlanSwitchBtn.setButtonNameText(langStrObject.GetLangString("ETCS_VLAN"));
        vlanIdInputBtn.setButtonNameText(langStrObject.GetLangString("ETCS_VLAN_ID"));
        vlanPriorityInputBtn.setButtonNameText(langStrObject.GetLangString("ETCS_VLAN_PRIORITY"));
        vlanIpInputBtn.setButtonNameText(langStrObject.GetLangString("ETCS_VLAN_IP"));
        vlanMaskInputBtn.setButtonNameText(langStrObject.GetLangString("ETCS_VLAN_MASK"));
    }

    function initStatus()
    {
        setVisibleValue(true);

        vlanSwitchBtn.setButtonNameText(langStrObject.GetLangString("ETCS_VLAN"));
        vlanSwitchBtn.setLine1VisibleValue(false);
        vlanSwitchBtn.setLine1Width(width);
        vlanSwitchBtn.setLine2VisibleValue(true);
        vlanSwitchBtn.setLine2Width(width);

        vlanIdInputBtn.setButtonNameText(langStrObject.GetLangString("ETCS_VLAN_ID"));
        vlanIdInputBtn.setInputText("3")
        vlanIdInputBtn.setInputTextFieldFocus(false);
        vlanIdInputBtn.setLine1VisibleValue(false);
        vlanIdInputBtn.setLine1Width(width);
        vlanIdInputBtn.setLine2VisibleValue(true);
        vlanIdInputBtn.setLine2Width(width);
        vlanIdInputBtn.setMaxLength();

        vlanPriorityInputBtn.setButtonNameText(langStrObject.GetLangString("ETCS_VLAN_PRIORITY"));
        vlanPriorityInputBtn.setInputText("6")
        vlanPriorityInputBtn.setInputTextFieldFocus(false);
        vlanPriorityInputBtn.setLine1VisibleValue(false);
        vlanPriorityInputBtn.setLine1Width(width);
        vlanPriorityInputBtn.setLine2VisibleValue(true);
        vlanPriorityInputBtn.setLine2Width(width);
        vlanPriorityInputBtn.setMaxLength();

        vlanIpInputBtn.setButtonNameText(langStrObject.GetLangString("ETCS_VLAN_IP"));
        vlanIpInputBtn.setInputText("10.0.0.1")
        vlanIpInputBtn.setInputTextFieldFocus(false);
        vlanIpInputBtn.setLine1VisibleValue(false);
        vlanIpInputBtn.setLine1Width(width);
        vlanIpInputBtn.setLine2VisibleValue(true);
        vlanIpInputBtn.setLine2Width(width);
        vlanIpInputBtn.setMaxLength();

        vlanMaskInputBtn.setButtonNameText(langStrObject.GetLangString("ETCS_VLAN_MASK"));
        vlanMaskInputBtn.setInputText("255.255.255.0")
        vlanMaskInputBtn.setInputTextFieldFocus(false);
        vlanMaskInputBtn.setLine1VisibleValue(false);
        vlanMaskInputBtn.setLine1Width(width);
        vlanMaskInputBtn.setLine2VisibleValue(false);
        vlanMaskInputBtn.setLine2Width(width);
        vlanMaskInputBtn.setMaxLength();

        initVlanData();
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
        var vlanId = vlanIdInputBtn.getInputText();
        var vlanPriority = vlanPriorityInputBtn.getInputText();
        var vlanIp = vlanIpInputBtn.getInputText();
        var vlanMask = vlanMaskInputBtn.getInputText();

        var isVlanIdValid = logicObject.isVlanIDValid(vlanId);
        if (true !== isVlanIdValid)
        {
            systemSettingKeyboard.setOKBtnEnableValue(false);
            return;
        }

        var isPriorityValid = logicObject.isVlanpriorityValid(vlanPriority);
        if (true !== isPriorityValid)
        {
            systemSettingKeyboard.setOKBtnEnableValue(false);
            return;
        }

        var isIpValid = logicObject.isIPAddressValid(vlanIp, vlanMask);
        if (true !== isIpValid)
        {
            systemSettingKeyboard.setOKBtnEnableValue(false);
            return;
        }
        systemSettingKeyboard.setOKBtnEnableValue(true);
    }

    function keyPressed(op)
    {
        if (vlanIdInputBtn.getInputTextFieldFocus() === true)
        {
            if (op === "DEL")
            {
                vlanIdInputBtn.removeData(op);
            }
            else if ((op >= "0" && op <= "9"))
            {
                vlanIdInputBtn.insertData(op);
            }
            else if (op === "C")
            {
                vlanIdInputBtn.setInputText("");
            }
        }
        else if (vlanPriorityInputBtn.getInputTextFieldFocus() === true)
        {
            if (op === "DEL")
            {
                vlanPriorityInputBtn.removeData(op);
            }
            else if ((op >= "0" && op <= "9"))
            {
                vlanPriorityInputBtn.insertData(op);
            }
            else if (op === "C")
            {
                vlanPriorityInputBtn.setInputText("");
            }
        }
        else if (vlanIpInputBtn.getInputTextFieldFocus() === true)
        {
            if (op === "DEL")
            {
                vlanIpInputBtn.removeData(op);
            }
            else if ((op >= "0" && op <= "9") || (op === "."))
            {
                vlanIpInputBtn.insertData(op);
            }
            else if (op === "C")
            {
                vlanIpInputBtn.setInputText("");
            }
        }
        else if (vlanMaskInputBtn.getInputTextFieldFocus() === true)
        {
            if (op === "DEL")
            {
                vlanMaskInputBtn.removeData(op);
            }
            else if ((op >= "0" && op <= "9") || (op === "."))
            {
                vlanMaskInputBtn.insertData(op);
            }
            else if (op === "C")
            {
                vlanMaskInputBtn.setInputText("");
            }
        }

        updateOKButtonStatus();
    }

    function vlanSwitchOnOff(value)
    {
        vlanIdInputBtn.visible = value
        vlanPriorityInputBtn.visible = value
        vlanIpInputBtn.visible = value
        vlanMaskInputBtn.visible = value
        vlanIdInputBtn.setInputTextFieldFocus(false);
        vlanPriorityInputBtn.setInputTextFieldFocus(false);
        vlanIpInputBtn.setInputTextFieldFocus(false);
        vlanMaskInputBtn.setInputTextFieldFocus(false);
        systemSettingKeyboard.setVisibleValue(false);

        if (true === value)
        {
            var vlanId = localConfigObject.readConfig("Config", "VlanID");
            var vlanPriority = localConfigObject.readConfig("Config", "VlanPriority");
            var vlanIp = localConfigObject.readConfig("Config", "VlanIP");
            var vlanMask = localConfigObject.readConfig("Config", "VlanMask");
            vlanIdInputBtn.setInputText(vlanId);
            vlanPriorityInputBtn.setInputText(vlanPriority);
            vlanIpInputBtn.setInputText(vlanIp);
            vlanMaskInputBtn.setInputText(vlanMask);
        }

        updateVlanData();
    }

    function okBtnReleaseSlot()
    {
        updateVlanData()
        vlanIdInputBtn.setInputTextFieldFocus(false);
        vlanPriorityInputBtn.setInputTextFieldFocus(false);
        vlanIpInputBtn.setInputTextFieldFocus(false);
        vlanMaskInputBtn.setInputTextFieldFocus(false);
        systemSettingKeyboard.setVisibleValue(false);
    }

    function initVlanData()
    {
        var vlanValid = localConfigObject.readConfig("Config", "VlanValid");
        var vlanId = localConfigObject.readConfig("Config", "VlanID");
        var vlanPriority = localConfigObject.readConfig("Config", "VlanPriority");
        var vlanIp = localConfigObject.readConfig("Config", "VlanIP");
        var vlanMask = localConfigObject.readConfig("Config", "VlanMask");
        if ("" === vlanValid)
        {
            vlanValid = "false";
            localConfigObject.insertConfig("VlanValid", "false");
            localConfigObject.insertConfig("VlanID", "3");
            localConfigObject.insertConfig("VlanPriority", "6");
            localConfigObject.insertConfig("VlanIP", "10.0.0.1");
            localConfigObject.insertConfig("VlanMask", "255.255.255.0");
        }
        vlanValid = ("false" === vlanValid) ? false : true;

        vlanIdInputBtn.visible = vlanValid
        vlanPriorityInputBtn.visible = vlanValid
        vlanIpInputBtn.visible = vlanValid
        vlanMaskInputBtn.visible = vlanValid

        vlanSwitchBtn.isEnable = vlanValid;
        vlanIdInputBtn.setInputText(vlanId);
        vlanPriorityInputBtn.setInputText(vlanPriority);
        vlanIpInputBtn.setInputText(vlanIp);
        vlanMaskInputBtn.setInputText(vlanMask);
    }

    function updateVlanData()
    {
        var vlanValid = vlanSwitchBtn.isEnable;
        var vlanId = vlanIdInputBtn.getInputText();
        var vlanPriority = vlanPriorityInputBtn.getInputText();
        var vlanIp = vlanIpInputBtn.getInputText();
        var vlanMask = vlanMaskInputBtn.getInputText();
        var ret = localConfigObject.writeConfig("Config", "VlanValid", vlanValid);

        if (true === vlanValid)
        {
            localConfigObject.writeConfig("Config", "VlanID", vlanId);
            localConfigObject.writeConfig("Config", "VlanPriority", vlanPriority);
            localConfigObject.writeConfig("Config", "VlanIP", vlanIp);
            localConfigObject.writeConfig("Config", "VlanMask", vlanMask);
        }
        else
        {
            vlanId = -1;
            vlanPriority = localConfigObject.readConfig("Config", "VlanPriority");
            vlanIp = localConfigObject.readConfig("Config", "VlanIP");
            vlanMask = localConfigObject.readConfig("Config", "VlanMask");
        }

        logicObject.setVlan(vlanId, vlanPriority, vlanIp, vlanMask);
    }

    color: "#e7e7e7"

    StripOnOffButton
    {
        id: vlanSwitchBtn
        anchors.top: parent.top
        anchors.topMargin: 22
        anchors.left: parent.left
        width: parent.width
        visible: true
    }

    StripInputButton
    {
        id: vlanIdInputBtn
        anchors.top: vlanSwitchBtn.bottom
        anchors.topMargin: 22
        anchors.left: parent.left
        width: parent.width
        visible: true
    }

    StripInputButton
    {
        id: vlanPriorityInputBtn
        anchors.top: vlanIdInputBtn.bottom
        anchors.left: parent.left
        width: parent.width
        visible: true
    }

    StripInputButton
    {
        id: vlanIpInputBtn
        anchors.top: vlanPriorityInputBtn.bottom
        anchors.left: parent.left
        width: parent.width
        visible: true
    }

    StripInputButton
    {
        id: vlanMaskInputBtn
        anchors.top: vlanIpInputBtn.bottom
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
        vlanSwitchBtn.switchChange.connect(vlanSwitchOnOff);
        vlanIdInputBtn.focusChange.connect(systemSettingKeyboard.setVisibleValue);
        vlanPriorityInputBtn.focusChange.connect(systemSettingKeyboard.setVisibleValue);
        vlanIpInputBtn.focusChange.connect(systemSettingKeyboard.setVisibleValue);
        vlanMaskInputBtn.focusChange.connect(systemSettingKeyboard.setVisibleValue);
    }
}
