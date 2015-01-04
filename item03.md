绝对不要以多态方式处理数组
===

C++允许通过基类的指针或引用来操作派生类所形成的数组。

假设有个BST类，和一个平衡二叉搜索树类BalancedBST：

```C++
class BST{...};
class BalancedBST:public BST{...};
```

现考虑有个函数，用来打印BSTs数组中的每一个BST的内容：

```C++
void printBSTArray(ostream& s,const BST array[] , int numElements)
{
    for(int i = 0 ; i < numElements ; ++i)
    {
        s<<array[i];
    }
}
```

当你将一个由BST对象组成的数组传给此函数没有问题，然而如果你将一个BalancedBST对象组成的数组交给此函数，会发生什么：

```C++
BalancedBST bBSTArray[10];
printBSTArray(cout,bBSTArray,10);
```

这时编译器不会报错，但是真的没错吗，看看print函数中的`s<<array[i]`，我们知道，`array[i]`是一个指针算术表达式的简写：实际上是`*(array+i)`。那么`array`指向的内存和`array+i`所指向的内存到底是多远，没错，由于我们在函数定义中参数声明的是BST类型的，所以`array`和`array+i`之间的距离是`i*sizeof(BST)`,至少编译器是这么认为的。但实际上数组每一元素的大小是`sizeof(BalancedBST)`，so...你懂的

同样的，如果你调用`delete[] array`，虽然没有显式记性指针算术运算，但实际上编译器还是会依次析构每一个对象：`array[i].BST::~BST();`,这样同样错误。

**简而言之，多态和指针算术不能混用。数组对象几乎总是会涉及指针的算术运算，所以数组和多态不要混用。**