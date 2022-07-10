# Longest_Substring_Without_Repeating_Characters

```c
int lengthOfLongestSubstring(char * s){
    int length = strlen(s);
    int count = 0,max = 0,head = 0,tail = 0;
    bool* hashmap = (bool *) calloc(128,sizeof(int));
    for( head = 0; head<=length-1; )
    {
        if(!hashmap[(int)s[head]])
        {
            hashmap[(int)s[head]] = 1;
            head++;
            count++;
            if(count > max) max = count;
        }
        else
        {
            while( hashmap[(int)s[head]] )
            {
                hashmap[(int)s[tail]] = 0;
                tail++;
                count--;
            }
        }
    }
    return max;
}
```

