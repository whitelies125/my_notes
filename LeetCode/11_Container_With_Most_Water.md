# Container With Most Water

~~~C++
class Solution {
public:
    int maxArea(vector<int>& height) {
        int left = 0;
        int right = height.size() - 1;
        int max = 0;
        int s = 0;
        while( left < right){
            s = (right - left) * min(height[left],height[right]); 
            if( s > max)
                max = s;
            if(height[left] <= height[right])
                left++;
            else
                right--;
        }
        return max;
    }
};
~~~

