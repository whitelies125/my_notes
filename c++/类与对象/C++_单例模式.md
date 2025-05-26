
一般而言，需要确保数据/资源唯一性，如果允许拷贝可能导致数据不一致引发问题时，又或者如无必要勿增实体，会使用到单例模式。

从我的日常经验看来，使用到单例模式的场景，多为配置系统（确保一个线程写入，其他线程只读）。
此外，为了保证项目的内存完全可控，也会借用单例模式，在组件/线程初始化时完成该组件所有内存的申请与初始化。即便这个内存可能只在部分流程代码中使用。
这样也避免了在业务流程中的执行初始化动作带来的额外延迟。（是真的有场景需要这种延迟优化的）。
## 单例模式

显而易见，ban 掉拷贝、移动操作：

```cpp
class Singleton {
public:
    ~Singleton() {
        if (arr_) delete[] arr_;
    };

    static Singleton& getInstance() {
        static Singleton instance; // C++11 起，线程安全
        return instance;
    }

    uint32_t Init()
    {
        constexpr size_t RSVD_LEN = 10;
        arr_ = new uint32_t[RSVD_LEN]; // 暂且假设是 new，实际可能是从内存池中获取
        len_ = RSVD_LEN;
        return 0;
    }
    
private:
    Singleton() = default; // 构造函数放在 private
    
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;

private:
    size_t len_;
    uint32_t* arr_;
};
```

关于函数内的 static 对象（local static object），可阅读 《effective C++》条款 4。

顺带吐槽，C++11 起 Meyers' Singleton 已经是线程安全的初始化方式了，怎么网上还在写什么线程不安全呢，没看明白。
## 使用场景

一般而言，我最熟的使用场景，在组件/线程初始化时，完成单实例的创建与初始化，逻辑大致如下：

```cpp
uint32_t InitGlobalObj()
{
    auto& ins = Singleton::getInstance();  // 创建实例
    uint32_t res = ins.Init(); // 完成初始化
    if (res) return res ;
    // auto& ins2 = Singleton::getInstance();
    // res = ins.Init(); // 完成初始化
    // if (res) return res ;
    //...
    return 0;
}

uint32_t ComponentsInit() // 组件/线程入口
    //...
    res = InitGlobalObj();
    if (res) return res;
    //...
    return 0;
}
```

当然可以定义宏减少重复代码：

```cpp
#define INIT_SINGLETON(x)             \
    {                                 \
        auto& ins = x::getInstance(); \
        uint32_t res = ins.Init();    \
        if (res) return res;          \
    }
    
uint32_t InitGlobalObj()
{
    INIT_SINGLETON_OBJ(MemPool) // 初始化内存池，优先进行
    INIT_SINGLETON_OBJ(UserRes)
    INIT_SINGLETON_OBJ(ResRecord)
    INIT_SINGLETON_OBJ(AlarmEvtRecord)
    INIT_SINGLETON_OBJ(Singleton)
    //...
    return 0;
}
uint32_t ComponentsInit()
{
    //...
    uint32_t res = InitGlobalObj();
    if (res) return res;
    //...
    return 0;
}

int main()
{
    uint32_t res = ComponentsInit();
    return res;
}
```

注意到对内存池的初始化优先进行，因为后续类实例可能是从内存池申请的内存。
（真踩过坑，当然肯定不是我写的代码）

## 单实例管理类

考虑到一些操作可能不方便在析构函数中执行，例如一些可能导致抛出异常的操作。
因此在单例类中再增添一个析构前处理函数：

```cpp
    void BeforeDtor() {
        //...
    }
```

同时线程退出，并不意味着程序退出。
部分场景需要线程退出时，也需要对各种全局类进行析构。

因此引入一个单实例管理类 Mng，用于管理所有单实例
逻辑变为，
T0：单例类注册到 Mng 类，
T1：Mng 类初始化，构造所有注册的单例类，并保存实例地址
T2：随后调用每个单实例的 Init 完成初始化。

如果收到线程退出的指令，则 Mng 类遍历所有注册的单实例，调用其 BeforeDtor，随后调用其析构函数。

写起来比较麻烦，大概逻辑是单例类都继承一个父类 Helper，方便 Mng 类进行管理操作。

同时又因为都是单例类，存在重复代码，因此可以再定义一个宏，把类的重复代码收编一下。

又因为访问单实例，是一个比较常见、频繁的操作，目前 Mng 已经有了所有注册的单实例数组了，因此可以定义一个函数/宏，从这个数组中访问，增加 cache 友好性。

所以还得给每个单例分一个 id 编号，这样通过下标直接访问 Mng 的单实例数组。当然加入的 id 标号也会为前面定义的函数/宏的一部分。

如果有些操作也是初始化操作，但是依赖于某几个单例类实例化且初始化后才能进行，那么还得引入一个 "单例类实例化且初始化后才进行的操作" 的注册、管理、调用类。

这么一层一层叠加，代码的混淆性就上来了 (つд⊂)
当然这其实也还好啦。

## 后话

最近新增了几个组件/线程，借鉴着老组件的代码写了一遍上面提到的东西。

真是被同事坑麻了。
什么漏了注册，业务跑起来，获取到的数据是空，导致挂了。
注册了，但是顺序没对，自己需要用到内存池，但是内存池在它后面。
注册了，但是没写初始化函数。
初始化了，但是内存池用成隔壁线程的了......

叠甲：
我并不比同事厉害，毕竟工作并不只是代码，同事的能力与实力都很强。
我只是闲着没事儿会翻一翻代码，无非在这方面的时间花费更多罢。
框架早早写完，偶尔调用接口也是抄已有的范式，当然选择又稳又快的方法，然后继续工作了。

也是挺不容易才碰上个新增组件的需求。
虽然大部分也是从已有组件抄的，但老代码总是多出来一堆堆不知道什么时候加的特殊处理了，精简一下提取出简洁的逻辑，那怎么能说是纯抄呢，是吧。

2025.0525


