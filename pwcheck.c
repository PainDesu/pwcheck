#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct password{
    char string[102];
    int length;
};

struct stats{
    int longestPass;
    int passCount;
    int charCount;
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

//Compares two strings, returns true if they are same
bool mystrComp(char* str1, char* str2){

    if(getLength(str1) != getLength(str2)){
        return false;
    }
    for(int i = 0; i <= getLength(str1); i++){
        if(str1[i] != str2[i]){
            return false;
        }
    }
    return true;
}

//Returns true if both capital and lower case letters are used in string
bool capitalCheck(struct password pass){
    bool small = false;
    bool capital = false;
    if (small == 0 && capital == 0){
        for (int i = 0; i < pass.length; i++)
        {
            if(small == false && pass.string[i] >= 'a' && pass.string[i] <= 'z' )
            {
                small = true;
            }
            if(capital == false && pass.string[i] >= 65 && pass.string[i] <= 90){
                capital = true;
            }
        }
    }
    return (small && capital);
}

//Checks password and adds values into stats
void statsCount (struct password pass, struct stats* stats){
    if(pass.length > stats->longestPass){
        stats->longestPass = pass.length;
    }
    stats->charCount = stats->charCount + pass.length;
    stats->passCount++;
    for(int i = 0; i < pass.length; i++){
        stats->uniqueChars[((int)pass.string[i] - 32)] = 1;
    }
}

int main(int argc, char* argv[])
{
    struct stats statistics = { .longestPass = 0, .passCount = 0, .uniqueChars = {0}};
    struct stats *stats = &statistics;
    struct password pass;
    while(fgets(pass.string, 102, stdin) != NULL){
        pass.length = getLength(pass.string) - 1;
        statsCount(pass, stats);
        if(capitalCheck(pass)){
            printf("%s", &pass.string);
        }
    }
    return 0;
}