# 88. Merge Sorted Array

~~~C++
class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        int count = m+n;
        int i = m-1;
        int j = n-1;
        while(count--){
            if(i<0){
                nums1[count] = nums2[j]; j--;
            }
            else if(j<0){
                nums1[count] = nums1[i]; i--;
            }
            else if(nums1[i] < nums2[j]){
                nums1[count] = nums2[j]; j--;
            }
            else{
                nums1[count] = nums1[i]; i--;
            }
        }
    }
};
~~~

