cmake_minimum_required(VERSION 2.8)

set(ARM_PREFIX arm-frc-linux-gnueabi)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_CXX_COMPILER ${ARM_PREFIX}-g++)
set(CMAKE_C_COMPILER ${ARM_PREFIX}-gcc)
set(CMAKE_CXX_FLAGS "-std=c++14 -Wformat=2 -Wall -Wextra -pedantic -Wno-psabi -Wno-unused-but-set-parameter" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "-O3" CACHE STRING "" FORCE)
