# 1282. Group the People Given the Group Size They Belong To

自己写的，时间复杂度 $O(n^2)$。

~~~C++
class Solution {
public:
    vector<vector<int>> groupThePeople(vector<int>& groupSizes) {
        vector<vector<int>> res;
        unordered_map<int,vector<int>> group_numbers;
        for(int i = 0; i < groupSizes.size(); ++i){
        	group_numbers[groupSizes[i]].push_back(i);
        }
		
        
            for(auto& vec : res){
                if(flag) break;
                // cout << vec.size() << " " << groupSizes[i] << endl;
                if(vec.size() == groupSizes[i]){
                    int j = 0;
                    for( ; j < vec.size(); ++j){
                        if(vec[j] == -1){
                            flag = 1;
                            break;
                        }
                    }
                    if(flag) vec[j] = i;
                }
            }
            if(!flag){
                // cout << "groupSizes[i]: " << groupSizes[i] << endl;
                vector<int> vec (groupSizes[i],-1);
                // cout << "vec.size(): " << vec.size() << endl;
                vec[0] = i;
                res.push_back(vec);
            }
        }
        return res;
    }
};
~~~

看了题解后的：

~~~C++
class Solution {
public:
    vector<vector<int>> groupThePeople(vector<int>& groupSizes) {
        vector<vector<int>> res;
        unordered_map<int,vector<int>> group_numbers;
        for(int i = 0; i < groupSizes.size(); ++i){
        	group_numbers[groupSizes[i]].push_back(i);
        }
		for(auto& [group_size, numbers] : group_numbers){ //C++17 结构化绑定
            for(int i = 0; i < numbers.size()/group_size; ++i){
                vector<int> vec;
                for(int j = i*group_size; j < (i+1)*group_size; ++j){
                    vec.push_back(numbers[j]);
                }
                res.push_back(vec);
            }
        }
        return res;
    }
};
~~~

2022.08.12

