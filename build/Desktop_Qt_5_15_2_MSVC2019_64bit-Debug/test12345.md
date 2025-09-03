# 1.1 Repeater类

## 1.说明

```markdown
Instantiates a number of Item-based components using a provided model.
使用提供的模型实例化许多基于项目的组件。

The Repeater type is used to create a large number of similar items. Like other view types, a Repeater has a model and a delegate: for each entry in the model, the delegate is instantiated in a context seeded with data from the model. A Repeater item is usually enclosed in a positioner type such as Row or Column to visually position the multiple delegate items created by the Repeater.
Repeater 类型用于创建大量类似的项目。与其他视图类型一样，Repeater 具有一个模型和一个委托：对于模型中的每个条目，委托在使用模型中的数据种子的上下文中实例化。Repeater 项通常包含在定位器类型（如 Row 或 Column）中，以直观地定位 Repeater 创建的多个委托项。

The following Repeater creates three instances of a Rectangle item within a Row:
以下 Repeater 在 Row 中创建 Rectangle 项的三个实例：
 import QtQuick

 Row {
     Repeater {
         model: 3
         Rectangle {
             width: 100; height: 40
             border.width: 1
             color: "yellow"
         }
     }
 }

A Repeater's model can be any of the supported data models. Additionally, like delegates for other views, a Repeater delegate can access its index within the repeater, as well as the model data relevant to the delegate. See the delegate property documentation for details.
Repeater的模型可以是任何受支持的数据模型。此外，与其他视图的委托一样，Repeater 委托可以访问其在 repeater 中的索引，以及与委托相关的模型数据。有关详细信息，请参阅委托属性文档。

Items instantiated by the Repeater are inserted, in order, as children of the Repeater's parent. The insertion starts immediately after the repeater's position in its parent stacking list. This allows a Repeater to be used inside a layout. For example, the following Repeater's items are stacked between a red rectangle and a blue rectangle:
由 Repeater 实例化的项按顺序作为 Repeater 父项的子项插入。插入操作会紧接在 Repeater 在其父项堆叠列表中的位置之后开始。这允许在布局中使用中继器。例如，以下 Repeater 的项目堆叠在红色矩形和蓝色矩形之间：
 Row {
     Rectangle { width: 10; height: 20; color: "red" }
     Repeater {
         model: 10
         Rectangle { width: 20; height: 20; radius: 10; color: "green" }
     }
     Rectangle { width: 10; height: 20; color: "blue" }
 }

Note: A Repeater item owns all items it instantiates. Removing or dynamically destroying an item created by a Repeater results in unpredictable behavior.
注意：Repeater 项拥有它实例化的所有项。删除或动态销毁由 Repeater 创建的项目会导致不可预测的行为。

Considerations when using Repeater
使用Repeater时的注意事项
The Repeater type creates all of its delegate items when the repeater is first created. This can be inefficient if there are a large number of delegate items and not all of the items are required to be visible at the same time. If this is the case, consider using other view types like ListView (which only creates delegate items when they are scrolled into view) or use the Dynamic Object Creation methods to create items as they are required.
Repeater 类型在首次创建时会生成它的所有 delegate 项。如果 delegate 项数量很多，而不需要同时显示所有项，这种做法可能效率不高。如果是这种情况，可以考虑使用其他视图类型，例如 ListView（它只会在滚动到可见区域时才创建 delegate 项），或者使用动态对象创建的方法，根据需要生成项。
Also, note that Repeater is Item-based, and can only repeat Item-derived objects. For example, it cannot be used to repeat QtObjects:
另请注意，Repeater 是基于 Item 的，并且只能重复 Item 派生的对象。例如，它不能用于重复 QtObjects：
 // bad code:
 Item {
     // Can't repeat QtObject as it doesn't derive from Item.
     Repeater {
         model: 10
         QtObject {}
     }
 }

```

## 2.重要属性

### [read-only] count : int

```markdown
This property holds the number of items in the model.
此属性保存模型中的项数。
```

### enabled : bool

```markdown
This flags enables key handling if true (default); otherwise no key handlers will be called.
如果为true（默认），此标志启用按键处理；否则将不调用按键处理程序。

Note: The number of items in the model as reported by count may differ from the number of created delegates if the Repeater is in the process of instantiating delegates or is incorrectly set up.
注意：如果 Repeater 正在实例化 delegate，或者设置不正确，模型中 count 报告的项数可能会与已创建的 delegate 数量不同。
```

### [default] delegate : Component

```markdown
The delegate provides a template defining each item instantiated by the repeater.
delegate 提供了一个模板，用于定义 Repeater 实例化的每个项。
Delegates are exposed to a read-only index property that indicates the index of the delegate within the repeater. For example, the following Text delegate displays the index of each repeated item:
delegate 可以访问一个只读的 index 属性，该属性表示该 delegate 在 Repeater 中的索引。例如，下面的 Text delegate 会显示每个重复项的索引：
 Column {
     Repeater {
         model: 10
         Text {
             required property int index
             text: "I'm item " + index
         }
     }
 }
I'm item 0
I'm item 1
...
I'm item 9
If the model is a string list or object list, the delegate is also exposed to a read-only modelData property that holds the string or object data. For example:
如果 model 是一个字符串列表或对象列表，delegate 还可以访问一个只读的 modelData 属性，该属性保存对应的字符串或对象数据。例如：
 Column {
     Repeater {
         model: ["apples", "oranges", "pears"]
         Text {
             required property string modelData
             text: "Data: " + modelData
         }
     }
 }
Data: apples
Data: oranges
Data: pears
If the model is a model object (such as a ListModel) the delegate can access all model roles as named properties, in the same way that delegates do for view classes like ListView.
如果 model 是一个模型对象（例如 ListModel），delegate 可以像 ListView 等视图类的 delegate 一样，通过命名属性访问模型的所有角色（roles）。
```

### model : var

```markdown
The model providing data for the repeater.
This property can be set to any of the supported data models:
    A number that indicates the number of delegates to be created by the repeater
    A model (e.g. a ListModel item, or a QAbstractItemModel subclass)
    A string list
    An object list
The type of model affects the properties that are exposed to the delegate.

为 Repeater 提供数据的模型。
该属性可以设置为任何受支持的数据模型：
    一个数字，表示 Repeater 要创建的 delegate 项数量
    一个模型（例如 ListModel 项，或 QAbstractItemModel 的子类）
    一个字符串列表
    一个对象列表
模型的类型会影响可供 delegate 访问的属性。
```

| Constant        | Description                                                  |
| --------------- | ------------------------------------------------------------ |
| Keys.BeforeItem | （默认）在正常项按键处理之前处理按键事件。 如果事件被接受，它将不会传递到项目。 |
| Keys.AfterItem  | 在正常项目密按键处理后处理按键事件。 如果项接受按键事件，则不会由Keys 附加属性处理程序处理它。 |

## 3.重要信号

### itemAdded(int index, Item item)

```markdown
This signal is emitted when an item is added to the repeater. The index parameter holds the index at which the item has been inserted within the repeater, and the item parameter holds the Item that has been added.
Note: The corresponding handler is onItemAdded.
当有项被添加到 Repeater 时，会发出此信号。index 参数表示该项在 Repeater 中被插入的位置，item 参数表示被添加的 Item。
注意：对应的处理函数是 onItemAdded。
```

### itemRemoved(int index, Item item)

```markdown
This signal is emitted when an item is removed from the repeater. The index parameter holds the index at which the item was removed from the repeater, and the item parameter holds the Item that was removed.
Do not keep a reference to item if it was created by this repeater, as in these cases it will be deleted shortly after the signal is handled.
Note: The corresponding handler is onItemRemoved.
当有项从 Repeater 中被移除时，会发出此信号。index 参数表示该项在 Repeater 中被移除的位置，item 参数表示被移除的 Item。
如果该 item 是由 Repeater 创建的，不要保留对它的引用，因为在信号处理完成后，它很快就会被删除。
注意：对应的处理函数是 onItemRemoved。
```

## 4.重要方法

### Item itemAt(index)

```markdown
Returns the Item that has been created at the given index, or null if no item exists at index.
返回在给定索引下创建的项目，如果索引中不存在项目，则返回空值。
```

# 1.1 Flickable 类

## 1.说明

```markdown
Provides a surface that can be "flicked".
提供一个可以“轻扫”的表面。

The Flickable item places its children on a surface that can be dragged and flicked, causing the view onto the child items to scroll. This behavior forms the basis of Items that are designed to show large numbers of child items, such as ListView and GridView.
Flickable 项将其子项放置在一个可以拖动和快速滑动的表面上，从而使子项的视图可以滚动。这种行为是设计用来显示大量子项的控件（如 ListView 和 GridView）的基础。

In traditional user interfaces, views can be scrolled using standard controls, such as scroll bars and arrow buttons. In some situations, it is also possible to drag the view directly by pressing and holding a mouse button while moving the cursor. In touch-based user interfaces, this dragging action is often complemented with a flicking action, where scrolling continues after the user has stopped touching the view.
在传统用户界面中，视图可以使用标准控件进行滚动，例如滚动条和箭头按钮。在某些情况下，也可以通过按住鼠标按钮并移动光标直接拖动视图。在基于触控的用户界面中，这种拖动动作通常会与快速滑动（flicking）动作结合使用，即用户停止触控后，滚动仍会继续。

Flickable does not automatically clip its contents. If it is not used as a full-screen item, you should consider setting the clip property to true.
Flickable 不会自动裁剪其内容。如果它不是全屏使用，建议将 clip 属性设置为 true。

The following example shows a small view onto a large image in which the user can drag or flick the image in order to view different parts of it.
下面的示例展示了一个对大图的小视图，用户可以通过拖动或快速滑动（flick）图像来查看不同部分。

import QtQuick

Flickable {
    width: 200; height: 200
    contentWidth: image.width; contentHeight: image.height

    Image { id: image; source: "bigImage.png" }
}


Items declared as children of a Flickable are automatically parented to the Flickable's contentItem. This should be taken into account when operating on the children of the Flickable; it is usually the children of contentItem that are relevant.
声明为 Flickable 子项的元素会自动成为 Flickable 的 contentItem 的子项。在操作 Flickable 的子项时需要注意，通常真正相关的是 contentItem 的子项。

For example, the bound of Items added to the Flickable will be available by contentItem.childrenRect
例如，添加到 Flickable 的子项的边界可以通过 contentItem.childrenRect 获取。

The following images demonstrate a flickable being flicked in various directions and the resulting contentX and contentY values. The blue square represents the flickable's content, and the black border represents the bounds of the flickable.
下面的图示演示了 Flickable 被在不同方向快速滑动（flick）时，contentX 和 contentY 的变化。蓝色方块表示 Flickable 的内容，黑色边框表示 Flickable 的边界。

Note: Due to an implementation detail, items placed inside a Flickable cannot anchor to the Flickable. Instead, use parent, which refers to the Flickable's contentItem. The size of the content item is determined by contentWidth and contentHeight.
注意：由于实现细节，放在 Flickable 内的子项无法直接锚定（anchor）到 Flickable 本身。应使用 parent，它指向 Flickable 的 contentItem。内容项的大小由 contentWidth 和 contentHeight 决定。
```

## 2.重要属性

## 3.重要信号

## 4.重要方法

# 1.2 ScrollView 类(Inherits: Control)

## 1.说明

```markdown
Scrollable view.
可滚动视图。

ScrollView provides scrolling for user-defined content. It can be used to either replace a Flickable, or to decorate an existing one.
ScrollView 为用户定义的内容提供滚动功能。它可以用来替代 Flickable，或者装饰已有的 Flickable。

The first example demonstrates the simplest usage of ScrollView.
第一个示例展示了 ScrollView 的最简单用法。

ScrollView {
    width: 200
    height: 200

    Label {
        text: "ABC"
        font.pixelSize: 224
    }
}

The second example illustrates using an existing Flickable, that is, a ListView.
第二个示例说明如何在已有的 Flickable（如 ListView）上使用 ScrollView。

ScrollView {
    width: 200
    height: 200

    ListView {
        model: 20
        delegate: ItemDelegate {
            text: "Item " + index

            required property int index
        }
    }
}

Note: As of Qt-6.0, ScrollView automatically clips its contents if you don't use a Flickable as a child. If this is not wanted, you can set your own Flickable as a child, and control the clip property on the Flickable explicitly.
注意：从 Qt 6.0 开始，如果没有使用 Flickable 作为子项，ScrollView 会自动裁剪其内容。如果不希望这样，可以自己将 Flickable 作为子项，并显式控制 Flickable 的 clip 属性。
```

```
Sizing(尺寸设置)
As with Flickable, there are several things to keep in mind when using ScrollView:
和 Flickable 一样，使用 ScrollView 时需要注意以下几点：

1.If only a single item is used within a ScrollView, the content size is automatically calculated based on the implicit size of its contained item. However, if more than one item is used (or an implicit size is not provided), the contentWidth and contentHeight properties must be set to the combined size of its contained items.
如果 ScrollView 内只使用单个子项，内容大小会根据该子项的隐式大小自动计算。但如果有多个子项（或未提供隐式大小），必须手动设置 contentWidth 和 contentHeight 为子项组合的大小。

2.If the content size is less than or equal to the size of the ScrollView, it will not be scrollable.
如果内容大小小于或等于 ScrollView 的尺寸，则无法滚动。

3.If you want the ScrollView to only scroll vertically, you can bind contentWidth to availableWidth (and vice versa for contentHeight). This will let the contents fill out all the available space horizontally inside the ScrollView, taking any padding or scroll bars into account.
如果希望 ScrollView 仅垂直滚动，可以将 contentWidth 绑定到 availableWidth（反之亦然）。这样内容会水平填充 ScrollView 内所有可用空间，同时考虑任何内边距或滚动条。


Scroll Bars(滚动条)
The horizontal and vertical scroll bars can be accessed and customized using the ScrollBar.horizontal and ScrollBar.vertical attached properties.
水平和垂直滚动条可以通过 ScrollBar.horizontal 和 ScrollBar.vertical 附加属性访问和自定义。

The following example adjusts the scroll bar policies so that the horizontal scroll bar is always off, and the vertical scroll bar is always on.
下面的示例设置滚动条策略，使水平滚动条始终关闭，垂直滚动条始终开启：

ScrollView {
    // ...
    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
    ScrollBar.vertical.policy: ScrollBar.AlwaysOn
}


Touch vs. Mouse Interaction(触控与鼠标交互)

On touch, ScrollView enables flicking and makes the scroll bars non-interactive.
在触控操作下，ScrollView 启用快速滑动（flicking），并且滚动条不可交互。

When interacted with a mouse device, flicking is disabled and the scroll bars are interactive.
使用鼠标操作时，快速滑动被禁用，滚动条可以交互。

Scroll bars can be made interactive on touch, or non-interactive when interacted with a mouse device, by setting the interactive property explicitly to true or false, respectively.
可以通过显式设置 interactive 属性来修改行为：在触控下设置为 true 使滚动条可交互，或在鼠标操作下设置为 false 使滚动条不可交互。

ScrollView {
    // ...
    ScrollBar.horizontal.interactive: true
    ScrollBar.vertical.interactive: true
}
```

## 2.重要属性

### contentChildren : list&lt;Item&gt;

```
This property holds the list of content children.
该属性保存内容子项（content children）的列表。

The list contains all items that have been declared in QML as children of the view.
该列表包含所有在 QML 中声明为视图子项的元素。

Note: Unlike contentData, contentChildren does not include non-visual QML objects.
注意：与 contentData 不同，contentChildren 不包含非可视的 QML 对象。
```

### [default] contentData : list&lt;QtObject&gt;

```
This property holds the list of content data.
该属性保存内容数据（content data）的列表。

The list contains all objects that have been declared in QML as children of the view.
该列表包含所有在 QML 中声明为视图子项的对象。

Note: Unlike contentChildren, contentData does include non-visual QML objects.
注意：与 contentChildren 不同，contentData 包含非可视的 QML 对象。
```

# 1.3 Pane类

## 1.说明

```markdown
Provides a background matching with the application style and theme.
提供与应用程序样式和主题相匹配的背景。

Pane provides a background color that matches with the application style and theme.
Pane 提供与应用程序样式和主题相匹配的背景颜色。

Pane does not provide a layout of its own, but requires you to position its contents, for instance by creating a RowLayout or a ColumnLayout.
Pane 本身不提供布局，需要你自己对其内容进行定位，例如通过创建 RowLayout 或 ColumnLayout。

Items declared as children of a Pane are automatically parented to the Pane's contentItem.
声明为 Pane 子项的元素会被自动设置为 Pane 的 contentItem 的子项。

Items created dynamically need to be explicitly parented to the contentItem.
动态创建的元素则需要显式地设置其父项为 contentItem。

As mentioned in Event Handling, Pane does not let click and touch events through to items beneath it.
如在 事件处理 (Event Handling) 中提到的，Pane 不会让点击和触摸事件传递到其下方的元素。

If wheelEnabled is true, the same applies to mouse wheel events.
如果 wheelEnabled 为 true，同样适用于鼠标滚轮事件。
```

