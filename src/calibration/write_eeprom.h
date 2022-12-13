#include <Arduino.h>
#include <EEPROM.h>

#include "config/eeprom_config.h"

void write_load_cell(int address, float value) {
    EEPROM.put(address, value);

    char message[256] = {0};
    strcat(message, "Wrote ");

    char str_value[32];
    dtostrf(value, 0, 5, str_value);
    strcat(message, str_value);

    strcat(message, " to EEPROM.");

    Serial.println(message);
}

void setup() {
    Serial.begin(57600);
    delay(1000);
    Serial.println("Writing EEPROM.");

    // Google sheets outputs the coefficients for a line of best fit in reverse order
    // of the columns, so make sure to flip the order to match the load cells correctly.
    write_load_cell(LOAD_CELL_1_CAL, 0.09050678);
    write_load_cell(LOAD_CELL_2_CAL, 0.05393877893);
    write_load_cell(LOAD_CELL_3_CAL, 0.07263319961);

    Serial.println("Complete");
}

void loop() {}