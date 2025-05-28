# numeric

[TOC]

#### iota

Fills the range `[first, last)` with sequentially increasing values, starting with `value` and repetitively evaluating ++value.

example：

~~~C++
#include <iostream>
#include <vector>
#include <numeric>

int main(){
    std::vector<int> nums(10);
    
    std::iota(nums.begin(), nums.end(), 0);
    for(auto& it : nums) std::cout << it << " ";
    std::cout << std::endl;

    std::iota(nums.begin(), nums.end(), 11);
    for(auto& it : nums) std::cout << it << " ";
    
    return 0;
}
//output:
//0 1 2 3 4 5 6 7 8 9 
//11 12 13 14 15 16 17 18 19 20
~~~

#### accumulate

Computes the sum of the given value `init` and the elements in the range `[first, last)`.

example:

~~~C++
#include <iostream>
#include <vector>
#include <numeric>

int main(){
    std::vector<int> nums(10);
    std::iota(nums.begin(), nums.end(), 0);
    for(auto& it : nums) std::cout << it << " ";
    std::cout << std::endl;

    // init = init + ele
    std::cout << std::accumulate(nums.begin(), nums.end(), 0) << std::endl;
    // init = lambda(init, ele);
    std::cout << std::accumulate(nums.begin(), nums.end(), 0,
                                [](int a, int b) {
                                    return (b > 5) ? (a + b) : a;
                                });
    return 0;
}
//output:
//0 1 2 3 4 5 6 7 8 9 
//45
//30
~~~

#### partial_sum

Computes the partial sums of the elements in the subranges of the range `[first, last)` and writes them to the range beginning at `d_first`.

~~~C++
#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

int main(){
    vector<int> nums(10);
    fill(nums.begin(), nums.end(), 1);
    for(auto& it : nums) cout << it << " ";
    cout << endl;

    vector<int> partial(nums.size());
    partial_sum(nums.begin(), nums.end(), partial.begin());
    for(auto& it : partial) cout << it << " ";
    cout << endl;
    
    // 可以原地排序
    partial_sum(nums.begin(), nums.end(), nums.begin());
    for(auto& it : nums) cout << it << " ";
    cout << endl;
    return 0;
}
//output:
//1 1 1 1 1 1 1 1 1 1 
//1 2 3 4 5 6 7 8 9 10
//1 2 3 4 5 6 7 8 9 10
~~~

#### 
