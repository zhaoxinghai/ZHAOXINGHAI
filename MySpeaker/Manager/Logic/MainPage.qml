import QtQuick 2.0
import "../Components"

Rectangle {

    property int businessBarHeight: 86
    property int viewAreaHeight: height - rootItem.statusBarHeight - businessBarHeight

    property int g_titleBarHeight: 60
    property string g_titleColor: "#404040"
    property string g_titleTextColor: "#f1f1f1"
    property int g_titleTextFontSize: 24
    property int g_titleTextFontSize2: 18
    property int micLedFlashStatus: 0

    function updateLanguageSlot()
    {

    }

    function initStatus()
    {
        setVisibleValue(true);
        logicObject.clearPredefineListModel();
        logicObject.createPredefineListModel(predefineView.getPredefineItemStatus(), predefineView.getSearchBarText());
        predefineView.updateEditAndOKStatus();
        predefineView.updateViewTypeBtnStatus();
        manualView.initZoneView();
        moreView.setVisibleValue(false);
        showBusinessBar();
    }

    function keepStatus()
    {
        setVisibleValue(true);
    }

    function changeHomeView(index)
    {
        if (index === "Predefined")
        {
            predefineView.initStatus();
            manualView.setVisibleValue(false);
            manualView.updateCallStatus();
            scheduleView.setVisibleValue(false);
            moreView.setVisibleValue(false);
        }
        else if(index === "Manual")
        {
            predefineView.setVisibleValue(false);
            manualView.initStatus();
            scheduleView.setVisibleValue(false);
            moreView.setVisibleValue(false);
        }
        else if(index === "Schedule")
        {
            predefineView.setVisibleValue(false);
            manualView.setVisibleValue(false);
            manualView.updateCallStatus();
            scheduleView.initStatus();
            moreView.setVisibleValue(false);
        }
        else if (index === "More")
        {
            predefineView.setVisibleValue(false);
            manualView.setVisibleValue(false);
            manualView.updateCallStatus();
            scheduleView.setVisibleValue(false);
            moreView.initStatus();
        }
    }

    function hideBusinessBar()
    {
        businessBar.visible = false;
    }

    function showBusinessBar()
    {
        businessBar.visible = true;
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function setClickedButton(name)
    {
        businessBar.setClickedButton(name);
    }

    function getVAPageVisible()
    {
        return moreView.getVAPageVisible();
    }

    color: g_titleColor

    MouseArea{
        anchors.fill: parent
        onClicked: {

        }
    }

    PredefineView{
        id: predefineView
        anchors.top: parent.top
        anchors.topMargin: rootItem.statusBarHeight
        width: parent.width
        height: viewAreaHeight
        visible: false
    }

    ManualView{
        id: manualView
        anchors.top: parent.top
        anchors.topMargin: rootItem.statusBarHeight
        width: parent.width
        height: viewAreaHeight
        visible: false
    }

    ScheduleView{
        id: scheduleView
        anchors.top: parent.top
        anchors.topMargin: rootItem.statusBarHeight
        width: parent.width
        height: viewAreaHeight
        visible: false
    }

    MoreView{
        id: moreView
        anchors.top: parent.top
        anchors.topMargin: rootItem.statusBarHeight
        width: parent.width
        height: viewAreaHeight
        visible: false
    }

    BusinessBar{
        id: businessBar
        anchors.bottom: parent.bottom
        width: parent.width
        height: businessBarHeight
    }

    Component.onCompleted: {
        changeHomeView("Predefined");
        rootItem.updateLanguage.connect(updateLanguageSlot);
    }
}
