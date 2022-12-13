# Notes

Running at 57600 Baud because that is what all of the things said to do.

## Things to Try

Try `void setGain(uint8_t gain = 128); //value must be 32, 64 or 128*`

The load cell looks like it outputs every 91 milliseconds.
The length of the EEPROM is 1024. (That is the number of bytes it has).
We can fit 1024 / 4 = 256 floats into EEPROM. That would list 256 * 0.091 = 23 seconds.
Assuming that I only use 90% of EEPROM, I will limit load cell output to every 200 ms.
That gives 23 * 0.9 * 200 / 91 = 45 seconds, which is a solid 1.5 SF on our 30 second benchmark.

Okay actually we are going to use an SD with thirty gigabytes of storage. That will pretty much never get filled up, but I will write code for that edge case that just deletes half of the file.

<!-- Create a shared file of some variable names. -->

I went with rewriting everything in C because I am not a huge fan of how Arduino is playing with C++. It does not support all of the standard libraries, people complain about memory fragmentation screwing things up, and all of the libraries seem to want C data types.

I believe that there is a glitch in the SD card library that occasionally pops up. Basically the reading of the bytes fails and it cannot find the file to open it. It is extremely difficult to debug because.

There are also some memory overflows leaking somewhere.


I am 90% sure that all of the load cells were working (increasing in output when load was aplied) at one point with the boards that they are now soldered to using the same pins.

Clock has about a 4.7 voltage difference from VCC on one Sparkfun board. Data is 1.66. Gnd is still 5


One of the load cells is working from within the HDX711 library.


Well I finally fixed most of the memory issues because I need to free memory when I use pointers to strings. Two options are to try to get the file system working and to make folders for archived runs. Other option is to store everything in the same file and just make the end user sort through it. 


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
// Time: 25085;     Load Cell output: 37420.80; Conclusion - baud has no effect. 

Record drain is very near to the maximum capacity of RAM for the Arduino Uno.

// DATA: https://docs.google.com/spreadsheets/d/1MPceNBFz5i5xC-sZ9yEknhpfw08qjMPKno4Q_jIPtjY/edit?usp=sharing
