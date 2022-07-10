# Valid Parentheses

~~~C++
class Solution {
public:
    bool isValid(string s) {
        stack<char> sta;
        for(char ch : s){
            if(ch == ')'){
                if( sta.empty() || sta.top() != '(' ) return false;
                sta.pop();
            }else if(ch == '}'){
                if( sta.empty() || sta.top() != '{' ) return false;
                sta.pop();
            }else if(ch == ']'){
                if( sta.empty() || sta.top() != '[' ) return false;
                sta.pop();
            }
            else{
                sta.push(ch);
            }
        }
        return sta.empty();
    }
};
~~~

