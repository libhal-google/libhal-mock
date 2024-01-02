// Copyright 2023 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <cstdio>

#include <libhal-mock/pwm.hpp>

int main()
{
  using namespace hal::literals;

  hal::mock::pwm mock;

  try {
    mock.frequency(10.0_kHz);

    mock.duty_cycle(0.25);
    std::printf("Spy history #1: %f\n",
                std::get<0>(mock.spy_duty_cycle.call_history().at(0)));
    mock.duty_cycle(0.50);
    std::printf("Spy history #2: %f\n",
                std::get<0>(mock.spy_duty_cycle.call_history().at(1)));
    mock.duty_cycle(-0.25);
    std::printf("Spy history #3: %f\n",
                std::get<0>(mock.spy_duty_cycle.call_history().at(2)));
    mock.duty_cycle(-1.0);
    std::printf("Spy history #4: %f\n",
                std::get<0>(mock.spy_duty_cycle.call_history().at(3)));
  } catch (...) {
    std::printf("Caught error successfully!\n");
    return -1;
  };

  return 0;
}