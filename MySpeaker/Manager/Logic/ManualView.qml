import QtQuick 2.0
import "../Components"

Rectangle {

    property int zoneViewHeight: height
    property int manualViewCallHeight: height + mainPage.businessBarHeight

    function setManualViewCallVisibleEnable()
    {
        manualViewCall.setVisibleValue(true);
    }

    function initStatus()
    {
        setVisibleValue(true);
        changeView("zoneView");
    }

    function updateCallStatus()
    {
        zoneView.stopQuickLive();
    }

    function changeView(index)
    {
        if (index === "zoneView")
        {
            zoneView.setVisibleValue(true);
            manualViewCall.setVisibleValue(false);
            mainPage.showBusinessBar();
        }
        else if (index === "manualViewCall")
        {
            zoneView.setVisibleValue(false);
            manualViewCall.initStatus();
            mainPage.hideBusinessBar();
        }
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function initZoneView()
    {
        zoneView.initZoneListModel();
    }

    function clearSearchBarText()
    {
        zoneView.clearSearchBarText();
    }

    ZoneView{
        id: zoneView
        width: parent.width
        height: zoneViewHeight
        anchors.top: parent.top
        visible: false
    }

    ManualViewCall{
        id: manualViewCall
        width: parent.width
        height: manualViewCallHeight
        visible: false
    }
}
