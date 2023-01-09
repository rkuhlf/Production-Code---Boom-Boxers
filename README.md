# Mass Measuring Ox Tank Stand

Launching rockets requires oxidizer. That oxidizer must be held by a stand. It's also nice to see how much oxidizer is loaded into the rocket. This is the code for recording the oxidizer tank mass, embedded into [a device for keeping the tank steady](http://localhost:3000/project/oxidizer-stand), and used by [Rice Eclipse](http://eclipse.rice.edu/).

It runs on an Arduino Uno, and is powered on for about five hours at a time. That entire time, it spits out the mass on top of the stand into a microSD card, which can be read after the launch or test has been completed.
