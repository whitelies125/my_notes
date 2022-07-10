# Integer to Roman

~~~C++
class Solution {
public:
    string intToRoman(int num) {
        
        string Roman_1[10] = {"","I","II","III","IV","V","VI","VII","VIII","IX"};
        string Roman_2[10] = {"","X","XX","XXX","XL","L","LX","LXX","LXXX","XC"};
        string Roman_3[10] = {"","C","CC","CCC","CD","D","DC","DCC","DCCC","CM"};
        string Roman_4[4] = {"","M","MM","MMM"};
        
        if( num < 10) return Roman_1[num];
        
        string res = "";
        int time = 1;
        while(num != 0){
            int this_time = num % 10;
            switch(time){
                case 1: res = Roman_1[this_time] + res; break;
                case 2: res = Roman_2[this_time] + res; break;
                case 3: res = Roman_3[this_time] + res; break;
                case 4: res = Roman_4[this_time] + res; break;
            }
            time ++;
            num /= 10;
        };
        return res;
    }
};
~~~

