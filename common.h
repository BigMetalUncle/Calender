
#ifndef COMMON_H
#define COMMON_H 

#include <stdint.h>
#include <stdio.h> 
#include <ctype.h>
#include "color-codes.h"

#define MONTH_DATA_SIZE 35

// Types:
typedef enum { JAN=1, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC, INVALID_MONTH, NULL_MONTH=0 } MONTH;
typedef enum { SUN=1, MON, TUE, WED, THU, FRI, SAT } WEEKDAY ;
typedef enum { FALSE=0, TRUE=1 } BOOL;
typedef uint16_t YEAR;

typedef struct
{   
    uint8_t day;
    WEEKDAY weekday;
    MONTH month;
    YEAR year; 
} CalenderFingerprint;





#endif 
