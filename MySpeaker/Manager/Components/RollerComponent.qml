import QtQuick 2.0

Rectangle{

    property alias info: data.text
    property alias textScale: data.scale
    property alias textOpacity: data.opacity
    property alias dataColor: data.color

   Text{
       id: data
       anchors.horizontalCenter: parent.horizontalCenter
       anchors.verticalCenter: parent.verticalCenter
       color: "#303030"
       font.family: rootItem.fontName
       font.pixelSize: 26
       scale: textScale
       opacity: textOpacity
       text: info
       width: parent.width
       elide: Text.ElideRight
       horizontalAlignment: Text.AlignHCenter
   }
}
