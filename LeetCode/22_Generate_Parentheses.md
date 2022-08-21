# 22. Generate Parentheses

~~~C++
class Solution {
public:
    void func(vector<string>& vec,int left, int right, string str){
        if(left > right) return;
        if(left) func(vec, left-1, right, str+"(");
        if(right) func(vec, left, right-1, str+")");
        if(!left && !right) vec.push_back(str);
    }
    
    vector<string> generateParenthesis(int n) {
        vector<string> vec;
        int left = n;
        int right = n;
        string str = "";
        func(vec, left, right, str);
        return vec;
    }
};
~~~

