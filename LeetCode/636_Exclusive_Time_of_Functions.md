# 636. Exclusive Time of Functions

~~~C++
class Solution {
public:
    struct Func{
        int number;
        int start;
        Func(int number, int start) : number{number}, start{start} {}
    };

    vector<int> exclusiveTime(int n, vector<string>& logs) {
        stack<Func> sta;
        vector<int> res (n,0);
        for(auto& log : logs){
            int number = stoi(log.substr(0,log.find(':')));
            bool is_start = log.substr(log.find(':')+1,1) == "s" ? true : false;
            int time = stoi(log.substr(log.rfind(':')+1));
            if(is_start){
                sta.push(Func(number, time));
            }
            else {
                Func& func = sta.top();
                res[number] += time+1 - func.start;
                sta.pop();
                if(!sta.empty())  res[sta.top().number] -= time+1 - func.start;
            }
        }
        return res;
    }
};
~~~

