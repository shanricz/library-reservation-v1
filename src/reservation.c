#include "headers/reservation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* validation and time-related logic.
TODO: validate_date(date): Checks if the date is in the correct format (MM/DD/YYYY).
TODO: validate_time(time): Checks if the time is in the correct format (HH:MM AM/PM).
TODO: validate_time_range(start_time, end_time): Ensures the end time is after the start time.
TODO: check_time_conflict(date, start_time, end_time): Checks if the requested time slot overlaps with existing reservations.
TODO: format_time_24hour(time): Converts time to 24-hour format (if needed).
TODO: format_time_12hour(time): Converts time to 12-hour format (if needed).
TODO: compare_times(start, end): Compares two time strings.
*/