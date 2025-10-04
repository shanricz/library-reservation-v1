#include "headers/main.h"
#include "headers/database.h"
#include "headers/reservation.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//db connection
sqlite3 *db;

int main() {

    printf("---> LIBRARY CONSULTATION ROOM RESERVATION SYSTEM <---\n");
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
    char date[MAX_DATE_LENGTH];

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
    char date[MAX_DATE_LENGTH];
    char start_time[MAX_TIME_LENGTH];
    char end_time[MAX_TIME_LENGTH];
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
    if(scanf("%s", student_num) != 1){
        printf("Error: Wrong student number format.\n");
        pause_screen();
        return;
    }

    //get date
    printf("Enter desired date [MM/DD/YYYY]: ");
    if(scanf("%s", date) != 1){
        printf("Error: Wrong date format.\n");
        pause_screen();
        return;
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
    if (scanf("%s", end_time) != 1){
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

    if (!validate_time_range(start_time, end_time)){
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


    //reservation id (DATE TODAY, RESERVATION DATE, NUMBER OF RESERVATION TODAY)
    //generate_reservation_id(char* reservation_id, size_t sizeof(reservation_id));
    //printf("Generated Reservation ID: %s\n", reservation_id);

    //confirm details
    /*printf("\nPlease confirm your reservation details:\n");
    printf("+-----+-------------------+-------------------+---------------------------+\n");
    printf("| %-3s | %-17s | %-17s | %-25s |\n", "Reservation ID", "Date", "Start Time", "End Time", "Student Name");
    printf("+-----+-------------------+-------------------+---------------------------+\n");
    printf("Confirm reservation? (Y/N): ");
    char confirm;
    if (scanf(" %c", &confirm) != 1 || (confirm != 'Y' && confirm != 'y')) {
        printf("Reservation cancelled by user.\n");
        pause_screen();
        return;
    }

    


}*/
    /*void generate_reservation_id(char* reservation_id, sizeof(char* reservation_id)){
    // date today, date of reservation, number of reservation today
    // Example: 100425-101025-001 (MonthDayYear-MonthDayYear-Number)

    time_t raw_time; // Stores the calendar time as a time_t object

    struct tm *local_time_info; // Pointer to a tm structure for local time

    char today_str[7]; // String to hold today's date in MMDDYY format

    char date[7]; // String to hold reservation date in MMDDYY format

    int reservation_count = 0; // Variable to hold the count of reservations for today
    
    time(&raw_time); // Get the current calendar time
    
    local_time_info = localtime(&raw_time); // Convert to local time structure
    
    strftime(today_str, sizeof(today_str), "%m%d%y", local_time_info); // Format today's date
    
    // Count existing reservations for today
    char sql[100];
    char* err_msg = 0;
    sprintf(sql, "SELECT COUNT(*) FROM reservations WHERE date = '%s';", today_str);
    int rc = sqlite3_exec(db, sql, callback_count_reservations, &reservation_count, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        reservation_count = 0; // Default to 0 on error
    }
    // Generate reservation ID
    sprintf(reservation_id, "%s-%s-%03d", today_str, date, reservation_count + 1);
    return reservation_id;
}*/

//create reservation
    int result = insert_reservation(student_name, student_num, date, start_time, end_time, reservation_id);
    if (result == 0) {
        printf("Reservation created successfully.\n");
    } else {
        printf("Failed to create reservation.\n");
    }
    pause_screen();
}

void cancel_reservation(){
    //cancel logic here
    int choice;
    int reservation_id;
    char search_name[MAX_NAME_LENGTH];

    clear_screen();
    printf("CANCEL A RESERVATION\n");
    printf("--------------------\n");
    printf("Cancel by:\n");
    printf("1. Reservation ID\n");
    printf("2. Student Name\n");
    printf("3. Return to Main Menu\n");
    printf("Enter your choice: ");
}

void search_reservations(){
    //search logic here
}

void clear_screen(){
    //clear logic here
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

void pause_screen(){
    //pause logic here
    printf("\nPress enter to continue...");
    getchar();
    getchar(); //clears newline from previous input
}



/* TODO: view_daily_schedule(): Prompts for a date, validates it, and displays all reservations for that day.
TODO: make_reservation(): Collects reservation details, validates input, checks for conflicts, and inserts a new reservation.
TODO: cancel_reservation(): Lets the user cancel a reservation by ID or student name.
TODO: search_reservations(): Allows searching for reservations by student name or reservation ID.
DONE: clear_screen(): Clears the console screen (platform-dependent).
DONE: pause_screen(): Pauses the screen until the user presses Enter.
*/