#include "common.h"
#include "calender-related-functions.h"

// No use of "00" here, but helps in indexing. 
char *str_nums[32] = { "00", "01", "02", "03", "04", "05", "06", "07", 
                       "08", "09", "10", "11", "12", "13", "14", "15", 
                       "16", "17", "18", "19", "20", "21", "22", "23",
                       "24", "25", "26", "27", "28", "29", "30", "31" };
uint8_t NoDaysInMonths[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DisplayMonth(char *MonthData[MONTH_DATA_SIZE], MONTH month, YEAR year)
{
    if (month >=1 && year >= 0)
        printf(BYEL"\n%s %04hu:\n"RST, MonthToStr(month, FALSE), year);
    else if (month >= 1)
        printf(BYEL"\n%s:\n"RST, MonthToStr(month, FALSE));
    else if (year >= 0)
        printf(BYEL"\n%04hu:\n"RST, year);
    
    puts(BLU"==========================================="RST);
    puts(BLU"|"BGRN" SUN "BLU"|"BGRN" MON "BLU"|"BGRN" TUE "BLU"|"BGRN" WED "
         BLU"|"BGRN" THU "BLU"|"BGRN" FRI "BLU"|"BGRN" SAT "BLU"|"RST);
    puts(BLU"|=========================================|"RST);
    for (int i = 0; i < 5; i++ )
    {
        printf(BLU"|"CYN"  %s "BLU"|"CYN"  %s "BLU"|"CYN"  %s "BLU"|"CYN"  %s "
               BLU"|"CYN"  %s "BLU"|"CYN"  %s "BLU"|"CYN"  %s "BLU"|\n"RST, MonthData[7*i + 0], 
                MonthData[7*i + 1], MonthData[7*i + 2], MonthData[7*i + 3], 
                MonthData[7*i + 4], MonthData[7*i + 5], MonthData[7*i + 6] );
        // Yeah. We could have used two loops for a cleaner code. But I am like this. 
        // Sorry if this slop hurts you.
        if ( i == 4)
            puts(BLU"==========================================="RST);
        else
            puts(BLU"|-----------------------------------------|"RST);
    }
    
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DisplayWeekdayOrder(WEEKDAY *WeekdayOrder)
{
    char *sep_or_end;
    fputs(BLU"Weekday Order:\n"BBLU"[", stdout);
    for (int i = 0; i < 7; i ++)
    {    
        sep_or_end = (i == 6) ? " ]\n"RST : ",";
        printf(BCYN" %s"BBLU"%s", WeekdayToStr(WeekdayOrder[i], TRUE), sep_or_end);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char * WeekdayToStr(WEEKDAY weekday, BOOL ShortName)
{
    switch(weekday)
    {
        case SUN:
            return (ShortName) ? "Sun" : "Sunday";
        case MON:
            return (ShortName) ? "Mon" : "Monday";
        case TUE:
            return (ShortName) ? "Tue" : "Tuesday";
        case WED:
            return (ShortName) ? "Wed" : "Wednesday";
        case THU:
            return (ShortName) ? "Thu" : "Thursday";
        case FRI:
            return (ShortName) ? "Fri" : "Friday";
        case SAT:
            return (ShortName) ? "Sat" : "Saturday";
        default:
            return NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char *MonthToStr(MONTH month, BOOL ShortName)
{
    switch (month) 
    {
        case JAN:
            return (ShortName) ? "Jan" : "January";
        case FEB:
            return (ShortName) ? "Feb" : "February";
        case MAR:
            return (ShortName) ? "Mar" : "March";
        case APR:
            return (ShortName) ? "Apr" : "April";
        case MAY:
            return "May";
        case JUN:
            return (ShortName) ? "Jun" : "June";
        case JUL:
            return (ShortName) ? "Jul" : "July";
        case AUG:
            return (ShortName) ? "Aug" : "August";
        case SEP:
            return (ShortName) ? "Sep" : "September";
        case OCT:
            return (ShortName) ? "Oct" : "October";
        case NOV:
            return (ShortName) ? "Nov" : "November";
        case DEC:
            return (ShortName) ? "Dec" : "December";
        default:
            return NULL;   
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FillMonthData(char *MonthData[MONTH_DATA_SIZE], CalenderFingerprint *Reference, MONTH month,
                   YEAR year, WEEKDAY *WeekdayOrder)
{
    // Month data will have four rows of 7 columns representing 7 weekdays. 
    // Data should be inserted accordingly. 
    // Check carefully to understad how data must be filled. 
    uint8_t  NoMonthDays = 0;
    uint16_t NoYearDays = (year % 4 == 0) ? 366 : 365; // if 366, then it's a LEAP YEAR. 
    
    // No need to Calc WeekDayOrder because we assume it's already done in main().

    /*
    // // TEST: We are testing by trying to predict the date 
    // //      of 30 Sept 2026 which is actually a Tuesday.
    // const uint64_t temp1 = GetDateID(31, MAR, 2030);
    // WEEKDAY temp2  = WeekdayOrder[temp1 % 7];
    // printf("DateID of 26 JAN 2026 is %lu\n", temp1);
    // printf("Is 26 Sep 2026 a %u ?\n", temp2);
    // // WORKS!
    */ 
    
    // Fill MonthData:
    // Find how many days in the month:
    if (month == FEB && NoYearDays == 366)
        NoMonthDays = 29; // We know for leap years, Feb has 29 days.
    else
        NoMonthDays = NoDaysInMonths[month - 1];
    
    // Find Weekday for 1st day of the month. Required for filling Data:
    const uint64_t DateOneID = GetDateID(1, month, year);
    WEEKDAY weekday = WeekdayOrder[DateOneID % 7]; 
    
    // Actually filling MonthData:
    for (int i = 0, j = 1; i < MONTH_DATA_SIZE; i++)
    {
        // Note: WEEKDAY Enums start from 1. So use (weekday-1) below.
        if (i >= (weekday - 1) && j <= NoMonthDays)
        {
            // Think of str_nums[num] as a 'mini-function' converting the num to a string .
            // You might feel this array thing shit, but i did this for readability.
            MonthData[i] = str_nums[j]; // Note: str_nums has a non-used "00" at the beginning 
                                        // to work with j. If you don't have that, you should do j-1
                                        // or declare as int j = 0 instead of j = 1
            j++;
        }
        else
            MonthData[i] = "  ";
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint64_t GetDateID(uint8_t day, MONTH month, YEAR year)
{
    uint16_t NoYearDays = (year % 4 == 0) ? 366 : 365; // if 366, then it's a LEAP YEAR. 
    int16_t PrevYear = year - 1; // This should stay int because the code would break for x/x/0000
    MONTH PrevMonth = month - 1; // Can become 0 which is not in the MONTH enum (expected) .
    
    // We have to calculate days passed till before previous month:
    uint32_t NoLeapYearsPassed = (uint32_t) ((float) PrevYear / 4 + 1); // +1 to count for year 0000
                                 // NoLeapYearsPassed also count how many extra days came.
                                 // Conversion to float is essential to work for dates x/x/0000
    
    // We need the following to add to  DateID
    uint16_t NoDaysPassedPrevMonths = 0;
    for (int i = 0; i < PrevMonth; i++)
    {
        // Note: Days of PrevMonth is also added in the following:
        NoDaysPassedPrevMonths += NoDaysInMonths[i];
    }
    // Note: If this was a Leap year and curr month is after February,
    //       We have to count for 29 Days instead of 28 for february.
    if (month > FEB && NoYearDays == 366)
        NoDaysPassedPrevMonths += 1; // We already added 28 days in prev loop.
                                     // February.
    
    // Days passed since 1/1/0000 including the former .
    uint64_t DateID = PrevYear * 365 + 365  + NoLeapYearsPassed + 
                      NoDaysPassedPrevMonths + day;
    
    return DateID;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Info on what functions do is given in header file. 
void GetWeekDayOrder(CalenderFingerprint *Reference, WEEKDAY *WeekdayOrder )
{
    // I. Calculate The unique DateID from Reference first. 
    
    uint64_t DateID = GetDateID(Reference->day, Reference->month, Reference->year);
    
    // Fill in WeekdayOrder:
    uint8_t WOi = DateID % 7;        // WeekOrder Index for the below day.
    WEEKDAY WOd = Reference->weekday;
    
    for (int i = 0; i < 7; i++)
    {
        WeekdayOrder[WOi] = WOd;
        WOi++ ;
        WOd++ ;
        if (WOi > 6)
            WOi = 0;
        if (WOd > SAT)
            WOd = SUN;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
