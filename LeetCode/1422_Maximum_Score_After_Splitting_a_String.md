# 1422. Maximum Score After Splitting a String

自己的：
时间复杂度：$O(n)$ ，
空间复杂度：$O(1)$ 。

~~~C++
class Solution {
public:
    int maxScore(string s) {        
        int cur_score = 0;
        int max_score = -1;
        int count_1 = 0;
        for(int i = 0; i < s.size()-1; ++i){
            if(s[i] == '0') ++ cur_score;
            if(s[i] == '1'){
                ++ count_1;
                -- cur_score;
            }
            max_score = max(max_score, cur_score);
        }
        if(s[s.size()-1] == '1') ++ count_1;
        return max_score + count_1;
    }
};
~~~

2022.08.14