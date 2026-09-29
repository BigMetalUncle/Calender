#include "common.h"
#include "processing-functions.h"
#include <string.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FUNC_RET ParseArgv(int argc, char *argv[], uint8_t *day1, uint8_t *day2, MONTH *month1,
                   MONTH *month2, YEAR *year1, YEAR *year2,
                   BOOL *year1Supplied, BOOL *year2Supplied)
{   
    if (argc == 1)
    {
        printf(BRED"ERROR:"MAG" No arguments were passed. Try '%s -h' for Usage info.\n"RST, argv[0]);
        return ERROR;
    }
    
    for (int i = 1; i < argc; i++)
    {
        uint8_t a =0;
        MONTH b = NULL_MONTH;
        YEAR c = 0;
        int temp;
        
        // 1. Deal with flags:
        if (argv[i][0] == '-')
        { 
            if (strlen(argv[i]) > 2) {
                printf(BRED"ERROR:"MAG" Flags must be one character length (Eg: '-h').\n"
                       "       '%s' is invalid and undefined. See help menu with '-h'."RST, argv[i]);
                return ERROR;
            }
            // Check and process for flags:
            else {
                switch (argv[i][1])
                {
                    case 'h':
                        user_help();
                        return QUIT;
                    case 'w':
                        return DISPLAY_WEEKDAY_ORDER;
                    default:
                        printf(BRED"ERROR:"MAG" Undefined argument given: '%s'.\n"
                               "       Use '-h' flag for help.\n"RST, argv[i]);
                        return ERROR;
                }
            }
        }
        // 2. Check if argument is a value of form DD-MM-YYYY        
        else if ((temp = StrToDate(argv[i], &a, &b, &c, TRUE)) != SUCCESS)
        {   
            // If the above succeeded, the year (ie. c, here) is certainly set or 
            // it was an invalid date format (ie. returned ERROR) .
            if (temp == ERROR)
                return ERROR;
            // Its only a question if a (day) or b (month) or both was set.
            // I. check for yea            
            if (temp == YEAR_ONLY || temp == MONTH_YEAR_ONLY || temp == DAY_MONTH_YEAR_ONLY){
                if (*year1Supplied){
                    if (*year2Supplied) {
                        puts(BRED"ERROR:"MAG" You gave values for Year more than twice."RST);
                        return ERROR;
                    }
                    else {
                        *year2 = c;
                        *year2Supplied = TRUE;
                    }
                }
                else {
                    *year1 = c;
                    *year1Supplied = TRUE;
                }
            }
            
            // II. check for month
            if (temp == MONTH_YEAR_ONLY || temp == DAY_MONTH_YEAR_ONLY) {
                if (*month1) {
                    if (*month2) {
                        puts(BRED"ERROR:"RST" You gave values for Month more than twice.");
                        return ERROR;
                    }
                    else 
                        *month2 = b;
                }
                else 
                    *month1 = b;
            }
            
            // III. check for day
            if (temp == DAY_MONTH_YEAR_ONLY) {
                if (*day1) {
                    if (*day2) {
                        puts(BRED"ERROR:"MAG" You gave values for Day more than twice."RST);
                        return ERROR;
                    }
                    else 
                        *day2 = a;
                }
                else 
                    *day1 = a;
            }
        }
        // 3. If argv[i] is a StrMonth . Eg: ./prog July 
        else if ((temp = StrToMonth(argv[i])) !=INVALID_MONTH)
        {
            
            if (*month1) {
                if (*month2) {
                    puts(BRED"ERROR:"MAG" You gave values for Month more than twice."RST);
                    return ERROR;
                }
                else 
                    *month2 = temp;
            }
            else 
                *month1 = temp;
        }
        
        // 4. tell they entered some undefined argument:
        else{
            printf(BRED"ERROR:"MAG" Undefined Argument. What did you mean by argument : '%s' ?\n"
                   "       If you meant something else check help utility to use the "
                   "currect Flag.\n"RST, argv[i]);
            return ERROR;
        }
    }
    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FUNC_RET StrToDate(char *str, uint8_t *day, MONTH *month, YEAR *year, BOOL LogErrors)
{
    uint8_t a; 
    MONTH b; 
    YEAR c;
    int temp;
    char dummy[2];
        
    // 1. Check if argument is a value of form DD-MM-YYYY
    if ((temp = sscanf(str, "%hhu-%u-%hu%1s", &a, &b, &c, dummy)) >= 1)
    {
        // YYYY
        if (temp == 1) {
            if (sscanf(str, "%hu%1s", &c, dummy) == 1){
                // The extra sscan to see if the argument was valid number only.
                *year = c;
                return YEAR_ONLY;
            }
            else {
                if (LogErrors) {
                    printf(BRED"ERROR:"MAG" You passed a non-valid year argument : '%s' .\n"RST, str);
                    return ERROR;
                }
            }
        }
        
        // MM-YYYY
        else if (temp == 2) {
            if (a <= 0 || a > 12) {
                if (LogErrors){
                    printf(BRED"ERROR:"MAG" Invalid Month of Date: '%s'\n"RST, str);
                    // printf("a = %u; b = %u; *month = %u\n", a, b, *month); //remove
                }
                return ERROR;
            }
            else{
                *month = a;
                *year = b;
                return MONTH_YEAR_ONLY;
            }
        }
        //xx-xx-xxxxstring
        else if (temp == 4) {
            if (LogErrors)    
                printf(BRED"ERROR:"MAG" Why does your date format has non-numbers at the end.\n"
                       "       That's illegal Format for Date : '%s'\n"RST, str);
            return ERROR;
        }// DD-MM-YY
        else {
            if (a <= 0 || a > 31 || b <= 0 || b > 12) {
                if (LogErrors)
                    printf(BRED"ERROR:"MAG" You have invalid Values for Month and/or Day : '%s'\n"RST, str);
                return ERROR;
            }
            else {
                *day = a;
                *month = b;
                *year = c;
                return DAY_MONTH_YEAR_ONLY;
            }
        } 
    }
    // Means the argument didn't look anything similar to a date. Main() can check for other stuff
    // But returning an error meant it looked like a date format, but was not valid .
    return SUCCESS;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MONTH StrToMonth(char *strMonth)
{   
    char buff[10] ; // The longest month 'September' is of len 9
    strncpy(buff, strMonth, sizeof(buff));
    strtolower(buff);
    char *arr[] = {"jan", "feb", "mar", "apr", "may", "jun", 
                   "jul", "aug", "sep", "oct" ,"nov", "dec" };
   
    for (int i = 0; i < 12; i++)
    {
        if (strstr(buff, arr[i]))
            return i+1; 
    }
    return INVALID_MONTH;
   
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Below is a helper function created by AI:
void strtolower(char *s)       // in‑place conversion
{
    for (; *s; ++s) *s = (char)tolower((unsigned char)*s);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void user_help()
{
    puts(BGRN"Calender Program Help menu:\n"RST
         BLU"Author: "BBLU"BigMetalUncle"RST" (Github.com)\n"
         
         BCYN"\nUSAGE: ./calender [$FLAGS] $ARGUMENTS\n"RST
         
         "\nNote: This program uses few arguments and still executes different kinds of tasks\n"
           "      based on understanding from what values was given. Take a look at the\n"
         BGRN"Available Flags:\n"RST
         BCYN"\t -h"RST"  -->  Print this help menu.\n"
         BCYN"\t -w"RST"  -->  Print the Weekday Order from the current reference, \n" 
         
         BGRN"\nAvailable Operations:\n"RST
         YEL"\t 1. Display a month of an year (most common operation).\n"
         "\t 2. Display all months of an year.\n"
         "\t 3. Display a paricular month of a range of years.\n"
         "\t 4. Display all months from a month1 of an year1 to another \n"
         "\t    month2 of  year2 including the former and latter.\n"
         "\t 5. Display all months of all years of a range of years.\n"
         "\t 6. Count No. Of days between two dates.\n"
         "\t 7. Get info like DateID and Weekday of a date.\n"RST
         
         BGRN"\nInstructions to Give arguments:\n"RST
         "\tGiving values is easy. You can give a values like "BCYN"'DD-MM-YYYY'"RST" to\n"
         "supply day, month and year in one go OR "BCYN"'MM-YYYY'"RST" if you only need to\n"
         "supply month and year.\n"
         "\tThere's also another way of giving months and years. (NOTE: This method is\n"
         "NOT applicable for days. "BYEL"To supply days, use 'DD-MM-YYYY' format only !"RST").\n"
         "You can supply months by sending arguments as literal month names or short names\n"
         "( "BYEL"For Example: "BCYN"'sept', 'sep', 'september'"BYEL" in any Case works for "BCYN"'September'"RST"\n"
         "Make sure the "BYEL"short name is atleast 3 chars long"RST" of the actuall full month name ).\n"
         "Supply an year by just supplying a Whole number as argument.\n"
         "\tAnd don't try supplying negative numbers ! :D Try at your own risk and see what happens.\n"
         
         "\n\tYou can supply upto two days, months and years respectively and a minimum\n"
         "of one year as argument for the program to do any of the above tasks accordingly\n"
         "or it throws an error.\n"
         
        BGRN "\nHow to Perform Tasks:\n"RST
         "Note: Feel free to supply values in any order !\n"
         
         "\n\t For #1 supply just supply 1 month and 1 year.\n"
         CYN"\t EXAMPLE: "BBLU"./calender Jan 2010"MAG"  # Print the month of Jan 2010. Supply in any order: 2010 Jan\n"
         RST
         
         "\n\t For #2 supply 1 year.\n"
         CYN"\t EXAMPLE: "BBLU"./calender 2010"MAG"   # Print all months in 2010\n"RST
         
         "\n\t For #3 supply 1 month and 2 years.\n"
         CYN"\t EXAMPLE: "BBLU"./calender 2010 2015 dec"MAG"   # Print all Decembers between 2010 and 2015.\n"RST
         
         "\n\t For #4 supply 2 months and 2 years.\n"
         CYN"\t EXAMPLE: "BBLU"./calender 5-2010 Sep 2015"MAG"   # Print all months between May 2010 and Sep 2015.\n"
         RST
         
         "\n\t For #5 supply just 2 years.\n"
         CYN"\t EXAMPLE: "BBLU"./calender 2010 2015"MAG"    # Print Jan to Dec of all years between 2010 and 2015.\n"
         RST
         
         "\n\t For #6 supply two full dates (ie. 'DD-MM-YYYY').\n"
         CYN"\t EXAMPLE: "BBLU"./calender 1-1-2015  31-12-2015"MAG"   # Does not include the former.\n"
         "                                                   # This Eg. results 364 days\n"RST
         
         "\n\t For #7 supply just 1 full date.\n"
         CYN"\t EXAMPLE: "BBLU"./calender 1-1-2015"MAG"   # Don't give 1 Jan 2015 because '1' will be taken as year1\n" 
         "\t                               # and '2015' as year2 giving result for #3. This Eg. results Thursday\n"
         RST);
}
