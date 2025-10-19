#include "headers/database.h"
#include "headers/reservation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//database connection
extern sqlite3* db;

//Const
#define MAX_NAME_LENGTH 100
#define MAX_DATE_LENGTH 11
#define MAX_TIME_LENGTH 10
#define DATABASE_PATH "data/library_reservations.db"

int close_database() {
    if (db) {
        sqlite3_close(db);
        db = NULL;
    }
    return 0; 
}

int initialize_database() {
    // Open database connection
    int rc;

    rc = sqlite3_open(DATABASE_PATH, &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // Create tables if they don't exist
    if (create_tables() != 0) {
        fprintf(stderr, "Failed to create tables.\n");
        sqlite3_close(db);
        return 1;
    }
    return 0;
}

int create_tables() {
    char* sql = "CREATE TABLE IF NOT EXISTS reservations ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "student_name TEXT NOT NULL,"
                "student_num TEXT NOT NULL,"
                "date TEXT NOT NULL,"
                "start_time TEXT NOT NULL,"
                "end_time TEXT NOT NULL,"
                "reservation_id TEXT NOT NULL UNIQUE,"
                //"email TEXT NOT NULL"
                "created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
                ");";
    char* err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 1;
    }
    return 0;
}


int check_time_conflict(const char* date, const char* start_time, const char* end_time) {
char sql[500];
char* err_msg = 0;
int conflict = 0;

// check for time conflicts
char start_24[MAX_TIME_LENGTH], end_24[MAX_TIME_LENGTH];
format_time_24hour((char*)start_time, start_24);
format_time_24hour((char*)end_time, end_24);

sprintf(sql, "SELECT COUNT(*) FROM reservations WHERE date = '%s' AND ("
             "(start_time < '%s' AND end_time > '%s') OR "
             "(start_time >= '%s' AND start_time < '%s') OR "
             "(end_time > '%s' AND end_time <= '%s')"
             ");", date, end_24, start_24, start_24, end_24, start_24, end_24);

    int rc = sqlite3_exec(db, sql, callback_count_reservations, &conflict, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 1;
    }

    return conflict > 0 ? 1 : 0;
}

int insert_reservation(const char* name, const char* student_num, const char* date, const char* start_time, const char* end_time, const char* reservation_id) {
    char* err_msg = 0;
    char sql[500];

    // convert to 24-hour format
    char start_24[MAX_TIME_LENGTH], end_24[MAX_TIME_LENGTH];
    format_time_24hour((char*)start_time, start_24);
    format_time_24hour((char*)end_time, end_24);

    // insert
    sprintf(sql, "INSERT INTO reservations (student_name, student_num, date, start_time, end_time, reservation_id) "
                 "VALUES ('%s', '%s', '%s', '%s', '%s', '%s');",
                 name, student_num, date, start_24, end_24, reservation_id);

    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 1;
    }

    return 0;
}

int delete_reservation(const char* reservation_id) {
    char sql[200];
    char* err_msg = 0;

    sprintf(sql, "DELETE FROM reservations WHERE reservation_id = '%s';", reservation_id);

    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 1;
    }

    return 0;
}

int get_reservations_by_date(const char* date) {
    char sql[200];
    char* err_msg = 0;

    sprintf(sql, "SELECT * FROM reservations WHERE date = '%s' ORDER BY start_time;", date);

    int rc = sqlite3_exec(db, sql, callback_print_reservations, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 1;
    }

    return 0;
}

int get_reservations_by_student_num(const char* student_num) {
    char sql[200];
    char* err_msg = 0;

    sprintf(sql, "SELECT * FROM reservations WHERE student_num = '%s' ORDER BY date, start_time;", student_num);

    int rc = sqlite3_exec(db, sql, callback_print_reservations, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 1;
    }

    return 0;
}

