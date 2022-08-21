# 1455. Check If a Word Occurs As a Prefix of Any Word in a Sentence

~~~
class Solution {
public:
    int isPrefixOfWord(string sentence, string searchWord) {
        int cnt = 0;
        for(int i = 0; i < (int)sentence.size()-(int)searchWord.size()+1; ++i){
            if(i != 0 && sentence[i-1]!= ' ') continue;
            else ++cnt;
            if(sentence.substr(i,searchWord.size()) == searchWord){
                return cnt;
            }
        }
        return -1;
    }
};
~~~

2022.08.20