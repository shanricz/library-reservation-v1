#include "headers/main.h"
#include "headers/database.h"
#include "headers/reservation.h"

int main() {
    sqlite3 *db;
    int rc = sqlite3_open("test.db", &db);
    if (rc) {
        printf("Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    } else {
        printf("Opened database successfully\n");
    }
    sqlite3_close(db);
    return 0;
}