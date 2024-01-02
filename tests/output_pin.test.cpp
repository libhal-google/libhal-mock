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

#include <libhal-util/output_pin.hpp>

#include <libhal-mock/output_pin.hpp>

#include <boost/ut.hpp>

namespace hal::mock {
void output_pin_mock_test()
{
  using namespace boost::ut;

  "hal::mock::output_pin::configure()"_test = []() {
    // Setup
    constexpr hal::output_pin::settings mock_settings_default{};
    constexpr hal::output_pin::settings mock_settings_custom{
      .resistor = pin_resistor::pull_down,
      .open_drain = true,
    };
    hal::mock::output_pin mock;

    // Exercise
    auto result1 = mock.configure(mock_settings_default);
    auto result2 = mock.configure(mock_settings_custom);

    // Verify
    expect(mock_settings_default ==
           std::get<0>(mock.spy_configure.call_history().at(0)));
    expect(mock_settings_custom ==
           std::get<0>(mock.
  };

  "hal::mock::output_pin::driver_level(bool)"_test = []() {
    // Setup
    hal::mock::output_pin mock;

    // Exercise
    auto result1 = mock.level(true);
    auto result2 = mock.level(false);

    // Verify
    expect(that % true ==
           std::get<0>(mock.spy_level.call_history().at(0)).state);
    expect(that % false ==
           std::get<0>(mock.spy_level.call_history().at(1)).state);
  };

  "hal::mock::output_pin::driver_level()"_test = []() {
    // Setup
    hal::mock::output_pin mock;

    // Exercise
    mock.level(true);
    auto result1 = mock.level();
    mock.level(false);
    auto result2 = mock.level();

    // Verify
    expect(that % true == result1.state);
    expect(that % false == result2.state);
  };

  "hal::mock::output_pin::reset()"_test = []() {
    // Setup
    constexpr hal::output_pin::settings mock_settings_default{};
    hal::mock::output_pin mock;
    mock.configure(mock_settings_default);
    mock.level(false);

    // Exercise
    mock.reset();

    // Verify
    expect(that % 0 == mock.spy_configure.call_history().size());
    expect(that % 0 == mock.spy_level.call_history().size());
  };
};
}  // namespace hal::mock
