#利用destructors避免泄漏资源#

假设为小动物编写一个软件，定义一个抽象基类ALA，再派生出针对小狗和小猫的具体类，其中有个虚函数processAdoption，负责因动物种类而异的必要处理动作。

```C++
class ALA{
public:
    virtual void processAdoption() = 0;
    ...
};

class Puppy:public ALA{
public:
    virtual void processAdoption();
    ...
};

class Kitten:public ALA{
public:
    virtual void processAdoption();
    ...
};
```

*其实后面的内容和Effective C++里的几个条款类似*

假如我有一个函数从文件读取数据产生一个object：

`ALA* readALA(istream& s);`

然后就是这样：

```C++
void processAdoptions(istream& dataSource)
{
    while(dataSource)
    {
        ALA *pa = readALA(dataSource);
        pa->processAdoption();
        delete pa;
    }
}
```

显然，如果`pa->processAdoption();`抛出异常，资源就无法正常释放。没错，这时候就是智能指针上场了：

```C++
void processAdoptions(istream& dataSource)
{
    while(dataSource)
    {
        auto_ptr<ALA> pa( readALA(dataSource) );
        pa->processAdoption();
    }
}
```

同样的，参加Effective C++条款13-16擅用资源管理类。