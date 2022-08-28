# 1460. Make Two Arrays Equal by Reversing Sub-arrays

自己的：

~~~C++
int a[1001] {0};
int b[1001] {0};
class Solution {
public:
    bool canBeEqual(vector<int>& target, vector<int>& arr) {
        unordered_map<int,int> m;
        for(int it : target) m[it] += 1;
        for(int it : arr) {
            m[it] -= 1;
            if(m[it] < 0) return false;
        }
        for(auto itor = m.begin(); itor != m.end(); ++itor){
            if(itor->second != 0) return false;
        }
        return true;
    }
};
~~~

别人的哈希：

~~~C++
int v[1001];
class Solution {
public:
    bool canBeEqual(vector<int>& target, vector<int>& arr, int cnt = 0) {
        memset(v, 0, sizeof v);
        for (auto& i : target) cnt += !v[i]++;
        for (auto& i : arr) cnt -= !--v[i];
        return !cnt;
    }
};
~~~

别人的 sort()：

~~~C++
class Solution {
public:
    bool canBeEqual(vector<int>& target, vector<int>& arr) {
        sort(target.begin(), target.end());
        sort(arr.begin(), arr.end());
        return target == arr;
    }
};
~~~

2022.08.24