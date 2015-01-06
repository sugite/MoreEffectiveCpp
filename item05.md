对定制的“类型转换函数”保持警觉
===

C++允许编译器在不同类型之间执行隐式转换，我们对内置类型的转换无能为力，但是当我们自己的类型登场时，便有了更多的控制力。两种函数允许编译器执行这样的转换：单自变量constructor 和隐式类型转换操作符。如此的constructor 可能声明拥有单一参数，也可能声明拥有多个参数，并且除了第一参数之外都有默认值：

```C++
class Name{
public:
    Name(const string& s); //可以把string转换为Name。
    ...
};

class Rational{
public:
    Rational(int numerator = 0 ,//可以把int转换为Rational
            int denominator = 1 );
    ...
};
```

而所谓的隐式类型转换操作符，是一个拥有奇怪名称的member function：关键词operator之后加上一个类型的名称。你不能为此函数指定返回值类型：

```C++
class Rational{
public:
    ...
    operator double() const ; //将Rational转换为double
};
//调用如下：
Rational r(1,2);
double d = 0.5*r;
```

重点来了：**为什么最好不要提供任何类型转换函数**

根本问题在于，在你从未打算也未预期的情况下，此类函数可能会被调用，而其结果可能是不正确、不直观的程序行为，很难调试。

让我们现处理隐式类型转换操作符，假设有一个class用来表现分数，你希望像内置类型一样地输出Rational objects内容，也就是希望这么做：

```C++
Rational r(1,2);
cout<<r; //应该输出“1/2”
```

更进一步假设你忘了为Rational写一个operator<<，那么你或许会认为上述打印动作不会成功，因为没有适当的operator<<可以调用，实际上不是的！因为编译器会将r隐式转换为double，就可以输出了！这就显示了隐式类型转换操作符的缺点：它们的出现可能导致错误的函数被调用。

解决办法就是换个函数，功能一样：

```C++
class Rational{
public:
    ...
    double asDouble() const ;
};
```

这就是为什么string object不能隐式转为char*的原因，而是要用c_str来执行上述转换行为。

最新的C++特性中有一个关键词explicit。这个特性之所以被导入，就是为了解决隐式类型转换带来的问题。只要将constructor 声明为explicit，编译器便不能因隐式类型转换而调用它们。不过显式类型转换仍是允许的。

不过，如果你的编译器不支持explicit，就只有用以下方法了。之前提到过一条规则：**没有任何一个转换程序可以内含一个以上的用户定制转换行为**。为了适当架构起你的classes，可以利用这项规则，让你希望拥有的“对象构造行为”合法化，并让你不希望允许的隐式构造非法化。

再次考虑Array template，你需要一种方法，不但允许以一个整数作为constructor 自变量来指定数组大小，又能阻止一个整数被隐式转换为一个临时Array对象。那么就产生一个新的class，名为ArraySize：

```C++
tempalte<class T>
class Array{
public:
    /*******************ArraySize class****************/
    class ArraySize{
    public：
        ArraySize(int numElemments):theSize(numElements){}
        int size() const {return theSize ;}
    private:
        int theSize ;
    };
    /**************************************************/
    
    Array(int lowBound , int highBound);
    Array(ArraySize size);
    ...
};
```

这个时候，如果再写`if(a==b[i])`,就会报错，因为编译器不能考虑将int转换为一个临时性的ArraySize对象，然后再根据这个临时对象产生必要的`Array<int>`对象，因为那将调用两个用户定制行为，这就和我们上述的原则相悖。

像ArraySize这种类被称为proxy classes，这是一项值得学习的技术。

综上，允许编译器执行隐式类型转换，将弊大于利。所以不要提供转换函数，除非你确定你需要它们。