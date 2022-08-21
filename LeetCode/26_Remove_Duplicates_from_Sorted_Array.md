# 26. Remove Duplicates from Sorted Array

自己做的：

~~~C++
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        if(nums.size() == 0) return 0;
        int i = 0;
        int count = 0;
        for(int j = 1; j < nums.size(); j ++){
            if(nums[i] == nums[j]){
                count ++;
            }else{
                i++;
                nums[i] = nums[j];
            }
        }
        return nums.size()-count;
    }
};
~~~

参考别人的后：

~~~C++
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        if(nums.size() == 0) return 0;
        int left = 0;
        for(int right =1; right< nums.size(); right++){
           if(nums[left] != nums[right])
               left++;
               nums[left] = nums[right];
           }
    	return left+1;
    }
};
~~~

