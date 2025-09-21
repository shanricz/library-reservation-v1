#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

//DB Functions Declarations
int initialize_database();
int create_tables();
int close_database();
int check_time_conflict(const char* date, const char* start_time, const char* end_time);



#endif // DATABASE_H