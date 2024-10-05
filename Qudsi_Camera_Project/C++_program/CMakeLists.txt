cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME placeholder)
project(${PROJECT_NAME})

add_compile_options(-std=c++17)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(OpenCV REQUIRED)

# Include directories and files
include_directories(
  PRIVATE
  "include"
  ${OpenCV_INCLUDE_DIRS}
)

set(LIBRARIES
    ${OpenCV_LIBS}
)

add_executable(spa4 src/spa4.cpp)
target_link_libraries(spa4 ${LIBRARIES})

add_executable(f2 src/f2.cpp)
target_link_libraries(f2 ${LIBRARIES})