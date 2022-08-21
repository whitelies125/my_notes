# 769. Max Chunks To Make Sorted

思路见 768 Max Chunks To Make Sorted II
这题是其简化版，无重复数字。

~~~C++
class Solution {
public:
    int maxChunksToSorted(vector<int>& arr) {
		// 先逆序遍历，记录每个右块的最小值。
        vector<int> right_min(arr.size(),-1);
        int min = 11;
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

