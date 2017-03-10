import QtQuick 2.0

Item {
    id: root;
    property int count: 50;
    property int staticCount: 1000;

    property real t;
    NumberAnimation on t { from: 0; to: 1; duration: 1000; loops: Animation.Infinite }
    onTChanged: {
        repeater.model = 0;
        repeater.model = root.count
    }

    Component.onCompleted: repeater.model = root.count

    Repeater {
        id: repeater
        Text {
            x: Math.random() * (root.width - width)
            y: Math.random() * (root.height - height)
            font.pixelSize: 10
//            renderType: Text.NativeRendering
            text: "天地玄黄，宇宙洪荒。日月盈昃，辰宿列张。寒来暑往，秋收冬藏。闰余成岁，律吕调阳。云腾致雨，露结为霜。金生丽水，玉出崑冈。剑号巨阙，珠称夜光。果珍李柰，菜重芥姜。海咸河淡，鳞潜羽翔。龙师火帝，鸟官人皇。始制文字，乃服衣裳。推位让国，有虞陶唐。弔民伐罪，周发商汤。坐朝问道，垂拱平章。爱育黎首，臣伏戎羌。"
        }
   }
}
