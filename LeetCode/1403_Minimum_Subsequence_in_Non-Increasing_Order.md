# 1403. Minimum Subsequence in Non-Increasing Order

~~~c++
class Solution {
public:
    vector<int> minSubsequence(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int sum {0};
        for(auto& it : nums) sum += it;
        
        int index = nums.size() - 1;
        vector<int> res;
        int rest = sum;
        while(rest >= (sum+1)/2){
            res.push_back(nums[index]);
            rest -= nums[index];
            --index;
        }
        return res;
    }
};
~~~

原本写的 hash，发现没有上面的快：

~~~C++
class Solution {
public:
    vector<int> minSubsequence(vector<int>& nums) {
        int a[101] {0};
        int sum = 0;
        for(auto it : nums){
            a[it] += 1;
            sum += it;
        }
        int rest = sum;
        vector<int> res;
        int i = 100;
        while(i > 0){
            if(a[i]){
                res.push_back(i);
                --a[i];
                rest -= i;
                if(rest < (sum+1)/2) break;
            } else {
                --i;
            }
        }
        return res;
    }
};
~~~

