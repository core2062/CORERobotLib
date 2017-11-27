# CORERobotLib
FRC Team C.O.R.E. 2062's Robotics library
## Compiler Setup
### Windows
- Download and install CMake

https://cmake.org/download/
- Download and install ninja-build to the ```C:/ninja``` directory

https://github.com/ninja-build/ninja/releases
- Download and install MinGW-w64 (Note that this is different from MinGW)

https://sourceforge.net/projects/mingw-w64/
- Download and install the 2017 FRC RoboRIO C++ Toolchain

http://first.wpi.edu/FRC/roborio/toolchains/
- Add MinGW-w64 and ninja-build to the system path

Open Control Panel>System and Security>System>Advanced system settings>Advanced>Environment Variables
Under the user variables section, edit the "Path" variables. Click "New" and add path to ninja.exe, which should be ```C:/ninja```. Repeat process to add the MinGW-w64 bin directory to the system path, which should be something like ```C:\Program Files\mingw-w64\x86_64-7.1.0-posix-seh-rt_v5-rev2\mingw64\bin``` and should contain g++.exe This will change based on the version of MinGW-w64 you are using, so be sure to check that it matches your installation! You will have to restart at this point for the changes to take effect!
### Ubuntu
- Add the 2017 WPILib C++ Toolchain for Linux repository according to the instructions found [here](http://first.wpi.edu/FRC/roborio/toolchains/)
```bash
sudo apt-add-repository ppa:wpilib/toolchain
```
- Update package list
```bash
sudo apt-get update
```
- Install basic build tools (cmake, make, python, wpilib toolchain, ninja-build, etc)
```bash
sudo apt-get install build-essential cmake python frc-toolchain ninja-build
```
## IDE Setup
### Eclipse
- Download and install eclipse along with the C++ FRC WPILib Development plugins

http://wpilib.screenstepslive.com/s/currentCS/m/getting_started/l/599679-installing-eclipse-c-java#getting_eclipse
Skip any sections pertaining to Java
- Download the cmake4eclipse and CMake Editor plugins from the Eclipse Marketplace

The Eclipse Marketplace can be found under Help>Eclipse Marketplace
## Building
### CLion
- Open CLion project
- Done
### Bash
- Create new directory called 'build'
```bash
mkdir build
cd build
```
- From inside that directory, generate the make files from the CMake files. This command must be used anytime a CMakeLists.txt file is changed!
```bash
cmake .. -DCMAKE_TOOLCHAIN_FILE=tools/roborio-toolchain.cmake
```
- Using make, build the CORERobotLib target. Adding the ```-j``` parameter followed by a number of cores will multithread the build.
```bash
make CORERobotLib
```
