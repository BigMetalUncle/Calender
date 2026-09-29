#ifndef PROCESSING_FUNCTIONS_H
#define PROCESSING_FUNCTIONS_H

#include "common.h"
#include <stdint.h>

typedef enum {ERROR, SUCCESS, QUIT, DAY_MONTH_YEAR_ONLY, MONTH_YEAR_ONLY, YEAR_ONLY,
              DISPLAY_WEEKDAY_ORDER} FUNC_RET;


FUNC_RET ParseArgv(int argc, char *argv[], uint8_t *day1, uint8_t *day2, MONTH *month1, 
                   MONTH *month2, YEAR *year1, YEAR *year2, 
                   BOOL *year1Supplied, BOOL *year2Supplied);
// Backup: 
/*
FUNC_RET ProcessFlag(int i, char *argv[]);
*/

FUNC_RET StrToDate(char *str, uint8_t *day, MONTH *month, YEAR *year, BOOL LogErrors);

MONTH StrToMonth(char *strMonth);

void strtolower(char *s);

void user_help();

#endif
