import QtQuick 2.2
import "Circle.js" as Circle

Rectangle {
        id:shutdownArea
        color: "black"
        anchors.fill: parent
        //"确认关机"的信号
        signal pullOutside
        //圆圈中心坐标
        property real circle_centerX: width/2
        property real circle_centerY: height/2
        //图标中心坐标
        property point powerImageCenter: Qt.point(0, 0);
        //鼠标是否按下的标志
        property bool pressFlag: false
        //鼠标是否移出的标志
        property bool isOut: false
        Timer {
            id: shutdownAreaTimer
            property int stateFlag: 0
            interval: 1000; running: true; repeat: true; triggeredOnStart: true
            onTriggered:  {
                console.log("shutdownDragTimer trigger")
                //定时器切换状态,随后关掉定时器
                stateFlag = stateFlag + 1
                if(stateFlag == 1) {
                    shutdownArea.state = "begin"
                } else if(stateFlag == 2) {
                    shutdownArea.state = "middle"
                } else if(stateFlag == 3) {
                    shutdownArea.state = "end"
                } else {
                    shutdownAreaTimer.stop()
                }
            }
        }
        //“拖动关机”和“确认关机”的标签
        Text {
            id: dragText
            y: (parent.height-height)/align
            text: dragOutText
            property string dragOutText: ""
            property string powerOffText: ""
            property int align: 4
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize:35
            color: "white"
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Image {
            id: circleImage
            width: (452/580)*parent.width    //滑动范围圆圈图片尺寸452*452, 滑动图标的尺寸128*128
            // 图标所在图片尺寸为:350*350
            height: width
            anchors.centerIn: parent
            source: circle_default_image
            property string circle_default_image: "qrc:/images/circle_default.png"
            property string circle_bold_image: "qrc:/images/circle_bold.png"
        }
        Image {
            id: powerImage
            //滑动范围圆圈图片尺寸452*452, 滑动图标的尺寸128*128
            // 图标所在图片尺寸为:350*350
            x: (580-350)/(2*580)*parent.width
            y: (580-350)/(2*580)*parent.height
            width: (350/580)*parent.width
            height: width
            source: power_default_image
            property string power_default_image: "qrc:/images/power_default.png"
            property string power_halo_image: "qrc:/images/power_halo.png"
            property string power_solid_image: "qrc:/images/power_solid.png"
            function moveCenter(centerX, centerY) {
                powerImage.x = centerX - width/2
                powerImage.y = centerY - height/2
            }
        }
        state: "initializtion"
        states: [
            State {
                name: "initializtion"
                PropertyChanges { target: dragText; visible: false }
                PropertyChanges { target: dragText; opacity: 0.0 }
                PropertyChanges { target: circleImage; visible: false }
                PropertyChanges { target: circleImage; scale: 0.0 }
                PropertyChanges { target: powerImage; visible: false }
                PropertyChanges { target: powerImage; scale: 0.0 }
            },
            State {
                name: "begin"
                PropertyChanges { target: dragText; visible: false }
                PropertyChanges { target: dragText; opacity: 0.0 }
                PropertyChanges { target: circleImage; visible: true }
                PropertyChanges { target: circleImage; scale: 0.0 }
                PropertyChanges { target: powerImage; visible: true }
                PropertyChanges { target: powerImage; scale: 0.0 }
            },
            State {
                name: "middle"
                PropertyChanges { target: dragText; visible: false }
                PropertyChanges { target: dragText; opacity: 0.0 }
                PropertyChanges { target: circleImage; visible: true }
                PropertyChanges { target: circleImage; scale: 128/452 }
                PropertyChanges { target: powerImage; visible: true }
                PropertyChanges { target: powerImage; scale: 1.0 }
            },
            State {
                name: "end"
                PropertyChanges { target: dragText; visible: true }
                PropertyChanges { target: dragText; opacity: 1.0 }
                PropertyChanges { target: circleImage; visible: true }
                PropertyChanges { target: circleImage; scale: 1.0 }
                PropertyChanges { target: powerImage; visible: true }
                PropertyChanges { target: powerImage; scale: 1.0 }
            }
        ]
        transitions: [
            Transition {
                from: "initializtion"; to: "begin"
            },
            Transition {
                from: "begin"; to: "middle"
                PropertyAnimation {
                    target: circleImage
                    properties: "scale"; duration: 1000
                }
                PropertyAnimation {
                    target: powerImage
                    properties: "scale"; duration: 1000
                }
            },
            Transition {
                from: "middle"; to: "end"
                PropertyAnimation {
                    target: circleImage
                    properties: "scale"; duration: 1000
                }
                PropertyAnimation {
                    target: dragText
                    properties: "opacity"; duration: 1000; easing.type: Easing.InExpo
                }
            },
            Transition {
                from: "end"; to: "initializtion"
            }
        ]
        MouseArea {
            id: dragArea
            anchors.fill: parent
            onPressed: {
                var power_result = Circle.contain(mouse.x, mouse.y, circle_centerX, circle_centerY, (128/350)*powerImage.width/2);
                if(power_result !== 1) {
                    //图标上或内
                    pressFlag = true;
                    powerImage.source = powerImage.power_halo_image;
                    dragText.text = "";
                }
            }
            onReleased: {
                powerImage.moveCenter(circle_centerX, circle_centerY);
                pressFlag = false;
                powerImage.source = powerImage.power_default_image;
                circleImage.source = circleImage.circle_default_image;
                dragText.text = dragText.dragOutText;
                dragText.align = 4;
                if(isOut) {
                    shutdownArea.pullOutside()
                }
            }
            onPositionChanged: {
                if(pressFlag) {
                    var circle_result = Circle.contain(mouse.x, mouse.y, circle_centerX, circle_centerY, circleImage.width/2);
                    if(circle_result === 1) {
                        isOut = true;
                        //圆圈外
                        var power_result = Circle.contain(mouse.x, mouse.y, powerImageCenter.x, powerImageCenter.y, (128/350)*powerImage.width/2);
                        if(power_result === 1) {
                            //圈圈外且图标外
                            powerImage.moveCenter(circle_centerX, circle_centerY);
                            pressFlag = false;
                            powerImage.source = powerImage.power_default_image;
                            circleImage.source = circleImage.circle_default_image;
                            dragText.text = dragText.dragOutText;
                            dragText.align = 4;
                            //     shutdownArea.pullOutside()
                        } else {
                            //圈圈外且非图标外
                            var crossover_point = Circle.getPoint(mouse.x, mouse.y, circle_centerX, circle_centerY, circleImage.width/2);
                            console.log("交点： " + crossover_point)
                            powerImage.moveCenter(crossover_point[0], crossover_point[1]);
                            powerImageCenter = Qt.point(crossover_point[0], crossover_point[1])
                            powerImage.source = powerImage.power_solid_image;
                            circleImage.source = circleImage.circle_bold_image;
                            dragText.text = dragText.powerOffText;
                            dragText.align = 2;
                        }
                    } else if (circle_result === 0) {
                        //圆圈上
                        powerImageCenter = Qt.point(mouse.x, mouse.y)
                        powerImage.source = powerImage.power_solid_image;
                        circleImage.source = circleImage.circle_bold_image;
                        dragText.text = dragText.powerOffText;
                        dragText.align = 2;
                        isOut = true;
                    } else {
                        //圆圈内
                        powerImageCenter = Qt.point(mouse.x, mouse.y)
                        powerImage.moveCenter(mouse.x, mouse.y)
                        powerImage.source = powerImage.power_halo_image;
                        circleImage.source = circleImage.circle_default_image;
                        dragText.text = "";
                        isOut = false;
                    }
                }
                if(isOut&&(!pressFlag)) {
                    //鼠标在圆圈外且在图标外，确认关机。
                    shutdownArea.pullOutside()
                }
            }
        }
        //该控件的信号
        signal languageChanged(string name)
        //该控件的相应信号的响应函数
        onLanguageChanged: {
            translator()
        }
        Component.onCompleted: {
            // 连接组件信号和控件的槽
            cmostranslate.langChanged.connect(shutdownArea.languageChanged);
            // 调用组件函数初始化语言名称
            cmostranslate.trans();
        }
        //翻译
        function translator() {
            dragText.dragOutText = qsTr("Drag Down")
            dragText.powerOffText = qsTr("Power off")
        }
}


