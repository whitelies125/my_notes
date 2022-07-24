# Search Insert Position

~~~c++
class Solution {
public:
    int searchInsert(vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size();
        int mid;
        while(left < right){
            mid = left+(right-left)/2;//避免溢出
            if(nums[mid] < target) left = mid+1;//左闭
            else right = mid;//右开
        }
        return left;//返回 right 也可，最后是一样的。
    }
};
~~~

注意这个二分查找。

采用的是左闭右开 [left,right) 区间，除了最后退出循环时，始终是左闭右开区间。
在退出循环时，left，right 相等。

这个写法，同时适用于区间为空、答案不存在、有重复元素、搜索开/闭的上/下界等情况
