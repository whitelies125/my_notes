# 17. Letter Combination of a Phone Number

递归，DFS，看了别人的

~~~C++
class Solution {
public:
    vector<string> letterCombinations(string digits) {
        if(digits == "") return {};
        int location = 0;
        string combination = "";
        vector<string> res;
        DFS(digits, location, combination = "", res);
        return res;
    }

    void DFS(string digits, int location, string combination,vector<string>& res){
        if( location == digits.size() ){
            res.push_back(combination);
            return ;
        }
        string i_to_s[10] = {"","","abc","def","ghi","jkl","mno","pqrs","tuv","wxyz"};
        string str = i_to_s[static_cast<int>(digits[location]-'0')];
        for( char ch : str){
            DFS(digits, location+1, combination+ch,res);
        }
        return ;
    }
};
~~~

