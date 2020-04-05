import QtQuick 2.0
import cn.cmos.service.imageDragWidget 1.0

//圆圈、图标和标签的控件
Rectangle {
        id: shutdownWidget
        anchors.fill: parent
        color: "black"
        //确认关机
        signal pullOutside
        //鼠标按下
        signal dragPress
        //鼠标在圆圈内移动
        signal dragMoveIn
        //鼠标在圆圈上移动
        signal dragMoveOn
        //鼠标释放
        signal dragRelease
        onDragPress: {
            dragOutText.visible = false
        }
        onDragMoveIn: {
            dragOutText.visible = false
            powerOffText.visible = false
        }
        onDragMoveOn: {
            powerOffText.visible = true
        }
        onDragRelease: {
            dragOutText.visible = true
            powerOffText.visible = false
        }
        //“拖动关机”的标签
        Text {
            id: dragOutText
            y: parent.width/6
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize:35
            color: "white"
            anchors.horizontalCenter: parent.horizontalCenter
        }
        //“确认关机”的标签
        Text {
            id: powerOffText
            visible: false
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize:35
            color: "white"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
        //圆圈和图标的控件
        ImageDragWidget {
            id: imagedragitem
            anchors.fill: parent
            Component.onCompleted: {
                // 连接控件信号和组件的槽
                this.dragOut.connect(shutdownWidget.pullOutside);
                this.dragPress.connect(shutdownWidget.dragPress);
                this.dragMoveIn.connect(shutdownWidget.dragMoveIn);
                this.dragMoveOn.connect(shutdownWidget.dragMoveOn);
                this.dragRelease.connect(shutdownWidget.dragRelease);
                console.log("open shutdown ImageDragWidget...")
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
            cmostranslate.langChanged.connect(shutdownWidget.languageChanged);
            // 调用组件函数初始化语言名称
            cmostranslate.trans();
        }
        //翻译
        function translator() {
            dragOutText.text = qsTr("Drag out to power off")
            powerOffText.text = qsTr("Power off")
        }
    }

