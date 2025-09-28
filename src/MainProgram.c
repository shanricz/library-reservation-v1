#include "headers/main.h"
#include "headers/database.h"
#include "headers/reservation.h"

//db connection (global)
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

    printf("Welcome to the Library Consultation Room Reservation System!\n");

    // Main program loop
    main_menu();

    // Close db connection
    close_database();
    printf("\nThank you for using the Library Reservation System!\n");
    
    return 0;
}

void main_menu(){
    int choice;
     do {
        clear_screen();
        printf("Main Menu:\n");
        printf("1. View Daily Schedule\n");
        printf("2. Make a Reservation\n");
        printf("3. Cancel a Reservation\n");
        printf("4. Search Reservations\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        if(scanf("%d", &choice) != 1){
            printf("Invalid input. Please enter a number between 1 and 5.\n");
            pause_screen();
            continue;
        }

        switch (choice) {
            case 1:
                view_daily_schedule();
                break;
            case 2:
                make_reservation();
                break;
            case 3:
                cancel_reservation();
                break;
            case 4:
                search_reservations();
                break;
            case 5:
                printf("Exiting the program...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                pause_screen();
        }
     } while (choice != 5);
}

/* 
TODO: main_menu(): Displays the main menu and handles user choices (view schedule, make/cancel/search reservations, exit).
TODO: view_daily_schedule(): Prompts for a date, validates it, and displays all reservations for that day.
TODO: make_reservation(): Collects reservation details, validates input, checks for conflicts, and inserts a new reservation.
TODO: cancel_reservation(): Lets the user cancel a reservation by ID or student name.
TODO: search_reservations(): Allows searching for reservations by student name or reservation ID.
TODO: clear_screen(): Clears the console screen (platform-dependent).
TODO: pause_screen(): Pauses the screen until the user presses Enter.
*/