# SPH 2d Simulation
## Build in VS:
* First make sure you installed CMake and choose visual studio as default compiler
* Type command lines in terminal
`mkdir build`
`cd build`
`cmake ..`
* Open the .sln project in build, choose `Properties->linker->commands` and add `/SAFESEF:NO`
* Now compiles and it should show the window
* The program will automatically write data into `output` directory.