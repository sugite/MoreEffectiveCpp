区别increment/decrement操作符的前置和后置形式
====

先看一个例子：

```C++
class UPInt{
public:
    UPInt& operator++();//前置式++
    const UPInt operator++(int);//参数类型用以区分上面的前置式
    
    UPInt& operator--();
    const UPInt operator--(int);
    
    UPInt& operator+=(int);
    ...
};

UPInt i;
++i;
i++;

--i;
i--;
```

下面讨论为什么前置式返回引用，而后置式返回const对象。

先看具体实现：

```C++
UPInt& UPInt::operator++()
{
    *this += 1 ;
    return *this;
}

const UPInt UPInt::operator++(int)
{
    UPInt old = *this ;
    *this += 1;
    return old;
}
```

很明显，后置式是返回一个临时对象的，所以当然不能返回引用，因为函数结束了临时对象是要销毁的，还引用啥？那么，为什么是const的呢，我们先暂时假设不是const的，那么"i++++;"这种操作将是合法的，但是这不正确，因为"i++++"中，第二个++改变的对象是第一个++返回的对象，而不是原对象，所以即使"++"了两次，i还是被累加了一次而已，这违反直觉，也容易引起混淆，所以最好的办法就是禁止它合法化。

注：++++i是合法的，可以自己分析。