int get_reservations_by_id(const char* reservation_id) {
    char sql[200];
    char* err_msg = 0;

    sprintf(sql, "SELECT * FROM reservations WHERE reservation_id = '%s';", reservation_id);

    int rc = sqlite3_exec(db, sql, callback_print_reservations, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 1;
    }

    return 0;
}

int get_all_reservations() {
    char* sql = "SELECT * FROM reservations ORDER BY date, start_time;";
    char* err_msg = 0;

    int rc = sqlite3_exec(db, sql, callback_print_reservations, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 1;
    }

    return 0;
}

int get_reservations_by_name(const char* name) {
    char sql[300];
    char* err_msg = 0;

    sprintf(sql, "SELECT * FROM reservations WHERE student_name LIKE '%%%s%%' ORDER BY date, start_time;", name);

    int rc = sqlite3_exec(db, sql, callback_print_reservations, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 1;
    }

    return 0;
}

// Callback implementations
int callback_print_reservations(void* data, int argc, char** argv, char** azColName) {
    (void)data;
    (void)azColName;
    
    // Format: reservation_id | start_time | end_time | student_name
    // Column order: id(0), student_name(1), student_num(2), date(3), start_time(4), end_time(5), reservation_id(6)
    if (argc >= 7) {
        const char* reservation_id = argv[6] ? argv[6] : "NULL";
        const char* student_name = argv[1] ? argv[1] : "NULL";
        const char* start_time = argv[4] ? argv[4] : "NULL";
        const char* end_time = argv[5] ? argv[5] : "NULL";
        
        // Convert 24-hour times back to 12-hour format for display
        char start_12[MAX_TIME_LENGTH], end_12[MAX_TIME_LENGTH];
        format_time_12hour((char*)start_time, start_12);
        format_time_12hour((char*)end_time, end_12);
        
        printf("| %-20s | %-17s | %-17s | %-25s |\n", 
               reservation_id, start_12, end_12, student_name);
    }
    return 0;
}

int callback_count_reservations(void* data, int argc, char** argv, char** azColName) {
    (void)azColName;
    if (argc > 0 && argv && argv[0]) {
        int* out = (int*)data;
        *out = atoi(argv[0]);
    }
    return 0;
}

/*int update_reservation(const char* reservation_id, const char* name, const char* date, const char* start_time, const char* end_time, const char* student_num) {
    char sql[500];
    char* err_msg = 0;

    // Convert times to 24-hour format for storage
    char start_24[MAX_TIME_LENGTH], end_24[MAX_TIME_LENGTH];
    format_time_24hour((char*)start_time, start_24);
    format_time_24hour((char*)end_time, end_24);

    // SQL update statement
    sprintf(sql, "UPDATE reservations SET student_name = '%s', student_num = '%s', date = '%s', start_time = '%s', end_time = '%s' "
                 "WHERE id = '%s';",
                 name, student_num, date, start_24, end_24, reservation_id);

    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 1;
    }

    return 0;
}*/

/* DONE: initialize_database(): Opens the SQLite database and creates tables if they don’t exist.
DONE: close_database(): Closes the database connection.
DONE: create_tables(): Creates the reservations table if it doesn’t exist.
DONE: insert_reservation(student_name, date, start_time, end_time): Adds a new reservation to the database.
DONE: delete_reservation(reservation_id): Removes a reservation by its ID.
DONE: get_reservations_by_date(date): Retrieves and displays all reservations for a given date.
DONE: search_reservations_by_name(name): Searches and displays reservations by student name.
DONE: search_reservations_by_id(id): Searches and displays a reservation by its ID.
TODO: get_all_reservations(): Retrieves and displays all reservations in the database.
TODO: update_reservation(reservation_id, ...): Updates details of an existing reservation.
DONE: check_time_conflict(date, start_time, end_time): Checks if a new reservation conflicts with existing ones.
DONE: callback_print_reservations(): Callback to print reservation details from SQL queries.
DONE: callback_count_reservations(): Callback to count reservations from SQL queries.
*/