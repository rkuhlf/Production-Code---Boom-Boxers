// Check if serial monitor is working

#include <Arduino.h>

// File should be uploaded to an Arduino that already has a calibration value saved at the Eeprom address.

// Time: 60206;     Load Cell output: 38649.79
// Time: 60707;     Load Cell output: 38495.86
// Time: 61209;     Load Cell output: 38354.64
// Time: 61711;     Load Cell output: 38221.56
// Time: 62213;     Load Cell output: 38100.67
// Time: 62713;     Load Cell output: 37990.64

// Time: 23078;     Load Cell output: 37420.28
// Time: 23580;     Load Cell output: 37420.37
// Time: 24082;     Load Cell output: 37420.58
// Time: 24584;     Load Cell output: 37420.58
// Time: 25085;     Load Cell output: 37420.80; Conclusion - baud has no effect


// After ten seconds it randomly went up to 70000

// Jiggling the wires I think makes it go down to 37000 again

// Store the last five seconds of mass flow rates.
// When the mass flow rate has been negative more than 90% of the time in the last five seconds, start recording.

// Record the mass flow rate until it is decreasing for less than 60% of the five seconds.
// With the theory that it will be decreasing half the time when there is no flow and 100% of the time when it is flowing.


// Assume that taring is unnecessary because we only care about the change in mass

#include <Arduino.h>
#include <iostream>

#include <config/drain_hx711_config.h>
#include <string>

// #include "helpers/logger.h"
#include "config/eeprom_config.h"
#include "helpers/load_cell_hookup.hpp"
#include <SPI.h>
#include <SD.h>

const char* DEBUG_FILE = "data.txt"; // "debug.txt"
const char* HEADER = "LC1,LC2,LC3,sum";
const byte cal_precision = 3;

unsigned long last_updated = 0;
int time_increment = 500;


LoadCellHookup load_cell1(2, 3, LOAD_CELL_1_CAL);
LoadCellHookup load_cell2(4, 5, LOAD_CELL_2_CAL);
LoadCellHookup load_cell3(6, 7, LOAD_CELL_3_CAL);

// RAM at 75% initially.
// That extra 10% is all in the SD file.
// Adding my write to file function makes it 85% and breaks everything.

// Without the load cell class it goes to 50%

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

    Serial.print("Initializing SD:");
    // Apparently some people have had to set the pin mode to high for it to work.
    // https://embedjournal.com/arduino-sd-card-initialization-failed/
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);
    delay(10);
    
    if (!SD.begin(10)) {
        Serial.println("fail");

        while (1);
    }
    Serial.println("done");

    load_cell1.setup();
    load_cell2.setup();
    load_cell3.setup();


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

float new_mass = 0;
byte incoming_byte = 0;
bool printing_mass = true;
char str_output[8];

void loop() {
    new_mass = load_cell1.get_mass() + load_cell2.get_mass() + load_cell3.get_mass();

    if (printing_mass) {
        // check for new data and that it has been long enough
        if (millis() > last_updated + time_increment) {
            last_updated = millis();
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
    

    if (Serial.available()) {
        incoming_byte = Serial.parseInt();

        Serial.print("Command:");
        Serial.println(incoming_byte);

        if (incoming_byte == 1) {
            printing_mass = false;
        }

        if (incoming_byte == 2) {
            printing_mass = true;
            debugln();
            debug(HEADER);
            debugln();
        }
    }
}