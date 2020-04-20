可交互的创意动态按钮
===

## 特点

- 所有颜色自定义
- 鼠标悬浮渐变
- 两种点击效果：鼠标点击渐变 / 水波纹动画（可多层波纹叠加）
- 额外鼠标移入/移出/按下/弹起的实时/延迟共8种事件
- 鼠标悬浮图标位置主动变化
- 鼠标拖动图标抖动反弹效果
- 鼠标进入父控件时开启出现效果，或启动时出现
- 延迟出现的动画效果（多个按钮连续）
- 记录开关状态
- 直接设置 x、y 的圆角显示
- 边框颜色设置
- 禁用时半透明+点击穿透效果
- 添加额外的边缘角标
- 三种前景模式：图标、文字、带遮罩的图标（任意变色）
- 支持QSS直接设置部分属性
- 与父类 QPushButton 兼容
- 时间准确性：根据时间戳计算动画进度，即使在低性能机器上也可准时完成动画
- 稳定性：一按钮多功能，完美兼容多种情况下焦点事件
- 极其强大的可扩展性，继承该按钮后可任意修改显示效果和动画效果



## 截图

![截图](pictures/picture.gif)



## 用法

```C++
// 方框按钮
InteractiveButtonBase* btn = new InteractiveButtonBase("text", this);
btn->setGeometry(300, 300, 100, 100);
```

```C++
// 圆形按钮带图标，以及鼠标靠近(父控件悬浮)出现动画
icon_btn = new WaterCircleButton(QIcon(":/icons/point_menu"), this);
icon_btn->setGeometry(100, 50, 32, 32);
icon_btn->setShowAni(true); // 调用showForeground2时出现动画
```

```C++
// 自定义颜色按钮（遮罩效果）
InteractiveButtonBase* pixmap_btn = new InteractiveButtonBase(QPixmap(":/icons/point_menu"), this);
pixmap_btn->setGeometry(132, 50, 32, 32);
```

```C++
// 椭圆形按钮
InteractiveButtonBase* float_btn = new WaterFloatButton("text", this);
float_btn->setGeometry(164, 50, 100, 32);
float_btn->setBgColor(QColor(102,51,204,192), QColor(102,51,204,255));
```

```C++
// 自定义按钮（win最小化按钮）
WinMenuButton* menu_btn = new WinMenuButton(this);
menu_btn->setGeometry(368, 10, 32, 32);
```



