# Notes

I went with rewriting everything in C because I am not a huge fan of how Arduino is playing with C++. It does not support all of the standard libraries, people complain about memory fragmentation screwing things up, and all of the libraries seem to want C data types. That being said, I did make a small wrapper for the load cell class because it seemed like setting the calibration values was throwing things off, so I just did it manually. I am not sure if this is actually the case (seems doubtful), and it would be good to check up on, since it is probably wasting memory somewhere.

Running at 57600 Baud because that is what all of the things said to do.

## Output

DATA: https://docs.google.com/spreadsheets/d/1MPceNBFz5i5xC-sZ9yEknhpfw08qjMPKno4Q_jIPtjY/edit?usp=sharing

### Load Cell Accuracy

The load cell looks like it outputs every 91 milliseconds.

Time | Output
--- | ---
60206 | 38649.79
60707 | 38495.86
61209 | 38354.64
61711 | 38221.56
62213 | 38100.67
62713 | 37990.64
Diff. Baud | Diff. Baud
23078 | 37420.28
23580 | 37420.37
24082 | 37420.58
24584 | 37420.58
25085 | 37420.80
Baud has no effect on load cell output.

### Output Memory

<!-- TODO: format this math nicely. -->
The length of the EEPROM is 1024. (That is the number of bytes it has).
We can fit 1024 / 4 = 256 floats into EEPROM. That would list 256 * 0.091 = 23 seconds.
Assuming that I only use 90% of EEPROM, I will limit load cell output to every 200 ms.
That gives 23 * 0.9 * 200 / 91 = 45 seconds, which is a solid 1.5 SF on our 30 second benchmark.

Okay actually we are going to use a microSD with 32 GB of storage. That will pretty much never get filled up.

### Output Battery Consumption

## Program

### Program Memory

Well I finally fixed most of the memory issues because I need to free memory when I use pointers to strings. Two options are to try to get the file system working and to make folders for archived runs. Other option is to store everything in the same file and just make the end user sort through it.

Record drain is very near to the maximum capacity of RAM for the Arduino Uno. Test_accuracy is also right on the edge, since I wanted to store as many values as possible to take the average. That means you can just adjust the `STORABLE_VALUES` variable to accomodate whatever device.

### Things to Try

Resolve error with the failure to open file. Try commenting out the separate section that opens it. Also try without the move() section.

Try with a USB stick and see if that is more reliable.

Separate the load cell platform into a different class so that it can be used if there are three load cells and it can be used if there is only one load cell.

Debug the logging setup in a vacuum because it seems like there is memory overflow going on.
Try `void setGain(uint8_t gain = 128); //value must be 32, 64 or 128*`

Write code so that if the microSD gets to 16 GB, half of the file is deleted.

## Troubleshooting

I believe that there is a glitch in the SD card library that occasionally pops up. Basically the reading of the bytes fails and it cannot find the file to open it. I am not sure what is going on, but it might have been a memory leak, or it might have been the frequency with which I was opening files to write to them.

Just a reminder that almost all of the code is very close to having memory overflows, so that is probably the issue if really unexpected output appears.

The microSD shields from Arduino fail shockingly frequently. If the device fails to initialize the SD storage, that is probably the reason why.

The soldered connections on the device are not particularly strong. It is very possible that they could break.
