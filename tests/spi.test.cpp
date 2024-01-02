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

#include <libhal-util/spi.hpp>

#include <algorithm>

#include <libhal-mock/spi.hpp>

#include <boost/ut.hpp>

namespace hal::mock {
void spi_mock_test()
{
  using namespace boost::ut;

  "hal::mock::write_only_spi::configure()"_test = []() {
    // Setup
    constexpr hal::spi::settings expected1 = { .clock_rate = 1'000,
                                               .clock_idles_high = false,
                                               .data_valid_on_trailing_edge =
                                                 false };

    constexpr hal::spi::settings expected2 = { .clock_rate = 10'000,
                                               .clock_idles_high = true,
                                               .data_valid_on_trailing_edge =
                                                 true };

    hal::mock::write_only_spi mock;
    mock.spy_configure.trigger_error_on_call(3);

    // Exercise + Verify
    mock.configure(expected1);
    expect(expected1 == std::get<0>(mock.spy_configure.call_history().at(0)));

    mock.configure(expected2);
    expect(expected2 == std::get<0>(mock.spy_configure.call_history().at(1)));

    throws([&]() { mock.configure(expected2); });
    expect(expected2 == std::get<0>(mock.spy_configure.call_history().at(2)));
  };

  "hal::mock::write_only_spi::transfer()"_test = []() {
    // Setup
    std::vector<hal::byte> out_1{ hal::byte{ 0xAA } };
    std::vector<hal::byte> out_2{ hal::byte{ 0xDD }, hal::byte{ 0xCC } };
    std::span<hal::byte> dummy{};
    hal::byte filler{ 0xFF };

    hal::mock::write_only_spi mock;

    // Exercise + Verify
    mock.transfer(out_1, dummy, filler);
    expect(out_1 == mock.write_record.at(0));

    mock.transfer(out_2, dummy, filler);
    expect(out_2 == mock.write_record.at(1));
    expect(mock.write_record.size() == 2);

    mock.reset();
    expect(mock.write_record.size() == 0);
  };
};
}  // namespace hal::mock
