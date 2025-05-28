环境为 windows mingw。
## 成员变量连续尾插法放置

```cpp
#include <iostream>
#include <cstdint>

using namespace std;

struct A {
    uint32_t a = 1;
};
struct B : A {
    uint32_t b = 2;
};
struct C : B {
    uint32_t c = 3;
};

int main()
{
    C c;
    A* pa = &c;

    uint32_t* ptr = (uint32_t*)&c;
    cout << *(ptr) << endl;        // 1
    cout << *(ptr + 1) << endl;    // 2
    cout << *(ptr + 2) << endl;    // 3
    return 0;
}
```

可见，实例 c 的内存布局是按继承链尾插布置。
本例中，假定实例 c 的首地址为 0x1000000000，内存布局则为：

0x1000000000，A::a = 1
0x1000000004，B::b = 2
0x1000000008，C::c = 3

## 同名隐藏

换成同名也是一样的，实则都是通过偏移访问：

```cpp
#include <iostream>
#include <cstdint>

using namespace std;

struct A {
    uint32_t a = 1;
};
struct B : A {
    uint32_t a = 2;
};
struct C : B {
    uint32_t a = 3;
};

int main()
{
    C c;
    A* pa = &c;
    B* pb = &c;
    C* pc = &c;

    uint32_t* ptr = (uint32_t*)&c;
    cout << *(ptr) << endl;      // 1
    cout << *(ptr + 1) << endl;  // 2
    cout << *(ptr + 2) << endl;  // 3
    cout << pa->a << endl;       // 1
    cout << pb->a << endl;       // 2
    cout << pc->a << endl;       // 3
    return 0;
}
```

## 多重继承：
多重继承也是类似的思路：

```cpp
#include <cstdint>
#include <iostream>

using namespace std;
struct A {
    uint32_t a = 1;
};
struct B {
    uint32_t b = 2;
};
struct C : A, B {
    uint32_t c = 3;
};

struct D : B, A {
    uint32_t c = 3;
};

int main()
{
    C c;

    uint32_t* ptr = (uint32_t*)&c;
    cout << *(ptr) << endl;      // 1
    cout << *(ptr + 1) << endl;  // 2
    cout << *(ptr + 2) << endl;  // 3

    D d;

    ptr = (uint32_t*)&d;
    cout << *(ptr) << endl;      // 2
    cout << *(ptr + 1) << endl;  // 1
    cout << *(ptr + 2) << endl;  // 3
    return 0;
}
```

显然是按继承顺序排列：

C ：
0xFFF0，A::a = 1
0xFFF4，B::b = 2
0xFFF8，C::c = 3

D ：
0xFFF0，B::b = 2
0xFFF4，A::a = 1
0xFFF8，C::c = 3

## 菱形继承

别搞。
见识有限，目前没见过菱形继承的场景。

2025.05.26