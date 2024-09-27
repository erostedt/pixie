cmake_minimum_required(VERSION 3.25)

project(pixie)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXAMPLES_PATH examples)
set(SRC_PATH src)
set(INCLUDE_PATH include)
set(CMAKE_C_STANDARD 17)

include_directories(${INCLUDE_PATH})
file(GLOB source_files ${SRC_PATH}/*.c)
add_library(${PROJECT_NAME} ${source_files})

file(GLOB examples ${EXAMPLES_PATH}/*)
foreach(example ${examples})
  get_filename_component(example_name ${example} NAME_WE)
  message("Building: ${example}")
  file(GLOB sources ${example}/*.c)
  add_executable(${example_name} ${sources})
  target_link_libraries(${example_name} m ${PROJECT_NAME})
endforeach()
