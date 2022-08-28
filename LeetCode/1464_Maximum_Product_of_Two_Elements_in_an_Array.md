# 1464. Maximum Product of Two Elements in an Array

~~~C++
class Solution {
public:
    int maxProduct(vector<int>& nums) {
        int max = 0;
        int max2 = 0;
        for(int i = 0; i < nums.size(); ++i){
            if(nums[i] > max){
                max2 = max;
                max = nums[i];
            }else if(nums[i] > max2){
                max2 = nums[i];
            }
        }
        return (max-1)*(max2-1);
    }
};
~~~

2022.08.25