```
Content Sizing(内容大小调整)

If only a single item is used within a Pane, it will resize to fit the implicit size of its contained item.
如果 Pane 中只包含一个元素，它会自动调整大小以适应该元素的隐式大小。
This makes it particularly suitable for use together with layouts.
这使得它特别适合与布局一起使用。
 Pane {
     ColumnLayout {
         anchors.fill: parent
         CheckBox { text: qsTr("E-mail") }
         CheckBox { text: qsTr("Calendar") }
         CheckBox { text: qsTr("Contacts") }
     }
 }
 
Sometimes there might be two items within the pane:
有时 Pane 中可能会有两个元素：
 Pane {
     SwipeView {
         // ...
     }
     PageIndicator {
         anchors.horizontalCenter: parent.horizontalCenter
         anchors.bottom: parent.bottom
     }
 }

In this case, Pane cannot calculate a sensible implicit size. Since we're anchoring the PageIndicator over the SwipeView, we can simply set the content size to the view's implicit size:
在这种情况下，Pane 无法计算出合理的隐式大小。由于我们将 PageIndicator 锚定在 SwipeView 上，因此可以直接将内容大小设置为视图的隐式大小：
 Pane {
     contentWidth: view.implicitWidth
     contentHeight: view.implicitHeight

     SwipeView {
         id: view
         // ...
     }
     PageIndicator {
         anchors.horizontalCenter: parent.horizontalCenter
         anchors.bottom: parent.bottom
     }
  }

If the contentItem has no implicit size and only one child, Pane will use the implicit size of that child. For example, in the following code, the Pane assumes the size of the Rectangle:
如果 contentItem 没有隐式大小且只有一个子元素，Pane 会使用该子元素的隐式大小。例如，在下面的代码中，Pane 会采用 Rectangle 的大小：
 Pane {
     Item {
         Rectangle {
             implicitWidth: 200
             implicitHeight: 200
             color: "salmon"
         }
     }
 }

```

## 2.重要属性

### contentChildren : list&lt;Item&gt;

```
This property holds the list of content children.
此属性保存内容子元素的列表。

The list contains all items that have been declared in QML as children of the pane.
该列表包含所有在 QML 中声明为 Pane 子元素的项。

Note: Unlike contentData, contentChildren does not include non-visual QML objects.
注意：与 contentData 不同，contentChildren 不包括非可视的 QML 对象。
```

### [default] contentData : list&lt;QtObject&gt;

```
This property holds the list of content data.
此属性保存内容数据的列表。

The list contains all objects that have been declared in QML as children of the pane.
该列表包含所有在 QML 中声明为 Pane 子元素的对象。

Note: Unlike contentChildren, contentData does include non-visual QML objects.
注意：与 contentChildren 不同，contentData 包括非可视的 QML 对象。
```

## contentHeight : real

```
This property holds the content height. It is used for calculating the total implicit height of the pane.
此属性保存内容高度，用于计算 Pane 的总隐式高度。
```

### contentWidth : real

```
This property holds the content width. It is used for calculating the total implicit width of the pane.
此属性保存内容宽度，用于计算 Pane 的总隐式宽度。
```



# 2.1 Text类

## 1.说明

```markdown
Specifies how to add formatted text to a scene.
指定如何将格式化文本添加到场景中。

Text items can display both plain and rich text. For example, red text with a specific font and size can be defined like this:
Text 项可以显示纯文本和富文本。例如，可以如下定义一个具有特定字体和大小的红色文本：

Text {
     text: "Hello World!"
     font.family: "Helvetica"
     font.pointSize: 24
     color: "red"
 }


Rich text is defined using HTML-style markup:
富文本通过 HTML 风格的标记来定义：

Text {
     text: "<b>Hello</b> <i>World!</i>"
 }


If height and width are not explicitly set, Text will attempt to determine how much room is needed and set it accordingly. Unless wrapMode is set, it will always prefer width to height (all text will be placed on a single line).
如果没有显式设置高度和宽度，Text 会尝试自动计算所需的空间并相应设置。除非设置了 wrapMode，否则它始终优先扩展宽度而不是高度（所有文本都会显示在同一行上）。

The elide property can alternatively be used to fit a single line of plain text to a set width.
elide 属性也可以用来让单行纯文本适应指定的宽度。

Note that the Supported HTML Subset is limited. Also, if the text contains HTML img tags that load remote images, the text is reloaded.
需要注意的是，支持的 HTML 子集是有限的。此外，如果文本中包含加载远程图像的 HTML <img> 标签，文本将会重新加载。

Text provides read-only text. For editable text, see TextEdit.
Text 提供只读文本。如果需要可编辑文本，请参阅 TextEdit。
```

## 2.重要属性

### [read-only] effectiveHorizontalAlignment : enumeration

### horizontalAlignment : enumeration

### verticalAlignment : enumeration

```markdown
Sets the horizontal and vertical alignment of the text within the Text item’s width and height. By default, the text is vertically aligned to the top. Horizontal alignment follows the natural alignment of the text, for example text that is read from left to right will be aligned to the left.
设置文本项宽度和高度内文本的水平和垂直对齐方式。默认情况下，文本在垂直方向上对齐到顶部。水平方向的对齐遵循文本的自然对齐方式，例如，从左到右阅读的文本会左对齐。

The valid values for horizontalAlignment are Text.AlignLeft, Text.AlignRight, Text.AlignHCenter and Text.AlignJustify. The valid values for verticalAlignment are Text.AlignTop, Text.AlignBottom and Text.AlignVCenter.
horizontalAlignment 的有效值为：Text.AlignLeft、Text.AlignRight、Text.AlignHCenter 和 Text.AlignJustify。
verticalAlignment 的有效值为：Text.AlignTop、Text.AlignBottom 和 Text.AlignVCenter。

Note that for a single line of text, the size of the text is the area of the text. In this common case, all alignments are equivalent. If you want the text to be, say, centered in its parent, then you will need to either modify the Item::anchors, or set horizontalAlignment to Text.AlignHCenter and bind the width to that of the parent.
请注意，对于单行文本，文本的区域就是文本的实际大小。在这种常见情况下，所有对齐方式是等效的。
如果希望文本在父项中居中，可以修改 Item::anchors，或者将 horizontalAlignment 设置为 Text.AlignHCenter，并将宽度绑定到父项的宽度。

When using the attached property LayoutMirroring::enabled to mirror application layouts, the horizontal alignment of text will also be mirrored. However, the property horizontalAlignment will remain unchanged. To query the effective horizontal alignment of Text, use the read-only property effectiveHorizontalAlignment.
当使用附加属性 LayoutMirroring::enabled 镜像应用程序布局时，文本的水平对齐方式也会被镜像。
但属性 horizontalAlignment 本身不会改变。若要获取 Text 的实际水平对齐方式，请使用只读属性 effectiveHorizontalAlignment。
```

### bottomPadding : real

### leftPadding : real

### padding : real

### rightPadding : real

### topPadding : real

```
These properties hold the padding around the content. This space is reserved in addition to the contentWidth and contentHeight.
这些属性用于保存内容周围的填充（padding）。这部分空间是在 contentWidth 和 contentHeight 之外额外预留的。
```

### [read-only] advance : size

```
The distance, in pixels, from the baseline origin of the first character of the text item, to the baseline origin of the first character in a text item occurring directly after this one in a text flow.
从当前文本项第一个字符的基线原点，到文本流中紧跟其后的文本项第一个字符基线原点的距离（单位：像素）。

Note that the advance can be negative if the text flows from the right to the left.
请注意，如果文本的流向是从右到左，则该 advance 值可能为负数。
```

### antialiasing : bool

```
Used to decide if the Text should use antialiasing or not. Only Text with renderType of Text.NativeRendering can disable antialiasing.
用于决定 Text 是否启用抗锯齿。只有当 renderType 为 Text.NativeRendering 时，才能禁用抗锯齿。

The default is true.
默认值为 true。
```

### baseUrl : url

```
This property specifies a base URL which is used to resolve relative URLs within the text.
此属性用于指定一个 基本 URL，以解析文本中的相对 URL。

Urls are resolved to be within the same directory as the target of the base URL meaning any portion of the path after the last '/' will be ignored.
相对 URL 会被解析到与基本 URL 目标所在的目录中，这意味着基本 URL 中最后一个 “/” 之后的路径部分将被忽略。
```

| Base URL                                 | Relative URL       | Resolved URL                                  |
| ---------------------------------------- | ------------------ | --------------------------------------------- |
| http://qt-project.org/                   | images/logo.png    | http://qt-project.org/images/logo.png         |
| http://qt-project.org/index.html         | images/logo.png    | http://qt-project.org/images/logo.png         |
| http://qt-project.org/content            | images/logo.png    | http://qt-project.org/content/images/logo.png |
| http://qt-project.org/content/           | images/logo.png    | http://qt-project.org/content/images/logo.png |
| http://qt-project.org/content/index.html | images/logo.png    | http://qt-project.org/content/images/logo.png |
| http://qt-project.org/content/index.html | ../images/logo.png | http://qt-project.org/images/logo.png         |
| http://qt-project.org/content/index.html | images/logo.png    | http://qt-project.org/images/logo.png         |

```
The default value is the url of the QML file instantiating the Text item.
默认值是实例化 Text 项的 QML 文件的 url。
```

### clip : bool

```
This property holds whether the text is clipped.
此属性用于指定文本是否被剪切。

Note that if the text does not fit in the bounding rectangle it will be abruptly chopped.
请注意，如果文本无法容纳在边界矩形内，它会被直接截断。

If you want to display potentially long text in a limited space, you probably want to use elide instead.
如果需要在有限空间内显示可能较长的文本，建议使用 elide 属性来替代。
```

### color : color

```
The text color.
文本颜色。

An example of green text defined using hexadecimal notation:
使用十六进制表示法定义绿色文本的示例：

 Text {
     color: "#00FF00"
     text: "green text"
 }

An example of steel blue text defined using an SVG color name:
使用 SVG 颜色名称定义钢蓝色文本的示例：
 Text {
     color: "steelblue"
     text: "blue text"
 }

```

### [read-only] contentHeight : real

```
Returns the height of the text, including height past the height which is covered due to there being more text than fits in the set height.
返回文本的总高度，包括因文本超过设定高度而被遮挡（未显示）的部分所占的高度。
```

### [read-only] contentWidth : real

```
Returns the width of the text, including the portion that extends beyond the set width if WrapMode is enabled and the text does not wrap sufficiently.
返回文本的宽度，包括在启用 WrapMode 且文本未充分换行时超出设定宽度的部分。
```

### elide : enumeration

```
Set this property to elide parts of the text fit to the Text item's width. The text will only elide if an explicit width has been set. This property cannot be used with rich text.
设置此属性可在文本超过 Text 项宽度时省略部分内容。只有在明确设置了宽度时，文本才会被省略。
该属性不能用于 富文本（rich text）。

Elide modes are:
省略模式如下：

Constant			Description				说明
Text.ElideNone		the default				默认，不省略
Text.ElideLeft		elide at the left		左侧省略
Text.ElideMiddle	elide in the middle		中间省略
Text.ElideRight		elide at the right		右侧省略

If this property is set to Text.ElideRight, it can be used with wrapped text. The text will only elide if maximumLineCount, or height has been set. If both maximumLineCount and height are set, maximumLineCount will apply unless the lines do not fit in the height allowed.
如果该属性设置为 Text.ElideRight，它可以和自动换行文本一起使用。只有在设置了 maximumLineCount 或 height 时，文本才会被省略。
如果同时设置了 maximumLineCount 和 height，则优先使用 maximumLineCount，除非文本行数超过允许的高度。

If the text is a multi-length string, and the mode is not Text.ElideNone, the first string that fits will be used, otherwise the last will be elided.
如果文本是一个 多长度字符串（multi-length string），且省略模式不是 Text.ElideNone，将使用第一个能够完全显示的字符串，否则最后一个字符串会被省略。

Multi-length strings are ordered from longest to shortest, separated by the Unicode "String Terminator" character U+009C (write this in QML with "\u009C" or "\x9C").
多长度字符串按照从长到短的顺序排列，用 Unicode “字符串终止符” 字符 U+009C 分隔（在 QML 中可以写作 "\u009C" 或 "\x9C"）。
```

### font.bold : bool

```
Sets whether the font weight is bold.
设置字体是否为粗体。
```

### font.capitalization : enumeration

```
Sets the capitalization for the text.
设置文本的大小写方式。

Constant			Description									常量说明
Font.MixedCase		The default, no change to the case.			默认情况，不修改大小写。
Font.AllUppercase	Renders the text as all uppercase.			将文本渲染为全大写。
Font.AllLowercase	Renders the text as all lowercase.			将文本渲染为全小写。
Font.SmallCaps		Renders the text using small caps.			将文本渲染为小型大写字母（small-caps）。
Font.Capitalize		Capitalizes the first letter of each word.	将每个单词的首字母大写。
```

### font.family : string

```
Sets the family name of the font. The family name is case insensitive and may optionally include a foundry name, e.g. "Helvetica [Cronyx]". If the family is available from more than one foundry and the foundry isn't specified, an arbitrary foundry is chosen. If the family isn't available a family will be set using the font matching algorithm.
设置字体的族名（family name）。族名不区分大小写，并且可以选择性地包含字体厂商名称，例如 "Helvetica [Cronyx]"。如果某个字体族由多个厂商提供但未指定厂商，则会任意选择一个厂商。如果指定的字体族不可用，则会通过字体匹配算法选择一个可用的字体族。
```

### font.hintingPreference : enumeration

```
Sets the preferred hinting on the text. This is a hint to the underlying text rendering system to use a certain level of hinting, and has varying support across platforms. See the table in the documentation for QFont::HintingPreference for more details.
设置文本的首选 hinting（字体微调） 方式。它是对底层文本渲染系统的一种提示，用于指定某种 hinting 水平，不同平台的支持程度可能不同。更多细节可参考 QFont::HintingPreference 文档中的表格。

Note: This property only has an effect when used together with render type Text.NativeRendering.
注意：此属性只有在 renderType 设置为 Text.NativeRendering 时才会生效。

Constant					 Description
Font.PreferDefaultHinting：	使用目标平台的默认 hinting 水平。

Font.PreferNoHinting：		尽可能不对字形轮廓进行 hinting。文本布局将符合排版标准，例如打印时的度量方式。

Font.PreferVerticalHinting：	尽可能只进行垂直方向 hinting，不进行水平方向 hinting，使字形在垂直方向对齐像素网格，在低像素密度屏幕上更清晰。但因水平方向未 hinting，文本在高分辨率设备上可缩放而不影响排版。

Font.PreferFullHinting：		尽可能在水平和垂直方向都进行 hinting，以优化目标设备上的可读性。但因度量依赖于目标尺寸，字形位置、换行和其他排版细节不会随设备缩放，因此在不同分辨率设备上可能呈现不同布局。
Text { 
    text: "Hello"; 
    renderType: Text.NativeRendering; 
    font.hintingPreference: Font.PreferVerticalHinting 
}
```

### font.italic : bool

```
Sets whether the font has an italic style.
设置字体是否为斜体样式。
```

### font.kerning : bool

```
Enables or disables the kerning OpenType feature when shaping the text. Disabling this may improve performance when creating or changing the text, at the expense of some cosmetic features. The default value is true.
在排版文本时启用或禁用 OpenType 的字距调整功能。禁用此功能可能会在创建或修改文本时提升性能，但会牺牲一些外观效果。默认值为 true。
 Text { text: "OATS FLAVOUR WAY"; font.kerning: false }
```

### font.letterSpacing : real

```
Sets the letter spacing for the font.
设置字体的 字母间距（letter spacing）。

Letter spacing changes the default spacing between individual letters in the font. A positive value increases the letter spacing by the corresponding pixels; a negative value decreases the spacing.
字母间距会改变字体中各个字母之间的默认间距。正值会增加字母之间的间距（以像素为单位），负值则会减小间距。
```

### font.pixelSize : int

```
Sets the font size in pixels.
Using this function makes the font device dependent. Use pointSize to set the size of the font in a device independent manner.
以像素为单位设置字体大小。使用此属性会使字体大小依赖于设备分辨率。若要以设备无关的方式设置字体大小，请使用 pointSize。
```

### font.pointSize : real

```
Sets the font size in points. The point size must be greater than zero.
以 点（point） 为单位设置字体大小。
点大小必须大于零。
```

### font.preferShaping : bool

```
Sometimes, a font will apply complex rules to a set of characters in order to display them correctly. In some writing systems, such as Brahmic scripts, this is required in order for the text to be legible, but in e.g. Latin script, it is merely a cosmetic feature.
有些字体会对一组字符应用复杂的排版规则，以确保正确显示。在某些文字系统中（例如婆罗米系文字），这是文本可读所必需的；而在拉丁文字中，这通常只是一个美观上的功能。

Setting the preferShaping property to false will disable all such features when they are not required, which will improve performance in most cases. The default value is true.
将 preferShaping 属性设置为 false 时，会在不需要这些排版特性的情况下禁用它们，从而在大多数情况下提升性能。该属性的默认值为 true。

Text { text: "Some text"; font.preferShaping: false }
```

