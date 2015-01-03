仔细区别pointers和references
========

明确一点：没有所谓的null reference，一个reference必须总代表某个对象。

以下代码是不可预期的，C++没有对此定义：

```C++
char *pc = 0 ;
char& rc = *pc ;
```

##初始化##

reference永远都不会是null。

所以C++要求reference必须得有初值：

```C++
string& rs ; //错误，没有初始化
string s("xyz");
string& rs = s ; //正确，rs指向s
```

##效率##

没有所谓的null reference这个事实意味着使用reference可能会比使用pointer更高效，因为使用reference之前不必检查其有效性：

```C++
void printDouble(const double& rd)
{
    cout<<rd;
}

void printDouble(const double* pd)
{
    if(pd) cout<<*pd ;
}
```

##可修改性##

pointer是可以被重新赋值的，但是reference确总是指向它最初获得的那个对象：

```C++
string s1("Nancy");
string s2("Clancy");

string& rs = s1 ;//rs代表s1
string *ps = &s1 ;//ps指向s1
rs = s2; //rs仍然代表s1,但是s1的值现在变成了Clancy
ps = &s2 ; //ps现在指向s2
```

##选择##

一般而言，当你需要考虑“不指向任何对象”的可能性时，或是考虑“在不同时间指向不同对象”的能力时，应该采用pointer。而当你确定“总是会代表某个对象”，而且“一旦代表了该对象就不能够再改变”，那么就用reference。

其他使用reference的情况：operator[]

```C++
vector<int> v(10);
v[5] = 10;
```

如果`operator[]`返回pointer，上述语句就应该是`*v[5] = 10`，但这使v看起来好像是个以指针形成的vector，事实上不是，所以这里应该用reference。