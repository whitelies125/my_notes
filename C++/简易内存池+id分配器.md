基础概念省去。

一般而言，对于内存池，通常应该提供以下功能：

```cpp
class MemPool {
public:
    uint32_t Init(); // 初始化内存池：内存块数量、大小
    void* Alloc(size_t size); // 返回大于等于 size 的内存首地址或 nullptr
    void Free(void* addr); // 释放内存
};
```

## 思路
我们可以先实现一个最简单的版本，也就是仅有 num 个相同 size 大小内存块的内存池。
随后，若要支持多组不同 num、size 的内存池，实则就是这个最简版本的数组。

最简版本的数据结构为，申请 num * size 大小的内存，分成 num 块即可。
由于此前我实现了 id分配器，正好派上用场，使用 id分配器来管理内存块。

实际上本文同时也是 id分配器的使用示例。
## 最简单版本

### Init
首先完成初始化 Init

```cpp
class block {
public:
    ~Block() {
        delete[] buff_;
    }
    uint32_t Init(size_t num, size_t size) {
        size_t total = num * size;
        buff_ = new uint8_t[total];
        size_ = size;
        return index.Init(num); // 初始化 id分配器，有效范围为 [0, size) 左闭右开
    }
    void* Alloc(size_t size);
    void Free(void* addr);

private:
    size_t size_;
    void* buff_;
    Index<uint32_t> index; // 使用 id分配器 控制内存块
};

```

### Alloc
这里我使用到了此前实现的 id分配器，由 id分配器来管理内存块。
省去了每次遇到管理独占资源类都需要手动重复写已使用、未使用、已耗尽的逻辑。

```cpp
class block {
public:
    ~Block() {
        delete[] buff_;
    }
    uint32_t Init(uint32_t num, uint32_t size);
    void* Alloc(uint32_t size) {
        if (size > size_) return nullptr;
        uint32_t id = index.AllocId(); // 申请一个未使用的 id
        if (id == UINT32_MAX) return nullptr;
        uint8_t offset = id * size_;   // 偏移 = id * 内存块大小
        return buff_ + offset;         // 返回第 id 个内存块的首地址
    }
    void Free(void* addr);

private:
    uint32_t size_;
    uint8_t* buff_;
    Index<uint32_t> index;  // 使用 id分配器控制内存块
};
```

### Free

Free 的逻辑更为简单。
当然还是建立在使用了 id分配器的情况下。

```cpp
class block {
public:
    ~Block() {
        delete[] buff_;
    }
    uint32_t Init(uint32_t num, uint32_t size);
    void* Alloc(uint32_t size);
    void Free(void* addr) {
        uint32_t id = ((uint8_t*)addr - buff_) / size_; // id = (addr - 内存池首地址) / 内存块大小
        index.FreeId(id); // id分配器释放 id 为可用
    }

private:
    uint32_t size_;
    uint8_t* buff_;
    Index<uint32_t> index;  // 使用 id分配器控制内存块
};
```

### 测试

简单的测试一下：

```cpp
int main()
    Block block;
    block.Init(3, 1024);
    cout << "alloc 1: " << (void*)block.Alloc(0) << endl;     // 申请成功
    cout << "alloc 2: " << (void*)block.Alloc(512) << endl;   // 申请成功
    cout << "alloc 3: " << (void*)block.Alloc(1025) << endl;  // 超出内存块大小，申请失败
    void* ptr = nullptr;
    cout << "alloc 4: " << (ptr = (void*)block.Alloc(1024)) << endl;  // 申请成功
    cout << "alloc 5: " << (void*)block.Alloc(1024) << endl;          // 内存池耗尽，申请失败
    block.Free(ptr);                                                  // 归还 1 个内存块
    cout << "alloc 6: " << (void*)block.Alloc(1024) << endl;          // 申请成功

}
/* output: 1024字节 = 0x400
alloc 1: 0x288c977e400
alloc 2: 0x288c977e800
alloc 3: 0
alloc 4: 0x288c977ec00
alloc 5: 0
alloc 6: 0x288c977ec00
*/
```

OK，简单搞定。

## 多 num\*size 组合 版本

支持多个 num * size 组合的版本，其实就是将多个最简版封装数组，再加上一点简单的遍历逻辑。
### 实现

```cpp
struct BlockInfo {
    uint32_t num;
    uint32_t size;
};

class MemPool {
public:
    ~MemPool() {
        delete[] blocks_;
    }
    // 传入 BlockInfo 数组，用于初始化内存池：内存块数量、大小
    uint32_t Init(BlockInfo* para, uint32_t paraLen)
    {
        blocks_ = new Block[paraLen];
        for (uint32_t i = 0; i < paraLen; ++i) {
            blocks_[i].Init(para[i].num, para[i].size);
        }
        len_ = paraLen;
        return 0;
    }
    void* Alloc(size_t size)
    {
        for (uint32_t i = 0; i < len_; ++i) {
            // 最佳适应算法，从小到大找到第一个合适大小的内存块
            if (void* ptr = blocks_[i].Alloc(size); ptr != nullptr) {
                return ptr;
            }
        }
        return nullptr;
    }
    void Free(void* addr)
    {
        for (uint32_t i = 0; i < len_; ++i) {
	        // block 中会判别是否属于自己内存块范围，然后再释放
            if (blocks_[i].Free(addr) == 0) {
                return;
            }
        }
        return;
    }

private:
    uint32_t len_;
    Block* blocks_; // 是多个单 num*size 组合的数组
};
```

### 测试

```cpp
int main()
{
    const BlockInfo para[] = { // 务必按内存占用从小到大排列
        {1, 1024},
        {1, 2048},
    };
    MemPool memPool;
    memPool.Init(para, sizeof(para) / sizeof(para[0]));

    cout << "alloc 1: " << (void*)memPool.Alloc(2049) << endl;  // 申请失败，超出最大内存块大小
    void* ptr = nullptr;
    cout << "alloc 2: " << (ptr = (void*)memPool.Alloc(0)) << endl;  // 申请成功，使用 1K 大小内存块
    cout << "alloc 3: " << (void*)memPool.Alloc(512) << endl;        // 申请成功，使用 2K 大小内存块
    cout << "alloc 4: " << (void*)memPool.Alloc(1024) << endl;       // 申请失败，内存池耗尽
    memPool.Free(ptr);                                               // 归还 1K 大小内存块
    cout << "alloc 5: " << (void*)memPool.Alloc(1025) << endl;       // 申请失败，1025B 超出可用最大内存块大小
    cout << "alloc 6: " << (void*)memPool.Alloc(1024) << endl;       // 申请成功，使用 1K 大小内存块
}
/* output:
alloc 1: 0
alloc 2: 0x2049b80fb00
alloc 3: 0x2049b80ff10
alloc 4: 0
alloc 5: 0x2049b80ff10
alloc 6: 0x2049b80fb00
*/
```

2025.05.28