### font.strikeout : bool

```
Sets whether the font has a strikeout style.
设置字体是否为删除线样式。
```

### font.styleName : string

```
Sets the style name of the font.
设置字体的样式名称。

The style name is case insensitive.
样式名称不区分大小写。

If set, the font will be matched against style name instead of the font properties font.weight, font.bold and font.italic.
如果设置了样式名称，字体将根据样式名称进行匹配，而不是根据 font.weight、font.bold 和 font.italic 等字体属性进行匹配。
```

### font.underline : bool

```
Sets whether the text is underlined.
设置文本是否显示下划线。
```

### font.weight : int

```
The requested weight of the font. The weight requested must be an integer between 1 and 1000, or one of the predefined values:
请求的字体粗细。请求的粗细值必须是 1 到 1000 之间的整数，或预定义值之一：
Constant				Description
Font.Thin				100
Font.ExtraLight			200
Font.Light				300
Font.Normal				400 (default)
Font.Medium				500
Font.DemiBold			600
Font.Bold				700
Font.ExtraBold			800
Font.Black				900 
Text { text: "Hello"; font.weight: Font.DemiBold }
```

### font.wordSpacing : real

```
Sets the word spacing for the font.
设置字体的 字间距（word spacing）。

Word spacing changes the default spacing between individual words. A positive value increases the word spacing by a corresponding amount of pixels, while a negative value decreases the inter-word spacing accordingly.
字间距会改变单词之间的默认间距。正值会按像素增加字间距，负值则会相应减少字间距。
```

### [read-only] fontInfo.bold : bool

```
The bold state of the font info that has been resolved for the current font and fontSizeMode. This is true if the weight of the resolved font is bold or higher.
当前字体及其 fontSizeMode 解析后得到的字体信息的粗体状态。如果解析得到的字体粗细为粗体或更高，则该值为 true。
```

### [read-only] fontInfo.family : string

```
The family name of the font that has been resolved for the current font and fontSizeMode.
当前字体及其 fontSizeMode 解析后得到的字体信息的族名。
```

### [read-only] fontInfo.italic : bool

```
The italic state of the font info that has been resolved for the current font and fontSizeMode.
当前字体及其 fontSizeMode 解析后得到的字体信息的斜体状态。
```

### [read-only] fontInfo.pixelSize : string

```
The pixel size of the font info that has been resolved for the current font and fontSizeMode.
当前字体及其 fontSizeMode 解析后得到的字体信息的像素大小。
```

### [read-only] fontInfo.pointSize : real

```
The pointSize of the font info that has been resolved for the current font and fontSizeMode.
当前字体及其 fontSizeMode 解析后得到的字体信息的点大小。
```

#### [read-only] fontInfo.styleName : string

```
The style name of the font info that has been resolved for the current font and fontSizeMode.
当前字体及其 fontSizeMode 解析后得到的字体信息的样式名称。
```

### [read-only] fontInfo.weight : int

```
The weight of the font info that has been resolved for the current font and fontSizeMode.
当前字体及其 fontSizeMode 解析后得到的字体信息的字重。
```

### fontSizeMode : enumeration

```
This property specifies how the font size of the displayed text is determined.
The possible values are:
此属性用于指定显示文本的字体大小如何确定。可选值如下：
Text.FixedSize （	默认）使用由 font.pixelSize 或 font.pointSize 指定的固定大小。
Text.HorizontalFit 	在不换行的情况下，使用不超过指定大小、且能适应项宽度的最大字体大小。
Text.VerticalFit 	使用不超过指定大小、且能适应项高度的最大字体大小。
Text.Fit 			使用不超过指定大小、且能同时适应项的宽度和高度的最大字体大小。

The font size of fitted text has a minimum bound specified by the minimumPointSize or minimumPixelSize property and maximum bound specified by either the font.pointSize or font.pixelSize properties.
已适配文本的字体大小有一个最小和最大限制：
最小值由 minimumPointSize 或 minimumPixelSize 属性指定。
最大值由 font.pointSize 或 font.pixelSize 属性指定。

Text { text: "Hello"; fontSizeMode: Text.Fit; minimumPixelSize: 10; font.pixelSize: 72 }

If the text does not fit within the item bounds with the minimum font size, the text will be elided as per the elide property.
如果在最小字体大小下仍无法适应控件边界，则文本会根据 elide 属性进行省略显示。

If the textFormat property is set to Text.RichText, this will have no effect at all as the property will be ignored completely.
当 textFormat 属性为 Text.RichText 时，本属性完全无效，将被忽略。

If textFormat is set to Text.StyledText, then the property will be respected provided there are no font size tags inside the text. If there are font size tags, those will take precedence, which may cause the behavior to not fully comply with the fontSizeMode setting.
当 textFormat 属性为 Text.StyledText 时：
    如果文本中没有字体大小标签，本属性将生效。
    如果文本中包含字体大小标签，则优先遵循这些标签设置。这可能导致行为不完全符合 fontSizeMode 的指定效果。
```

### [read-only] hoveredLink : string

```
This property contains the link string when the user hovers a link embedded in the text. The link must be in rich text or HTML format and the hoveredLink string provides access to the particular link.
当用户将鼠标悬停在文本中嵌入的链接上时，此属性会包含该链接字符串。
链接必须是 富文本 或 HTML 格式，而 hoveredLink 字符串可用于访问该特定链接。
```

### [read-only] lineCount : int

```
Returns the number of lines visible in the text item.
返回文本项中 可见的行数。

This property is not supported for rich text.
此属性 不支持富文本。
```

### lineHeight : real

```
Sets the line height for the text. The value can be in pixels or a multiplier depending on lineHeightMode.
为文本设置 行高。其数值可以是 像素值，也可以是 倍数，取决于 lineHeightMode。

The default value is a multiplier of 1.0. The line height must be a positive value.
默认值是 1.0 倍。行高必须为正数。
```

### lineHeightMode : enumeration

```
This property determines how the line height is specified. The possible values are:
此属性用于决定 行高（line height） 的指定方式。可选值如下：
Constant					Description
Text.ProportionalHeight		默认值。将行高设置为字体行高的倍数。例如，设置为 2 表示双倍行距。
Text.FixedHeight			将行高设置为固定的像素值。
```

### linkColor : color

```
The color of links in the text.
文本中链接的颜色。

This property works with the StyledText textFormat, but not with RichText.
此属性适用于 StyledText 的 textFormat，但不适用于 RichText。

Link color in RichText can be specified by including CSS style tags in the text.
在 RichText 中，可以通过在文本中包含 CSS 样式标签 来指定链接颜色。
```

### maximumLineCount : int

```
Set this property to limit the number of lines that the text item will show.
设置此属性以限制文本项显示的行数。

If elide is set to Text.ElideRight, the text will be elided appropriately.
如果 elide 设置为 Text.ElideRight，文本将会被适当省略。

By default, this is the value of the largest possible integer.
默认情况下，此属性的值为可能的最大整数。

This property is not supported for rich text.
此属性 不支持富文本（RichText）。
```

### minimumPixelSize : int

```
This property specifies the minimum font pixel size of text scaled by the fontSizeMode property.
此属性指定由 fontSizeMode 属性缩放的文本的最小字体像素大小。

If the fontSizeMode is Text.FixedSize or font.pixelSize is -1 this property is ignored.
如果 fontSizeMode 为 Text.FixedSize 或 font.pixelSize 为 -1，则此属性会被忽略。
```

### minimumPointSize : int

```
This property specifies the minimum font point size of text scaled by the fontSizeMode property.
此属性指定由 fontSizeMode 属性缩放的文本的最小字体点数（point size）。

If the fontSizeMode is Text.FixedSize or the font.pointSize is -1 this property is ignored.
如果 fontSizeMode 为 Text.FixedSize 或 font.pointSize 为 -1，则此属性会被忽略。
```

### renderType : enumeration

```
Override the default rendering type for this component. Supported render types are:
重写此组件的默认渲染类型。支持的渲染类型如下：
Constant				Description
Text.QtRendering		使用每个字形的可缩放距离场（distance field）进行渲染。
Text.NativeRendering	使用平台特定的技术进行渲染。

Select Text.NativeRendering if you prefer text to look native on the target platform and do not require advanced features such as transformation of the text. Using such features in combination with the NativeRendering render type will lend poor and sometimes pixelated results.
如果希望文本在目标平台上看起来 原生，并且不需要诸如文本变换等高级功能，请选择 Text.NativeRendering。
在 NativeRendering 渲染类型下使用这些高级功能可能导致渲染效果不佳，有时甚至出现像素化。

The default rendering type is determined by QQuickWindow::textRenderType().
默认渲染类型由 QQuickWindow::textRenderType() 决定。
```

### [since 6.0] renderTypeQuality : int

```
Override the default rendering type quality for this component. This is a low-level customization which can be ignored in most cases. It currently only has an effect when renderType is Text.QtRendering.
重写此组件的默认渲染类型质量。这是一个底层的自定义属性，在大多数情况下可以忽略。目前，仅当 renderType 为 Text.QtRendering 时，此属性才会生效。

The rasterization algorithm used by Text.QtRendering may give artifacts at large text sizes, such as sharp corners looking rounder than they should. If this is an issue for specific text items, increase the renderTypeQuality to improve rendering quality, at the expense of memory consumption.
Text.QtRendering 使用的光栅化算法在大字号文本时可能会产生一些瑕疵，例如尖角看起来比实际更圆。如果某些文本项出现这种问题，可以通过增大 renderTypeQuality 来提高渲染质量，但会增加内存消耗。

The renderTypeQuality may be any integer over 0, or one of the following predefined values:
renderTypeQuality 可以是任意大于 0 的整数，或者以下预定义值之一：
Constant							Description
Text.DefaultRenderTypeQuality		-1 (default)
Text.LowRenderTypeQuality			26
Text.NormalRenderTypeQuality		52
Text.HighRenderTypeQuality			104
Text.VeryHighRenderTypeQuality		208

This property was introduced in Qt 6.0.
该属性在 Qt 6.0 中引入。
```

### style : enumeration

```
Set an additional text style.
设置附加的文本样式。

Supported text styles are:
支持的文本样式如下：
Constant		Description
Text.Normal		- the default 	（普通/正常）
Text.Outline 					（轮廓/描边）
Text.Raised 					（凸起/浮雕）
Text.Sunken 	Row {			（凹陷/蚀刻）
                 Text { font.pointSize: 24; text: "Normal" }
                 Text { font.pointSize: 24; text: "Raised"; style: Text.Raised; styleColor: "#AAAAAA" }
                 Text { font.pointSize: 24; text: "Outline";style: Text.Outline; styleColor: "red" }
                 Text { font.pointSize: 24; text: "Sunken"; style: Text.Sunken; styleColor: "#AAAAAA" }
                }
```

### styleColor : color

```
Defines the secondary color used by text styles.
定义文本样式使用的辅助颜色。

styleColor is used as the outline color for outlined text, and as the shadow color for raised or sunken text. If no style has been set, it is not used at all.
styleColor 用作描边文本的轮廓颜色，或者用于 凸起（raised） 或 凹陷（sunken） 文本的阴影颜色。如果未设置任何样式，则不会使用该颜色。
Text { font.pointSize: 18; text: "hello"; style: Text.Raised; styleColor: "gray" }
```

### text : string

```
The text to display. Text supports both plain and rich text strings.
要显示的文本。文本可以是纯文本或富文本字符串。

The item will try to automatically determine whether the text should be treated as styled text. This determination is made using Qt::mightBeRichText(). However, detection of Markdown is not automatic.
该项会尝试 自动判断 文本是否应被视为样式化文本。这个判断是通过 Qt::mightBeRichText() 方法完成的。不过，Markdown 检测不是自动进行的。
```

### textFormat : enumeration

```
The way the text property should be displayed.
文本属性的显示方式。

Supported text formats are:
支持的文本格式有：
Text.AutoText 					（默认）通过 Qt::mightBeRichText() 启发式检测。
Text.PlainText 					所有样式标签都会被当作普通文本处理。
Text.StyledText 				优化后的基础富文本，类似于 HTML 3.2。
Text.RichText 					HTML 4 的一个子集。
Text.MarkdownText CommonMark 	加上 GitHub 扩展（支持表格和任务列表，自 Qt 5.14 起）。

If the text format is Text.AutoText the Text item will automatically determine whether the text should be treated as styled text. This determination is made using Qt::mightBeRichText(), which can detect the presence of an HTML tag on the first line of text, but cannot distinguish Markdown from plain text.
如果 textFormat 设置为 Text.AutoText，Text 元件会自动判断文本是否应被视为样式化文本。这个判断通过 Qt::mightBeRichText() 完成，它可以检测文本首行中是否存在 HTML 标签，但无法区分 Markdown 与纯文本。

Text.StyledText is an optimized format supporting some basic text styling markup, in the style of HTML 3.2:
Text.StyledText 是一种优化后的格式，支持一些基础的文本样式标记，风格类似 HTML 3.2：

<b></b> - 加粗
<del></del> - 删除线（表示已移除的内容）
<s></s> - 删除线（表示不再准确或不再相关的内容）
<strong></strong> - 加粗
<i></i> - 斜体
<br> - 换行
<p> - 段落
<u> - 下划线文字
<font color="color_name" size="1-7"></font> - 字体颜色与大小
<h1> 到 <h6> - 标题
<a href=""> - 超链接
<img src="" align="top,middle,bottom" width="" height=""> - 内嵌图片
<ol type="">、<ul type=""> 和 <li> - 有序与无序列表
<pre></pre> - 预格式化文本
所有实体（Entities）

The Text.StyledText parser is strict, requiring tags to be correctly nested.
Text.StyledText 的解析器非常严格，要求标签必须正确嵌套。

 Column {
     Text {
         font.pointSize: 24
         text: "<b>Hello</b> <i>World!</i>"
     }
     Text {
         font.pointSize: 24
         textFormat: Text.RichText
         text: "<b>Hello</b> <i>World!</i>"
     }
     Text {
         font.pointSize: 24
         textFormat: Text.PlainText
         text: "<b>Hello</b> <i>World!</i>"
     }
     Text {
         font.pointSize: 24
         textFormat: Text.MarkdownText
         text: "**Hello** *World!*"
     }
 }

Text.RichText supports a larger subset of HTML 4, as described on the Supported HTML Subset page. You should prefer using Text.PlainText, Text.StyledText or Text.MarkdownText instead, as they offer better performance.
Text.RichText 支持更大范围的 HTML 4 子集，具体内容可参考 Supported HTML Subset 页面。不过建议优先使用 Text.PlainText、Text.StyledText 或 Text.MarkdownText，因为它们具有更好的性能。

Note: With Text.MarkdownText, and with the supported subset of HTML, some decorative elements are not rendered as they would be in a web browser:
注意： 在 Text.MarkdownText 和所支持的 HTML 子集中，有些装饰性元素不会像网页浏览器中那样渲染：
    code blocks use the default monospace font but without a surrounding highlight box
    代码块会使用默认的等宽字体，但不会带有背景高亮框。
    block quotes are indented, but there is no vertical line alongside the quote
    引用块会缩进，但不会显示在引用旁边的竖线。
    horizontal rules are not rendered
    水平分割线不会被渲染。
```

### [read-only] truncated : bool

```
Returns true if the text has been truncated due to maximumLineCount or elide.
This property is not supported for rich text.
如果文本因为 maximumLineCount 或 elide 被截断，则返回 true。
此属性在富文本中不受支持。
```

### wrapMode : enumeration

```
Set this property to wrap the text to the Text item's width. The text will only wrap if an explicit width has been set. wrapMode can be one of:
将此属性设置为根据 Text 项的宽度换行。只有在显式设置了宽度时，文本才会换行。
wrapMode 可以是以下值之一：
Text.NoWrap：		（默认）不进行换行。如果文本中换行符不足，contentWidth 将会超过设定的宽度。
Text.WordWrap：		仅在单词边界处换行。如果某个单词过长，contentWidth 仍会超过设定的宽度。
Text.WrapAnywhere：	可以在行内的任意位置换行，即使发生在单词中间。
Text.Wrap：			如果可能，会在单词边界处换行；否则将在行内合适的位置换行，即使在单词中间。
```

## 3.重要信号

### lineLaidOut(object line)

