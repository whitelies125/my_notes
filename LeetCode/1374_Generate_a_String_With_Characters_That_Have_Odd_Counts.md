# 1347. Generate a String With Characters That Have Odd Counts

~~~C++
class Solution {
public:
    string generateTheString(int n) {
        string str = "";
        if(n%2){
            while(n--){
                str += 'a';
            }
        }else{
            str += 'b';
            while(--n){
                str += 'a';
            }
        }
        return str;
    }
};
~~~

