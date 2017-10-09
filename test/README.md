# CORERobotLib_Test
Unit testing for FRC Team C.O.R.E. 2062's Robotics library
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
cmake ..
```
- Using make, build the CORERobotLib_Test target. Adding the ```-j``` parameter followed by a number of cores will multithread the build.
```bash
make CORERobotLib_Test
```
- Run the CORERobotLib_Test target
```bash
./CORERobotLib_Test
```
