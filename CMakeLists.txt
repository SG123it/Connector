cmake_minimum_required(VERSION 3.10...3.20)
#-----------------
set(P_NAME "Connector")
set(P_VER 1.0)
#-----------------
project(${P_NAME} VERSION ${P_VER})

set(src_files
    src/main.cpp
    
    src/Configuration_worker.cpp
    src/Synchronization_logic.cpp
)
include_directories(headers)

add_executable(${P_NAME} ${src_files})
target_compile_definitions(${P_NAME} PRIVATE VERSION=${P_VER})