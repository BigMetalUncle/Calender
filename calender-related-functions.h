
#ifndef CALENDER_RELATED_FUNCTIONS_H
#define CALENDER_RELATED_FUNCTIONS_H

#include "common.h"

// Variables:
extern char  *str_nums[32];
extern uint8_t NoDaysInMonths[12];


// Functions:
void FillMonthData(char *MonthData[MONTH_DATA_SIZE], CalenderFingerprint *Reference, MONTH month, 
                   YEAR year, WEEKDAY *WeekdayOrder);
                   
// Returns a Unique Date Identifier (DateID). Supports from 1/1/0000 to x/x/xxxx:
uint64_t GetDateID(uint8_t day, MONTH month, YEAR year); 
 
// It finds the WEEKDAY of 1/1/0000 date (this is a leap year 0000) . This is used to generate
// The WeekdayOrder arr[7] for the given reference:
void GetWeekDayOrder(CalenderFingerprint *Reference, WEEKDAY *WeekdayOrder );

//function to display the WeekdayOrder array:
// Note: assumes Weekdayorder is already set.
void DisplayWeekdayOrder(WEEKDAY *WeekdayOrder);

// Function to get the string pointer name of a MONTH month:
char *MonthToStr(MONTH month, BOOL ShortName);

// Do I actually need the below function ? Does it come in use?
// Function to get string pointer name of a WEEKDAY weekday:
char * WeekdayToStr(WEEKDAY weekday, BOOL ShortName);

// Function to actually print the Data (ie. display calender):
// give -1 if you don't wanna print day, month, or year (don't give zero because 0000 year is possible)
// But giving 0 for day or month doesn't work.
void DisplayMonth(char *MonthData[MONTH_DATA_SIZE], MONTH month, YEAR year);


#endif
