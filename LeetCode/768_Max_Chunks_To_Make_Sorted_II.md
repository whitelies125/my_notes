# Max Chunks To Make Sorted II

假定分为左块和右块，若左块最大值小于等于右边最小值，则成立，左块确实可成为一个块，count++。

自己的：
时间复杂度：$O(n^2)$ ，
空间复杂度：$O(1)$ 。

~~~C++
class Solution {
public:
    int maxChunksToSorted(vector<int>& arr) {
        int count = 0;
        int left_max = -1;
        int right_min = 100000001;
        for(int i = 0; i < arr.size(); i++){
            if(right_min > arr[i]){
                right_min = arr[i];
            }
        }
        if(left_max < right_min) count++;
        for(int i = 0; i < arr.size(); i++){
            //find left_max;
            if(arr[i] > left_max) left_max = arr[i];
            //find right_min;
            if(arr[i] == right_min){
                if(i == arr.size()-1) right_min = -1;
                else right_min = arr[i+1];
                for(int j = i+1; j < arr.size(); j++){
                    right_min = right_min < arr[j] ? right_min : arr[j];
                }
            }
            if(left_max <= right_min) count++;
        }
        return count;
    }
};
~~~

改进一下，不用每次都重新计算一边右块最大值：
时间复杂度：$O(2n)$ ，
空间复杂度：$O(n)$ 。

~~~C++
class Solution {
public:
    int maxChunksToSorted(vector<int>& arr) {
		// 先逆序遍历，记录每个右块的最小值。
        vector<int> right_min(arr.size(),-1);
        int min = 100000001;
        for(int i = arr.size()-1; i >= 0; --i){
            min = min < arr[i] ? min : arr[i];
            right_min[i] = min;
        }
		// 正序遍历，每次将一个元素加入左块，
        int count = 0;
        int left_max = -1;
        for(int i = 0; i < arr.size(); ++i){
            // 判断 左块最大值 < 右块最小值
            if(left_max <= right_min[i]) count ++;
            // 将一个元素加入左块，
            left_max = left_max > arr[i] ? left_max : arr[i];
        }

        return count;
    }
};
~~~





把数组分为左右两部分，若 left 中最大值 < right 最小值。



4351266

