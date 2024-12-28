#include <cstring>
#include <iostream>
#include <map>

class TextBlock {
private:
    std::string text;
public:
    TextBlock(const std::string& rhs) : text(rhs){}
    const char& operator[] (std::size_t position) const { // const 对象调用
        return text[position];
    }

    char operator[] (std::size_t position) { // 非 const 对象调用
        return text[position];
    }
};


class Cls {
private:
    int* const ptr;
public:
    Cls(int* n) : ptr(n) {}
    // 修改的内存并不是 cls对象成员, 因此该成员函数符合 C++ 对 constness 的定义（bitwise constness)
    // 则使用 const 修饰该成员函数无误，编译器不会报错
    void func(int n) const {
        *ptr = n;
    }
};

class CTextBlock {
private:
    char *pText;
public:
    CTextBlock(char* ptr) : pText(ptr) {}
    // 返回所指内存的引用，仍符合 C++ 对 constness 的定义（bitwise constness)
    // 则使用 const 修饰该成员函数无误，编译器不会报错
    char& operator[] (std::size_t position) const {
        return pText[position];
    }
};

class BitwiseCMapFind {
private:
    std::map<int, int> m;
public:
    const std::map<int, int>::const_iterator get(const int key) const {
        return m.find(key); // 每次调用都需要搜索
    }
};

class LogicalCMapCacheFind {
private:
    mutable std::map<int, int>::const_iterator cache; // 缓存上一次的查询结果
    std::map<int, int> m;
public:
    const std::map<int, int>::const_iterator get(const int key) const {
        // cache 命中时，直接返回结果，省去在 map 中搜索的性能开销
        if (cache->first != key) {
            const auto itor = m.find(key);
            if (itor != m.end()) {
                cache = itor; // 若不以 mutable 修饰，则编译器会报错，因为 const 成员函数不能对处理对象的成员进行修改
            }
        }
        return cache;
    }
};

int main() {
    TextBlock tb ("Hello");
    std::cout << tb[0] << std::endl; // 调用无 const 修饰的 operator[]
    const TextBlock ctb ("World");
    std::cout << ctb[0] << std::endl; // 调用 const 修饰的 operator[]

    // bitwise constness
    int n = 0;
    const Cls cls (&n);
    cls.func(1); // 调用 const 修饰的 func 函数，无报错

    std::string str {"Rest"};
    const CTextBlock cctb (str.data());
    char* pc = &cctb[0];
    char& ch = cctb[0];
    *pc = 'S';
    ch = 'T';
    std::cout << str << std::endl;
    // 虽然如今的编译器会提示:
    // Using the result of an assignment as a condition without parentheses (fixes available) [-Wparentheses]
    // 但只是对赋值语句结果用作判断条件的提示，而赋值操作本身仍合法的
    if (cctb[0] = 'A') {
        std::cout << str << std::endl;
    }

    return 0;
}
