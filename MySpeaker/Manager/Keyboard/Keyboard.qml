import QtQuick 2.0

Rectangle {

    signal addCharSignal(var ch);
    signal delCharSignal();
    signal clearSignal();
    signal okSignal();

    function updateLanguageSlot()
    {
        //initStatus();
    }

    function initStatus()
    {
        selectZone.resetStatus();
        mainBoard.setPage(1);
        mainBoard.updateKeyButtons();
        mainBoard.setVisibleValue(true);
        numberBoard.setVisibleValue(false);
        symListModel.clearListModel();
        symListModel.createListModel();
        symbolBoard.setVisibleValue(false);
        setVisibleValue(true);
    }

    function quitStatus()
    {
        selectZone.resetStatus();
        mainBoard.setPage(1);
        mainBoard.setVisibleValue(true);
        numberBoard.setVisibleValue(false);
        symListModel.clearListModel();
        symListModel.createListModel();
        symbolBoard.setVisibleValue(false);
        setVisibleValue(false);
    }

    function setVisibleValue(value)
    {
        visible = value;
    }

    function setOKBtnEnableStatus()
    {
        key_ok.setEnabled(true);
        key_ok.setDataColor("#303030");
        num_key_ok.setEnabled(true);
        num_key_ok.setDataColor("#303030");
    }

    function setOKBtnDisableStatus()
    {
        key_ok.setEnabled(false);
        key_ok.setDataColor("#40666666");
        num_key_ok.setEnabled(false);
        num_key_ok.setDataColor("#40666666");
    }

    width: 480
    height: 366
    clip: true
    color: "#00ffffff"

    MouseArea{
        anchors.fill: parent
        onClicked: {

        }
    }


    Rectangle{

        function resetStatus()
        {
            listModel.clearListModel();
            setVisibleValue(false);
        }

        function setVisibleValue(value)
        {
            visible = value;
        }

        id: selectZone
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width
        height: 46
        visible: false

        Rectangle{
            anchors.top: parent.top
            anchors.left: parent.left
            z:  1
            width: 480
            height: 1
            color: "#e0e0e0"
            visible: true
        }

        ListModel {

            property string key: ""

            function updateListModel(key)
            {
                clearListModel();
                setKey(key);
                createListModel();
            }

            function setKey(value)
            {
                key = value;
            }

            function getKey()
            {
                return key;
            }

            function createListModel()
            {
                var count = keyboardObject.getStringListSize(key);

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
            anchors.fill: parent
            width: parent.width - 40
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 20
            boundsBehavior: Flickable.StopAtBounds
            clip: true
            orientation: ListView.Horizontal

            model:listModel
            delegate:KeyTextButton{
                    width: 60
                    height: 46
                    dataText: keyboardObject.getCharFromKeyMap(listModel.getKey(), nIndex);
                    onClickedSignal: {
                        addCharSignal(dataText);
                        selectZone.resetStatus();
                    }
                }
        }
    }

    Rectangle{
        property int page: 1

        function updateKeyButtons()
        {
            key_1.setKey("KEY_1_" + mainBoard.page);
            key_1.setDataText(keyboardObject.getKeyText(key_1.getKey()));
            key_2.setKey("KEY_2_" + mainBoard.page);
            key_2.setDataText(keyboardObject.getKeyText(key_2.getKey()));
            key_3.setKey("KEY_3_" + mainBoard.page);
            key_3.setDataText(keyboardObject.getKeyText(key_3.getKey()));
            key_4.setKey("KEY_4_" + mainBoard.page);
            key_4.setDataText(keyboardObject.getKeyText(key_4.getKey()));
            key_5.setKey("KEY_5_" + mainBoard.page);
            key_5.setDataText(keyboardObject.getKeyText(key_5.getKey()));
            key_6.setKey("KEY_6_" + mainBoard.page);
            key_6.setDataText(keyboardObject.getKeyText(key_6.getKey()));
            key_7.setKey("KEY_7_" + mainBoard.page);
            key_7.setDataText(keyboardObject.getKeyText(key_7.getKey()));
            key_8.setKey("KEY_8_" + mainBoard.page);
            key_8.setDataText(keyboardObject.getKeyText(key_8.getKey()));
            key_9.setKey("KEY_9_" + mainBoard.page);
            key_9.setDataText(keyboardObject.getKeyText(key_9.getKey()));
        }

        function setPage(value)
        {
            page = value;
        }

        function getPage()
        {
            return page;
        }

        function setVisibleValue(value)
        {
            visible = value;
        }

        id: mainBoard
        anchors.top: selectZone.bottom
        anchors.left: parent.left
        width: parent.width
        height: 320
        visible: true
        Rectangle{
            anchors.top: parent.top
            anchors.left: parent.left
            z:  1
            width: 480
            height: 1
            color: "#e0e0e0"
            visible: true
        }
        Rectangle{
            anchors.top: parent.top
            anchors.topMargin: 80
            anchors.left: parent.left
            z:  1
            width: 480
            height: 1
            color: "#e0e0e0"
            visible: true
        }
        Rectangle{
            anchors.top: parent.top
            anchors.topMargin: 160
            anchors.left: parent.left
            z:  1
            width: 480
            height: 1
            color: "#e0e0e0"
            visible: true
        }
        Rectangle{
            anchors.top: parent.top
            anchors.topMargin: 240
            anchors.left: parent.left
            z:  1
            width: 480
            height: 1
            color: "#e0e0e0"
            visible: true
        }
        Rectangle{
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 120
            z:  1
            width: 1
            height: 320
            color: "#e0e0e0"
            visible: true
        }
        Rectangle{
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 240
            z:  1
            width: 1
            height: 320
            color: "#e0e0e0"
            visible: true
        }
        Rectangle{
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 360
            z:  1
            width: 1
            height: 320
            color: "#e0e0e0"
            visible: true
        }
        Grid {
            id: gridZone
            anchors.top: parent.top
            anchors.left: parent.left
            width: 360
            height: parent.height
            columns: 3
            KeyTextButton {id: key_1; key: "KEY_1_" + mainBoard.page; dataText: "";
                            bkgReleasedColor: "#f8f8f8"
                            onClickedSignal: {
                                listModel.updateListModel(key)
                                selectZone.setVisibleValue(true);
                            }
                        }
            KeyTextButton {id: key_2; key: "KEY_2_" + mainBoard.page; dataText: keyboardObject.getKeyText(key);
                            bkgReleasedColor: "#f8f8f8"
                            onClickedSignal: {
                                listModel.updateListModel(key)
                                selectZone.setVisibleValue(true);
                            }
                            }
            KeyTextButton {id: key_3; key: "KEY_3_" + mainBoard.page; dataText: keyboardObject.getKeyText(key)
                            bkgReleasedColor: "#f8f8f8"
                            onClickedSignal: {
                                listModel.updateListModel(key)
                                selectZone.setVisibleValue(true);
                            }
                            }
            KeyTextButton {id: key_4; key: "KEY_4_" + mainBoard.page; dataText: keyboardObject.getKeyText(key)
                            bkgReleasedColor: "#f8f8f8"
                            onClickedSignal: {
                                listModel.updateListModel(key)
                                selectZone.setVisibleValue(true);
                            }
                        }
            KeyTextButton {id: key_5; key: "KEY_5_" + mainBoard.page; dataText: keyboardObject.getKeyText(key)
                            bkgReleasedColor: "#f8f8f8"
                            onClickedSignal: {
                                listModel.updateListModel(key)
                                selectZone.setVisibleValue(true);
                            }
                        }
            KeyTextButton {id: key_6; key: "KEY_6_" + mainBoard.page; dataText: keyboardObject.getKeyText(key)
                            bkgReleasedColor: "#f8f8f8"
                            onClickedSignal: {
                                listModel.updateListModel(key)
                                selectZone.setVisibleValue(true);
                            }
                        }
            KeyTextButton {id: key_7; key: "KEY_7_" + mainBoard.page; dataText: keyboardObject.getKeyText(key)
                            bkgReleasedColor: "#f8f8f8"
                            onClickedSignal: {
                                listModel.updateListModel(key)
                                selectZone.setVisibleValue(true);
                            }
                        }
            KeyTextButton {id: key_8; key: "KEY_8_" + mainBoard.page; dataText: keyboardObject.getKeyText(key)
                            bkgReleasedColor: "#f8f8f8"
                            onClickedSignal: {
                                listModel.updateListModel(key)
                                selectZone.setVisibleValue(true);
                            }
                        }
            KeyTextButton {id: key_9; key: "KEY_9_" + mainBoard.page; dataText: keyboardObject.getKeyText(key)
                            bkgReleasedColor: "#f8f8f8"
                            onClickedSignal: {
                                listModel.updateListModel(key)
                                selectZone.setVisibleValue(true);
                            }
                        }
            KeyTextButton {id: key_ch; dataText: "#+="
                            onClickedSignal: {
                                listModel.clearListModel();
                                selectZone.setVisibleValue(false);
                                mainBoard.setVisibleValue(false);
                                numberBoard.setVisibleValue(false);
                                symbolBoard.setVisibleValue(true);
                                symColumnZone.resetSymColumnZone();
                            }
                        }
            KeyImageButton {id: key_0; imageSource: logicObject.getLocalImagePath() + "key_space.png"
                            onClickedSignal: {
                                selectZone.resetStatus();
                                addCharSignal(" ");
                            }
                        }
            KeyTextButton {id: key_C; dataText: "C";
                    onClickedSignal: {
                        selectZone.resetStatus();
                        clearSignal();
                    }
                }
        }
        Column {
            id: columnZone
            anchors.top: parent.top
            anchors.right: parent.right
            width: 120
            height: parent.height
            KeyImageButton{id: key_del; imageSource: logicObject.getLocalImagePath() + "key_del.png";
                            bkgReleasedColor: "#efefef"
                            onClickedSignal: {
                                selectZone.resetStatus();
                                delCharSignal();
                            }
                        }


            KeyImageButton{id: key_switch; imageSource: logicObject.getLocalImagePath() + "key_page_B.png"
                            bkgReleasedColor: "#efefef"
                            onClickedSignal: {
                                if (mainBoard.getPage() === 1)
                                {
                                    setButtonImageSource(logicObject.getLocalImagePath() + "key_page_A.png");
                                    mainBoard.setPage(2);
                                }
                                else if (mainBoard.getPage() === 2)
                                {
                                    setButtonImageSource(logicObject.getLocalImagePath() + "key_page_B.png");
                                    mainBoard.setPage(1);
                                }

                                mainBoard.updateKeyButtons();
                                selectZone.resetStatus();
                            }
                        }

            KeyTextButton {id: key_num; dataText: "123"
                            bkgReleasedColor: "#efefef"
                            onClickedSignal: {
                                selectZone.resetStatus();
                                mainBoard.setVisibleValue(false);
                                numberBoard.setVisibleValue(true);
                                symbolBoard.setVisibleValue(false);
                            }
                        }
            KeyTextButton {id: key_ok; dataText: "OK"
                            bkgReleasedColor: "#efefef"
                            onClickedSignal: {
                                okSignal();
                            }
                        }
        }
    }

    Rectangle{

        function setVisibleValue(value)
        {
            visible = value;
        }

        id: numberBoard
        anchors.top: selectZone.bottom
        anchors.left: parent.left
        width: parent.width
        height: 320
        visible: false

        Rectangle{
            anchors.top: parent.top
            anchors.left: parent.left
            z:  1
            width: 480
            height: 1
            color: "#e0e0e0"
            visible: true
        }
        Rectangle{
            anchors.top: parent.top
            anchors.topMargin: 80
            anchors.left: parent.left
            z:  1
            width: 480
            height: 1
            color: "#e0e0e0"
            visible: true
        }
        Rectangle{
            anchors.top: parent.top
            anchors.topMargin: 160
            anchors.left: parent.left
            z:  1
            width: 480
            height: 1
            color: "#e0e0e0"
            visible: true
        }
        Rectangle{
            anchors.top: parent.top
            anchors.topMargin: 240
            anchors.left: parent.left
            z:  1
            width: 480
            height: 1
            color: "#e0e0e0"
            visible: true
        }
        Rectangle{
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 120
            z:  1
            width: 1
            height: 320
            color: "#e0e0e0"
            visible: true
        }
        Rectangle{
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 240
            z:  1
            width: 1
            height: 320
            color: "#e0e0e0"
            visible: true
        }
        Rectangle{
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 360
            z:  1
            width: 1
            height: 320
            color: "#e0e0e0"
            visible: true
        }
        Grid {
            id: numGridZone
            anchors.top: parent.top
            anchors.left: parent.left
            width: 360
            height: parent.height
            columns: 3
            KeyTextButton {id: num_key_1; dataText: "1";
                            bkgReleasedColor: "#f8f8f8"
                            onClickedSignal: {
                                addCharSignal(dataText);
                            }
                        }
            KeyTextButton {id: num_key_2; dataText: "2";
                            bkgReleasedColor: "#f8f8f8"
                            onClickedSignal: {
                                addCharSignal(dataText);
                            }
                        }
            KeyTextButton {id: num_key_3; dataText: "3";
                            bkgReleasedColor: "#f8f8f8"
                            onClickedSignal: {
                                addCharSignal(dataText);
                            }
                        }
            KeyTextButton {id: num_key_4; dataText: "4";
                            bkgReleasedColor: "#f8f8f8"
                            onClickedSignal: {
                                addCharSignal(dataText);
                            }
                        }
            KeyTextButton {id: num_key_5; dataText: "5";
                            bkgReleasedColor: "#f8f8f8"
                            onClickedSignal: {
                                addCharSignal(dataText);
                            }
                        }
            KeyTextButton {id: num_key_6; dataText: "6";
                            bkgReleasedColor: "#f8f8f8"
                            onClickedSignal: {
                                addCharSignal(dataText);
                            }
                        }
            KeyTextButton {id: num_key_7; dataText: "7";
                            bkgReleasedColor: "#f8f8f8"
                            onClickedSignal: {
                                addCharSignal(dataText);
                            }
                        }
            KeyTextButton {id: num_key_8; dataText: "8";
                            bkgReleasedColor: "#f8f8f8"
                            onClickedSignal: {
                                addCharSignal(dataText);
                            }
                        }
            KeyTextButton {id: num_key_9; dataText: "9";
                            bkgReleasedColor: "#f8f8f8"
                            onClickedSignal: {
                                addCharSignal(dataText);
                            }
                        }
            KeyTextButton {id: num_key_comma; dataText: ",";
                            bkgReleasedColor: "#f8f8f8"
                            onClickedSignal: {
                                addCharSignal(dataText);
                            }
                        }
            KeyTextButton {id: num_key_0; dataText: "0";
                            bkgReleasedColor: "#f8f8f8"
                            onClickedSignal: {
                                addCharSignal(dataText);
                            }
                        }
            KeyTextButton {id: num_key_end; dataText: ".";
                            bkgReleasedColor: "#f8f8f8"
                            onClickedSignal: {
                                addCharSignal(dataText);
                            }
                        }
        }
        Column {
            id: numColumnZone
            anchors.top: parent.top
            anchors.right: parent.right
            width: 120
            height: parent.height
            KeyImageButton{id: num_key_del; imageSource: logicObject.getLocalImagePath() + "key_del.png"
                            bkgReleasedColor: "#efefef"
                            onClickedSignal: {
                                delCharSignal();
                            }
                        }

            KeyImageButton{id: num_key_space; imageSource: logicObject.getLocalImagePath() + "key_space.png"
                            bkgReleasedColor: "#efefef"
                            onClickedSignal: {
                                addCharSignal(" ");
                            }
                        }

            KeyTextButton {id: num_key_ok; dataText: ""
                            bkgReleasedColor: "#efefef"
                            enabled: false
                            onClickedSignal: {
                                //okSignal();
                            }
                        }
            KeyImageButton {id: num_key_back; imageSource: logicObject.getLocalImagePath() + "key_back.png";
                            bkgReleasedColor: "#efefef"
                            onClickedSignal: {
                                selectZone.setVisibleValue(false);
                                mainBoard.setVisibleValue(true);
                                numberBoard.setVisibleValue(false);
                                symbolBoard.setVisibleValue(false);
                            }
                        }
        }
    }

    Rectangle{

        property string key: "KEY_SYM"

        function resetStatus()
        {

        }

        function setVisibleValue(value)
        {
            visible = value;
        }

        id: symbolBoard
        anchors.top: selectZone.bottom
        anchors.left: parent.left
        width: parent.width
        height: 320

        Rectangle{
            anchors.top: parent.top
            anchors.left: parent.left
            z:  1
            width: 480
            height: 1
            color: "#e0e0e0"
            visible: true
        }
        Rectangle{
            anchors.top: parent.top
            anchors.topMargin: 79
            anchors.right: parent.right
            z:  1
            width: 120
            height: 1
            color: "#e0e0e0"
            visible: true
        }
        Rectangle{
            anchors.top: parent.top
            anchors.topMargin: 159
            anchors.right: parent.right
            z:  1
            width: 120
            height: 1
            color: "#e0e0e0"
            visible: true
        }
        Rectangle{
            anchors.top: parent.top
            anchors.topMargin: 239
            anchors.right: parent.right
            z:  1
            width: 120
            height: 1
            color: "#e0e0e0"
            visible: true
        }
        Rectangle{
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 360
            z:  1
            width: 1
            height: 320
            color: "#e0e0e0"
            visible: true
        }
        ListModel {

            function createListModel()
            {
                var count = keyboardObject.getStringListSize(symbolBoard.key);

                for (var i = 0; i < count; i++)
                {
                    symListModel.append({"nIndex": i});
                }
            }

            function clearListModel()
            {
               if (symListModel.count > 0)
               {
                    remove(0, symListModel.count);
               }
            }

            id: symListModel
        }
        GridView{
            id: symbolGridView
            width: 360
            height: 320
            anchors.top: parent.top
            anchors.left: parent.left
            boundsBehavior: Flickable.StopAtBounds
            snapMode: GridView.SnapToRow
            cellWidth: 90
            cellHeight: 80
            clip: true
            model: symListModel
            delegate: KeyTextButton{
                width: symbolGridView.cellWidth
                height: symbolGridView.cellHeight
                bottomLineVisible: true
                rightLineVisible: (index%4 == 3) ? false : true
                bkgReleasedColor: "#f8f8f8"
                dataText: keyboardObject.getCharFromKeyMap(symbolBoard.key, nIndex);
                onClickedSignal: {
                    addCharSignal(dataText);
                }
            }

            onMovementEnded: {
                symColumnZone.setUpAndDownStatus();
            }
        }
        Column {

            function resetSymColumnZone()
            {
                symbolGridView.contentY = 0;
                setUpAndDownStatus();
            }

            function setUpAndDownStatus()
            {
                if (symbolGridView.contentY <= 0)
                {
                    sym_key_up.disableStatus();
                }
                else
                {
                    sym_key_up.enableStatus();
                }
                if (symbolGridView.contentY < symbolGridView.contentHeight - symbolBoard.height)
                {
                    sym_key_down.enableStatus();
                }
                else
                {
                    sym_key_down.disableStatus();
                }
            }

            id: symColumnZone
            anchors.top: parent.top
            anchors.left: symbolGridView.right
            width: 120
            height: 80

            KeyImageButton{id: sym_key_del; imageSource: logicObject.getLocalImagePath() + "key_del.png";
                            bkgReleasedColor: "#efefef"
                            onClickedSignal: {
                                delCharSignal();
                            }
                        }
            KeyImageButton
            {
                function enableStatus()
                {
                    setEnabled(true);
                    setButtonImageSource(logicObject.getLocalImagePath() + "key_arrow_up_A.png");
                }

                function disableStatus()
                {
                    setEnabled(false);
                    setButtonImageSource(logicObject.getLocalImagePath() + "key_arrow_up_B.png");
                }

                id: sym_key_up;
                imageSource: logicObject.getLocalImagePath() + "key_arrow_up_A.png"
                bkgReleasedColor: "#efefef"
                onClickedSignal: {
                    if (symbolGridView.contentY >= symbolBoard.height)
                    {
                        symbolGridView.contentY = symbolGridView.contentY - symbolBoard.height;
                    }
                    else if (symbolGridView.contentY < symbolBoard.height && symbolGridView.contentY > 0)
                    {
                        symbolGridView.contentY = 0;
                    }

                    symColumnZone.setUpAndDownStatus();
                }
            }
            KeyImageButton {
                function enableStatus()
                {
                    setEnabled(true);
                    setButtonImageSource(logicObject.getLocalImagePath() + "key_arrow_down_A.png");
                }

                function disableStatus()
                {
                    setEnabled(false);
                    setButtonImageSource(logicObject.getLocalImagePath() + "key_arrow_down_B.png");
                }

                id: sym_key_down;
                imageSource: logicObject.getLocalImagePath() + "key_arrow_down_A.png"
                bkgReleasedColor: "#efefef"
                onClickedSignal: {

                    if (symbolGridView.contentY <= symbolGridView.contentHeight - 2*symbolBoard.height)
                    {
                        symbolGridView.contentY = symbolGridView.contentY + symbolBoard.height;
                    }
                    else if (symbolGridView.contentY > symbolGridView.contentHeight - 2*symbolBoard.height)
                    {
                        symbolGridView.contentY = symbolGridView.contentHeight - symbolBoard.height;
                    }

                    symColumnZone.setUpAndDownStatus();
                }
            }
            KeyImageButton {id: sym_key_back; imageSource: logicObject.getLocalImagePath() + "key_back.png";
                            bkgReleasedColor: "#efefef"
                            onClickedSignal: {
                                selectZone.setVisibleValue(false);
                                mainBoard.setVisibleValue(true);
                                numberBoard.setVisibleValue(false);
                                symbolBoard.setVisibleValue(false);
                            }
                        }
        }
    }

    Component.onCompleted: {
        rootItem.updateLanguage.connect(updateLanguageSlot);
    }
}

