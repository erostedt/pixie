cmake_minimum_required(VERSION 3.25)

project(pixie)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(DEMOS_PATH ./demos)
set(SRC_PATH ./src)
set(INCLUDE_PATH ./include)
set(CMAKE_C_STANDARD 17)

include_directories(${INCLUDE_PATH})

file(GLOB pixie_demos ${DEMOS_PATH}/*)
foreach(pixie_demo ${pixie_demos})
  get_filename_component(demo_name ${pixie_demo} NAME_WE)
  message("Building: ${pixie_demo})")
  file(GLOB sources ${pixie_demo}/*.c)
  add_executable(${demo_name} ${sources})
  target_link_libraries(${demo_name} m)
endforeach()
