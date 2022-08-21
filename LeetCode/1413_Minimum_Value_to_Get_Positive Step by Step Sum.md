# 1413. Minimum Value to Get Positive Step by Step Sum

~~~C++
class Solution {
public:
    int minStartValue(vector<int>& nums) {
        int min = 0;
        int sum = 0;
        for(auto& it : nums){
            sum += it;
            min = min < sum ? min : sum;
        }
        return -min+1;
    }
};
~~~

