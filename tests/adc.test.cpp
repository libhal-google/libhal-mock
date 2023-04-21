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

#include <libhal-mock/adc.hpp>

#include <deque>

#include <boost/ut.hpp>

namespace hal::mock {
void adc_mock_test()
{
  using namespace boost::ut;

  // Setup
  auto expected1 = adc::read_t{ .sample = 0.25f };
  auto expected2 = adc::read_t{ .sample = 0.5f };
  auto expected3 = adc::read_t{ .sample = 1.0f };
  hal::mock::adc mock;
  std::deque percentages{ expected1, expected2, expected3 };
  std::queue queue(percentages);

  // Exercise
  mock.set(queue);
  auto result1 = mock.read().value();
  auto result2 = mock.read().value();
  auto result3 = mock.read().value();

  // Verify
  expect(that % expected1.sample == result1.sample);
  expect(that % expected2.sample == result2.sample);
  expect(that % expected3.sample == result3.sample);
};
}  // namespace hal::mock
