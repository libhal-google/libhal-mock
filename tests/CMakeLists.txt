cmake_minimum_required(VERSION 3.15)

project(unit_test VERSION 0.0.1 LANGUAGES CXX)

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_BUILD_TYPE "Debug")

find_package(ut REQUIRED CONFIG)
find_package(libhal-mock REQUIRED CONFIG)

add_executable(${PROJECT_NAME}
  main.test.cpp
  adc.test.cpp
  can.test.cpp
  dac.test.cpp
  input_pin.test.cpp
  interrupt_pin.test.cpp
  motor.test.cpp
  output_pin.test.cpp
  pwm.test.cpp
  spi.test.cpp
  steady_clock.test.cpp
  testing.test.cpp
  timer.test.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC . ../include)
target_compile_options(${PROJECT_NAME} PRIVATE
  --coverage
  -fprofile-arcs
  -ftest-coverage
  -Werror
  -Wall
  -Wextra
  -Wshadow
  -Wnon-virtual-dtor
  -Wno-gnu-statement-expression
  -pedantic
  -fsanitize=address)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)
target_link_options(${PROJECT_NAME} PRIVATE
  --coverage
  -fprofile-arcs
  -ftest-coverage
  -fsanitize=address)
target_link_libraries(${PROJECT_NAME} PRIVATE
  Boost::ut
  libhal::mock
)

add_custom_command(TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND}
  -P ${CMAKE_SOURCE_DIR}/remove_gcda.cmake
  COMMENT "Deleting Previous Coverage Files..."
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
