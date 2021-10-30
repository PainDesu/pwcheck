#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct password{
    char string[102];
    int length;
};

struct stats{
    int shortestPass;
    int passCount;
    int charCount;
    int uniqueChars[93];
    int uniqueCount;
    double avg;
};

struct params{
    int level;
    int param;
    bool stats;
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
    int size;
    if(getLength(str1) != getLength(str2)){
        return false;
    }
    else{
        size = getLength(str1);
    }
    for(int i = 0; i <= size; i++){
        if(str1[i] != str2[i]){
            return false;
        }
    }
    return true;
}

/* Function: capital_check
*  ------------------------
*   Checks if password contains both capital and lower case letters
*
*   pass: struct password that gets checked
*
*   returns: True if both capital and lower case are used
*/
bool capital_check(struct password pass){
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

//Returns true if passowrd contains special symbol
bool checkSpecialChar(struct password pass){
    for (int i = 0; i <= pass.length; i++)
    {
        if ((pass.string[i] >= 33 && pass.string[i] <= 47) || (pass.string[i] >= 58 && pass.string[i] <= 64) || (pass.string[i] >= 91 && pass.string[i] <= 96) || (pass.string[i] >= 123 && pass.string[i] <= 126))
        {
            return true;
        }
        
    }
    return false;
}

//Returns true if password contains number
bool check_num(struct password pass){
    for(int i = 0; i <= pass.length; i++){
        if((pass.string[i] >= '0') && (pass.string[i] <= '9')){
            return true;
        }
    }
    return false;
}

//Returns true if 2nd level of checks works
bool groups_check(struct password pass, int param){
    if(capital_check(pass) == false){
        return false;
    };
    int count = 2 + checkSpecialChar(pass) + check_num(pass);
    int checkNum = param;
    
    if(checkNum > 4){
        checkNum = 4;
    }

    if(count >= checkNum){
        return true;
    }
    return false;
}

//Returns size of longest repeating chars
bool longest_repeat_check(struct password pass, int param){
    if(groups_check(pass, param) == false){
        return false;
    }
    int current = 0;
    for (int i = 1; i < pass.length; i++)
    {
        if(pass.string[i-1] == pass.string[i]){
            current++;
        }
        if (current > param)
        {
            return false;
        }
    }
    return true;
}

bool substrings_check(struct password pass, int param){
    if(longest_repeat_check(pass, param) == false){
        return false;
    }
    char currentSubstring[param];
    for(int i = 0; i < (pass.length - param); i++){
        
        //Fill currentSubstring
        for(int a = 0; a < param; a++){
            currentSubstring[a] = pass.string[a+i];
        }

        //Check for substring
        for(int a = 0; a < pass.length; a++){
            for (int b = 0; b <= param; b++){
                if(currentSubstring[b] != pass.string[a + b + 1]){
                    break;
                }
                if(b == (param - 1)){
                    return false;
                }
            }
        }
    }
    return true;
}

bool set_params(char* argv[], struct params* parameters){
    int buffer = strtol(argv[0], NULL, 10);
    if(buffer == 0){
        fprintf(stderr, "Invalid parameters");
        return false;
    }
    else{
        parameters->level = buffer;
    }

    buffer = strtol(argv[1], NULL, 10);

    if (buffer == 0){
        fprintf(stderr, "Invalid parameters");
        return false;
    }
    else{
        parameters->param = buffer;
    }
    return true;
}

/*  Checks password and adds values into stats
    Parameters:
    struct password == Current password that gets added to stats
    struct stats* == Pointer to stats struct, that gets updated
*/
void statsCount (struct password pass, struct stats* stats){
    if(pass.length < stats->shortestPass){
        stats->shortestPass = pass.length;
    }
    stats->charCount = stats->charCount + pass.length;
    stats->passCount++;
    for(int i = 0; i < pass.length; i++){
        stats->uniqueChars[((int)pass.string[i] - 32)] = 1;
    }
}

void final_stats_count(struct stats* stats){
    stats->avg = stats->charCount / stats->passCount;
    for(int i = 0; i <= 93; i++){
        stats->uniqueCount = stats->uniqueCount + stats->uniqueChars[i];
    }
}

int main(int argc, char* argv[])
{
    struct stats statistics = { .shortestPass = 100, .passCount = 0, .uniqueChars = {0}, .avg = 0.0, .uniqueCount = 0};
    struct stats *stats = &statistics;
    struct password pass;
    struct params parameters;
    struct params *params = &parameters;
    char* arguments[2];
    bool valid;
    if(argc >= 3 && argc <= 4){
        if(argc == 4){
            if(mystrComp(argv[3], "--stats")){
                parameters.stats = true;
            }
            else{
                fprintf(stderr, "Invalid arguments");
                return EXIT_FAILURE;
            }
        };
        for(int i = 1; i <= 2; i++){
            arguments[i-1] = argv[i]; 
        }
        if(set_params(arguments, params) == false){
            return EXIT_FAILURE;
        }
    }

    
    while(fgets(pass.string, 102, stdin) != NULL){
        pass.length = getLength(pass.string) - 1; //-1 for '\n'
        statsCount(pass, stats);
        valid = false;
        switch (parameters.level)
        {
        case 1:
            valid = capital_check(pass);
            break;
        case 2:
            valid = groups_check(pass, parameters.param);
            break;
        case 3: 
            valid = longest_repeat_check(pass, parameters.param);
            break;
        case 4:
            valid = substrings_check(pass, parameters.param);
            break;
        default:
            fprintf(stderr, "Unexpected level value");
            break;
        }
        if(valid){
            fprintf(stdout, "%s", pass.string);
        }
    }
        if(parameters.stats){
            fprintf(stdout,"Statistika: \n Ruznych znaku: %d \n Minimalni delka: %d \n Prumerna delka: %.1f \n", statistics.charCount, statistics.charCount, statistics.passCount/1.0f );
        }
    return EXIT_SUCCESS;
}