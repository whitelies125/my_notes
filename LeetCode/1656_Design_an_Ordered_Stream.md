# 1656. Design an Ordered Stream

~~~C++
class OrderedStream {
private:
    vector<string> vec;
    int last_stop_id;
public:
    OrderedStream(int n) : vec(n), last_stop_id(0) {}
    
    vector<string> insert(int idKey, string value) {
        vec[idKey-1] = value;
        vector<string> res;
        while(last_stop_id < vec.size() && !vec[last_stop_id].empty()){
            res.push_back(vec[last_stop_id]);
            ++ last_stop_id;
        }
        return res;
    }
};

/**
 * Your OrderedStream object will be instantiated and called as such:
 * OrderedStream* obj = new OrderedStream(n);
 * vector<string> param_1 = obj->insert(idKey,value);
 */
~~~

