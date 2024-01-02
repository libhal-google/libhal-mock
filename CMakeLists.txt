# Copyright 2023 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.15)

# Because libhal-mock is a header only library, this cmake file only builds and
# executes the unit tests
project(libhal LANGUAGES CXX)

libhal_unit_test(SOURCES
  tests/main.test.cpp
  tests/adc.test.cpp
  tests/can.test.cpp
  tests/dac.test.cpp
  tests/input_pin.test.cpp
  tests/interrupt_pin.test.cpp
  tests/motor.test.cpp
  tests/output_pin.test.cpp
  tests/pwm.test.cpp
  tests/spi.test.cpp
  tests/steady_clock.test.cpp
  tests/testing.test.cpp
  tests/timer.test.cpp

  PACKAGES
  libhal
  libhal-util

  LINK_LIBRARIES
  libhal
  libhal::util)
