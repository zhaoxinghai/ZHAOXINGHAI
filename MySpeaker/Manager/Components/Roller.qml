import QtQuick 2.0

PathView{

    signal changeValueSignal(var index);

    width: 100
    height: 100
    pathItemCount: 5
    preferredHighlightBegin: 0.5
    preferredHighlightEnd: 0.5
    highlightRangeMode: PathView.StrictlyEnforceRange
    maximumFlickVelocity: 1200
    flickDeceleration: 800
    clip: true
    //delegate: monthDelegate
    onMovementEnded: {
        changeValueSignal(currentIndex);
    }
}
