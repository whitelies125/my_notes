# KMP

~~~C++
#include <iostream>
#include <vector>
#include <string>

int* get_next(std::string sub_str){
    int* next = (int*)malloc(sub_str.size() * sizeof(int));
    int i = 0;
    int j = -1;
    next[0] = -1;
    while(i < (int)sub_str.size()-1){        
        if( j == -1 || sub_str.at(i) == sub_str.at(j)){
            ++i;
            ++j;
            next[i] = j;
        } else {
            j = next[j];
        }
    }
    // std::cout << "next : ";
    // for(int i = 0; i < sub_str.size(); i++) std::cout << next[i] << " ";
    // std::cout << std::endl;
    return next;
}

int is_substr(std::string& str, std::string& sub_str){    
    int* next = get_next(sub_str);
    int i = 0;
    int j = 0;
    while( i<(int)str.size() && j<(int)sub_str.size()){
        if( j == -1 || str.at(i) == sub_str.at(j)){
            i++;
            j++;
        }
        else{
            j = next[j];
        }
    }
    if( j >= sub_str.size()) return i-sub_str.size();
    else return -1;
}

int main() {
    std::string str = "ababceeababdababe";
    std::string sub_str = "ababe";
    int index = is_substr(str, sub_str);
    if(index != -1) std::cout << index << " " << str.substr(index,sub_str.size());
    else std::cout << "Not sub_str";
    return 0;
}
~~~