```
This signal is emitted for each line of text that is laid out during the layout process in plain text or styled text mode. It is not emitted in rich text mode. The specified line object provides more details about the line that is currently being laid out.
在布局过程中，每当一行文本被排版完成时，该信号都会被触发（仅在 纯文本 或 样式化文本 模式下有效）。在 富文本模式 下不会触发。指定的 line 对象提供了当前正在排版的这一行的更多细节。

This gives the opportunity to position and resize a line as it is being laid out. It can for example be used to create columns or lay out text around objects.
这使得在排版过程中有机会对某一行进行定位和调整大小。例如，可以用来创建多列文本，或者在对象周围环绕排版文字。

The properties of the specified line object are:
指定的 line 对象的属性包括：
属性名称	  	  说明
number：			 (只读)行号，从 0 开始。
x：				该行在 Text 元件内的 x 坐标。
y：				该行在 Text 元件内的 y 坐标。
width：			该行的宽度。
height：			该行的高度。
implicitWidth 	(只读)：该行在未经过宽度修改时，根据内容自然占用的宽度。
isLast： 		(只读)是否为最后一行。如果修改了 width 属性，该值可能会发生变化。

For example, this will move the first 5 lines of a Text item by 100 pixels to the right:
例如，下面的代码会将 Text 元件的前 5 行向右移动 100 像素：
 onLineLaidOut: (line)=> {
     if (line.number < 5) {
         line.x = line.x + 100
         line.width = line.width - 100
     }
 }

The following example will allow you to position an item at the end of the last line:
以下示例演示了如何将一个元素定位到最后一行的末尾：
 onLineLaidOut: (line)=> {
     if (line.isLast) {
         lastLineMarker.x = line.x + line.implicitWidth
         lastLineMarker.y = line.y + (line.height - lastLineMarker.height) / 2
     }
 }
 
Note: The corresponding handler is onLineLaidOut.
注意： 对应的处理器是 onLineLaidOut。
```

### linkActivated(string link)

```
This signal is emitted when the user clicks on a link embedded in the text. The link must be in rich text or HTML format and the link string provides access to the particular link.
当用户点击文本中嵌入的链接时，会触发此信号。链接必须是 富文本 或 HTML 格式，参数 link 字符串提供对该特定链接的访问。

Text {
    textFormat: Text.RichText
    text: "See the <a href=\"http://qt-project.org\">Qt Project website</a>."
    onLinkActivated: (link)=> console.log(link + " link activated")
}

The example code will display the text See the Qt Project website.
上述代码会显示文本：See the Qt Project website.

Clicking on the highlighted link will output http://qt-project.org link activated to the console.
点击高亮的链接时，会在控制台输出：http://qt-project.org link activated

Note: The corresponding handler is onLinkActivated.
注意： 对应的处理器是 onLinkActivated。
```

### linkHovered(string link)

```
This signal is emitted when the user hovers a link embedded in the text. The link must be in rich text or HTML format and the link string provides access to the particular link.
当用户将鼠标悬停在文本中嵌入的链接上时，会触发此信号。链接必须是 富文本 或 HTML 格式，参数 link 字符串提供对该特定链接的访问。

Note: The corresponding handler is onLinkHovered.
注意： 对应的处理器是 onLinkHovered。
```

### 4.重要方法

### forceLayout()

```
ChatGPT 说：

Triggers a re-layout of the displayed text.
触发已显示文本的重新布局。
```

### linkAt(real x, real y)

```
Returns the link string at point x, y in content coordinates, or an empty string if no link exists at that point.
返回内容坐标中点 (x, y) 处的链接字符串；如果该点没有链接，则返回空字符串。
```



# 2.2 TextInput类

## 1.说明

```markdown
The TextInput type displays a single line of editable plain text.
TextInput is used to accept a line of text input. Input constraints can be placed on a TextInput item (for example, through a validator or inputMask), and setting echoMode to an appropriate value enables TextInput to be used for a password input field.

TextInput类型显示一行可编辑的纯文本。
TextInput用于接受一行文本输入。 输入约束可以放置在TextInput项上（例如，通过验证器或inputMask），并且将echoMode设置为适当的值可以使TextInput用于密码输入字段。
```

## 2.重要属性

#### [read-only] acceptableInput : bool

```markdown
This property is always true unless a validator or input mask has been set. If a validator or input mask has been set, this property will only be true if the current text is acceptable to the validator or input mask as a final string (not as an intermediate string).
除非设置了验证器（validator）或输入掩码（input mask），否则该属性始终为 true。如果设置了验证器或输入掩码，则仅当当前文本作为最终字符串（而非中间字符串）符合验证器或输入掩码要求时，该属性才为 true。
```

### echoMode : enumeration

```
Specifies how the text should be displayed in the TextInput.
指定文本在 TextInput 中的显示方式。
Constant					Description
TextInput.Normal			(Default) 显示文本内容本身。（默认）
TextInput.Password			显示平台相关的密码掩码字符，而非实际字符。
TextInput.NoEchoDisplays 	不显示任何文本。.
TextInput.Password			编辑时显示实际输入字符，其他情况同 TextInput.Password。
```

### passwordCharacter : string

```
This is the character displayed when echoMode is set to Password or PasswordEchoOnEdit. By default, it is the password character used by the platform theme.
当 echoMode 设置为 Password 或 PasswordEchoOnEdit 时，用于显示的字符。默认值为平台主题使用的密码字符。

If this property is set to a string with more than one character, the first character is used. If the string is empty, the value is ignored and the property is not set.
如果该属性设置为包含多个字符的字符串，则只使用第一个字符；如果字符串为空，则该值会被忽略，属性不会被设置。
```

### inputMask : string

```
Allows you to set an input mask on the TextInput, restricting the allowable text inputs. 
允许在 TextInput 上设置输入掩码，以限制可输入的文本内容。

If no mask is set, inputMask() returns an empty string.
如果未设置掩码，inputMask() 将返回空字符串。

The input mask is an input template string. It can contain the following elements:
输入掩码是一个输入模板字符串。它可以包含以下元素：
Mask Characters		掩码字符：定义在该位置允许输入的字符类别。
Meta Characters		元字符：具有各种特殊含义。
Separators			分隔符：其他所有字符都被视为不可修改的分隔符。

The following table shows the mask and meta characters that can be used in an input mask.
下表列出了可在输入掩码中使用的掩码字符和元字符。

Mask Character		Meaning
A	必须输入的字母字符，例如 A-Z、a-z。
a	可输入的字母字符，但不是必须。
N	必须输入的字母或数字字符，例如 A-Z、a-z、0-9。
n	可输入的字母或数字字符，但不是必须。
X	必须输入的非空白字符。
x	可输入的非空白字符，但不是必须。
9	必须输入的数字字符，例如 0-9。
0	可输入的数字字符，但不是必须。
D	必须输入的大于零的数字字符，例如 1-9。
d	可输入的大于零的数字字符，但不是必须，例如 1-9。
#	可输入的数字字符，或加/减号，但不是必须。
H	必须输入的十六进制字符，A-F、a-f、0-9。
h	输入的十六进制字符，但不是必须。
B	必须输入的二进制字符，0-1。
b	可输入的二进制字符，但不是必须。
>	后续所有字母字符转换为大写。
<	后续所有字母字符转换为小写。
!	关闭大小写转换。
;c	结束输入掩码，并将空白字符设置为 c。
[ ] { }	保留字符。
\	使用 \ 对上述特殊字符进行转义，以将其用作分隔符。

When created or cleared, the line edit will be filled with a copy of the input mask string where the meta characters have been removed, and the mask characters have been replaced with the blank character (by default, a space).
创建或清空时，行编辑控件会填充输入掩码字符串的副本，其中元字符被移除，掩码字符被替换为空白字符（默认是空格）。

When an input mask is set, the text() method returns a modified copy of the line edit content where all the blank characters have been removed. The unmodified content can be read using displayText().
当设置了输入掩码后，text() 方法返回行编辑内容的修改副本，其中所有空白字符都已被移除。未修改的内容可通过 displayText() 方法读取。

The hasAcceptableInput() method returns false if the current content of the line edit does not fulfil the requirements of the input mask.
如果行编辑当前内容不符合输入掩码的要求，hasAcceptableInput() 方法将返回 false。

Examples:
Mask								Notes
000.000.000.000;_					IP 地址，空白字符为 _。
HH:HH:HH:HH:HH:HH;_					MAC 地址。
0000-00-00	ISO Date; 				日期，空白字符为空格。
>AAAAA-AAAAA-AAAAA-AAAAA-AAAAA;#	许可证号，空白字符为 #，所有字母字符转换为大写。

To get range control (e.g., for an IP address) use masks together with validators.
若需要范围控制（例如 IP 地址），可将掩码与验证器（validator）结合使用。
```

### validator : Validator

```
Allows you to set a validator on the TextInput. When a validator is set, the TextInput will only accept input which leaves the text property in an acceptable or intermediate state. The accepted signal will only be sent if the text is in an acceptable state when Enter is pressed.
允许在 TextInput 上设置验证器（validator）。设置验证器后，TextInput 仅接受使 text 属性处于可接受或中间状态的输入。只有当文本在按下 Enter 时处于可接受状态，accepted 信号才会被触发。

Currently supported validators are IntValidator, DoubleValidator, and RegularExpressionValidator.
当前支持的验证器包括 IntValidator、DoubleValidator 和 RegularExpressionValidator。

Example: Allows input of integers between 11 and 31:
示例：允许输入 11 到 31 之间的整数：

import QtQuick 2.0
TextInput {
    validator: IntValidator { bottom: 11; top: 31 }
    focus: true
}
```

## 3.重要信号

### accepted()

```
This signal is emitted when the Return or Enter key is pressed.
当按下 Return 或 Enter 键时，会发出此信号。

Note that if there is a validator or inputMask set on the text input, the signal will only be emitted if the input is in an acceptable state.
注意：如果在文本输入上设置了验证器（validator）或输入掩码（inputMask），则只有在输入内容处于可接受状态时才会发出该信号。

Note: The corresponding handler is onAccepted.
注意：对应的处理函数是 onAccepted。
```

### editingFinished()

```
This signal is emitted when the Return or Enter key is pressed or the text input loses focus.
当按下 Return 或 Enter 键，或者文本输入框失去焦点时，会发出此信号。

Note that if there is a validator or inputMask set on the text input and enter/return is pressed, this signal will only be emitted if the input follows the inputMask and the validator returns an acceptable state.
注意：如果在文本输入上设置了验证器（validator）或输入掩码（inputMask），在按下 Enter/Return 键时，只有当输入内容符合输入掩码且验证器返回可接受状态时才会发出该信号。

Note: The corresponding handler is onEditingFinished.
注意：对应的处理函数是 onEditingFinished。
```

### textEdited()

```
This signal is emitted whenever the text is edited.
每当文本被编辑时，都会发出此信号。

Unlike textChanged(), this signal is not emitted when the text is changed programmatically, for example, by changing the value of the text property or by calling clear().
与 textChanged() 不同，当文本是通过代码修改时（例如修改 text 属性的值或调用 clear()），不会发出此信号。

Note: The corresponding handler is onTextEdited.
注意：对应的处理函数是 onTextEdited。
```



## 4.重要方法

### undo()

```
Undoes the last operation if undo is available.
如果可以撤销，则撤销上一次操作。

Deselects any current selection, and updates the selection start to the current cursor position.
取消当前所有选中内容，并将选择的起始位置更新为当前光标位置。
```

### redo()

```
Redoes the last operation if redo is available.
如果可以重做，则重做上一次操作。
```

# 2.3 TextField类 - 继承与 TextInput 增加了背景和占位字符

## 1.说明

```markdown
Single-line text input field.
单行文本输入框。

TextField is a single line text editor.
TextField 是一个单行文本编辑器。

TextField extends TextInput with a placeholder text functionality, and adds decoration.
TextField 在 TextInput 的基础上增加了占位文本（placeholder）功能，并添加了装饰效果。

TextField {
     placeholderText: qsTr("Enter name")
}

```

## 2.重要属性

### background : Item

```
This property holds the background item.
该属性保存背景项。

Note: If the background item has no explicit size specified, it automatically follows the control's size. In most cases, there is no need to specify width or height for a background item.
注意：如果背景项没有显式指定大小，它会自动跟随控件的大小。在大多数情况下，无需为背景项指定宽度或高度。

Note: Most controls use the implicit size of the background item to calculate the implicit size of the control itself. If you replace the background item with a custom one, you should also consider providing a sensible implicit size for it (unless it is an item like Image which has its own implicit size).
注意：大多数控件会使用背景项的隐式大小来计算控件自身的隐式大小。如果你用自定义的背景项替换默认背景项，应当考虑为其提供一个合理的隐式大小（除非该项是像 Image 这样的元素，它本身就具有隐式大小）。
```

### placeholderText : string

```
This property holds the hint that is displayed in the TextField before the user enters text.
该属性保存显示在 TextField 中的提示文本，在用户输入内容之前可见。
```

### [since QtQuick.Controls 2.5 (Qt 5.12)] placeholderTextColor : color

```
This property holds the color of placeholderText.
该属性保存 placeholderText 的颜色。

This property was introduced in QtQuick.Controls 2.5 (Qt 5.12).
该属性在 QtQuick.Controls 2.5 (Qt 5.12) 中引入。
```



## 3.重要信号

### pressAndHold(MouseEvent event)

```markdown
This signal is emitted when there is a long press (the delay depends on the platform plugin).
当发生长按操作时会发出此信号（具体延迟取决于平台插件）。

The event parameter provides information about the press, including the x and y coordinates of the press, and which button is pressed.
事件参数提供了有关按下操作的信息，包括按下的 x、y 坐标，以及按下了哪个按钮。

Note: The corresponding handler is onPressAndHold.
注意：对应的处理函数是 onPressAndHold。
```

### [since QtQuick.Controls 2.1 (Qt 5.8)] pressed(MouseEvent event)

```
This signal is emitted when the text field is pressed by the user.
当用户按下文本框时，会发出此信号。

The event parameter provides information about the press, including the x and y coordinates of the press, and which button is pressed.
事件参数提供了有关按下的信息，包括 x、y 坐标 以及按下了哪个按钮。

Note: The corresponding handler is onPressed.
注意：对应的处理函数是 onPressed。

This signal was introduced in QtQuick.Controls 2.1 (Qt 5.8).
此信号在 QtQuick.Controls 2.1 (Qt 5.8) 中引入。
```

### [since QtQuick.Controls 2.1 (Qt 5.8)] released(MouseEvent event)

```
This signal is emitted when the text field is released by the user.
当用户释放文本框时，会发出此信号。

The event parameter provides information about the release, including the x and y coordinates of the press, and which button is pressed.
事件参数提供了有关释放操作的信息，包括 按下的 x、y 坐标 以及按下了哪个按钮。

Note: The corresponding handler is onReleased.
注意：对应的处理函数是 onReleased。

This signal was introduced in QtQuick.Controls 2.1 (Qt 5.8).
此信号在 QtQuick.Controls 2.1 (Qt 5.8) 中引入。
```



# 2.4 TextEdit类

## 1.说明

```markdown
Displays multiple lines of editable formatted text.
显示多行可编辑的格式化文本。

The TextEdit item displays a block of editable, formatted text.
TextEdit 元素显示一块可编辑的、格式化的文本。

It can display both plain and rich text. For example:
它可以显示纯文本和富文本。例如：

TextEdit {
    width: 240
    text: "<b>Hello</b> <i>World!</i>"
    font.family: "Helvetica"
    font.pointSize: 20
    color: "blue"
    focus: true
}
在这个例子中，TextEdit 显示加粗的 "Hello" 和斜体的 "World!"，字体为 Helvetica，字号 20，颜色为蓝色，并且获得焦点。

Setting focus to true enables the TextEdit item to receive keyboard focus.
将 focus 设置为 true 可以使 TextEdit 元素获得键盘焦点。

Note that the TextEdit does not implement scrolling, following the cursor, or other behaviors specific to a look and feel.
请注意，TextEdit 并不实现滚动、跟随光标或其他特定外观行为。

For example, to add flickable scrolling that follows the cursor:
例如，如果想要添加能够跟随光标的可滑动滚动效果：
Flickable {
    id: flick

    width: 300; height: 200
    contentWidth: edit.contentWidth
    contentHeight: edit.contentHeight
    clip: true

    function ensureVisible(r) {
        if (contentX >= r.x)
            contentX = r.x
        else if (contentX + width <= r.x + r.width)
            contentX = r.x + r.width - width

        if (contentY >= r.y)
            contentY = r.y
        else if (contentY + height <= r.y + r.height)
            contentY = r.y + r.height - height
    }

    TextEdit {
        id: edit
        width: flick.width
        focus: true
        wrapMode: TextEdit.Wrap
        onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)
    }
}
A particular look and feel might use smooth scrolling (e.g., using SmoothedAnimation), might have a visible scrollbar, or a scrollbar that fades in to show location, etc.
某些特定的界面风格可能会使用平滑滚动（例如 SmoothedAnimation）、可见的滚动条，或者会淡入显示位置的滚动条等。

Clipboard support is provided by the cut(), copy(), and paste() functions.
剪贴板功能通过 cut()、copy() 和 paste() 函数提供。

Text can be selected by mouse in the usual way, unless selectByMouse is set to false; and by keyboard with the Shift+arrow key combinations, unless selectByKeyboard is set to false.
文本可以通过鼠标按常规方式选择（除非 selectByMouse 设置为 false）；也可以通过 Shift+方向键组合使用键盘选择（除非 selectByKeyboard 设置为 false）。

To select text programmatically, you can set the selectionStart and selectionEnd properties, or use selectAll() or selectWord().
要通过代码选择文本，可以设置 selectionStart 和 selectionEnd 属性，或使用 selectAll() 或 selectWord()。

You can translate between cursor positions (characters from the start of the document) and pixel points using positionAt() and positionToRectangle().
可以使用 positionAt() 和 positionToRectangle() 在光标位置（从文档开头的字符索引）与像素坐标之间进行转换。
```

