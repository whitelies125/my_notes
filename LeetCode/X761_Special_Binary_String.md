# Special Binary String

原本自己写了个 DFS，时间复杂度太高，s.size() = 20（最长 50）都超时了。

下面是看了题解写的，分治。

~~~C++
class Solution {
public:
    string makeLargestSpecial(string s) {
        vector<string> vec;
        int count = 0;
        int start = 0;
        for(int i = 0; i < s.size(); ++i){
            if(s[i] == '1') ++count;
            if(s[i] == '0') --count;
            if(count == 0){
                //cout << s.substr(start,i-start+1) << endl;
                vec.push_back("1" + makeLargestSpecial(s.substr(start+1,i-start)) + "0");
                start = i+1;
            }
        }
        sort(vec.begin(),vec.end(),greater<string>());
        // string res = "";
        // for(auto& it : vec) res += it;
        string res = accumulate(vec.begin(), vec.end(), ""s);
        return res;
    }
};
~~~

