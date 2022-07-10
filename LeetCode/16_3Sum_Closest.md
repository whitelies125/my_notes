# 3Sum Closest

~~~C++
class Solution {
public:
    int threeSumClosest(vector<int>& nums, int target) {
        sort(nums.begin(),nums.end());
        int sum = nums[0] + nums[1] + nums[2];
        int diff = abs(target - sum);
        for(int i = 0; i < nums.size()-2; i++){
            int j = i+1;
            int k = nums.size()-1;
            while( j < k){
                int temp = nums[i] + nums[j] + nums[k];
                if(abs(target - temp) < diff){
                    sum = temp;
                    diff = abs(target - temp);
                }
                if( temp < target){
                    j++;
                }else{
                    k--;
                }
            }
        }
        return sum;
    }
};
~~~

