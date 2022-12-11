#include <SPI.h>
#include <SD.h>
File myFile;
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");

  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }

  Serial.println("initialization done.");
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("This is a test file :)");
    myFile.println("testing 1, 2, 3.");
    for (int i = 0; i < 20; i++) {
      myFile.println(i);
    }
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void loop() {
  // nothing happens after setup
}




























// #include <Arduino.h>
// #include <SPI.h>
// #include <SD.h>

// #include "helpers/logger.h"


// File data_file;

// // MOSI (green) - 11
// // MISO (yellow) - 12
// // CLK (blue) - 13 
// // CS (white) - 10

// void setup() {
//   // Open serial communications and wait for port to open:
//   Serial.begin(57600);
//   while (!Serial) {
//     ; // wait for serial port to connect. Needed for native USB port only
//   }


//   Serial.print("Initializing SD card...");

//   if (!SD.begin(10)) {
//     Serial.println("initialization failed!");
//     while (1);
//   }
//   debug("initialization done.");

// }

// void loop() {
//   const char* file_name = "data.txt";
//   data_file = SD.open(file_name, FILE_WRITE);

//   if (!data_file) {
//     // If it did not open we cannot do anything
//     debug("error opening: ");
//     debug(file_name);
//     return;
//   }
  
  
//   data_file.println("testing 1, 2, 3.");
//   data_file.close();
//   debug("Wrote to file...");
// }


