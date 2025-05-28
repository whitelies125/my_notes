# algorithm

[TOC]

#### max_element / min_element

Finds the greatest/smallest element in the range `[first, last)`.

example:

~~~C++
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

int main(){
    std::vector<int> nums(10);
    
    std::iota(nums.begin(), nums.end(), 0);
    for(auto& it : nums) std::cout << it << " ";
    std::cout << std::endl;

    auto it = max_element(nums.begin(), nums.end());
    std::cout << *it << std::endl;

    it = max_element(nums.begin(), nums.end(),//也可使用谓词
        [](int a, int b){
            return a > b;
        });
    std::cout << *it;

    return 0;
}
//output:
//0 1 2 3 4 5 6 7 8 9 
//9
//0
~~~

#### minmax_element

Finds the smallest and greatest element in the range `[first, last)`.

example:

~~~C++
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

int main(){
    std::vector<int> nums(10);
    
    std::iota(nums.begin(), nums.end(), 0);
    for(auto& it : nums) std::cout << it << " ";
    std::cout << std::endl;

    // 返回的是 pair<最小值迭代器, 最大值迭代器>
    auto [min, max] = minmax_element(nums.begin(), nums.end());
    std::cout << "min : " << *min << std::endl;
    std::cout << "max : " << *max << std::endl;

    return 0;
}
//output:
//0 1 2 3 4 5 6 7 8 9 
//min : 0
//max : 9
~~~

#### lower_bound / upper_bound

#### lower_bound

returns an iterator to the first element **not less** than the given value

#### upper_bound

returns an iterator to the first element **greater** than the given value

~~~C++
#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;
using std::cout;

int main(){
    vector<int> vec {1,2,3,3,3,6,7};
    
    // 返回第一个大于等于的迭代器 index = 2, value = 3;
    cout << lower_bound(vec.begin(),vec.end(), 3) - vec.begin() << endl;
    // 返回第一个大于的迭代器 index = 5, value = 6;
    cout << upper_bound(vec.begin(),vec.end(), 3) - vec.begin() << endl;
    
    // 返回第一个大于等于的迭代器，index = 5 value = 6
    cout << lower_bound(vec.begin(),vec.end(), 5) - vec.begin() << endl;
    // 返回第一个大于的迭代器，index = 5 value = 6
    cout << upper_bound(vec.begin(),vec.end(), 5) - vec.begin() << endl;
    
    // 下限溢出，返回 0; index = 0,
    cout << lower_bound(vec.begin(),vec.end(), -1) - vec.begin() << endl;
    cout << upper_bound(vec.begin(),vec.end(), -1) - vec.begin() << endl;

    // 上限溢出，返回 vec.end(); index = 7
    cout << lower_bound(vec.begin(),vec.end(), 8) - vec.begin() << endl;
    cout << upper_bound(vec.begin(),vec.end(), 8) - vec.begin() << endl;
    return 0;
}
~~~