## 2.重要属性

### persistentSelection : bool

```
Whether the TextEdit should keep the selection visible when it loses active focus to another item in the scene.
当 TextEdit 失去活动焦点（被场景中的其他项获取焦点）时，是否仍应保持选中的文本可见。

By default this is set to false.
默认情况下，该值为 false。
```

### [read-only] selectionStart : int

```
The cursor position before the first character in the current selection.
光标位于当前选区第一个字符之前的位置。

This property is read-only. To change the selection, use select(start,end), selectAll(), or selectWord().
该属性是只读的。若要更改选区，请使用 select(start, end)、selectAll() 或 selectWord()。
```

### [read-only] selectionEnd : int

```
The cursor position after the last character in the current selection.
光标位于当前选区最后一个字符之后的位置。

This property is read-only. To change the selection, use select(start,end), selectAll(), or selectWord().
该属性是只读的。若要更改选区，请使用 select(start, end)、selectAll() 或 selectWord()。
```

### [read-only] selectedText : string

```
This read-only property provides the text currently selected in the text edit.
这个只读属性提供了当前在文本编辑框中被选中的文本。

It is equivalent to the following snippet, but is faster and easier to use.
它等价于下面的代码片段，但更快、更易用。

// myTextEdit 是 TextEdit 的 id
myTextEdit.text.toString().substring(
	myTextEdit.selectionStart, myTextEdit.selectionEnd
);
```

### cursorPosition : int

```
The position of the cursor in the TextEdit. The cursor is positioned between characters.
TextEdit 中文本光标的位置。光标位于字符之间。

Note: The characters in this case refer to the string of QChar objects, therefore 16-bit Unicode characters, and the position is considered an index into this string.
注意：这里的“字符”指的是一串 QChar 对象，也就是 16 位的 Unicode 字符，光标位置被认为是该字符串中的一个索引。

This does not necessarily correspond to individual graphemes in the writing system, as a single grapheme may be represented by multiple Unicode characters, such as in the case of surrogate pairs, linguistic ligatures or diacritics.
这并不一定对应于书写系统中的单个「字素」（grapheme），因为一个字素可能由多个 Unicode 字符组合而成，例如代理对（surrogate pair）、连字（ligature）或附加符号（diacritic）。
```

## 3.重要信号

### editingFinished()

```
This signal is emitted when the text edit loses focus.
当文本编辑框失去焦点时会发射该信号。

Note: The corresponding handler is onEditingFinished.
注意：对应的处理函数是 onEditingFinished。
```

### linkActivated(string link)

```
This signal is emitted when the user clicks on a link embedded in the text. The link must be in rich text or HTML format and the link string provides access to the particular link.
当用户点击文本中嵌入的链接时会发射该信号。链接必须是富文本或 HTML 格式，参数 link 字符串提供了所点击的具体链接内容。

Note: The corresponding handler is onLinkActivated.
注意：对应的处理函数是 onLinkActivated。
```

### linkHovered(string link)

```
This signal is emitted when the user hovers a link embedded in the text. The link must be in rich text or HTML format and the link string provides access to the particular link.
当用户将鼠标悬停在文本中嵌入的链接上时会发射该信号。链接必须是富文本或 HTML 格式，参数 link 字符串提供了所悬停的具体链接内容。

Note: The corresponding handler is onLinkHovered.
注意：对应的处理函数是 onLinkHovered。
```

## 4.重要方法

### string getFormattedText(int start, int end)

```
Returns the section of text that is between the start and end positions.
返回位于起始位置和结束位置之间的那段文本。

The returned text will be formatted according the textFormat property.
返回的文本会根据 textFormat 属性进行格式化。
```

### void append(string text)

```
Appends a new paragraph with text to the end of the TextEdit.
在 TextEdit 的末尾添加一个新段落，并填入指定文本。

In order to append without inserting a new paragraph, call myTextEdit.insert(myTextEdit.length, text) instead.
如果希望追加文本而不创建新段落，可以使用：

myTextEdit.insert(myTextEdit.length, text)
```

### copy()

```
Copies the currently selected text to the system clipboard.
将当前选中的文本复制到系统剪贴板。
```

### cut()

```
Moves the currently selected text to the system clipboard.
将当前选中的文本剪切到系统剪贴板。
```

### paste()

```
Replaces the currently selected text by the contents of the system clipboard.
将当前选中的文本替换为系统剪贴板中的内容。
```

### insert(int position, string text)

```
Inserts text into the TextEdit at position.
在指定位置将文本插入到 TextEdit 中。
```

### string remove(int start, int end)

```
Removes the section of text that is between the start and end positions from the TextEdit.
从 TextEdit 中删除位于起始位置和结束位置之间的文本段。
```



# 2.5 TextArea类 - 继承与 TextEdit 增加了背景和占位字符

## 1.说明

```markdown
Multi-line text input area.
多行文本输入区域。

TextArea is a multi-line text editor. TextArea extends TextEdit with a placeholder text functionality, and adds decoration.
TextArea 是一个多行文本编辑器。它在 TextEdit 的基础上增加了占位文本功能，并提供了装饰效果。

TextArea {
    placeholderText: qsTr("Enter description")
}

TextArea is not scrollable by itself. Especially on screen-size constrained platforms, it is often preferable to make entire application pages scrollable. On such a scrollable page, a non-scrollable TextArea might behave better than nested scrollable controls. Notice, however, that in such a scenario, the background decoration of the TextArea scrolls together with the rest of the scrollable content.
TextArea 本身不可滚动。尤其在屏幕尺寸受限的平台上，通常更推荐让整个应用页面可滚动。在这样的可滚动页面中，非滚动的 TextArea 往往比嵌套的可滚动控件表现更好。但需要注意，在这种情况下，TextArea 的背景装饰会随页面其他可滚动内容一起滚动。

Scrollable TextArea
可滚动的 TextArea
If you want to make a TextArea scrollable, for example, when it covers an entire application page, it can be placed inside a ScrollView.
如果你希望 TextArea 可滚动，例如它覆盖整个应用页面时，可以将它放在 ScrollView 中。

ScrollView {
    id: view
    anchors.fill: parent

    TextArea {
        text: "TextArea\n...\n...\n...\n...\n...\n...\n"
    }
}
A TextArea that is placed inside a ScrollView does the following:
放在 ScrollView 中的 TextArea 会实现以下效果：
 - Sets the content size automatically
   自动设置内容大小
 - Ensures that the background decoration stays in place
   确保背景装饰保持固定
 - Clips the content
   裁剪内容
  
Tab Focus
Tab 键焦点
By default, pressing the tab key while TextArea has active focus results in a tab character being input into the control itself. To make tab pass active focus onto another item, use the attached KeyNavigation properties:
默认情况下，当 TextArea 拥有活动焦点时按 Tab 键，会在控件内输入一个 Tab 字符。若希望 Tab 键将焦点传递给其他控件，可以使用附加的 KeyNavigation 属性：
TextField {
    id: textField
}

TextArea {
    KeyNavigation.priority: KeyNavigation.BeforeItem
    KeyNavigation.tab: textField
}
```

## 2.重要属性

### TextArea.flickable : TextArea

```
This property attaches a text area to a Flickable.
该属性将一个 TextArea 附加到 Flickable 上。
```



# 3.1 IntValidator类

## 1.说明 

```markdown
Defines a validator for integer values.
定义一个用于整数值的验证器。

The IntValidator type provides a validator for integer values.
IntValidator 类型提供了对整数值的验证功能。

If no locale is set IntValidator uses the default locale to interpret the number and will accept locale specific digits, group separators, and positive and negative signs. In addition, IntValidator is always guaranteed to accept a number formatted according to the "C" locale.
如果未设置区域（locale），IntValidator 会使用默认区域来解析数字，并接受区域特定的数字、分组符号，以及正负号。此外，IntValidator 始终保证接受按照 “C” 区域格式化的数字。

See also DoubleValidator, RegularExpressionValidator, and Validating Input Text.
另请参见 DoubleValidator、RegularExpressionValidator 以及 验证输入文本。
```

## 2.重要属性

### bottom : int

```markdown
This property holds the validator's lowest acceptable value.
此属性表示验证器允许的最小值。

By default, this property's value is derived from the lowest signed integer available (typically -2147483647).
默认情况下，此属性的值取自可用的最小有符号整数（通常为 -2147483647）。
```

### locale : string

```
This property holds the name of the locale used to interpret the number.
此属性表示用于解析数字的区域（locale）名称。
```

### top : int

```
This property holds the validator's highest acceptable value.
此属性表示验证器允许的最大值。

By default, this property's value is derived from the highest signed integer available (typically 2147483647).
默认情况下，此属性的值取自可用的最大有符号整数（通常为 2147483647）。
```

# 3.2 DoubleValidator类

## 1.说明

```markdown
Defines a validator for non-integer numbers.
为非整数（浮点数）定义一个验证器。

The DoubleValidator type provides a validator for non-integer numbers.
DoubleValidator 类型用于验证非整数值。

Input is accepted if it contains a double that is within the valid range and is in the correct format.
如果输入包含的浮点数在有效范围内且格式正确，则输入被 接受。

Input is accepted but invalid if it contains a double that is outside the range or is in the wrong format; e.g., too many digits after the decimal point or is empty.
如果输入包含的浮点数超出范围或格式不正确（例如小数点后位数过多或为空），则输入被 接受但无效。

Input is rejected if it is not a double.
如果输入不是浮点数，则输入被 拒绝。

Note:
If the valid range consists of just positive doubles (e.g., 0.0 to 100.0) and input is a negative double, then it is rejected.
如果有效范围仅包含正数（例如 0.0 到 100.0），而输入为负数，则被 拒绝。

If notation is set to DoubleValidator.StandardNotation and the input contains more digits before the decimal point than a double in the valid range may have, it is also rejected.
如果 notation 设置为 DoubleValidator.StandardNotation，且输入小数点前的数字超过有效范围允许的位数，也会被 拒绝。

If notation is DoubleValidator.ScientificNotation, and the input is not in the valid range, it is accepted but invalid. The value may yet become valid by changing the exponent.
如果 notation 设置为 DoubleValidator.ScientificNotation，且输入不在有效范围内，则被 接受但无效。通过调整指数值，输入可能变为有效。
```

## 2.重要属性

### bottom : real

```markdown
This property holds the validator's minimum acceptable value.
此属性表示验证器允许的最小值。
By default, this property contains a value of -infinity.
默认情况下，此属性的值为 -∞（负无穷）。
```

### decimals : int

```
This property holds the validator's maximum number of digits after the decimal point.
此属性表示小数点后的最大位数。
By default, this property contains a value of 1000.
默认情况下，此属性的值为 1000。
```

### locale : string

```
This property holds the name of the locale used to interpret the number.
此属性表示用于解析数字的区域（locale）名称。
```

### notation : enumeration

```
This property holds the notation of how a string can describe a number.
此属性表示字符串描述数字的表示法（notation）。

The possible values for this property are:
此属性的可选值如下：

Constant							Description
DoubleValidator.StandardNotation	仅允许带可选符号的十进制数字（例如 -0.015）
DoubleValidator.ScientificNotation	（默认值）允许数字带指数部分（例如 1.5E-2）
```

### top : real

```
This property holds the validator's maximum acceptable value.
此属性表示验证器允许的最大值。

By default, this property contains a value of infinity.
默认情况下，此属性的值为 ∞（正无穷）。
```

# 3.3 RegularExpressionValidator类

## 1.说明

```markdown
Provides a string validator.
提供一个字符串验证器。

The RegularExpressionValidator type provides a validator that counts as valid any string which matches a specified regular expression.
RegularExpressionValidator 类型提供一个验证器，任何 匹配指定正则表达式 的字符串都被视为有效。
```

## 2.重要属性

### regularExpression : regularExpression

```markdown
This property holds the regular expression used for validation.
此属性表示用于验证的正则表达式。

Note that this property should be a regular expression in JS syntax, e.g., /a/ for the regular expression matching "a".
请注意，此属性应使用 JavaScript 语法的正则表达式，例如 /a/ 表示匹配 "a" 的正则表达式。

By default, this property contains a regular expression with the pattern .* that matches any string.
默认情况下，此属性为 .*，可匹配任意字符串。

Example: A TextInput with a RegularExpressionValidator
示例：TextInput 使用 RegularExpressionValidator

TextInput {
    id: hexNumber
    validator: RegularExpressionValidator { regularExpression: /[0-9A-F]+/ }
}


More examples of regular expressions:
更多正则表达式示例：

A list of numbers with one to three digits separated by a comma:
一个由 1 到 3 位数字组成、以逗号分隔的数字列表
/\d{1,3}(?:,\d{1,3})+$/


An amount consisting of up to 3 digits before the decimal point, and 1 to 2 after the decimal point:
小数点前最多 3 位数字，小数点后 1 到 2 位数字的金额
/(\d{1,3})([.,]\d{1,2})?$/
```



# 4.1 ButtonGroup类

## 1.说明

```markdown
Mutually-exclusive group of checkable buttons.
互斥的可选按钮组。

ButtonGroup is a non-visual, mutually exclusive group of buttons. It is used with controls such as RadioButton, where only one of the options can be selected at a time.
ButtonGroup 是一个非可视的互斥按钮组。它用于像 RadioButton 这样的控件，其中一次只能选择一个选项。

The most straight-forward way to use ButtonGroup is to assign a list of buttons. For example, the list of children of a positioner or a layout that manages a group of mutually exclusive buttons.
使用 ButtonGroup 最直接的方法是分配一个按钮列表。例如，分配一个位置器或布局管理的互斥按钮组的子元素列表。

ButtonGroup {
    buttons: column.children
}

Column {
    id: column

    RadioButton {
        checked: true
        text: qsTr("DAB")
    }

    RadioButton {
        text: qsTr("FM")
    }

    RadioButton {
        text: qsTr("AM")
    }
}

Mutually exclusive buttons do not always share the same parent item, or the parent layout may sometimes contain items that should not be included in the button group. Such cases are best handled using the group attached property.
互斥按钮并不总是共享相同的父元素，或者父布局有时可能包含不应包括在按钮组中的元素。这种情况最好使用 group 附加属性来处理。

ButtonGroup { id: radioGroup }

Column {
    Label {
        text: qsTr("Radio:")
    }

    RadioButton {
        checked: true
        text: qsTr("DAB")
        ButtonGroup.group: radioGroup
    }

    RadioButton {
        text: qsTr("FM")
        ButtonGroup.group: radioGroup
    }

    RadioButton {
        text: qsTr("AM")
        ButtonGroup.group: radioGroup
    }
}


More advanced use cases can be handled using the addButton() and removeButton() methods.
更高级的使用场景可以通过 addButton() 和 removeButton() 方法来处理。
```

## 2.重要属性

### buttons : list&lt;AbstractButton&gt;

```
This property holds the list of buttons.
此属性保存按钮列表。

ButtonGroup {
    buttons: column.children
}

Column {
    id: column

    RadioButton {
        checked: true
        text: qsTr("Option A")
    }

    RadioButton {
        text: qsTr("Option B")
    }
}
```

### [since QtQuick.Controls 2.4 (Qt 5.11)] checkState : enumeration

```
This property holds the combined check state of the button group.
此属性保存按钮组的组合选中状态。

Available states:
可用状态：
Qt.Unchecked — 没有按钮被选中。
Qt.PartiallyChecked — 部分按钮被选中。
Qt.Checked — 所有按钮都被选中。

Setting the check state of a non-exclusive button group to Qt.Unchecked or Qt.Checked unchecks or checks all buttons in the group, respectively. Qt.PartiallyChecked is ignored.
将非互斥按钮组的选中状态设置为 Qt.Unchecked 或 Qt.Checked 会分别取消选中或选中组中的所有按钮。Qt.PartiallyChecked 会被忽略。

Setting the check state of an exclusive button group to Qt.Unchecked unchecks the checkedButton. Qt.Checked and Qt.PartiallyChecked are ignored.
将互斥按钮组的选中状态设置为 Qt.Unchecked 会取消选中当前选中的按钮。Qt.Checked 和 Qt.PartiallyChecked 会被忽略。
```

### checkedButton : AbstractButton

```
This property holds the currently selected button in an exclusive group, or null if there is none or the group is non-exclusive.
此属性保存互斥组中当前选中的按钮，如果没有选中按钮或组不是互斥的，则为 null。

By default, it is the first checked button added to an exclusive button group.
默认情况下，它是第一个被添加到互斥按钮组中且选中的按钮。
```

### [since QtQuick.Controls 2.3 (Qt 5.10)] exclusive : bool

