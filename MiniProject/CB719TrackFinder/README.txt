Readme for compling the track finder algorithm and associated classes.
This folder contains a main.cpp, with two class header files, event 
and hit with their associated cpp files as well as a helperfunctions.cpp 
used to simplify the final main script. 
There is also a python plotting analysis.py program and a PDF 
outlining the methodology and results.

C++11 is needed for this compilation.

Run make in the desired director with the command make.
 The ROOT libraries are needed as is a working g++ compiler
Ensure both the onetrack.raw and manytracks.raw are in the build directory

This will compile to a executable called track_finder.exe

This can be run with ./track_finder.exe -
The program has a menu functionality for navigating various options

The python program analysis.py is included and will produce histograms 
and fits of the data but requires several python packages to run

