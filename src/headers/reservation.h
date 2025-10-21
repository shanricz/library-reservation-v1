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
#define MAX_TIME_LENGTH 10
#define MAX_RESERVATION_ID_LENGTH 50
#define MAX_STUD_ID_LENGTH 11
#define MAX_ROOM_LENGTH 50
#define NUM_ROOMS 4

static const char* CONSULTATION_ROOMS[NUM_ROOMS] = {
     "Room A", "Room B", "Room C", "Room D"
};

//Reservation Struct
typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    char student_num[MAX_STUD_ID_LENGTH]; //Stud num == 10 chars + null terminator
    char date[MAX_DATE_LENGTH];
    char start_time[MAX_TIME_LENGTH];
    char end_time[MAX_TIME_LENGTH];
    char reservation_id[MAX_RESERVATION_ID_LENGTH];
    char consultation_room[MAX_ROOM_LENGTH];
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