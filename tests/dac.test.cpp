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

#include <libhal-mock/dac.hpp>

#include <boost/ut.hpp>

namespace hal::mock {
void dac_mock_test()
{
  using namespace boost::ut;

  // Setup
  hal::mock::dac mock;
  constexpr auto expected1 = float(0.5);
  constexpr auto expected2 = float(0.25);
  mock.spy_write.trigger_error_on_call(3);

  // Exercise + Verify
  mock.write(expected1);
  expect(expected1 == std::get<0>(mock.spy_write.call_history().at(0)));

  mock.write(expected2);
  expect(expected2 == std::get<0>(mock.spy_write.call_history().at(1)));

  throws([&]() { mock.write(expected2); });
  expect(expected2 == std::get<0>(mock.spy_write.call_history().at(2)));
};
}  // namespace hal::mock
