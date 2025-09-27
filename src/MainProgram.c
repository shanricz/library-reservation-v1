#include "headers/main.h"
#include "headers/database.h"
#include "headers/reservation.h"

sqlite3 *db;

int main() {

    printf("--->LIBRARY CONSULTATION ROOM RESERVATION SYSTEM <---\n");
    printf("Initializing system...\n\n");

    //Initialize db
    if (init_database() != 0) {
        printf("Error: Failed to initialize database. Exiting...\n");
        pause_screen();
        return 1;
    }
    return 0;
}

/* TODO: main(): Entry point. Initializes the database, shows the welcome message, and starts the main menu loop.
TODO: main_menu(): Displays the main menu and handles user choices (view schedule, make/cancel/search reservations, exit).
TODO: view_daily_schedule(): Prompts for a date, validates it, and displays all reservations for that day.
TODO: make_reservation(): Collects reservation details, validates input, checks for conflicts, and inserts a new reservation.
TODO: cancel_reservation(): Lets the user cancel a reservation by ID or student name.
TODO: search_reservations(): Allows searching for reservations by student name or reservation ID.
TODO: clear_screen(): Clears the console screen (platform-dependent).
TODO: pause_screen(): Pauses the screen until the user presses Enter.
*/