```
This property holds whether the button group is exclusive. The default value is true.
此属性表示按钮组是否为互斥组。默认值为 true。

If this property is true, then only one button in the group can be checked at any given time. The user can click on any button to check it, and that button will replace the existing one as the checked button in the group.
如果该属性为 true，则在任意时刻组内只能选中一个按钮。用户可以点击任意按钮进行选中，该按钮将替换组内当前选中的按钮。

In an exclusive group, the user cannot uncheck the currently checked button by clicking on it; instead, another button in the group must be clicked to set the new checked button for that group.
在互斥组中，用户不能通过点击当前选中的按钮来取消选中；必须点击组内的另一个按钮来设置新的选中按钮。

In a non-exclusive group, checking and unchecking buttons does not affect the other buttons in the group. Furthermore, the value of the checkedButton property is null.
在非互斥组中，选中或取消选中按钮不会影响组内其他按钮。此外，checkedButton 属性的值为 null。
```

### ButtonGroup.group : ButtonGroup[Attached Property]

```
This property attaches a button to a button group.
此属性用于将按钮附加到按钮组。

ButtonGroup { id: group }

RadioButton {
    checked: true
    text: qsTr("Option A")
    ButtonGroup.group: group
}

RadioButton {
    text: qsTr("Option B")
    ButtonGroup.group: group
}
```

## 3.重要信号

### [since QtQuick.Controls 2.1 (Qt 5.8)] clicked(AbstractButton button)

```
This signal is emitted when a button in the group has been clicked.
当组中的按钮被点击时，会发出此信号。

This signal is convenient for implementing a common signal handler for all buttons in the same group.
该信号便于为组内所有按钮实现一个通用的信号处理器。

ButtonGroup {
    buttons: column.children
    onClicked: console.log("clicked:", button.text)
}

Column {
    id: column
    Button { text: "First" }
    Button { text: "Second" }
    Button { text: "Third" }
}


Note: The corresponding handler is onClicked.
注意：对应的处理器为 onClicked。
```

## 4.重要方法

### void addButton(AbstractButton button)

```
Adds a button to the button group.
向按钮组中添加一个按钮。

Note: Manually adding objects to a button group is typically unnecessary. The buttons property and the group attached property provide a convenient and declarative syntax.
注意：通常无需手动向按钮组添加对象。buttons 属性和 group 附加属性提供了方便且声明式的语法。
```

### void removeButton(AbstractButton button)

```
Removes a button from the button group.
从按钮组中移除一个按钮。

Note: Manually removing objects from a button group is typically unnecessary. The buttons property and the group attached property provide a convenient and declarative syntax.
注意：通常无需手动从按钮组中移除对象。buttons 属性和 group 附加属性提供了方便且声明式的语法。
```

# 4.2 Button类

## 1.说明

```markdown
Push-button that can be clicked to perform a command or answer a question.
可点击的推按钮，用于执行命令或回答问题。

Button presents a push-button control that can be pushed or clicked by the user. Buttons are normally used to perform an action, or to answer a question. Typical buttons are OK, Apply, Cancel, Close, Yes, No, and Help.
Button 提供了一个可由用户点击或按下的推按钮控件。按钮通常用于执行操作或回答问题。典型按钮有 OK、Apply、Cancel、Close、Yes、No 和 Help。

Button inherits its API from AbstractButton. For instance, you can set text, display an icon, and react to clicks using the AbstractButton API.
Button 继承自 AbstractButton 的 API。例如，你可以设置文本、显示图标，并使用 AbstractButton 的 API 响应点击事件。

A button emits the signal clicked() when it is activated by the user. Connect to this signal to perform the button's action. Buttons also provide the signals canceled(), doubleClicked(), pressed(), released() and pressAndHold() for long presses.
当用户激活按钮时，按钮会发出 clicked() 信号。连接此信号即可执行按钮的操作。按钮还提供 canceled()、doubleClicked()、pressed()、released() 和 pressAndHold()（长按）信号。

See the snippet below on how to connect to the button's signals.
请参见下面的代码片段，了解如何连接按钮信号：

RowLayout {
    Button {
        text: "Ok"
        onClicked: model.submit()
    }
    Button {
        text: "Cancel"
        onClicked: model.revert()
    }
}

```

## 2.重要属性

### buttons : list&lt;AbstractButton&gt;

```
This property holds the list of buttons.
此属性保存按钮列表。

ButtonGroup {
    buttons: column.children
}

Column {
    id: column

    RadioButton {
        checked: true
        text: qsTr("Option A")
    }

    RadioButton {
        text: qsTr("Option B")
    }
}
```

### [since QtQuick.Controls 2.4 (Qt 5.11)] checkState : enumeration

```
This property holds the combined check state of the button group.
此属性保存按钮组的组合选中状态。

Available states:
可用状态：
Qt.Unchecked — 没有按钮被选中。
Qt.PartiallyChecked — 部分按钮被选中。
Qt.Checked — 所有按钮都被选中。

Setting the check state of a non-exclusive button group to Qt.Unchecked or Qt.Checked unchecks or checks all buttons in the group, respectively. Qt.PartiallyChecked is ignored.
将非互斥按钮组的选中状态设置为 Qt.Unchecked 或 Qt.Checked 会分别取消选中或选中组中的所有按钮。Qt.PartiallyChecked 会被忽略。

Setting the check state of an exclusive button group to Qt.Unchecked unchecks the checkedButton. Qt.Checked and Qt.PartiallyChecked are ignored.
将互斥按钮组的选中状态设置为 Qt.Unchecked 会取消选中当前选中的按钮。Qt.Checked 和 Qt.PartiallyChecked 会被忽略。
```

### checkedButton : AbstractButton

```
This property holds the currently selected button in an exclusive group, or null if there is none or the group is non-exclusive.
此属性保存互斥组中当前选中的按钮，如果没有选中按钮或组不是互斥的，则为 null。

By default, it is the first checked button added to an exclusive button group.
默认情况下，它是第一个被添加到互斥按钮组中且选中的按钮。
```

### [since QtQuick.Controls 2.3 (Qt 5.10)] exclusive : bool

```
This property holds whether the button group is exclusive. The default value is true.
此属性表示按钮组是否为互斥组。默认值为 true。

If this property is true, then only one button in the group can be checked at any given time. The user can click on any button to check it, and that button will replace the existing one as the checked button in the group.
如果该属性为 true，则在任意时刻组内只能选中一个按钮。用户可以点击任意按钮进行选中，该按钮将替换组内当前选中的按钮。

In an exclusive group, the user cannot uncheck the currently checked button by clicking on it; instead, another button in the group must be clicked to set the new checked button for that group.
在互斥组中，用户不能通过点击当前选中的按钮来取消选中；必须点击组内的另一个按钮来设置新的选中按钮。

In a non-exclusive group, checking and unchecking buttons does not affect the other buttons in the group. Furthermore, the value of the checkedButton property is null.
在非互斥组中，选中或取消选中按钮不会影响组内其他按钮。此外，checkedButton 属性的值为 null。
```

### ButtonGroup.group : ButtonGroup[Attached Property]

```
This property attaches a button to a button group.
此属性用于将按钮附加到按钮组。

ButtonGroup { id: group }

RadioButton {
    checked: true
    text: qsTr("Option A")
    ButtonGroup.group: group
}

RadioButton {
    text: qsTr("Option B")
    ButtonGroup.group: group
}
```

## 3.重要信号

### [since QtQuick.Controls 2.1 (Qt 5.8)] clicked(AbstractButton button)

```
This signal is emitted when a button in the group has been clicked.
当组中的按钮被点击时，会发出此信号。

This signal is convenient for implementing a common signal handler for all buttons in the same group.
该信号便于为组内所有按钮实现一个通用的信号处理器。

ButtonGroup {
    buttons: column.children
    onClicked: console.log("clicked:", button.text)
}

Column {
    id: column
    Button { text: "First" }
    Button { text: "Second" }
    Button { text: "Third" }
}


Note: The corresponding handler is onClicked.
注意：对应的处理器为 onClicked。
```

## 4.重要方法

### void addButton(AbstractButton button)

```
Adds a button to the button group.
向按钮组中添加一个按钮。

Note: Manually adding objects to a button group is typically unnecessary. The buttons property and the group attached property provide a convenient and declarative syntax.
注意：通常无需手动向按钮组添加对象。buttons 属性和 group 附加属性提供了方便且声明式的语法。
```

### void removeButton(AbstractButton button)

```
Removes a button from the button group.
从按钮组中移除一个按钮。

Note: Manually removing objects from a button group is typically unnecessary. The buttons property and the group attached property provide a convenient and declarative syntax.
注意：通常无需手动从按钮组中移除对象。buttons 属性和 group 附加属性提供了方便且声明式的语法。
```

# 4.3 AbstractButton类

## 1.说明

```markdown
Abstract base type providing functionality common to buttons.
提供按钮通用功能的抽象基类。

AbstractButton provides the interface for controls with button-like behavior; for example, push buttons and checkable controls like radio buttons and check boxes. As an abstract control, it has no delegate implementations, leaving them to the types that derive from it.
AbstractButton 为具有按钮行为的控件提供接口，例如推按钮以及可选控件如单选按钮和复选框。作为一个抽象控件，它没有具体的委托实现，由继承它的子类型来完成具体实现。
```

## 2.重要属性

### indicator : Item

```
This property holds the indicator item.
此属性保存指示器项。
```

### [since QtQuick.Controls 2.4 (Qt 5.11)] checkState : enumeration

```
This property holds the combined check state of the button group.
此属性保存按钮组的组合选中状态。

Available states:
可用状态：
Qt.Unchecked — 没有按钮被选中。
Qt.PartiallyChecked — 部分按钮被选中。
Qt.Checked — 所有按钮都被选中。

Setting the check state of a non-exclusive button group to Qt.Unchecked or Qt.Checked unchecks or checks all buttons in the group, respectively. Qt.PartiallyChecked is ignored.
将非互斥按钮组的选中状态设置为 Qt.Unchecked 或 Qt.Checked 会分别取消选中或选中组中的所有按钮。Qt.PartiallyChecked 会被忽略。

Setting the check state of an exclusive button group to Qt.Unchecked unchecks the checkedButton. Qt.Checked and Qt.PartiallyChecked are ignored.
将互斥按钮组的选中状态设置为 Qt.Unchecked 会取消选中当前选中的按钮。Qt.Checked 和 Qt.PartiallyChecked 会被忽略。
```

### checkedButton : AbstractButton

```
This property holds the currently selected button in an exclusive group, or null if there is none or the group is non-exclusive.
此属性保存互斥组中当前选中的按钮，如果没有选中按钮或组不是互斥的，则为 null。

By default, it is the first checked button added to an exclusive button group.
默认情况下，它是第一个被添加到互斥按钮组中且选中的按钮。
```

### [since QtQuick.Controls 2.3 (Qt 5.10)] exclusive : bool

```
This property holds whether the button group is exclusive. The default value is true.
此属性表示按钮组是否为互斥组。默认值为 true。

If this property is true, then only one button in the group can be checked at any given time. The user can click on any button to check it, and that button will replace the existing one as the checked button in the group.
如果该属性为 true，则在任意时刻组内只能选中一个按钮。用户可以点击任意按钮进行选中，该按钮将替换组内当前选中的按钮。

In an exclusive group, the user cannot uncheck the currently checked button by clicking on it; instead, another button in the group must be clicked to set the new checked button for that group.
在互斥组中，用户不能通过点击当前选中的按钮来取消选中；必须点击组内的另一个按钮来设置新的选中按钮。

In a non-exclusive group, checking and unchecking buttons does not affect the other buttons in the group. Furthermore, the value of the checkedButton property is null.
在非互斥组中，选中或取消选中按钮不会影响组内其他按钮。此外，checkedButton 属性的值为 null。
```

### ButtonGroup.group : ButtonGroup[Attached Property]

```
This property attaches a button to a button group.
此属性用于将按钮附加到按钮组。

ButtonGroup { id: group }

RadioButton {
    checked: true
    text: qsTr("Option A")
    ButtonGroup.group: group
}

RadioButton {
    text: qsTr("Option B")
    ButtonGroup.group: group
}
```

## 3.重要信号

### [since QtQuick.Controls 2.1 (Qt 5.8)] clicked(AbstractButton button)

```
This signal is emitted when a button in the group has been clicked.
当组中的按钮被点击时，会发出此信号。

This signal is convenient for implementing a common signal handler for all buttons in the same group.
该信号便于为组内所有按钮实现一个通用的信号处理器。

ButtonGroup {
    buttons: column.children
    onClicked: console.log("clicked:", button.text)
}

Column {
    id: column
    Button { text: "First" }
    Button { text: "Second" }
    Button { text: "Third" }
}


Note: The corresponding handler is onClicked.
注意：对应的处理器为 onClicked。
```

## 4.重要方法

### void addButton(AbstractButton button)

```
Adds a button to the button group.
向按钮组中添加一个按钮。

Note: Manually adding objects to a button group is typically unnecessary. The buttons property and the group attached property provide a convenient and declarative syntax.
注意：通常无需手动向按钮组添加对象。buttons 属性和 group 附加属性提供了方便且声明式的语法。
```

### void removeButton(AbstractButton button)

```
Removes a button from the button group.
从按钮组中移除一个按钮。

Note: Manually removing objects from a button group is typically unnecessary. The buttons property and the group attached property provide a convenient and declarative syntax.
注意：通常无需手动从按钮组中移除对象。buttons 属性和 group 附加属性提供了方便且声明式的语法。
```

# 4.4 CheckBox类

## 1.说明

```markdown
Check button that can be toggled on or off.
可切换的复选按钮，可打开或关闭。

[√] First
[ ] Second
[√] Third
CheckBox presents an option button that can be toggled on (checked) or off (unchecked). Check boxes are typically used to select one or more options from a set of options. For larger sets of options, such as those in a list, consider using CheckDelegate instead.
CheckBox 提供了一个可切换的选项按钮，可选中（checked）或取消选中（unchecked）。复选框通常用于从一组选项中选择一个或多个选项。对于较大的选项集合，例如列表中的选项，可以考虑使用 CheckDelegate。

CheckBox inherits its API from AbstractButton. For instance, the state of the checkbox can be set with the checked property.
CheckBox 继承自 AbstractButton 的 API。例如，可以通过 checked 属性设置复选框的状态。

In addition to the checked and unchecked states, there is a third state: partially checked. The partially checked state can be enabled using the tristate property. This state indicates that the regular checked/unchecked state can not be determined; generally because of other states that affect the checkbox. This state is useful when several child nodes are selected in a treeview, for example.
除了 checked 和 unchecked 状态外，还有第三种状态：部分选中（partially checked）。可以通过 tristate 属性启用部分选中状态。此状态表示无法确定常规的选中/未选中状态，通常是由于影响复选框的其他状态导致的。例如，当树视图中选择了多个子节点时，这个状态非常有用。

ColumnLayout {
    CheckBox {
        checked: true
        text: qsTr("First")
    }
    CheckBox {
        text: qsTr("Second")
    }
    CheckBox {
        checked: true
        text: qsTr("Third")
    }
}


Hierarchical checkbox groups can be managed with a non-exclusive ButtonGroup.
层级复选框组可以通过非互斥 ButtonGroup 管理。
[-] Parent
	[√] Child 1
	[ ] Child 2

The following example illustrates how the combined check state of children can be bound to the check state of the parent checkbox:
下面的示例说明了如何将子复选框的组合选中状态绑定到父复选框的选中状态：
Column {
    ButtonGroup {
        id: childGroup
        exclusive: false
        checkState: parentBox.checkState
    }

    CheckBox {
        id: parentBox
        text: qsTr("Parent")
        checkState: childGroup.checkState
    }

    CheckBox {
        checked: true
        text: qsTr("Child 1")
        leftPadding: indicator.width
        ButtonGroup.group: childGroup
    }

    CheckBox {
        text: qsTr("Child 2")
        leftPadding: indicator.width
        ButtonGroup.group: childGroup
    }
}
```

## 2.重要属性

### checkState : enumeration

```
This property holds the check state of the checkbox.
此属性保存复选框的选中状态。

Available states:
可用状态：
Qt.Unchecked — 复选框未选中。
Qt.PartiallyChecked — 复选框部分选中。此状态仅在启用 tristate 时使用。
Qt.Checked — 复选框已选中。
```

### [since QtQuick.Controls 2.4 (Qt 5.11)] nextCheckState : function

