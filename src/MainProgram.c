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
        printf(">>>LIBRARY CONSULTATION ROOM RESERVATION SYSTEM<<<\n");
        printf("Main Menu:\n");
        printf("1. View Daily Schedule\n");
        printf("2. Make a Reservation\n");
        printf("3. Cancel a Reservation\n");
        //TODO: LAGAY KAYO DITO BAGO, EDIT RESERVATION
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

/*TODO: MAKE SURE NA ON POINT VALIDATIONS NATIN, DI PWEDE MAG ENTER NG INVALID DATES/TIMES OR NG SPECIAL CHARACTERS NA DI PWEDE
TODO: IMPROVE UI/UX, MAKE IT MORE INTERACIVE AND USER-FRIENDLY
TODO: YUNG AUTOMATIC MAG AADD NG SLASH PARA DI NA HASSLE SA USER
*/
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
    printf("+----------------------+-------------------+-------------------+---------------------------+\n");
    printf("| %-20s | %-17s | %-17s | %-25s |\n", "ID", "Start Time", "End Time", "Student Name");
    printf("+----------------------+-------------------+-------------------+---------------------------+\n");

        // Get reservations for the date
    int result = get_reservations_by_date(date);

    if (result != 0) {
        printf("Error retrieving schedule data.\n");
    }

    printf("+----------------------+-------------------+-------------------+---------------------------+\n");
    pause_screen();
}

/*
TODO: MAKE SURE NA MAY VALIDATIONS
TODO: INAACCEPT YUNG SPACES SA NAME
TODO: MORE PRESENTABLE UI
TODO: VALIDATION SA STUDENT NUMBER (LENGTH, FORMAT) LIKE KUNG PANO FORMAT SA  ID NATIN DAPAT GANON LANG IAACCEPT
*/
void make_reservation(){
    char student_name[MAX_NAME_LENGTH];
    char student_num[MAX_STUD_ID_LENGTH];
    char date[MAX_DATE_LENGTH];
    char start_time[MAX_TIME_LENGTH];
    char end_time[MAX_TIME_LENGTH];
    char reservation_id[MAX_RESERVATION_ID_LENGTH];

    clear_screen();
    printf("MAKE A RESERVATION\n");
    printf("------------------\n");

   
    clear_input_buffer(); // Clear input buffer (THIS ALLOWS USER TO HAVE SPACES IN THEIR NAME)

   printf("Enter student name: ");
    if(fgets(student_name, sizeof(student_name), stdin) == NULL){
        printf("Error reading input.\n");
        pause_screen();
        return;
    }

    // Remove newline character
    student_name[strcspn(student_name, "\n")] = 0;


    //LACKS VALIDATION
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

     // generate reservation id (DATE TODAY, RESERVATION DATE, NUMBER OF RESERVATION TODAY)
    generate_reservation_id(reservation_id, sizeof(reservation_id), date);
    printf("Generated Reservation ID: %s\n", reservation_id);

     //confirm details
    printf("\nPlease confirm your reservation details:\n");
    printf("+------------------+------------+-------------------+-------------------+---------------------------+\n");
    printf("+------------------+------------+-------------------+-------------------+---------------------------+\n");
    printf("| %-20s | %-10s | %-17s | %-17s | %-25s |\n", "Reservation ID", "Date", "Start Time", "End Time", "Student Name");
    printf("+------------------+------------+-------------------+-------------------+---------------------------+\n");
    printf("| %-20s | %-10s | %-17s | %-17s | %-25s |\n", reservation_id, date, start_time, end_time, student_name);
    printf("+------------------+------------+-------------------+-------------------+---------------------------+\n");
    printf("Confirm reservation? (Y/N): ");
    char confirm;
    if (scanf(" %c", &confirm) != 1 || (confirm != 'Y' && confirm != 'y')) {
        printf("Reservation cancelled by user.\n");
        pause_screen();
        return;
    }
    
    //create reservation
    int result = insert_reservation(student_name, student_num, date, start_time, end_time, reservation_id);
    if (result == 0) {
        printf("Reservation created successfully.\n");
    } else {
        printf("Failed to create reservation.\n");
    }
    pause_screen();
}

//RESERVATION ID
    // Format: TODAY_DATE-RESERVATION_DATE-NUMBER_OF_RESERVATIONS_TODAY
    // Example: 100425-101025-001 (MMDDYY-MMDDYY-XXX)

