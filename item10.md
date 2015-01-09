#在constructors内阻止资源泄漏#

假设要设计一个电话薄软件，要有联系人姓名地址相片语音等：

```C++
class Image{
public:
    Image(const string& imageDataFileName);
    ...
};

class AudioClip{
public:
    AudioClip(const string& audioDataFileName);
    ...
};

class PhoneNumber{...};

class BookEntry{
public:
    BookEntry(  const string& name,
                const string& address = "" ,
                const string& imageFileName = "" ,
                const string& audioClipFileName = "" );
    ~BookEntry();
    
    //电话号码通过此函数加入
    void addPhoneNumber(const PhoneNumber& number);
    ...
    
private:
    string theName ;
    string theAddress;
    list<PhoneNumber> thePhone;
    Image *theImage;
    AudioClip *theAudioClip;
};


BookEntry::BookEntry(const string& name,
                    const string& address  ,
                    const string& imageFileName  ,
                    const string& audioClipFileName )
                    :theName(name),theAddress(address),
                    theImage(0),theAudioClip(0)
{
    if(imageFileName != "")
        theImage = new Image(imageFileName);
    if(audioClipFileName != "")
        theAudioClip = new AudioClip(audioClipFileName);
}

BookEntry::~BookEntry()
{
    delete theImage;
    delete theAudioClip;  //在C++中delete空指针是合法的
}


```

问题来了（遇到问题第一反应就应该是哪里会抛异常）:

```C++
    if(audioClipFileName != "")
        theAudioClip = new AudioClip(audioClipFileName);
```

上面这段代码，抛出exception怎么办，这时候theImage所指的对象该由谁来删除，放心，程序在这里抛异常了肯定会终止，不会执行到析构部分的，因为：**C++只会析构已构造完成的对象，对象只有在其constructor 执行完毕才算是完全构造妥当。这里的BookEntry还没构造完成就抛异常了，所以其析构函数不会执行，所以theImage不会被delete。**

C++为什么要这么干？

**如果析构函数被调用于一个尚未完全构造好的对象身上，这个析构函数如何知道该做些什么呢，唯一的办法就是记住constructor 执行到哪一步了，然后析构函数就可以检查这些数据，但是如此繁重的记忆工作必然会降低constructor 的速度。**

所以我们或许能这样解决上述问题：将异常捕获在构造函数内：

```C++
BookEntry::BookEntry(const string& name,
                    const string& address  ,
                    const string& imageFileName  ,
                    const string& audioClipFileName )
                    :theName(name),theAddress(address),
                    theImage(0),theAudioClip(0)
{
    try{
        if(imageFileName != "")
            theImage = new Image(imageFileName);
        if(audioClipFileName != "")
            theAudioClip = new AudioClip(audioClipFileName);
    }catch(...){
        delete theImage ;
        delete theAudioClip;
        throw;
    }
}
```

这样的确可以，但是，如果我们让theImage和theAudioClip都变成常量指针：

```C++
class BookEntry{
public:
    ...
private:
    ...
    Image * const theImage;
    AudioClip * const theAudioClip;
};
```

这样的指针是必须通过构造函数的初值链表加以初始化的，因为const只能赋值一次嘛，不能现赋值为null，然后再去new了，这时候该怎么搞呢，我们现在已经不能把代码写在{}内了。

没关系啊，可以把语句一起放到一个私有函数里，让theImage和theAudioClip在其中获得初值：

```C++
class BookEntry{
public:
    ...
private:
    ...
    Image* initImage(const string& imageFileName);
    AudioClip* initAudio(const string& audioFileName);
};

BookEntry::BookEntry(const string& name,
                    const string& address  ,
                    const string& imageFileName  ,
                    const string& audioClipFileName )
                    :theName(name),theAddress(address),
                    theImage(initImage(imageFileName)),
                    theAudioClip(initAudio(audioFileName))
{
    
}

Image* BookEntry::initImage(const string& fileName)
{
    return fileName == "" ? 0 : new Image(fileName);
}

AudioClip* BookEntry::initAudio(const string& fileName)
{
    try{
        return fileName == "" ? 0 : new AuidoClip(filename);
    }catch(...){
        delete theImage ;
        throw;
    }
}


```

看似完美了。。。

但是你忘了吗，前面提到的，用对象来管理资源！！！！！！！！！！！！！

没错，就是auto_ptr！！！！！！

将theImage和theAudioClip改为auto_ptr不就完了吗？？？？

```C++
class BookEntry{
public:
    ...
private:
    ...
    const auto_ptr<Image> theImage;
    const auto_ptr<AudioClip> theAudioClip;
};

```


记住：

**用auto_ptr对象取代pointer class members！！！！！！**

