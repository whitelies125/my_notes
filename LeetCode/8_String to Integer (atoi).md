# String to Integer (atoi)

~~~c++
class Solution {
public:
    int myAtoi(string s) {
        bool is_negative = false;
        int i = 0;
        while( s[i] == ' ')
            i++;
        if( s[i] == '-' || s[i] == '+'){
            is_negative = s[i] == '-' ? true : false;
            i++;
        }
        int ans = 0;
        while( '0' <= s[i] && s[i] <= '9'){
            if(ans > (INT_MAX -(s[i]-'0') )/10)
                return is_negative ? INT_MIN : INT_MAX;
            ans = 10*ans + (s[i]-'0');
            i++;
        }
        return is_negative ? -ans : ans;
    }
};
~~~

