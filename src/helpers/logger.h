#ifndef LOGGER
#define LOGGER


#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#include <time.h>
#include <string>
#include <sstream>

#include <ctime>


// Debug includes all of the information in case it breaks.
#define DEBUG_FILE "data.txt" // "debug.txt"
bool debug_setup = false;
// Raw data includes only the time outputs and the voltages of the load cells.
#define DATA_FILE DEBUG_FILE // "raw.txt"
// Drain data includes only the weights and mass flow values over time.
#define DRAIN_DATA_FILE DEBUG_FILE // "drain.txt"


void get_time(char* out) {
    unsigned long now;
    now = millis();
    
    sprintf(out, "%lu", now);
}

File myFile;
bool write_to_file(const char* to_write, const char* file_name, bool include_time = true) {
    myFile = SD.open(file_name, FILE_WRITE);

    if (!myFile) {
        // If it did not open we cannot do anything
        // We are really screwed if we cannot debug anything because you have to have your computer plugged in to see what happens
        Serial.print("error opening ");
        Serial.println(file_name);
        return false;
    }

    char* to_write_mutable = (char*) malloc(sizeof(char) * 64);
    to_write_mutable[0] = 0;

    if (include_time) {
        // char time[32];
        // get_time(time);

        // strcat(to_write_mutable, time);
        // strcat(to_write_mutable, "ms :  ");
    }

    // strcat breaks it
    strcat(to_write_mutable, to_write);

    Serial.print("Writing output to ");
    Serial.print(file_name);
    Serial.print(": ");
    Serial.println(to_write_mutable);

    myFile.println(to_write_mutable);
    myFile.close();

    free(to_write_mutable);

    return true;
}

bool debug(const char* info) {
    Serial.println(info);
    return write_to_file(info, DEBUG_FILE);
}

bool data(const char* data) {
    return debug(data);

    Serial.println(data);
    return write_to_file(data, DATA_FILE);
}

bool drain_data(const char* data) {
    return debug(data);
    Serial.println(data);
    return write_to_file(data, DRAIN_DATA_FILE);
}



bool move(const char* file_name, const char* dest_name) {
    // SD supports multiple open files.
    // https://forum.arduino.cc/t/yes-sd-can-open-more-than-1-file-at-a-time-with-sketch/290287
    // File orig_file = SD.open(file_name, FILE_READ);
    // File dest_file = SD.open(dest_name, FILE_WRITE);

    // size_t n;  
    // uint8_t buf[64];
    // while ((n = orig_file.read(buf, sizeof(buf))) > 0) {
    //     dest_file.write(buf, n);
    // }

    // orig_file.close();
    // dest_file.close();

    // SD.remove((char*) file_name);

    return true;
}

void new_file_name(const char* file_name, int appended, char* out) {
    char str_appended[32];

    sprintf(str_appended, "%d", appended);
    // FIXME: I am pretty sure this is not going to give me the behavior that I want. I am concatenating over and over again to the thing.
    strcat(out, file_name);
    strcat(out, str_appended);
}

void get_unique_file_name(const char* file_name, char* out) {
    int appended = 0;

    new_file_name(file_name, appended, out);

    while (SD.exists(out)) {
        new_file_name(file_name, appended, out);
        appended++;
    }

    // out is the correct value once the file no longer exists
}

bool prepare_output(const char* file_name) {
    Serial.println("Random allocation to trip memory error.");
    Serial.print("Preparing output for ");
    Serial.println((char *) file_name);
    // If it already exists, move it to an archive folder.
    if (SD.exists((char *) file_name)) {
        Serial.println("File exists, moving.");

    //     if (!SD.exists((char*) "archive")) {
    //         // TODO: Make the if condition also check that it is a folder.
    //         SD.mkdir((char*) "archive");
    //     }

        // FIXME: this seems to trigger memory overflow exceptions, but I really doubt that it is the cause
        // char target_name[32] = "archive/";
        // Serial.println(sizeof(target_name));
        // strcat(target_name, file_name);
        // Serial.println(target_name);

    //     char dest_name[32];
    //     get_unique_file_name(target_name, dest_name);

    //     if (move(file_name, dest_name)) {
    //         Serial.println("Successfully moved.");
    //     }
    }

    write_to_file(file_name, file_name, false);

    return true;
}

bool setup_logger() {
    Serial.print("Initializing SD card...");
    // Apparently some people have had to set the pin mode to high for it to work.
    // https://embedjournal.com/arduino-sd-card-initialization-failed/
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);
    delay(10);
    
    if (!SD.begin(10)) {
        Serial.println("initialization failed!");

        return false;
    }
    Serial.println("initialization done.");

    // if (prepare_output((char*) DEBUG_FILE)) {
    //     debug_setup = true;
    // }
    // prepare_output((char*) DATA_FILE);
    // prepare_output((char*) DRAIN_DATA_FILE);

    return true;
}



#endif