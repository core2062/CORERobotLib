# CORERobotLib
FRC Team C.O.R.E. 2062's Robotics library
## Setup
### Windows
- Enable Windows Subsystem for Linux
https://msdn.microsoft.com/en-us/commandline/wsl/install_guide
- Open Command Prompt
- Type ```bash ``` to enter the bash terminal inside of Command Prompt. This will allow you to follow any Linux/Bash directives in thease instructions.
- Follow Ubuntu setup instructions to complete setup
### Ubuntu
- Add the 2017 WPILib C++ Toolchain for Linux repository according to the instructions from [here](http://first.wpi.edu/FRC/roborio/toolchains/)
```bash
sudo apt-add-repository ppa:wpilib/toolchain
```
- Update package list
```bash
sudo apt-get update
```
- Install basic build tools (cmake, make, python, wpilib toolchain, etc)
```bash
sudo apt-get install build-essential cmake python frc-toolchain
```
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
