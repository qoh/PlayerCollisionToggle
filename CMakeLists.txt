cmake_minimum_required(VERSION 3.0)
project(PlayerCollisionToggle)
set(CMAKE_CXX_FLAGS "-static -static-libgcc -static-libstdc++")
add_library(PlayerCollisionToggle SHARED
	PlayerCollisionToggle/main.cpp
	PlayerCollisionToggle/torque.cpp)
target_link_libraries(PlayerCollisionToggle psapi)
