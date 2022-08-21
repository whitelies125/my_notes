# 899. Orderly Queue

我的：

~~~C++
class Solution {
public:
    string orderlyQueue(string s, int k) {
        if(k>1){
            sort(s.begin(),s.end());
            return s;
        }

        char min = 'z';
        for(int i = 0; i < s.size(); i++){
            if(min > s[i]) min = s[i];
        }
        vector<int> min_index;
        for(int i = 0; i < s.size(); i++){
            if(s[i] == min) min_index.push_back(i);
        }
        if(min_index.size() == 1){
            return s.substr(min_index[0])+s.substr(0,min_index[0]);
        }

        vector<string> strs;
        for(int i = 0; i < min_index.size(); i++){
            if(i != min_index.size()-1 ){
                strs.push_back(s.substr(min_index[i],min_index[i+1]-min_index[i]));
            } else {
                strs.push_back(s.substr(min_index[i])+s.substr(0,min_index[0]));
            }
        }

        string min_str = strs[0];
        int index = min_index[0];
        for(int i = 0; i < strs.size(); i++){
            if(min_str > strs[i]){
                min_str = strs[i];
                index = min_index[i];
            }
        }
        
        return s.substr(index)+s.substr(0,index);
    }
};
~~~

别人的，循环左移。

~~~C++
class Solution
{
public:
    string orderlyQueue(string s, int k)
    {
        if (k > 1)
        {
            sort(s.begin(), s.end(), [](char &a, char &b)
                 { return a < b; });
            return s;
        }
        else
        {
            string t = s;
            for (int i = 0; i < s.length(); i++)
            {
                s = s.substr(1, s.size() - 1) + s[0];
                if (s.compare(t) < 0)
                {
                    t = s;
                }
            }
            return t;
        }
    }
};
~~~

别人的，再后面拼接，避免循环左移的麻烦。

~~~C++
class Solution
{
public:
    string orderlyQueue(string s, int k)
    {
        if (k > 1)
        {
            sort(s.begin(), s.end(), [](char &a, char &b)
                 { return a < b; });
            return s;
        }
        else
        {
            string t = s;
            for (int i = 0; i < s.length(); i++)
            {
                s = s.substr(1, s.size() - 1) + s[0];
                if (s.compare(t) < 0)
                {
                    t = s;
                }
            }
            return t;
        }
    }
};
~~~

