#include "headers/main.h"
#include "headers/database.h"
#include "headers/reservation.h"

//db connection (global)
sqlite3 *db;

int main() {

    printf("--->LIBRARY CONSULTATION ROOM RESERVATION SYSTEM <---\n");
    printf("Initializing system...\n\n");

    //Initialize db
    if (initialize_database() != 0) {
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

void view_daily_schedule() {
    char date[MAX_DATE_LENTGH];

    clear_screen();
    printf("VIEW DAILY SCHEDULE\n");
    printf("-------------------\n");
    printf("Enter date (MM/DD/YYYY): ");

    if(scanf("%s", date) != 1){
        printf("Error reading date input.\n");
        pause_screen();
        return;
    };

    // Validate date
    if (!validate_date(date)) {
        printf("Invalid date format.\n");
        pause_screen();
        return;
    }

    //Show sched
    printf("\nSchedule for %s\n", date);
    printf("+-----+-------------------+-------------------+---------------------------+\n");
    printf("| %-3s | %-17s | %-17s | %-25s |\n", "ID", "Start Time", "End Time", "Student Name");
    printf("+-----+-------------------+-------------------+---------------------------+\n");

    // Get reservations for the date
    int result = get_reservations_by_date(date);

    if (result != 0) {
        printf("Error retrieving schedule data.\n");
    }

    printf("+-----+-------------------+-------------------+---------------------------+\n");

}

void make_reservation(){
    char student_name[MAX_NAME_LENGTH];
    char student_num[MAX_STUD_ID_LENGTH];
    char date[MAX_DATE_LENTGH];
    char start_time[MAX_TIME_LENGTH];
    char end_time[MAX_TIME_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char reservation_id[MAX_RESERVATION_ID_LENGTH];

    clear_screen();
    printf("MAKE A RESERVATION\n");

    //get stud name
    printf("Enter student name: ");
    if(scanf("%s", student_name) != 1){
        printf("Error reading input.\n");
        pause_screen();
        return;
    };

    // Remove newline character
    student_name[strcspn(student_name, "\n")] = 0;

    //get stud num [limited character]
    printf("Enter student number: ");
    if(scanf("%s"), student_num){
        printf("Error: Wrong student number format.");
        pause_screen();
        return;
    }

    //get date
    printf("Enter desired date [MM/DD/YYYY]: ");
    if(scanf("%s", date) != 1){
        printf("Error: Wrong date format.");
        pause_screen();
        return 1;
    }

    //time start
    printf("Enter start time (HH:MM AM/PM): ");
    if (scanf("%s", start_time) != 1){
        printf("Error reading start time.\n");
        pause_screen();
        return;
    }

    //end time
    printf("Enter end time (HH:MM AM/PM): ");
    if (scanf("%s", start_time) != 1){
        printf("Error reading end time.\n");
        pause_screen();
        return;
    }

    //validate date
    if (!validate_date(date)) {
        printf("Invalid date format. Please use MM/DD/YYYY.\n");
        pause_screen();
        return;
    }

    //validate time
    if (!validate_time(start_time) || !validate_time(end_time)){
        printf("Invalid time format.");
        pause_screen();
        return;
    }

    if (!validate_date_range(start_time, end_time)){
        printf("End time must be after the start time.\n");
        pause_screen();
        return;
    }

    //check for time conflicts
    if (check_time_conflict(date, start_time, end_time)){
        printf("Time slot is reserved by other student. Please choose a different time.");
        pause_screen();
        return;
    }

    //get email


    //reservation id (DATE TODAY, RESERVATION DATE, NUMBER OF RESERVATION TODAY)

    //create reservation
    int result = insert_reservation(student_name, student_num, date, start_time, end_time, reservation_id, email);


}

void cancel_reservation(){
    //cancel logic here
}

void search_reservation(){
    //search logic here
}

void clear_screen(){
    //clear logic here
}

void pause_screen(){
    //pause logic here
}

/* TODO: view_daily_schedule(): Prompts for a date, validates it, and displays all reservations for that day.
TODO: make_reservation(): Collects reservation details, validates input, checks for conflicts, and inserts a new reservation.
TODO: cancel_reservation(): Lets the user cancel a reservation by ID or student name.
TODO: search_reservations(): Allows searching for reservations by student name or reservation ID.
TODO: clear_screen(): Clears the console screen (platform-dependent).
TODO: pause_screen(): Pauses the screen until the user presses Enter.
*/