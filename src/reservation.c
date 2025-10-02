#include "headers/reservation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>


int validate_date(const char* date) {
    // Expected format: MM/DD/YYYY
    if (strlen(date) != 10)
    return 0; //check length
    if (date[2] != '/' || date[5] != '/')
    return 0; //check slashes(seperators)

    if (!isdigit(date[0]) || !isdigit(date[1]) ||
        !isdigit(date[3]) || !isdigit(date[4]) ||
        !isdigit(date[6]) || !isdigit(date[7]) ||
        !isdigit(date[8]) || !isdigit(date[9]))
    return 0; //check digits

    //extract month, day, year
    int month = (date[0] - '0') * 10 + (date[1] - '0');
    int day = (date[3] - '0') * 10 + (date[4] - '0');
    int year = (date[6] - '0') * 1000 + (date[7] - '0')
    * 100 + (date[8] - '0') * 10 + (date[9] - '0');

    //get current year
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    int current_year = t->tm_year + 1900;

    //basic validations
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0; // Simplified check, not accounting for month lengths or leap years
    if (year < current_year || year > current_year) return 0;
    
    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))){
        days_in_month[1] = 29; // Leap year
    }

    if (day > days_in_month[month - 1]) return 0; // Invalid day for month

    return 1; //valid date
}

int validate_time(const char* time) {
    if (time == NULL) return 0;  // Null check
    size_t len = strlen(time);
    if (len < 7 || len > 8) return 0;  // Only 7 or 8 chars allowed
    // Determine positions based on length
    int colon_pos = (len == 8) ? 2 : 1;  // Colon after 2 digits (8-char) or 1 digit (7-char)
    int space_pos = (len == 8) ? 5 : 4;  // Space before AM/PM
    int ampm_start = space_pos + 1;      // Start of AM/PM (6 or 5)
    int ampm_end = space_pos + 2;        // End of AM/PM (7 or 6)
    // Check separators: colon and space
    if (time[colon_pos] != ':') return 0;
    if (time[space_pos] != ' ') return 0;
    // Check AM/PM (case-insensitive)
    char ampm_first = toupper(time[ampm_start]);
    char ampm_second = toupper(time[ampm_end]);
    if ((ampm_first != 'A' && ampm_first != 'P') || ampm_second != 'M') {
        return 0;
    }
    // Check digits: hour (before colon), minutes (after colon, before space)
    // Hour digits: positions 0 to colon_pos-1
    for (int i = 0; i < colon_pos; i++) {
        if (!isdigit(time[i])) return 0;
    }
    // Minute digits: always 2 positions after colon
    int min_start = colon_pos + 1;
    if (!isdigit(time[min_start]) || !isdigit(time[min_start + 1])) return 0;
    // Extract and parse hour (1 or 2 digits)
    int hour;
    if (len == 7) {
        // Single digit: e.g., time[0] = '1'
        hour = time[0] - '0';
    } else {
        // Two digits: e.g., time[0]-'0'*10 + time[1]-'0'
        hour = (time[0] - '0') * 10 + (time[1] - '0');
    }

    return 1; //valid time
}

int validate_time_range(const char* start_time, const char* end_time) {
    // Convert times to 24-hour format for comparison
    char start_24[MAX_TIME_LENGTH];
    char end_24[MAX_TIME_LENGTH];
    format_time_24hour((char*)start_time, start_24);
    format_time_24hour((char*)end_time, end_24);

    // Compare times
    if (compare_times(start_24, end_24) >= 0) {
        return 0; // End time is not after start time
    }
    return 1; // Valid time range
}

int is_time_conflict(const char* start1, const char* end1, const char* start2, const char* end2) {
    // Convert times to 24-hour format for comparison
    char start1_24[MAX_TIME_LENGTH];
    char end1_24[MAX_TIME_LENGTH];
    char start2_24[MAX_TIME_LENGTH];
    char end2_24[MAX_TIME_LENGTH];
    format_time_24hour((char*)start1, start1_24);
    format_time_24hour((char*)end1, end1_24);
    format_time_24hour((char*)start2, start2_24);
    format_time_24hour((char*)end2, end2_24);

    // Check for overlap
    if (compare_times(start1_24, end2_24) < 0 && compare_times(start2_24, end1_24) < 0) {
        return 1; // Conflict exists
    }
    return 0; // No conflict
}

void format_time_12hour(char* time_24, char* time_12) {
    // Convert 24-hour format (HH:MM) to 12-hour format (HH:MM AM/PM)
     int hour, minute;
    sscanf(time_24, "%d:%d", &hour, &minute);
    
    int hour_12 = hour;
    char* period = "AM";
    
    if (hour == 0) {
        hour_12 = 12;
    } else if (hour == 12) {
        period = "PM";
    } else if (hour > 12) {
        hour_12 = hour - 12;
        period = "PM";
    }
    
    sprintf(time_12, "%d:%02d %s", hour_12, minute, period);
}

void format_time_24hour(char* time_12, char* time_24) {
    // Convert 12-hour format (HH:MM AM/PM) to 24-hour format (HH:MM)
    int hour, minute;
    char period[3];
    sscanf(time_12, "%d:%d %2s", &hour, &minute, period);
    
    if (strcmp(period, "PM") == 0 && hour != 12) {
        hour += 12;
    } else if (strcmp(period, "AM") == 0 && hour == 12) {
        hour = 0;
    }
    
    sprintf(time_24, "%02d:%02d", hour, minute);
}

int compare_times(const char* time1, const char* time2) {
    // Compare two times in HH:MM format
    int hour1, minute1, hour2, minute2;
    sscanf(time1, "%d:%d", &hour1, &minute1);
    sscanf(time2, "%d:%d", &hour2, &minute2);
    
    if (hour1 != hour2) {
        return hour1 - hour2;
    }
    return minute1 - minute2;
}
/* validation and time-related logic.
TODO: validate_date(date): Checks if the date is in the correct format (MM/DD/YYYY).
TODO: validate_time(time): Checks if the time is in the correct format (HH:MM AM/PM).
TODO: validate_time_range(start_time, end_time): Ensures the end time is after the start time.
TODO: check_time_conflict(date, start_time, end_time): Checks if the requested time slot overlaps with existing reservations.
TODO: format_time_24hour(time): Converts time to 24-hour format (if needed).
TODO: format_time_12hour(time): Converts time to 12-hour format (if needed).
TODO: compare_times(start, end): Compares two time strings.
*/