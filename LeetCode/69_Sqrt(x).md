# Sqrt(x)

~~~c++
class Solution {
public:
    int mySqrt(int x) {
        if(x == 0)  return 0;
        int left = 1;
        int right = x/2;
        while(left < right){
            int mid = left+(right-left)/2;
            if(mid < x/mid ) left = mid +1;
            else right = mid;
        }
        return left > x/left ? left-1 : left;
    }
};
~~~

