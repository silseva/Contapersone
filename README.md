## Contapersone

System developed for Liceo Zucchi high school, Monza, Italy.
This repo is made up of two folders: one contains the program to be flashed onto an Arduino Leonardo that is used to manage the inputs from sensor and counting stuff, the other
containing the sources for a program to be run on a PC that fetches data from the Arduino.

Installation procedure for the PC side:
* make shure you have GCC 5.0 or higher (this is needed to compile correctly Regex class)
* cd to PC folder
* edit the makefile to specify a directory in wich install the program
* optionally change the program's name
* give the "make" command
* give the "make install" command to install the executable

Usage is: *executablename serialPort templateFileName outputFileName*, as an example *contapersone /dev/ttyS0 template.html output.html*
For further info see www.silvanoseva.eu/contapersone.html (currently the page is only in italian language)