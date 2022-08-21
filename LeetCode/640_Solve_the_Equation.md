# 640. Solve the Equation

思路倒是不难，只是自己代码写得不够简洁：

~~~c++
class Solution {
public:
    string solveEquation(string equation) {
        int k = 0;//kx
        int sum = 0;
        char flag = '+';
        int op = 1;// 1:add; 0:subtract
        int has_x = 0;
        string s = "";
        for(int i = 0; i < equation.size(); ++i){
            if(equation[i] == '+' || equation[i] == '-' || equation[i] == '='){
                if(equation[i] == '='){
                    flag = '-';
                }
                last:
                if(!s.empty()){
                    if(has_x){
                        if(op) k += (s.size() == 1) ? 1 : stoi(s.substr(0,s.size()-1));
                        else k -= (s.size() == 1) ? 1 : stoi(s.substr(0,s.size()-1));
                        has_x = 0;
                    }else{
                        if(op) sum += stoi(s);
                        else sum -= stoi(s);
                    }
                    s = "";
                }
                op = equation[i] == flag ? 1 : 0;
            }
            else{
                if(equation[i] == 'x') has_x = 1;
                s += equation[i];
                if(i == equation.size()-1) goto last;
            }
        }
        if(k == 0) return sum == 0 ? "Infinite solutions" : "No solution";
        if(k != 0) return sum == 0 ? "x=0" : "x=" + to_string(-sum/k);
        return "";
    }
};
~~~

