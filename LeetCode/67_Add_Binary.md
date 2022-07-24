# Add Binary

~~~C++
class Solution {
public:
    string addBinary(string a, string b) {
        string& l = a.size() > b.size() ? a : b;
        string& s = a.size() > b.size() ? b : a;
        
        int i = 0;
        int over = 0;
        while(i < l.size()){
            if(i < s.size())
                l[l.size()-1-i] += s[s.size()-1-i]-'0' + over;
            else
                l[l.size()-1-i] += over;
            over = 0;
            if(l[l.size()-1-i] > '1'){
                l[l.size()-1-i] -= 2;
                over = 1;
            }
            i++;
        }
        if(over) return "1" + l;
        return l;
    }
};
~~~

