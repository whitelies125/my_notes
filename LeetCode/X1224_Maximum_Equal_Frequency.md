# 1224. Maximum Equal Frequency

看了题解才写出来的，自己的时间复杂度过高。

一开始用 map，实测用数组实现哈希更快：

~~~C++
int cnt[100001];// 数字 x 出现的次数
int m[100001];//出现次数为 k 次的数字的种类。
			  //例如 [1,1,2,2]，则 m[2] = 2，表示出现次数为 2 的数字种类为 2 种。
class Solution {
public:
    int maxEqualFreq(vector<int>& nums) {
		memset(m,0,sizeof(m));
        memset(cnt,0,sizeof(cnt));
        int res = 0;
        int max_ = 0;
        for(int i = 0; i < nums.size(); i++){
            if(cnt[nums[i]] > 0) m[cnt[nums[i]]] --;
            cnt[nums[i]] ++;
            max_ = max(max_, cnt[nums[i]]);
            m[cnt[nums[i]]] ++;
            
            // all = max_
            if(max_ == 1) res = i;
            // only one = max_, others = max-1
            if(max_ + (max_-1)*m[max_-1] == i+1 && m[max_] == 1)
                res = i;
            // only one = 1, others = max_
            if(max_*m[max_] + m[1] == i+1 && m[1] == 1)
                res = i;            
        }
        return res+1;
    }
};
~~~

