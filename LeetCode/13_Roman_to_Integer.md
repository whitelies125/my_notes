# 13. Roman to Integer

~~~C++
class Solution {
public:
    int k(char c){
        if(c == 'I') return 1;
        else if(c == 'V') return 5;
        else if(c == 'X') return 10;
        else if(c == 'L') return 50;
        else if(c == 'C') return 100;
        else if(c == 'D') return 500;
        else if(c == 'M') return 1000;
        else return 0;
    }
    
    int romanToInt(string s) {
        int sum = 0;
        int temp = 0;
        for(char ch: s){
            if(temp < k(ch))
                sum = sum + k(ch) - 2*temp;
            else
                sum = sum + k(ch);
            temp = k(ch);
        }
        return sum;
    }
};
~~~

