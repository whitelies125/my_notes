# Reformat The String

自己写的扫一遍：

~~~C++
class Solution {
public:
    string reformat(string s) {
        string res = "";
        int i = 0;
        int j = 0;
        int count_i = 0;
        int count_j = 0;
        while(i < s.size() && j < s.size()){
            int flag_i = 0;
            for( ; i < s.size(); ++i){
                if('0' <= s[i] && s[i] <= '9'){
                    res += s[i];
                    ++count_i;
                    flag_i = 1;
                    if(abs(count_i - count_j) > 1 ) return "";
                    ++i;
                    break;
                }
            }
            if(flag_i){
                for( ; j < s.size(); ++j){
                    if('a' <= s[j] && s[j] <= 'z'){
                        res += s[j];
                        ++count_j;
                        if(abs(count_i - count_j) > 1 ) return "";
                        ++j;
                        break;
                    }
                }
            }
        }
        if(i == s.size()) {
            for( ; j < s.size(); ++j){
                if('a' <= s[j] && s[j] <= 'z'){
                    res = s[j] + res;
                    ++count_j;
                    if(abs(count_i - count_j) > 1 ) return "";
                }
            }
        }
        if(j == s.size()) {
            for( ; i < s.size(); ++i){
                if('0' <= s[i] && s[i] <= '9'){
                    res = s[i] + res;
                    ++count_i;
                    if(abs(count_i - count_j) > 1 ) return "";
                }
            }
        }
        return res;
    }
};
~~~

部分参考了题解写的扫两遍，原地：

~~~C++
class Solution {
public:
    string reformat(string s) {
        int cnt_num = 0;
        int cnt_char = 0;
        for(auto& it : s){
            if(isdigit(it)) ++cnt_num;
            if(isalpha(it)) ++cnt_char;
        }
        if(abs(cnt_num - cnt_char) > 1) return "";
        bool flag = cnt_num > cnt_char;
        int i = 0;
        int j = 1;
        for( ; i < s.size(); i += 2) {
            if(isdigit(s[i]) != flag) { // 这个原地是参考的，妙啊
                while(isdigit(s[j]) != flag) {
                    j += 2;
                }
                cout << i << " " << j << endl;
                swap(s[i], s[j]);
            }
        }
        return s;
    }
};
~~~

