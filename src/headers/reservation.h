 #ifndef RESERVATION_H
 #define RESERVATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Const
#define MAX_NAME_LENGTH 100
#define MAX_EMAIL_LENGTH 100
#define MAX_DATE_LENGTH 11
#define MAX_TIME_LENGTH 8
#define MAX_RESERVATION_ID_LENGTH 10

//Reservation Struct
typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char date[MAX_DATE_LENGTH];
    char start_time[MAX_TIME_LENGTH];
    char end_time[MAX_TIME_LENGTH];
    char student_num[11]; //Stud num == 10 chars + null terminator
} Reservation;

//Function Declarations
int validate_date(const char* date);
int validate_time(const char* time);
int validate_time_range(const char* start_time, const char* end_time);
int is_time_conflict(const char* start1, const char* end1, const char* start2, const char* end2);
void format_time_12hour(char* time_24, char* time_12);
void format_time_24hour(char* time_12, char* time_24);
int compare_times(const char* time1, const char* time2);

 #endif // RESERVATION_H