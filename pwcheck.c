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

/* Function: get_length
*  ------------------------
*   Calculates the lenght of a string
*
*   str*: string that gets checked
*
*   returns: Length of a string
*/
int get_length(char* str){
    int ans = 0;
    for (int i = 0; str[i] !='\0'; i++)
    {
        ans++;
    }
    if(str[ans] == '\n'){
        return ans - 1;
    }
    return ans;
}

/* Function: my_str_comp
*  ------------------------
*   Compares two strings if they are the same
*
*   str1*: First compared string
*   str2*: Second compared string
*
*   returns: True if the strings are the same
*/
bool my_str_comp(char* str1, char* str2){
    int size;
    if(get_length(str1) != get_length(str2)){
        return false;
    }
    else{
        size = get_length(str1);
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

/* Function: check_special_char
*  ------------------------
*   Checks if password contains special chars
*
*   pass: struct password that gets checked
*
*   returns: True if special char is used
*/
bool check_special_char(struct password pass){
    for (int i = 0; i <= pass.length; i++)
    {
        if ((pass.string[i] >= 33 && pass.string[i] <= 47) || (pass.string[i] >= 58 && pass.string[i] <= 64) || (pass.string[i] >= 91 && pass.string[i] <= 96) || (pass.string[i] >= 123 && pass.string[i] <= 126))
        {
            return true;
        }
        
    }
    return false;
}

/* Function: check_num
*  ------------------------
*   Checks if password contains number
*
*   pass: struct password that gets checked
*
*   returns: True if number is used
*/
bool check_num(struct password pass){
    for(int i = 0; i <= pass.length; i++){
        if((pass.string[i] >= '0') && (pass.string[i] <= '9')){
            return true;
        }
    }
    return false;
}

/* Function: groups_check
*  ------------------------
*   Checks if password contains number of groups of different possible chars
*
*   pass: struct password that gets checked
*   param: number of groups that need to be checked
*
*   returns: True if the number of groups is higher or equal to param
*/
bool groups_check(struct password pass, int param){
    if(capital_check(pass) == false){
        return false;
    };
    int count = 2 + check_special_char(pass) + check_num(pass);
    int checkNum = param;
    
    if(checkNum > 4){
        checkNum = 4;
    }

    if(count >= checkNum){
        return true;
    }
    return false;
}

/* Function: longest_repeat_check
*  ------------------------
*   Checks if password contains substring of same repeating char longer then param
*
*   pass: struct password that gets checked
*   param: lenght of substring that is not allowed
*
*   returns: True if there are no substrings of same repeating char longer then param
*/
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

/* Function: substring_check
*  ------------------------
*   Checks if password contains substrings of size param
*
*   pass: struct password that gets checked
*   param: lenght of substrings that is not allowed
*
*   returns: True if no 2 substrings of lenght param are found in pass
*/
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

/* Function: set_params
*  ------------------------
*   Takes arguments and puts them into struct params
*
*   argv[][]: Arguments that get transfered
*   parameters*: Pointer to struct params that get updated
*
*   returns: True if arguments are valid and got transfered into parameters
*/
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

/* Function: stats_count
*  ------------------------
*   Adds current password into the stats
*
*   pass: struct password that gets added into the counts
*   stats*: pointer to the current stats struct that keeps the statistics
*/
void stats_count (struct password pass, struct stats* stats){
    if(pass.length < stats->shortestPass){
        stats->shortestPass = pass.length;
    }
    stats->charCount = stats->charCount + pass.length;
    stats->passCount++;
    for(int i = 0; i < pass.length; i++){
        stats->uniqueChars[((int)pass.string[i] - 32)] = 1;
    }
}

/* Function: final_stats_count
*  ------------------------
*   Does last calculation of stats before printing
*
*   stats*: pointer to the current stats struct that keeps the statistics
*/
void final_stats_count(struct stats* stats){
    if(stats->passCount == 0){
        stats->avg = 0;
    }
    else{
        stats->avg = stats->charCount / stats->passCount;
    }
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
            if(my_str_comp(argv[3], "--stats")){
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
        pass.length = get_length(pass.string);
        stats_count(pass, stats);
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
            final_stats_count(stats);
            fprintf(stdout,"Statistika: \n Ruznych znaku: %d \n Minimalni delka: %d \n Prumerna delka: %.1f \n", statistics.uniqueCount, statistics.shortestPass, statistics.avg );
        }
    return EXIT_SUCCESS;
}