#include "headers/main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*utility/helper functions.
String manipulation helpers.
Input sanitization.
Date/time formatting helpers.
Error handling/logging.
*/

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}