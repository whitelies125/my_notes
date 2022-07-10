# Zigzag Conversion

~~~C++
class Solution {
public:
    string convert(string s, int numRows) {
        if( numRows == 1) return s;
        string str;
        for( int i = 0; i < numRows; i++ ){
            bool round = true;
            int temp = -1;
            for( int j = i; j < (signed int)s.size(); ){
                if(temp != j ){
                    str += s[j];
                }
                temp = j;
                if( round )
                    j += 2*(numRows-i)-2;
                else
                    j += 2*(i+1)-2;
                round = !round;
            }
        }
        return str;
    }
};
~~~

