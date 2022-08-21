# 1408. String Matching in an Array

KMP 算法：

~~~C++
class Solution {
public:
    int* get_next(std::string sub_str){
        int* next = (int*)malloc(sub_str.size() * sizeof(int));
        int i = 0;
        int j = -1;
        next[0] = -1;
        while(i < (int)sub_str.size()-1){
            if( j == -1 || sub_str[i] == sub_str[j]){
                ++i;
                ++j;
                next[i] = j;
            } else {
                j = next[j];
            }
        }
        return next;
    }

    bool is_substr(std::string& str, std::string& sub_str){
        int* next = get_next(sub_str);
        int i = 0;
        int j = 0;
        while( i<(int)str.size() && j<(int)sub_str.size()){
            if( j == -1 || str[i] == sub_str[j]){
                i++;
                j++;
            }
            else{
                j = next[j];
            }
        }    
        if( j >= sub_str.size()) return true;
        else return false;
    }

    vector<string> stringMatching(vector<string>& words) {
        vector<string> res;
        for(int i = 0; i < words.size(); i++){
            for(int j = 0; j < words.size(); j++){
                if(i != j && words[i].size() < words[j].size()){
                    if(is_substr(words[j], words[i])){                        
                        res.push_back(words[i]);
                        break;
                    }
                }
            }
        }
        return res;
    }
};
~~~

