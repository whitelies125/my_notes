# 75. Sort Colors

~~~C++
class Solution {
public:
 
    void sortColors(vector<int>& nums) {
        int head = 0;
        int tail = nums.size() - 1;
        int mid = 0;
        while(mid <= tail){
            switch(nums[mid]){
                case 0:{
                    swap(nums[head],nums[mid]);
                    head++;
                    mid++;
                    break;
                }
                case 1:{
                    mid++;
                    break;
                }
                case 2:{
                    swap(nums[mid],nums[tail]);
                    tail--;
                    break;
                }
            }
        }
    }
};
~~~

