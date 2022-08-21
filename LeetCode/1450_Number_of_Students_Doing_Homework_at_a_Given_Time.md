# 1450. Number of Students Doing Homework at a Given Time

~~~C++
class Solution {
public:
    int busyStudent(vector<int>& startTime, vector<int>& endTime, int queryTime) {
        int res = 0;
        for(int i = 0; i < startTime.size(); ++i){
            if(queryTime <= endTime[i] && startTime[i] <= queryTime) res++;
        }
        return res;
    }
};
~~~

