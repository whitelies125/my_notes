# numeric

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

