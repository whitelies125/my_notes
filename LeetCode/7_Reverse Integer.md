# 7. Reverse Integer

~~~C++
class Solution {
public:
    int reverse(int x) {
        if( x == -2147483648){
            return 0;
        }
        bool is_negative = 0;
        if( x < 0){
            is_negative = 1;
            x = -x;
        }
        int ans = 0;
        while( x != 0 ){
            if( ans > (2147483647 - x%10)/10 )
                return 0;
            else{
                ans = 10*ans + x%10;
                x /= 10;
            }
        }
        if(is_negative) ans = -ans;
        return ans;
    }
};
~~~

