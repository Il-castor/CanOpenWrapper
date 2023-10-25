cmake_minimum_required(VERSION 3.5)
project(canopen_wrapper)

if(NOT CMAKE_C_STANDARD)
    set(CMAKE_C_STANDARD 99)
endif()

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

include_directories(
    "include"
    "include/net"
    "include/canopen_utils"
)

add_library(canbus_network src/net/canbus_network.cpp)
add_library(canbus_wrapper src/net/canbus_wrapper.cpp)
add_library(canopen_utils src/canopen_utils/utils.cpp)
add_library(canbus src/canbus.cpp)

add_executable(main main.cpp)

target_link_libraries(canbus_wrapper canbus_network)
target_link_libraries(canbus canbus_wrapper)

target_link_libraries( main
    canbus_network
    canbus
)

install(TARGETS 
    main
    DESTINATION ./$(PROJECT_NAME)
)

set(CMAKE_BUILD_TYPE Debug)