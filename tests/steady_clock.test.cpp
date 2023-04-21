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

#include <libhal-mock/steady_clock.hpp>

#include <deque>

#include <boost/ut.hpp>

namespace hal::mock {
void steady_clock_mock_test()
{
  using namespace boost::ut;

  // Setup
  auto expected1 = steady_clock::uptime_t{ .ticks = 10 };
  auto expected2 = steady_clock::uptime_t{ .ticks = 50 };
  auto expected3 = steady_clock::uptime_t{ .ticks = 100 };
  auto expected_frequency =
    steady_clock::frequency_t{ .operating_frequency = 1.0_Hz };
  hal::mock::steady_clock mock;
  std::deque uptimes{ expected1, expected2, expected3 };
  std::queue queue(uptimes);

  // Exercise
  mock.set_frequency(expected_frequency);
  mock.set_uptimes(queue);
  auto result0 = mock.frequency().value();
  auto result1 = mock.uptime().value();
  auto result2 = mock.uptime().value();
  auto result3 = mock.uptime().value();

  // Verify
  expect(that % expected_frequency.operating_frequency ==
         result0.operating_frequency);
  expect(that % expected1.ticks == result1.ticks);
  expect(that % expected2.ticks == result2.ticks);
  expect(that % expected3.ticks == result3.ticks);
  expect(!bool{ mock.uptime() });
};
}  // namespace hal::mock
