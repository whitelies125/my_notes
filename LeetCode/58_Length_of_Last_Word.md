# 58. Length of Last Word

~~~C++
class Solution {
public:
    int lengthOfLastWord(string s) {
        int count = 0;
        int i = s.size()-1;
        while(s[i] == ' ') i --;
        while(i >= 0 && s[i] != ' '){
            count ++;
            i --;
        }
        return count;
    }
};
~~~