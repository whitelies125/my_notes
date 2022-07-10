# Longest Palindromic Substring

~~~c
char * longestPalindrome(char * s){
    int i = 0;
    int left = 0;
    int right = 0;
    int max_left = 0;
    int max_right = 0;
    int max_length = 0;
    for( i = 0; i < strlen(s); i++){
        left = i;
        right = i;
        while(s[left] == s[right]){
            left --;
            right ++;
            if(left < 0 || right > strlen(s) ) break;
        }
        left += 1;
        right -= 1;
        if( max_length < right - left){
            max_left = left;
            max_right = right;
            max_length = max_right - max_left;
        }
    }
    for( i = 0; i < strlen(s); i++)
    {
        left = i;
        right = i + 1;
        while(s[left] == s[right]){
            left --;
            right ++;
            if(left < 0 || right > strlen(s) ) break;
        }
        left += 1;
        right -= 1;
        if( max_length < right - left){
            max_left = left;
            max_right = right;
            max_length = max_right - max_left;
        }
    }
    char *str = (char *)malloc( (max_length+2) * sizeof(char) );
    strncpy(str, s+max_left, max_length+1);
    str[max_length+1] = '\0';
    return str;
}
~~~

