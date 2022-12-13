// File should be uploaded to an Arduino that already has a calibration value saved at the Eeprom address.

// Store the last five seconds of mass flow rates.
// When the mass flow rate has been negative more than 90% of the time in the last five seconds, start recording.

// Record the mass flow rate until it is decreasing for less than 60% of the five seconds.
// With the theory that it will be decreasing half the time when there is no flow and 100% of the time when it is flowing.


// Assume that taring is unnecessary because we only care about the change in mass

#include <string>

// TODO: see if I can import EEPROM or if that requires too much memory.
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#include "config/drain_hx711_config.h"
#include "config/load_cell_wiring_config.h"
#include "config/eeprom_config.h"
#include "config/SPI_config.h"
#include "load_cell_hookup.hpp"

const char* DEBUG_FILE = "data.txt";
const char* HEADER = "LC1,LC2,LC3,sum";
const byte cal_precision = 3;

unsigned long last_updated = 0;
int time_increment = 100;


LoadCellHookup load_cell1(LOAD_CELL_1_DOUT, LOAD_CELL_1_SCK, LOAD_CELL_1_CAL);
LoadCellHookup load_cell2(LOAD_CELL_2_DOUT, LOAD_CELL_2_SCK, LOAD_CELL_2_CAL);
LoadCellHookup load_cell3(LOAD_CELL_3_DOUT, LOAD_CELL_3_SCK, LOAD_CELL_3_CAL);

File myFile;
bool write_to_file(const char* to_write, const char* file_name) {
    myFile = SD.open(file_name, FILE_WRITE);

    if (!myFile) {
        // If it did not open we cannot do anything
        // We are really screwed if we cannot debug anything because you have to have your computer plugged in to see what happens
        Serial.print("error opening ");
        Serial.println(file_name);
        return false;
    }

    Serial.print("Writing to ");
    Serial.print(file_name);
    Serial.print(": ");
    Serial.println(to_write);

    myFile.print(to_write);
    myFile.close();

    return true;
}

bool debug(const char* to_write) {
    return write_to_file(to_write, DEBUG_FILE);
}

bool debugln() {
    return debug("\n");
}

void setup() {
    Serial.begin(57600); delay(100);

    Serial.print("Init SD:");
    // Apparently some people have had to set the pin mode to high for it to work.
    // https://embedjournal.com/arduino-sd-card-initialization-failed/
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);
    delay(10);
    
    if (!SD.begin(CS_PIN)) {
        Serial.println("fail");

        while (1);
    }
    Serial.println("done");

    load_cell1.setup();
    load_cell2.setup();
    load_cell3.setup();

    // Newlines to separate from older runs.
    debugln();
    debugln();

    // Write the calibrations that were used to the file.
    debug("Cals:");
    debugln();
    char str_cal[7];
    dtostrf(load_cell1.calibration_value, 0, cal_precision, str_cal);
    debug(str_cal);
    debugln();
    dtostrf(load_cell2.calibration_value, 0, cal_precision, str_cal);
    debug(str_cal);
    debugln();
    dtostrf(load_cell3.calibration_value, 0, cal_precision, str_cal);
    debug(str_cal);
    debugln();

    debugln();
    debug(HEADER);
    debugln();
}

// TODO: try adding prev_mass back in.
float new_mass = 0;
char str_output[16];

void loop() {
    new_mass = load_cell1.get_mass() + load_cell2.get_mass() + load_cell3.get_mass();

    // Check for new data and that it has been long enough.
    if (millis() > last_updated + time_increment) {
        last_updated = millis();

        // Convert the time into a string for output.
        ltoa(last_updated, str_output, 10);
        debug(str_output);
        debug(",");

        dtostrf(load_cell1.get_voltage(), 0, 2, str_output);
        debug(str_output);
        debug(",");
        dtostrf(load_cell2.get_voltage(), 0, 2, str_output);
        debug(str_output);
        debug(",");
        dtostrf(load_cell3.get_voltage(), 0, 2, str_output);
        debug(str_output);
        debug(",");
        dtostrf(new_mass, 0, 2, str_output);
        debug(str_output);
        debugln();
    }
}
