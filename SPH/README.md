# SPH 2d Simulation
## How to build in VS:
* First make sure you have installed CMake and choose visual studio as default compiler
* Type command lines in terminal
`mkdir build`
`cd build`
`cmake ..`
* Open the .sln project in build, choose `Properties->linker->command` and add `/SAFESEH:NO`
* Now compiles and the program should show the window
* The program will automatically write data into `output` directory.