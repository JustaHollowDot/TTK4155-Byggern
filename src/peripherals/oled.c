#include "oled.h"

#define OLED_COMMAND_START 0x1000
#define OLED_DATA_START 0x1200

volatile char *oled_command_position = (char *) OLED_COMMAND_START;
volatile char *oled_data_position = (char *) OLED_DATA_START;

