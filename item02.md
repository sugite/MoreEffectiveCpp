最好使用C++转型操作符
=====

为解决C旧式转型的缺点（一是并非唯一选择，二是难以辨识），C++导入了4个新的转型操作符：`static_cast,const_cast,dynamic_cast,reinterpret_cast`，对大部分使用目的而言，面对这些操作符你唯一需要知道的便是，过去习惯的写码形式：

`(type) expression` 现在应该改为这样：

`static_cast<type>(expression)`

举个例子，int转为double，旧式C转型：

```C
int firstNumber, secondNumber ;
...
double result = ((double)firstNumber)/secondNumber ;
```

如果采用新的C++转型，应该这样：

```C++
double result = static_cast<double>(firstNumber)/secondNumber;
```

`static_cast`基本拥有与C旧式转型相同的威力与意义，其他新式C++转型操作符适用于更集中的目的。`const_cast`用来改变表达式中的常量性或易变性：

```C++
class Widget{...};
class SpecialWidget:public Widget{...};
void update(SpecialWidget *psw);
SpecialWidget sw ;
const SpecialWidget& csw = sw ;//csw是一个代表sw的reference
                                //并视之为一个const对象。
update(&csw); //错，不能将const的指针传给需要非const指针的函数
update(const_cast<SpecialWidget*>(&csw));//正确
Widget *pw = new SpecialWidget;
update(pw);//错误,pw是Widget*类型的。
```

第二个特殊化的转型操作符是`dynamic_cast`，用来执行继承体系中“安全的向下转型或跨系转型动作”，也旧式将指向基类对象的指针或引用转为指向派生类或兄弟类对象的指针或引用，同时还能得知是否转型成功：

```C++
Widget *pw;
...
update(dynamic_cast<SpecialWidget*>(pw));//正确，如果转型失败，则传入的是null
void updateViaRef(SpecialWidget& rsw);
updateViaRef(dynamic_cast<SpecialWidget&>(*pw));//如果失败抛出一个exception
```

最后一个转型操作符是`reinterpret_cast`，这个操作符的转型结果与编译平台息息相关，所以不具有移植性。它最常用的用途是转换“函数指针”类型，假设有一个存储函数指针的数组：

```C++
typedef void (*FuncPtr)();
FuncPtr funcPtrArray[10];
```

由于某种原因，希望将函数`int doSomething()`的一个指针放进数组中，如果没有转型，不可能办到这一点，但是现在可以这么做：

```C++
funcPtrArray[0] = reinterpret_cast<FuncPtr>(&doSomethig);
```
