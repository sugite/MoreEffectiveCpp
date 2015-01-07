了解各种不同意义的new和delete
====

##new operator 和 operator new##

当我们写出`string *ps = new string("Memory Management");`时，所使用的new是所谓的new operator。这个操作符由语言内建，动作分为：

1. 分配足够的内存，用来放置某类型的对象。
2. 它调用一个constructor ，为刚才分配的内存中的那个对象设定初值。

你能够改变的是用来容纳对象的那块内存的分配行为。new operator调用某个函数执行必要的内存分配动作，这个函数就是operator new。函数operator new通常声明如下：

`void* operator new(size_t size);`

你可以将operator new重载，加上额外的参数，但是第一参数的类型必须是size_t。调用如下：

`void *rawMemory = operator new(sizeof(string));`

当你的编译器看到这样一个句子：

`string *ps = new string("MemoryManagement");`

它必须产生一些代码：

```C++
void *memory = operator new(sizeof(string));

call string::string("MemoryManagement") on *momery;

string *ps = static_cast<string*>(memory);
```

上述第二步涉及调用一个constructor ，程序员没有权利这么做。编译器可以。

##Placement new##

使用方法
在很多情况下，placement new的使用方法和其他普通的new有所不同。这里提供了它的使用步骤。

###第一步  缓存提前分配

为了保证通过placement new使用的缓存区的memory alignmen(内存队列)正确准备，使用普通的new来分配它：

`class Task ;`

`char * buff = new [sizeof(Task)]; //分配内存`

(请注意auto或者static内存并非都正确地为每一个对象类型排列，所以，你将不能以placement new使用它们。)

###第二步：对象的分配

在刚才已分配的缓存区调用placement new来构造一个对象。

`Task *ptask = new(buff) Task`

这里指定一个额外的自变量buff作为new operator隐式调用operator new时所用。

###第三步：使用

按照普通方式使用分配的对象：

`ptask->suspend();`

`ptask->resume();`


###第四步：对象的毁灭

**一旦你使用完这个对象，你必须调用它的析构函数来毁灭它。按照下面的方式调用析构函数：**

`ptask->~Task(); //调用外在的析构函数`

###第五步：释放

你可以反复利用缓存并给它分配一个新的对象（重复步骤2，3，4）如果你不打算再次使用这个缓存，你可以象这样释放它：

`delete [] buff;`

跳过任何步骤就可能导致运行时间的崩溃，内存泄露，以及其它的意想不到的情况。如果你确实需要使用placement new，请认真遵循以上的步骤。

##删除（deletion）与内存释放（Deallocation）

内存释放动作是由函数operator delete执行，通常声明如下：

`void operator delete(void *memory);`

因此动作`delete ps;`会造成编译器产生近似这样的代码：

```C++
ps->~string();
operator delete(ps);
```

如果你只打算处理原始的未设置初值的内存，应该完全回避new operator和delete operator，改调用operator new取得内存并以operator delete归还给系统：

```C++
void *buffer = operator new(50*sizeof(char));
operator delete(buffer);
```

很显然这就相当于在C++中调用malloc和free。

##数组


其实就是有一个operator new数组版的兄弟：operator new[]，但是operator new[]是相当晚的时候加入C++特性中的，所以你的编译器或许尚未支持它，如果是这样，全局operator new会被用来为每个数组分配内存，在这样的编译器下定制“数组内存分配行为”是很困难的，因为你得改写全局版的operator new 才行。