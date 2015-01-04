非必要不提供default constructor
===

考虑下面这个针对公司仪器而设计的class，在其中，仪器识别码是一定的有的一个constructor自变量：

```c++
class EquipmentPiece{
public:
    EquipmentPiece(int IDNumber);
    ...
};
```

由于此类缺乏default constructor，其运行可能在三种情况下出现问题。

第一个情况是在产生数组的时候。一般而言没有任何方法可以为数组中的对象指定constructor自变量，所以几乎不可能产生一个有EquipmentPiece objects 构成的数组：

```C++
EquipmentPiece bestPieces[10]; //错误，无法调用EquipmentPiece ctors
EquipmentPiece *bestPieces = new EquipmentPiece[10];//错误，另有一些问题
```

有三个方法可以侧面解决这个问题，第一个是使用non-heap数组：

```C++
int ID1,ID2...ID10;
EquipmentPiece bestPieces[] = {
    EquipmentPiece(ID1),
    ...
    EquipmentPiece(ID10)
};
```

但是这种方法无法延伸至heap数组。所以更一般的做法是使用指针数组而非对象数组：

```C++
typedef EquipmentPiece* PEP;
PEP bestPieces[10];
for(int i = 0 ; i < 10 ; ++i)
    bestPieces[i] = new EquipmentPiece(ID number);
```

此方法的缺点：容易忘记释放内存导致资源泄漏，并且需要的内存总量比较大，因为需要存指针和对象。“过度使用内存”这个问题可以避免，方法是先为此数组分配raw memory，然后使用“placement new”在这块内存上构造EquipmentPiece objects：

```C++
//分配足够的raw memory
void *rawMemory = operator new[](10*sizeof(EquipmentPiece));

//让bestPieces指向此块内存，是这块内存被视为一个EquipmentPiece数组
EquipmentPiece *bestPieces = static_cast<EquipmentPiece*>(rawMemory);

//利用“placement new”构造这块内存中的EquipmentPiece objeces
for(int i = 0 ; i < 10 ; ++i)
    new (&bestPieces[i]) EquipmentPiece(ID number);
```

这种方法的缺点是，大部分程序员不熟悉，维护困难，而且还得在对象结束生命时以手动方式调用其dtors，最后还得以调用operator delete[]的方式释放raw memory：

```C++
for(int i = 9 ; i>=0 ; --i)
    bestPieces[i].~EquipmentPiece();
    
operator delete[](rawMemory);
```

而直接`delete[] bestPieces;`属于未定义行为。


----------

第二个情况是：它们将不适用于许多template-based container classes。其实这点和上述问题类似，因为许多template内几乎总是会产生一个以template类型参数作为类型而架构起来的数组：

```C++
template<class T>
class Array{
public:
    Array(int size);
private:
    T *data;
};

template<class T>
Array<T>::Array(int size)
{
    date = new T[size]; //数组中的每个元素都调用T::T()。
}
```

那么，到底要还是不要提供一个default constructor 呢？其实还是得看情况。添加无意义的default constructor ，也会影响classes的效率，如果member functions必须测试字段是否真被初始化了，其调用者便必须为测试行为付出时间代价，并为测试代码付出空间代价，因为可执行文件和程序库都变大了。如果default constructor 可以确保对象的所有字段被正确初始化，上述所有成本便可免除，如果无法提供这种保证，那么对好避免让default constructor 出现。

