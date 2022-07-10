# Longest Common Prefix

~~~C++
class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        string res = "";
        int time = 0;
        for(int i = 0; i < strs[0].size(); i++){
            for(int j = 1; j < strs.size(); j++){
                if( strs[0][i] != strs[j][time]) return res;
            }
            res += strs[0][i];
            time ++;
        }
        return res;
    }
};
~~~

