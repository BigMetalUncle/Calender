// #include <stdio.h>
#include "common.h"
#include "calender-related-functions.h"
#include "processing-functions.h"

int main(int argc, char *argv[])
{
    // I. VARIABLES:
    // We print a month of a year at a time. 
    // The following info is needed for this.
    uint8_t day1 = 0;
    uint8_t day2 = 0;
    MONTH month1 = NULL_MONTH; //ie. 0
    MONTH month2 = NULL_MONTH;
    YEAR year1 = 0; 
    YEAR year2 = 0;
    // We can't set a specific value for year because 0 is also a valid year.
    // Hence, we use the following variables:
    BOOL year1Supplied = FALSE;
    BOOL year2Supplied = FALSE;
    
    char *MonthData[MONTH_DATA_SIZE];
    WEEKDAY WeekdayOrder[7] = {0};
    
    // The following acts a reference to calculate the WeekdayOrder :
    // It means: 26th September of 2026 was Saturday. Now use that info 
    // to prcossing and finaly show the right calender.
    CalenderFingerprint Reference = { 1, THU, JAN, 2026 } ;
    
    // NOTE:  Weekday Order would not change for this timeline. 
    //        So you need not worry generating one each program execution.
    //        It would be optimized if we just hardcode a weekorder.
    //        However you can use this in any way you want. BUT If you are from another 
    //        or timeline where 1 Jan 2026 is not a Thu, Then you should better 
    //        universe leave it as {0} and give a reference point from your timeline to generate the 
    //        appropriate WeekdayOrder.   XD  . Have a nice day !
    
    /* 
    WeekdayOrder[0] = THU;
    WeekdayOrder[1] = FRI;
    WeekdayOrder[2] = SAT;
    WeekdayOrder[3] = SUN;
    WeekdayOrder[4] = MON;
    WeekdayOrder[5] = TUE;
    WeekdayOrder[6] = WED;
    */
    
    // Find WeekdayOrder in the beginning of the program 
    // itself if not hardcoded or there's a change:
    if (!WeekdayOrder[0])
        GetWeekDayOrder(&Reference, WeekdayOrder);
    
    // Process Arguemnts and execute accordingly:
    FUNC_RET ret = ParseArgv(argc, argv, &day1, &day2, &month1, &month2, &year1, &year2, 
                             &year1Supplied, &year2Supplied);
    YEAR big, small;
    
    if (ret == ERROR)
        return 1;
    else if (ret == QUIT)
        return 0;
    else if (ret == DISPLAY_WEEKDAY_ORDER)
    {
        DisplayWeekdayOrder(WeekdayOrder);
        return 0;
    }
    else if (ret == SUCCESS)
    {
        // TODO: More to go up here. Start with checks of day followed by months and then years.
        //1.
        if (day1)
        {
            uint64_t dID1, dID2;
            // 1.1 Display the No. Of days between two dates:
            if (day2)
            {
                dID1 = GetDateID(day1, month1, year1);
                dID2 = GetDateID(day2, month2, year2);
                printf(BCYN"%lu"RST" Days Between "BBLU"%hhu-%u-%hu"RST" and "BBLU"%hhu-%u-%hu"RST
                        " excluding the former.\n", (dID1 > dID2) ? (dID1 - dID2) : (dID2 - dID1),
                        day1, month1, year1, day2, month2, year2);
            }
            // 1.2 Display info of the date like DateID, weekday, etc;
            else 
            {
                dID1 = GetDateID(day1, month1, year1);
                char *weekday = WeekdayToStr(WeekdayOrder[dID1 %7], FALSE);
                printf("\nDate: "BBLU"%hhu %s %hu"RST"\nWeekday: "BCYN"%s"RST"\nDateID: "BCYN"%lu\n"RST,
                       day1, MonthToStr(month1, TRUE), year1, weekday, dID1);
            }
        }
        // 2 , 3
        else if (year1Supplied)
        {   
            // 2.
            if (year2Supplied)
            {
                if (year1 > year2) { big = year1; small = year2; }
                else {big = year2; small = year1; }
                    
                // 2.1 Display all months between month1 of year1 to month2 of year2
                if (month2)
                {   
                    MONTH start, end;
                    for (YEAR year = small; year <= big; year++)
                    {   
                        start = JAN;
                        end = DEC;
                        if ( year == small ) {
                            if (small == year1)
                                start = month1;
                            else 
                                start = month2;
                        }
                        else if (year == big) {
                            if (big == year1)
                                end = month1;
                            else 
                                end = month2;
                        }
                        
                        for (MONTH month = start; month <= end; month++)
                        {
                            FillMonthData(MonthData, &Reference, month, year, WeekdayOrder);
                            DisplayMonth(MonthData, month, year);
                        }
                    }
                }
                // 2.2 Display same month of a range of years:
                else if (month1)
                {                
                    for (YEAR year = small; year <= big; year++)
                    {
                        FillMonthData(MonthData, &Reference, month1, year, WeekdayOrder);
                        DisplayMonth(MonthData, month1, year);
                    }    
                }
                // 2.3 Display Jan to Dec of years between year1 and year2
                else 
                {   
                    for (YEAR year = small; year <= big; year++)
                    {
                        for (MONTH month = JAN; month <= DEC; month++)
                        {
                            FillMonthData(MonthData, &Reference, month, year, WeekdayOrder);
                            DisplayMonth(MonthData, month, year);
                        }
                    }
                }
            }
            // 3.1 Most common operation : Print a given month of a given year: 
            else if (month1)
            {
                FillMonthData(MonthData, &Reference, month1, year1, WeekdayOrder);
                DisplayMonth(MonthData, month1, year1);
            }
            // 3.2 Display all months of a year:
            else 
            {
                for (MONTH month = JAN; month <= DEC; month++)
                {    
                    FillMonthData(MonthData, &Reference, month, year1, WeekdayOrder);
                    DisplayMonth(MonthData, month, year1);
                }
            }
        }
        // 4. Control flow reaching here is unusual. The reason is  
        //    explaiend in the beginning of this if-else.
        else if(!year1Supplied)
        {
            puts(BRED"ERROR:"MAG" No year supplied. The minimum requirements of this version \n"
                 "       of the calender program is atleast an year. See help '-h' for more info."RST);
                 return 1;
        }   
        else
        {
            puts(BRED"ASSERT_ERROR:"MAG" Control flow came in unusual place. If you encountered this,\n"
                 "              there is a bug in the program and consider reporting it to the author."RST);
            return -1;
        }
    }
    
    
    return 0; 
}

