#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sqlite3.h>

//Const Declarations
#define MAX_NAME_LENGTH 100
#define MAX_EMAIL_LENGTH 100 //this is for notifications for the student
#define MAX_DATE_LENTGH 11
#define MAX_TIME_LENGTH 8
#define DB_PATH "data/library_reservations.db"

//Func Declarations
int main();
void main_menu();
void view_daily_schedule();
void make_reservation();
void cancel_reservation();
void search_reservations();
void clear_screen();
void pause_screen();

#endif // MAIN_H