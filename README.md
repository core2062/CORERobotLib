# CORERobotLib
FRC Team C.O.R.E. 2062's Robotics library
## Compiler and VSCode Setup
### Windows
- Download and install VSCode according to the instructions found on WPILib's screensteps:
  - https://wpilib.screenstepslive.com/s/currentCS/m/cpp
- Download the third party libraries that are used for frc, including CTRE
  - http://www.ctr-electronics.com/control-system/hro.html#product_tabs_technical_resources
- Update all of the submodules within the library
  - Open a Git Bash terminal and navigate to the 3rdparty folder of the CORERobotLib using the 'cd' command
  - When you are at the 3rd party folder, run the command 'git submodule update' in order to update all of the modules within the project
  - Build the library and ensure that there is no compilation errors