void generate_reservation_id(char* out_id, size_t out_size, const char* reservation_date) {
    // Simple format: TODAY_DATE-RESERVATION_DATE-TIMESTAMP
    // Example: 101325-120125-123456 (MMDDYY-MMDDYY-HHMMSS)
 // Simple format: TODAY_DATE-RESERVATION_DATE-TIMESTAMP
    // Example: 101325-120125-123456 (MMDDYY-MMDDYY-HHMMSS)
    
    if (out_size < 20) {
        if (out_size > 0) out_id[0] = '\0';
        return;
    }
    
    // Get current time
    time_t now = time(NULL);
    struct tm* today = localtime(&now);
    
    // Today's date in MMDDYY format
    char today_str[7];
    strftime(today_str, sizeof(today_str), "%m%d%y", today);
    
    // Convert reservation date from MM/DD/YYYY to MMDDYY format
    char res_date_str[8]; // Increased size to prevent truncation
    if (strlen(reservation_date) == 10 && reservation_date[2] == '/' && reservation_date[5] == '/') {
        int month, day, year;
        if (sscanf(reservation_date, "%2d/%2d/%4d", &month, &day, &year) == 3) {
            snprintf(res_date_str, sizeof(res_date_str), "%02d%02d%02d", 
                     month, day, year % 100);
        } else {
            strcpy(res_date_str, "000000");
        }
    } else {
        strcpy(res_date_str, "000000");
    }
    
    // Use timestamp for uniqueness (HHMMSS)
    char timestamp_str[7];
    strftime(timestamp_str, sizeof(timestamp_str), "%H%M%S", today);
    
    // Generate ID: TODAY_DATE-RESERVATION_DATE-TIMESTAMP
    snprintf(out_id, out_size, "%s-%s-%s", today_str, res_date_str, timestamp_str);
}

/*
TODO: ETO WALA PA TALAGA, KAYO NA BAHALA IF PAANO NYO IIMPLEMENT; CHECK OTHER C FILE FOR REFERENCE || HEADERS
*/
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
    printf("2. Return to Main Menu\n");
    printf("Enter your choice: ");
}

/*
TODO: VALIDATIONS
TODO: MORE PRESENTABLE UI
TODO: SEARCH BY NAME OR RESERVATION ID (KELANGAN TAMA YUNG INPUT)
*/
void search_reservations(){
    int choice;
    char search_term[MAX_NAME_LENGTH];
    char search_id[MAX_RESERVATION_ID_LENGTH];

    clear_screen();
    printf("SEARCH RESERVATIONS\n");
    printf("-------------------\n");
    printf("Search by:\n");
    printf("1. Student Name\n");
    printf("2. Reservation ID\n");
    printf("3. Back to Main Menu\n\n");
    printf("Enter your choice (1-3): ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        pause_screen();
        return;
    }

    switch (choice) {
        case 1:
            printf("Enter student name: ");
            clear_input_buffer(); // Clear input buffer
            if (fgets(search_term, sizeof(search_term), stdin) == NULL) {
                printf("Error reading name.\n");
                pause_screen();
                return;
            }
            search_term[strcspn(search_term, "\n")] = 0; // Remove newline

            printf("\nSearch results for '%s':\n", search_term);
            printf("+----------------------+-------------------+-------------------+---------------------------+\n");
            printf("| %-20s | %-17s | %-17s | %-25s |\n", "ID", "Start Time", "End Time", "Student Name");
            printf("+----------------------+-------------------+-------------------+---------------------------+\n");
            get_reservations_by_name(search_term);
            printf("+----------------------+-------------------+-------------------+---------------------------+\n");
            break;

        case 2:
            printf("Enter reservation ID: ");
            if (scanf("%s", search_id) != 1) {
                printf("Error reading reservation ID.\n");
                pause_screen();
                return;
            }

            printf("\nSearch results for ID '%s':\n", search_id);
            printf("+----------------------+-------------------+-------------------+---------------------------+\n");
            printf("| %-20s | %-17s | %-17s | %-25s |\n", "ID", "Start Time", "End Time", "Student Name");
            printf("+----------------------+-------------------+-------------------+---------------------------+\n");
            get_reservations_by_id(search_id);
            printf("+----------------------+-------------------+-------------------+---------------------------+\n");
            break;

        case 3:
            return;

        default:
            printf("Invalid choice.\n");
            break;
    }

    pause_screen();
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


//TODO: EDIT RESERVATION FUNCTION NA DIN