```
This property holds a callback function that is called to determine the next check state whenever the checkbox is interactively toggled by the user via touch, mouse, or keyboard.
此属性保存一个回调函数，当用户通过触控、鼠标或键盘交互切换复选框状态时，该函数会被调用以确定下一个选中状态。

By default, a normal checkbox cycles between Qt.Unchecked and Qt.Checked states, and a tri-state checkbox cycles between Qt.Unchecked, Qt.PartiallyChecked, and Qt.Checked states.
默认情况下，普通复选框在 Qt.Unchecked 和 Qt.Checked 状态之间循环，而三态复选框在 Qt.Unchecked、Qt.PartiallyChecked 和 Qt.Checked 状态之间循环。

The nextCheckState callback function can override the default behavior. The following example implements a tri-state checkbox that can present a partially checked state depending on external conditions, but never cycles to the partially checked state when interactively toggled by the user.
nextCheckState 回调函数可以覆盖默认行为。下面的示例实现了一个三态复选框，它可以根据外部条件呈现部分选中状态，但在用户交互切换时永远不会循环到部分选中状态。

CheckBox {
    tristate: true
    checkState: allChildrenChecked ? Qt.Checked :
                anyChildChecked ? Qt.PartiallyChecked : Qt.Unchecked

    nextCheckState: function() {
        if (checkState === Qt.Checked)
            return Qt.Unchecked
        else
            return Qt.Checked
    }
}
```

### tristate : bool

```
This property holds whether the checkbox is a tri-state checkbox.
此属性表示复选框是否为三态复选框。

In the animation below, the first checkbox is tri-state:
在下面的动画中，第一个复选框是三态复选框：

The default is false, i.e., the checkbox has only two states.
默认值为 false，即复选框仅有两种状态。
```

## 3.重要信号

## 4.重要方法

# 4.5 Slider 类

## 1.说明

```
Used to select a value by sliding a handle along a track.
用于通过沿着轨道滑动一个滑块来选择一个值。

Slider is used to select a value by sliding a handle along a track. The user can drag the handle to a specific position within a defined range, and the slider will adjust the value accordingly.
Slider 用于通过沿着轨道滑动一个滑块来选择一个值。用户可以将滑块拖动到定义范围内的特定位置，滑块会根据该位置调整值。

 Slider {
     from: 1
     value: 25
     to: 100
 }

The position property is expressed as a fraction of the control's size, in the range 0.0 - 1.0. The visualPosition property is the same, except that it is reversed in a right-to-left application. The visualPosition is useful for positioning the handle when styling Slider. 
position 属性 表示为控件大小的比例，范围在 0.0 到 1.0 之间。visualPosition 属性 与 position 相同，不同之处在于在右到左的布局中，它的值是反向的。visualPosition 在样式化 Slider 时，用于定位滑块的把手。
In the example above, visualPosition will be 0.24 in a left-to-right application, and 0.76 in a right-to-left application.
在上面的示例中，visualPosition 在从左到右的应用中为 0.24，而在从右到左的应用中为 0.76。
```

## **2.重要属性**

### from : real

```
This property holds the starting value for the range. The default value is 0.0.
此属性保存范围的起始值。默认值为 0.0。
```

### **handle : Item**

This property holds the handle item.

这个属性保存滑块的滑动部分。

### horizontal : bool

This property holds whether the slider is horizontal. This property was introduced in QtQuick.Controls 2.3 (Qt 5.10).

这个属性表示滑块是否是水平的。此属性在 QtQuick.Controls 2.3（Qt 5.10）中引入。

### **implicitHandleHeight : real**

This property holds the implicit handle height. The value is equal to handle ? handle.implicitHeight : 0.

这个属性保存隐式的滑块高度。其值等于 `handle ? handle.implicitHeight : 0`。

### **implicitHandleWidth : real**

This property holds the implicit handle width. The value is equal to handle ? handle.implicitWidth : 0.

这个属性保存隐式的滑块宽度。其值等于 `handle ? handle.implicitWidth : 0`。

### **live : bool**

This property holds whether the slider provides live updates for the value property while the handle is dragged. The default value is true.

这个属性表示滑块在拖动时是否实时更新值。默认值为 true。

### **orientation : enumeration**

This property holds the orientation. Possible values: `Qt.Horizontal` (default), `Qt.Vertical`.

这个属性表示滑块的方向。可能的值：`Qt.Horizontal`（默认），`Qt.Vertical`。

### **position : real**

```
This property holds the logical position of the handle. The position is expressed as a fraction of the control's size, in the range 0.0 - 1.0.
此属性保存滑块柄的逻辑位置。该位置表示为控件大小的一个比例，范围在 0.0 - 1.0 之间。
```

### **pressed : bool**

```
This property holds whether the slider is pressed by either touch, mouse, or keys.
此属性保存滑块是否正被触摸、鼠标或按键按下。
```

### **snapMode : enumeration**

```
This property holds the snap mode. The snap mode determines how the slider handle behaves with regards to the stepSize.
此属性保存捕捉模式。捕捉模式决定滑块柄在与 stepSize 相关时的行为方式。
```

#### 可选值：

- **`Slider.NoSnap`**
     滑块不会对齐（默认值）。
     **描述**: 滑块在拖动时不进行对齐。
- **`Slider.SnapAlways`**
     滑块在拖动时会进行对齐。
     **描述**: 滑块在拖动过程中总是会对齐到最近的步长。
- **`Slider.SnapOnRelease`**
     滑块在拖动时不会对齐，但在释放滑块时才会对齐到最近的步长。
     **描述**: 滑块在拖动时不会对齐，但只有当滑块被释放时，才会对齐到最近的步长。

### **stepSize : real**

```
This property holds the step size. The default value is 0.0.
此属性保存步长。默认值为 0.0。
```

### **to : real**

```
This property holds the end value for the range. The default value is 1.0.
此属性保存范围的结束值。默认值为 1.0。
```

### **touchDragThreshold : qreal**

This property holds the threshold (in logical pixels) at which a touch drag event will be initiated. The mouse drag threshold won't be affected.

这个属性表示触摸拖动事件启动的阈值（逻辑像素）。鼠标拖动阈值不受影响。

### **value : real**

```
This property holds the value in the range from from to to. The default value is 0.0.
此属性保存位于 from 到 to 范围内的值。默认值为 0.0。
```

### **visualPosition : real**

```
This property holds the visual position of the handle. The position is expressed as a fraction of the control's size, in the range 0.0 - 1.0.
此属性保存滑块柄的视觉位置。该位置表示为控件大小的一个比例，范围在 0.0 - 1.0 之间。
```

## **3.重要信号**

### **moved()**

```
This signal is emitted when the slider has been interactively moved by the user by either touch, mouse, wheel, or keys.
当用户通过触摸、鼠标、滚轮或按键交互式地移动滑块时，会发出此信号。
```

## **4.重要方法**

### **decrease()**

```
Decreases the value by stepSize or 0.1 if stepSize is not defined.
如果 stepSize 已定义，则将值减少 stepSize；如果未定义，则减少 0.1。
```

### **increase()**

```
Increases the value by stepSize or 0.1 if stepSize is not defined.
如果 stepSize 已定义，则将值增加 stepSize；如果未定义，则增加 0.1。
```

### **valueAt(position: real)**

```
Returns the value for the given position.
返回指定位置对应的值。
```

# 4.6 ComboBox类

## 1.说明

```markdown
Combined button and popup list for selecting options.
用于选择选项的组合按钮和弹出列表。

ComboBox is a combined button and popup list. It provides a means of presenting a list of options to the user in a way that takes up the minimum amount of screen space.
ComboBox is populated with a data model. The data model is commonly a JavaScript array, a ListModel or an integer, but other types of data models are also supported.
ComboBox是一个组合的按钮和弹出列表。 它提供了一种以占用最少屏幕空间的方式向用户呈现选项列表的方法。
ComboBox用数据模型填充。 数据模型通常是JavaScript数组、ListModel或整数，但也支持其他类型的数据模型。
 ComboBox {
     model: ["First", "Second", "Third"]
 }
```

```
Editable ComboBox

ComboBox can be made editable. An editable combo box auto-completes its text based on what is available in the model.
The following example demonstrates appending content to an editable combo box by reacting to the accepted signal.
ComboBox可以进行编辑。 可编辑组合框根据模型中的可用内容自动完成其文本。
下面的示例演示通过响应接受的信号将内容追加到可编辑的组合框中。
 ComboBox {
     editable: true
     model: ListModel {
         id: model
         ListElement { text: "Banana" }
         ListElement { text: "Apple" }
         ListElement { text: "Coconut" }
     }
     onAccepted: {
         if (find(editText) === -1)
             model.append({text: editText})
     }
 }

```

```
ComboBox's Popup

By default, clicking outside of ComboBox's popup will close it, and the event is propagated to items lower in the stacking order. To prevent the popup from closing, set its closePolicy:
默认情况下，在ComboBox弹出窗口之外单击将关闭它，并且事件将传播到堆叠顺序较低的项目。 要防止弹出窗口关闭，请设置其closePolicy:
popup.closePolicy: Popup.CloseOnEscape

To prevent event propagation, set its modal property to true:
要防止事件传播，请将其modal属性设置为true:
popup.modal: true
```

```
ComboBox is able to visualize standard data models that provide the modelData role:
    models that have only one role
    models that do not have named roles (JavaScript array, integer)
ComboBox能够可视化提供modelData角色的标准数据模型:
    只有一个角色的模型
    没有命名角色的模型（JavaScript数组，整数）

When using models that have multiple named roles, ComboBox must be configured to use a specific text role for its display text and delegate instances. If you want to use a role of the model item that corresponds to the text role, set valueRole. The currentValue property and indexOfValue() method can then be used to get information about those values.
使用具有多个命名角色的模型时，必须将ComboBox配置为将特定文本角色用于其显示文本和委托实例。 如果要使用与文本角色相对应的模型项的角色，请设置valueRole。 然后可以使用currentValue属性和indexOfValue()方法来获取有关这些值的信息。
 ApplicationWindow {
     width: 640
     height: 480
     visible: true

     // Used as an example of a backend - this would usually be
     // e.g. a C++ type exposed to QML.
     QtObject {
         id: backend
         property int modifier
     }

     ComboBox {
         textRole: "text"
         valueRole: "value"
         // When an item is selected, update the backend.
         onActivated: backend.modifier = currentValue
         // Set the initial currentIndex to the value stored in the backend.
         Component.onCompleted: currentIndex = indexOfValue(backend.modifier)
         model: [
             { value: Qt.NoModifier, text: qsTr("No modifier") },
             { value: Qt.ShiftModifier, text: qsTr("Shift") },
             { value: Qt.ControlModifier, text: qsTr("Control") }
         ]
     }
 }

Note: If ComboBox is assigned a data model that has multiple named roles, but textRole is not defined, ComboBox is unable to visualize it and throws a ReferenceError: modelData is not defined.
注意：如果为ComboBox分配了一个具有多个命名角色的数据模型，但未定义textRole，则ComboBox无法对其进行可视化，并引发ReferenceError:modeldata未定义。
```



## 2.重要属性

### currentIndex : int

```

```

### displayText : string

```

```

### model : model

```

```

### [since QtQuick.Controls 2.2 (Qt 5.9)] editable : bool

```

```



## 3.重要信号

### void activated(int index)

```

```

### void highlighted(int index)

```
This signal is emitted when the item at index in the popup list is highlighted by the user.
The highlighted signal is only emitted when the popup is open and an item is highlighted, but not necessarily activated.
Note: The corresponding handler is onHighlighted.
```



## 4.重要方法

# 4.7 Image类

## 1.说明

```markdown
Image 类型显示一张图片。

图片的源通过 source 属性指定为一个 URL。图片可以是 Qt 支持的任何标准图像格式，包括位图格式如 PNG 和 JPEG，以及矢量图形格式如 SVG。如果需要显示动画图片，可以使用 AnimatedSprite 或 AnimatedImage。

如果没有指定 width 和 height 属性，Image 会自动使用加载图片的大小。默认情况下，指定 width 和 height 属性会导致图片按指定的大小进行缩放。此行为可以通过设置 fillMode 属性来改变，从而允许图片被拉伸或平铺。
```

```
示例用法

以下示例展示了 Image 类型的最简单用法。

import QtQuick

Image {
    source: "pics/qtlogo.png"
}
```

```
压缩纹理文件

当底层图形 API 在运行时支持时，图像也可以通过压缩纹理文件提供。内容必须是一个简单的 RGB(A) 格式 2D 纹理。支持的压缩方案仅受限于底层驱动程序和 GPU。以下容器文件格式受支持：

    PKM（从 Qt 5.10 开始）
    KTX（从 Qt 5.11 开始）
    ASTC（从 Qt 5.13 开始）

注意：
纹理文件中图像的垂直方向通常没有明确定义。不同的纹理压缩工具在何时进行输入图像的垂直翻转有不同的默认设置和选项。如果纹理文件中的图像出现倒置，可能需要在资产处理过程中切换翻转设置。或者，可以通过 transform 属性应用适当的转换，或者通过设置 mirrorVertically 属性来便捷地翻转 Image 元素：

transform: [ Translate { y: -myImage.height }, Scale { yScale: -1 } ]
或者
mirrorVertically: true


注意：
半透明原始图像需要在纹理压缩前进行 alpha 预乘，才能正确显示在 Qt Quick 中。这可以通过以下 ImageMagick 命令行完成：

convert foo.png \( +clone -alpha Extract \) -channel RGB -compose Multiply -composite foo_pm.png

不要混淆容器格式（如 KTX）与容器文件中存储的实际纹理数据格式。例如，读取 KTX 文件在所有平台上都受到支持，与运行时使用的 GPU 驱动无关。然而，这并不保证文件中使用的压缩纹理格式在运行时会被支持。例如，如果 KTX 文件包含格式为 ETC2 RGBA8 的压缩数据，而运行时使用的 3D 图形 API 实现不支持 ETC2 压缩纹理，则 Image 元素将不会显示任何内容。

注意：
压缩纹理格式的支持不受 Qt 的控制，应用程序或设备开发人员需要确保压缩纹理数据在目标环境中采用适当的格式。

不要假设压缩格式的支持是特定于平台的，它也可能特定于特定平台上使用的驱动程序和 3D API 实现。实际上，同一平台（如 Windows）上来自同一厂商的不同 3D 图形 API 实现（如 Vulkan 和 OpenGL），即便是针对相同硬件，也可能提供不同的压缩纹理格式集。

当仅面向桌面环境（Windows、macOS、Linux）时，一般建议考虑使用 DXTn/BCn 格式，因为这些格式通常在这些平台上的 Direct3D、Vulkan、OpenGL 和 Metal 实现中具有广泛的支持。相比之下，针对移动或嵌入式设备时，ETC2 或 ASTC 格式可能是更好的选择，因为这些通常是 OpenGL ES 实现所支持的格式。

计划在桌面、移动和嵌入式硬件上运行的应用程序应仔细规划和设计其压缩纹理的使用。很可能仅依赖于单一格式是不足够的，因此应用程序可能需要根据平台进行分支，以使用适当格式的压缩纹理，或者在某些情况下跳过使用压缩纹理。
```

```
自动检测文件扩展名

如果源 URL 指示的本地文件或资源不存在，Image 元素会尝试自动检测文件扩展名。如果通过将支持的图像文件扩展名附加到源 URL 后可以找到一个存在的文件，那么该文件将被加载。

文件搜索首先尝试查找压缩纹理容器文件扩展名。如果搜索失败，它会尝试使用常规图像文件类型的扩展名进行搜索。例如：

// 假设 "pics" 目录包含以下文件：
// dog.jpg
// cat.png
// cat.pkm

Image {
    source: "pics/cat.png"     // 加载 cat.png
}

Image {
    source: "pics/dog"         // 加载 dog.jpg
}

Image {
    source: "pics/cat"         // 通常加载 cat.pkm，但如果没有 OpenGL 支持，则加载 cat.png
}


此功能便于在不同的目标平台上部署不同类型的图像资源文件。这对于调整应用程序性能并适应不同的图形硬件非常有用。

此功能是在 Qt 5.11 中引入的。
```

```
性能
默认情况下，本地可用的图像会立即加载，并且用户界面会被阻塞，直到加载完成。如果要加载一张大图像，最好通过启用 asynchronous 属性将图像加载放入低优先级线程中。

如果图像来自网络而不是本地资源，它会自动异步加载，并根据需要更新 progress 和 status 属性。

图像会被内部缓存和共享，因此如果多个 Image 元素使用相同的源，只有一份图像会被加载。

注意：
图像通常是 QML 用户界面中占用内存最多的部分。建议对于不属于用户界面的一部分的图像，通过 sourceSize 属性限制其大小。对于从外部资源或用户提供的内容尤其重要。

另见 Qt Quick 示例 - 图像元素，QQuickImageProvider，以及 QImageReader::setAutoDetectImageFormat()。
```



## 2.重要属性

### [read-only] paintedHeight : real

### [read-only] paintedWidth : real

```
这些属性保存实际绘制的图像大小。在大多数情况下，它与 width 和 height 相同，但当使用 Image.PreserveAspectFit 或 Image.PreserveAspectCrop 时，paintedWidth 或 paintedHeight 可能会比 Image 项目的 width 和 height 小或大。
```

### horizontalAlignment : enumeration

### verticalAlignment : enumeration

```
设置图像的水平和垂直对齐方式。默认情况下，图像是居中对齐的。
horizontalAlignment 的有效值为：
    Image.AlignLeft
    Image.AlignRight
    Image.AlignHCenter

verticalAlignment 的有效值为：
    Image.AlignTop
    Image.AlignBottom
    Image.AlignVCenter
```

