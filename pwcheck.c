#include <stdio.h>
#include <stdlib.h>

struct password{
    char string[101];
    int length;
};

struct stats{
    int longestPass;
    int passCount;
    int charCount;
    float avg;
    int uniqueChars[93];
};

//Takes string and returns it's length
int getLength(char* str){
    int ans = 0;
    for (int i = 0; str[i] !='\0'; i++)
    {
        ans++;
    }
    return ans;
}

//Compares two strings, returns 1 if they are same
int mystrComp(char* str1, char* str2){

    if(getLength(str1) != getLength(str2)){
        return 0;
    }
    for(int i = 0; i <= getLength(str1); i++){
        if(str1[i] != str2[i]){
            return 0;
        }
    }
    return 1;
}

int main(int argc, char* argv[])
{
    struct stats stats = { .longestPass = 0, .passCount = 0, .avg = 0.0};
        for(int i = 0; i <= 93; i++){
            stats.uniqueChars[i] = 0;
        };
    struct password buffer;
    while(fgets(buffer.string, 101, stdin) != NULL){
        buffer.length = getLength(buffer.string) - 1;
        
        if(buffer.length >= stats.longestPass){
            stats.longestPass == buffer.length;
        }
    }
    return 0;
}