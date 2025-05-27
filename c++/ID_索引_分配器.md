代码里时常需要进行不重复 ID 或索引的分配。
例如，新用户接入，为其分配一个 id。与其他组件/模块建立链接，需要一个 id 等。

## 思路

需要的功能大致如下：

```cpp
template <typename T>
class Index {
    // 只支持 unsigned int 类型
    static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value);
public:
    uint32_t Init(T size);  // 用于初始化 id 生成范围
    T AllocId();            // 申请分配一个唯一 id
    uint32_t FreeId(T id);  // 释放 id
};
```

数据结构考虑静态链表，且数组下标直接作为 id 与尾指针。
数组元素的值，分为 3 种：
1. 若为特殊值 a，则表示此 id 已分配；
2. 若为特数字 b，则表示为链表尾最后一个元素；
3. 其他值为有效值，标识下一个可分配 id，即逻辑上的下一个结点的数组下标。
数组长度自然是可分配 id 的最大值。

考虑到内存占用，所以使用模板，使用者根据所需 id 范围传入 u8，u16，u32，u64。

## 初步实现

细心实现头结点、尾结点的移动即可

```cpp
#include <cstdint>
#include <iostream>
#include <type_traits>

using namespace std;

template <typename T>
class Index {
    // 只支持 unsigned int 类型
    static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value);

public:
    enum NodeSatuc : T {
        NODE_USED = static_cast<T>(-1),
        NODE_TAIL = static_cast<T>(-2)
    };

    static constexpr T INVALID_ID = static_cast<T>(-1);

public:
    Index() : len_ {0}, node_(nullptr), head_(0), tail_(0) {}

    // 用于初始化 id 范围
    uint32_t Init(T size) {
        if (size == 0 || size >= INVALID_ID) {
            return INVALID_ID;
        }
        node_ = new T[size]; // 假设直接 new
        len_ = size;
        T i = 0;
        for (; i < len_ - 1; ++i) {
            node_[i] = i + 1;  // 初始时，每个结点后一个结点即是下标+1
        }
        node_[i] = NODE_TAIL;  // 标识最后一个结点
        head_ = 0;
        tail_ = len_ - 1;
        return 0;
    }
    // 申请分配一个 id
    T AllocId() {
        if (head_ == NODE_TAIL) {
            return INVALID_ID;
        }
        T id = head_;
        head_= node_[head_];
        node_[id] = NODE_USED;

        if (head_ == NODE_TAIL) {
            tail_ = NODE_TAIL;
        }
        return id;
    }
    // 释放 id
    uint32_t FreeId(T id) {
        if (id >= len_) {
            return INVALID_ID;
        }
        if (node_[id] != NODE_USED) {
            return INVALID_ID;
        }
        if (head_ == NODE_TAIL) {
            node_[id] = NODE_TAIL;
            head_ == id;
            tail_ = id;
        } else {
            node_[id] = NODE_TAIL;
            node_[tail_] = id;
            tail_ = id;
        }
        return 0;
    }
    // 测试时用于打印
    void Print() {
        for (T i = 0; i < len_; ++i) {
            if (node_[i] == NODE_USED) cout << "_,";
            else if (node_[i] == NODE_TAIL) cout << "T,";
            else cout << (uint32_t)node_[i] << ",";
        }
        cout << endl;
    }

private:
    T len_;
    T* node_;
    T head_;
    T tail_;
};
```

简单测试一下
```cpp
int main() {
    uint32_t size = 5;
    Index<uint8_t> index;
    index.Init(size);
    cout << "alloc test" << endl;
    for (uint32_t i = 0; i < size; ++i) {
        cout << "id : " << (uint32_t)index.AllocId() << ", status : ";
        index.Print();
    }
    cout << (uint32_t)index.AllocId() << ", status : ";
    index.Print();
    cout << "free test" << endl;
    for (uint8_t i = 0; i < size; ++i) {
        cout << "id : " << (uint32_t)index.FreeId(i) << ", status : ";
        index.Print();
    }
    cout << (uint32_t)index.FreeId(0) << ", status : ";
    index.Print();
    cout << (uint32_t)index.FreeId(5) << ", status : ";
    index.Print();
    return 0;
}

/* output:
alloc test
id : 0, status : _,2,3,4,T,
id : 1, status : _,_,3,4,T,
id : 2, status : _,_,_,4,T,
id : 3, status : _,_,_,_,T,
id : 4, status : _,_,_,_,_,
255, status : _,_,_,_,_,
free test
id : 0, status : T,_,_,_,_,
id : 0, status : 1,T,_,_,_,
id : 0, status : 1,2,T,_,_,
id : 0, status : 1,2,3,T,_,
id : 0, status : 1,2,3,4,T,
255, status : 1,2,3,4,T,
255, status : 1,2,3,4,T,
*/
```

## 静态内存

考虑到简单实现中，内存来源于外部堆或内存池，仍需要进行内存申请；且多数场景下，所需的 id 分配范围是已知的。
因此再提供一个栈内存的实现：

```cpp
template <typename T, size_t N>
class ArrIndex {
    static_assert(N > 0);
    static_assert(N < std::numeric_limits<T>::max() - 1);
public:
    uint32_t Init(); // 范围编译器已确定
    T allocId();
    uint32_t freeId(T id);

private:
    T node_[N]; // 内存来源
};
```

注意到 ArrIndex 与 Index 的逻辑相同，仅仅内存来源不同，因此可以提取公共逻辑为父类 IndexBase，子类提供内存来源即可。

由此，代码结构变为：
```cpp
template <typename T>
class IndexBase {
    // 只支持 unsigned int 类型
    static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value);

public:
    enum NodeSatuc : T { NODE_USED = static_cast<T>(-1), NODE_TAIL = static_cast<T>(-2) };
    static constexpr T INVALID_ID = static_cast<T>(-1);

public:
    IndexBase() : len_ {0}, node_(nullptr), head_(0), tail_(0) {}

    // 用于初始化 id 范围，从子类获取内存
    uint32_t Init(size_t size, T* node);
    // 申请分配一个 id
    T AllocId();
    // 释放 id
    uint32_t FreeId(T id);
};

// 内存从外部获取使用这个
template <typename T>
class Index : public IndexBase<T> {
    static constexpr T INVALID_ID = static_cast<T>(-1);
    
public:
    uint32_t Init(T size) {
        if (size == 0 || size >= INVALID_ID) {
            return INVALID_ID;
        }
        T* buff = new T[size];
        return IndexBase<T>::Init(size, buff);
    }
};

// 静态内存使用这个
template <typename T, T N>
class ArrIndex : public IndexBase<T> {
    static_assert(N > 0);
    static_assert(N < std::numeric_limits<T>::max() - 1);

public:
    ArrIndex() {
        IndexBase<T>::Init(N, &node_[0]);
    }

private:
    T node_[N];
};
```

差不多简单搞定 id/索引分配器。
id 分配器在很多地方都有用途，不限于业务逻辑，涉及到独占资源申请与释放的场合都可能使用。

2025.05.27