# 70. Climbing Stairs

~~~C++
class Solution {
public:
    int climbStairs(int n) {
        int a = 1;
        int b = 0;
        int c = 0;
        while(n--){
            c = b;
            b = a;
            a = c+b;
        }
        return a;
    }
};
~~~