### currentFrame : int

### [read-only] frameCount : int

```
currentFrame 是当前显示的帧。默认值为 0。如果图像包含多帧，可以将其设置为 0 到 frameCount - 1 之间的数字，以显示不同的帧。

frameCount 是图像中的帧数。大多数图像只有一帧。
```

### asynchronous : bool

```
指定本地文件系统上的图像应在单独的线程中异步加载。默认值为 false，这会导致用户界面线程在图像加载时被阻塞。将 asynchronous 设置为 true 在需要保持响应式用户界面，而不是立即显示图像时非常有用。

请注意，此属性仅对从本地文件系统读取的图像有效。通过网络资源（例如 HTTP）加载的图像始终是异步加载的。
```

### autoTransform : bool

```
此属性决定图像是否应自动应用图像转换元数据，如 EXIF 方向。默认情况下，此属性设置为 false。
```

### cache : bool

```
指定图像是否应被缓存。默认值为 true。将 cache 设置为 false 在处理大图像时非常有用，可以确保它们不会被缓存，从而避免占用小型“UI 元素”图像的缓存空间。
```

### fillMode : enumeration

```
设置此属性以定义当源图像与项目的大小不同时发生的情况。
    Image.Stretch: 图像被缩放以适应大小。
    Image.PreserveAspectFit: 图像按比例均匀缩放以适应大小，且不会裁剪。
    Image.PreserveAspectCrop: 图像按比例均匀缩放以填充大小，必要时进行裁剪。
    Image.Tile: 图像在水平和垂直方向上重复平铺。
    Image.TileVertically: 图像在水平上拉伸，并在垂直方向上平铺。
    Image.TileHorizontally: 图像在垂直方向上拉伸，并在水平上平铺。
    Image.Pad: 图像不进行任何变换。
请注意，默认情况下 clip 属性为 false，这意味着即使 fillMode 设置为 PreserveAspectCrop，图像也可能会绘制在其边界矩形之外。
```

### mipmap : bool

```
此属性决定图像在缩放或变换时是否使用 mipmap 过滤。

Mipmap 过滤 在缩小时比平滑过滤提供更好的视觉质量，但可能会带来性能开销（包括图像初始化时和渲染时的性能消耗）。

默认情况下，此属性设置为 false。
```

### mirror : bool

```
此属性决定图像是否应水平翻转（实际上是显示一个镜像图像）。

默认值为 false。
```

### [since 6.2] mirrorVertically : bool

```
此属性决定图像是否应垂直翻转（实际上是显示一个镜像图像）。

默认值为 false。
此属性在 Qt 6.2 中引入。
```

### [read-only] progress : real

```
此属性表示图像加载的进度，从 0.0（未加载任何内容）到 1.0（加载完成）。
```

### smooth : bool

```
此属性决定图像在缩放或变换时是否进行平滑过滤。平滑过滤可以提供更好的视觉质量，但在某些硬件上可能会较慢。如果图像以其自然大小显示，则此属性对视觉效果或性能没有影响。
默认情况下，此属性设置为 true。
```

### source : url

```
Image 可以处理 Qt 支持的任何图像格式，且可以从 Qt 支持的任何 URL 协议加载图像。
URL 可以是绝对路径，也可以是相对于组件 URL 的相对路径。
```

### sourceClipRect : rect

```
此属性（如果设置）保存要加载的源图像的矩形区域。
sourceClipRect 与 sourceSize 属性配合使用，当只需要加载图像的一部分时，有助于节省系统资源。

例如：
Rectangle {
    width: ...
    height: ...

    Image {
        anchors.fill: parent
        source: "reallyBigImage.svg"
        sourceSize.width: 1024
        sourceSize.height: 1024
        sourceClipRect: Qt.rect(100, 100, 512, 512)
    }
}


在上面的示例中，我们首先将 SVG 图像概念性地缩放到 1024x1024，然后从距离顶部和左边缘 100 像素的位置裁剪出一个 512x512 像素的区域。因此，sourceSize 决定了缩放比例，而实际输出的图像大小为 512x512 像素。

某些图像格式能够通过仅渲染指定区域来节省 CPU 时间。其他格式则需要先加载整个图像，然后再将其裁剪为指定区域。

如果将此属性清空，可以通过将 sourceClipRect 设置为 undefined 来重新加载整个图像。

注意：
动态更改此属性会导致图像源被重新加载，如果图像不在磁盘缓存中，可能需要从网络重新加载。

注意：
不支持子像素裁剪：给定的矩形将传递给 QImageReader::setScaledClipRect()。
```

### sourceSize : size

```
此属性保存完整图像的缩放宽度和高度。
与 width 和 height 属性不同，sourceSize 设置了加载图像的最大像素数，以确保大型图像不会占用不必要的内存。例如，确保图像在内存中的大小不超过 1024x1024 像素，无论 Image 的 width 和 height 值是多少：

Rectangle {
    width: ...
    height: ...

    Image {
        anchors.fill: parent
        source: "reallyBigImage.jpg"
        sourceSize.width: 1024
        sourceSize.height: 1024
    }
}


如果图像的实际大小大于 sourceSize，图像将被缩小。如果仅设置一个维度的大小大于 0，另一个维度将按比例设置，以保持源图像的纵横比。（fillMode 与此无关。）

如果同时设置了 sourceSize.width 和 sourceSize.height，图像将被缩小以适应指定的大小（除非使用 PreserveAspectCrop 或 PreserveAspectFit，则会按最佳裁剪/适应尺寸进行缩放），并保持图像的纵横比。缩放后图像的实际大小可以通过 Item::implicitWidth 和 Item::implicitHeight 获取。

如果源是固有可缩放的图像（例如 SVG），此属性决定了加载图像的大小，而不考虑固有大小。避免动态更改此属性；与图像相比，渲染 SVG 较慢。

如果源是不可缩放的图像（例如 JPEG），则加载的图像不会大于此属性指定的大小。对于某些格式（目前仅支持 JPEG），整个图像实际上不会被加载到内存中。

如果同时设置了 sourceClipRect 属性，sourceSize 确定缩放比例，但会被裁剪为剪裁矩形的大小。

可以通过将 sourceSize 设置为 undefined 来清除此属性，恢复图像的自然大小。

注意：
动态更改此属性会导致图像源被重新加载，如果图像不在磁盘缓存中，可能需要从网络重新加载。
```

### [read-only] status : enumeration

```
此属性保存图像加载的状态。它可以是以下之一：
    Image.Null: 没有设置图像
    Image.Ready: 图像已加载完成
    Image.Loading: 图像正在加载
    Image.Error: 加载图像时发生错误

可以使用此状态来提供更新或响应状态变化。例如，您可以：

触发状态变化：

State { name: 'loaded'; when: image.status == Image.Ready }


实现 onStatusChanged 信号处理程序：

Image {
    id: image
    onStatusChanged: if (image.status == Image.Ready) console.log('Loaded')
}


绑定状态值：

Text { text: image.status == Image.Ready ? 'Loaded' : 'Not loaded' }
```

# 4.8 BorderImage类

## 1.说明

```
BorderImage 类型用于通过缩放或平铺图像的部分来创建图像边框。

BorderImage 将源图像（通过 source 属性指定）分成 9 个区域，如下所示：
1 | 2 | 3 
------------
4 | 5 | 6
------------
7 | 8 | 9
    角落区域（区域 1、3、7 和 9）完全不进行缩放。
    区域 2 和 8 按照 horizontalTileMode 进行缩放。
    区域 4 和 6 按照 verticalTileMode 进行缩放。
    中间区域（区域 5）同时按 horizontalTileMode 和 verticalTileMode 进行缩放。

图像的各个区域是通过 border 属性组定义的，该组描述了从源图像每个边缘开始使用作为边框的距离。
这样，BorderImage 可以根据给定的边距和缩放/平铺规则自动生成合适的边框效果，而不会变形角落部分或中间区域。
```

```
示例用法
以下示例展示了不同模式对图像的效果。为了说明不同区域的划分，图像上覆盖了指导线，显示了如上所述的图像区域。

为了比较，使用一个简单的 Image 元素来显示未缩放的图像。这里我们覆盖了线条，以显示如何使用 BorderImage 来划分图像：
Image {
    source: "pics/borderframe.png"
    anchors.centerIn: parent
}

当使用 BorderImage 显示图像时，border 属性用于确定图像的哪些部分将位于未缩放的角落区域，哪些部分将被水平和垂直拉伸。然后，您可以将其大小设置为大于原始图像。由于 horizontalTileMode 属性设置为 BorderImage.Stretch，图像中区域 2 和 8 的部分将水平拉伸。由于 verticalTileMode 属性设置为 BorderImage.Stretch，图像中区域 4 和 6 的部分将垂直拉伸：
BorderImage {
    anchors { fill: parent; margins: 1 }
    border { left: 30; top: 30; right: 30; bottom: 30 }
    horizontalTileMode: BorderImage.Stretch
    verticalTileMode: BorderImage.Stretch
    source: "pics/borderframe.png"
}

再次使用一个较大的 BorderImage 来显示图像。当 horizontalTileMode 属性设置为 BorderImage.Repeat 时，图像中区域 2 和 8 的部分将被平铺，以填充项的顶部和底部空间。类似地，当 verticalTileMode 属性设置为 BorderImage.Repeat 时，图像中区域 4 和 6 的部分将被平铺，以填充项的左右空间：
BorderImage {
    anchors { fill: parent; margins: 1 }
    border { left: 30; top: 30; right: 30; bottom: 30 }
    horizontalTileMode: BorderImage.Repeat
    verticalTileMode: BorderImage.Repeat
    source: "pics/borderframe.png"
}

在某些情况下，区域 2 和 8 的宽度可能不是源图像中对应区域宽度的整数倍。同样，区域 4 和 6 的高度可能也不是源图像中对应区域高度的整数倍。如果使用 BorderImage.Round 模式，它将选择一个整数数量的平铺并将其缩小以适应：
BorderImage {
    anchors { fill: parent; margins: 6 }
    border { left: 30; top: 30; right: 30; bottom: 30 }
    horizontalTileMode: BorderImage.Round
    verticalTileMode: BorderImage.Round
    source: "pics/borderframe.png"
}
```



## 2.重要属性

### horizontalTileMode : enumeration

### verticalTileMode : enumeration

```
此属性描述了如何重复或拉伸边框图像的中间部分。
    BorderImage.Stretch: 将图像缩放以适应可用区域。
    BorderImage.Repeat: 将图像平铺，直到没有空间为止。可能会裁剪最后一张图像。
    BorderImage.Round: 类似于 Repeat，但会缩小图像，确保最后一张图像不被裁剪。

每个属性的默认平铺模式是 BorderImage.Stretch。
```

### currentFrame : int

### [read-only] frameCount : int

```
currentFrame 是当前可见的帧。默认值为 0。如果图像包含多个帧，可以将其设置为 0 到 frameCount - 1 之间的数字，以显示不同的帧。

frameCount 是图像中的帧数。大多数图像只有一帧。
```

### asynchronous : bool

```
指定本地文件系统上的图像应在单独的线程中异步加载。默认值为 false，这会导致用户界面线程在图像加载时被阻塞。将 asynchronous 设置为 true 在需要保持响应式用户界面，而不是立即显示图像时非常有用。

请注意，此属性仅对从本地文件系统读取的图像有效。通过网络资源（例如 HTTP）加载的图像始终是异步加载的。
```

### border.bottom : int

### border.left : int

### border.right : int

### border.top : int

```
这四条边框线（2 条水平线和 2 条垂直线）将图像分成 9 个区域，如下所示：
1 | 2 | 3 
------------
4 | 5 | 6
------------
7 | 8 | 9

每条边框线（左、右、上、下）指定了相对于源图像边缘的偏移量，单位为像素。默认情况下，每条边框线的值为 0。

例如，以下定义将底部边框线设置为距离图像底部 10 像素的位置：

BorderImage {
    border.bottom: 10
    // ...
}


边框线也可以使用 .sci 文件来指定。
```

### cache : bool

```
指定图像是否应被缓存。默认值为 true。将 cache 设置为 false 在处理大图像时非常有用，可以确保它们不会被缓存，从而避免占用小型“UI 元素”图像的缓存空间。
```

### mirror : bool

```
此属性决定图像是否应水平翻转（实际上是显示一个镜像图像）。

默认值为 false。
```

### [read-only] progress : real

```
此属性表示图像加载的进度，从 0.0（未加载任何内容）到 1.0（加载完成）。
```

### smooth : bool

```
此属性决定图像在缩放或变换时是否进行平滑过滤。平滑过滤可以提供更好的视觉质量，但在某些硬件上可能会较慢。如果图像以其自然大小显示，则此属性对视觉效果或性能没有影响。
默认情况下，此属性设置为 true。
```

### source : url

```
此属性保存指向源图像的 URL。

BorderImage 可以处理 Qt 支持的任何图像格式，并且可以从 Qt 支持的任何 URL 协议加载图像。

此属性也可以用来引用 .sci 文件，.sci 文件是一个 QML 特定的文本格式，指定了边框、图像文件和给定边框图像的平铺规则。

以下 .sci 文件将图像 picture.png 的边框设置为每边 10 像素：
    border.left: 10
    border.top: 10
    border.bottom: 10
    border.right: 10
    source: "picture.png"

URL 可以是绝对路径，也可以是相对于组件 URL 的相对路径。
```

### sourceSize : size

```
此属性保存已加载图像的实际宽度和高度。

在 BorderImage 中，此属性是只读的。
```

### [read-only] status : enumeration

```
此属性描述图像加载的状态。它可以是以下之一：
    BorderImage.Null: 没有设置图像
    BorderImage.Ready: 图像已加载完成
    BorderImage.Loading: 图像正在加载
    BorderImage.Error: 加载图像时发生错误
```



# 4.9 ListView类

## 1.说明

```
BorderImage 类型用于通过缩放或平铺图像的部分来创建图像边框。

BorderImage 将源图像（通过 source 属性指定）分成 9 个区域，如下所示：
1 | 2 | 3 
------------
4 | 5 | 6
------------
7 | 8 | 9
    角落区域（区域 1、3、7 和 9）完全不进行缩放。
    区域 2 和 8 按照 horizontalTileMode 进行缩放。
    区域 4 和 6 按照 verticalTileMode 进行缩放。
    中间区域（区域 5）同时按 horizontalTileMode 和 verticalTileMode 进行缩放。

图像的各个区域是通过 border 属性组定义的，该组描述了从源图像每个边缘开始使用作为边框的距离。
这样，BorderImage 可以根据给定的边距和缩放/平铺规则自动生成合适的边框效果，而不会变形角落部分或中间区域。
```

```
示例用法
以下示例展示了不同模式对图像的效果。为了说明不同区域的划分，图像上覆盖了指导线，显示了如上所述的图像区域。

为了比较，使用一个简单的 Image 元素来显示未缩放的图像。这里我们覆盖了线条，以显示如何使用 BorderImage 来划分图像：
Image {
    source: "pics/borderframe.png"
    anchors.centerIn: parent
}

当使用 BorderImage 显示图像时，border 属性用于确定图像的哪些部分将位于未缩放的角落区域，哪些部分将被水平和垂直拉伸。然后，您可以将其大小设置为大于原始图像。由于 horizontalTileMode 属性设置为 BorderImage.Stretch，图像中区域 2 和 8 的部分将水平拉伸。由于 verticalTileMode 属性设置为 BorderImage.Stretch，图像中区域 4 和 6 的部分将垂直拉伸：
BorderImage {
    anchors { fill: parent; margins: 1 }
    border { left: 30; top: 30; right: 30; bottom: 30 }
    horizontalTileMode: BorderImage.Stretch
    verticalTileMode: BorderImage.Stretch
    source: "pics/borderframe.png"
}

再次使用一个较大的 BorderImage 来显示图像。当 horizontalTileMode 属性设置为 BorderImage.Repeat 时，图像中区域 2 和 8 的部分将被平铺，以填充项的顶部和底部空间。类似地，当 verticalTileMode 属性设置为 BorderImage.Repeat 时，图像中区域 4 和 6 的部分将被平铺，以填充项的左右空间：
BorderImage {
    anchors { fill: parent; margins: 1 }
    border { left: 30; top: 30; right: 30; bottom: 30 }
    horizontalTileMode: BorderImage.Repeat
    verticalTileMode: BorderImage.Repeat
    source: "pics/borderframe.png"
}

在某些情况下，区域 2 和 8 的宽度可能不是源图像中对应区域宽度的整数倍。同样，区域 4 和 6 的高度可能也不是源图像中对应区域高度的整数倍。如果使用 BorderImage.Round 模式，它将选择一个整数数量的平铺并将其缩小以适应：
BorderImage {
    anchors { fill: parent; margins: 6 }
    border { left: 30; top: 30; right: 30; bottom: 30 }
    horizontalTileMode: BorderImage.Round
    verticalTileMode: BorderImage.Round
    source: "pics/borderframe.png"
}
```



## 2.重要属性

### horizontalTileMode : enumeration
