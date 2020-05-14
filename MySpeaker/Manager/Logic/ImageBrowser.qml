import QtQuick 2.0
import QtQml.Models 2.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

import "../Components"

Rectangle {

    function initStatus()
    {
        imageBrowserObject.clearImageList();
        imageBrowserObject.createImageList();
        listModel.clearListModel();
        listModel.createListModel();
        setVisibleValue(true);
    }

    function quitStatus()
    {
        setVisibleValue(false);
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    ListModel {

        function createListModel()
        {
            var count;

            count = imageBrowserObject.getImageListCount();

            if (count === 0)
            {
                listModel.append({"nIndex": 0});
                return;
            }

            for (var i = 0; i < count; i++)
            {
                listModel.append({"nIndex": i});
            }
        }

        function clearListModel()
        {
           if (listModel.count > 0)
           {
                remove(0, listModel.count);
           }
        }

        id: listModel
    }

    ListView{
        id: listView
        width: parent.width
        height: parent.height
        anchors.top: parent.top
        anchors.left: parent.left
        clip: true
        boundsBehavior: Flickable.StopAtBounds
        snapMode: ListView.SnapOneItem
        orientation: ListView.Horizontal
        footer: Rectangle{height: 8}
        maximumFlickVelocity: 1000
        flickDeceleration: 200
        model: visualModel
    }

    DelegateModel {
        id: visualModel

        property alias selectedItems: gp
        groups: [
            DelegateModelGroup{
                id: gp
                name: "selected"
            }
        ]

        model: listModel

        delegate:Rectangle{
                id: listViewElement
                width: 480
                height: 800
                CommonImage{
                    id: image
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter:parent.horizontalCenter
                    visible: true
                    source: imageBrowserObject.getImageUriFileName(nIndex)
                }

                MouseArea{
                    anchors.fill: parent
                    onDoubleClicked: {
                        quitStatus();
                    }
                }
        }
    }
}
