# Palindrome Number

~~~C++
class Solution {
public:
    bool isPalindrome(int x) {
        if( x < 0) return false;
        if( x < 10) return true;
        int temp = x;
        int sum = 0;
        while( x){
            if( sum > (INT_MAX - x%10)/10 ) return false;
            sum = 10*sum + x%10;
            x /= 10;            
        }
        return (temp == sum);
    }
};
~~~

