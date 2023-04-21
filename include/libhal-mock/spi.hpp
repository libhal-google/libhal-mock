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

#pragma once

#include "testing.hpp"
#include <libhal/spi.hpp>
#include <libhal/units.hpp>

namespace hal::mock {
/**
 * @brief Mock spi implementation for use in unit tests and simulations with a
 * spy functions for configure() and a record for the transfer() out data. The
 * record ignores the in buffer and just stores the data being sent so it can be
 * inspected later.
 */
struct write_only_spi : public hal::spi
{
  /**
   * @brief Reset spy information for both configure() and transfer()
   *
   */
  void reset()
  {
    spy_configure.reset();
    write_record.clear();
  }

  /// Spy handler for hal::spi::configure()
  spy_handler<settings> spy_configure;
  /// Record of the out data from hal::spi::transfer()
  std::vector<std::vector<hal::byte>> write_record;

private:
  status driver_configure(const settings& p_settings) override
  {
    return spy_configure.record(p_settings);
  };

  result<transfer_t> driver_transfer(
    std::span<const hal::byte> p_data_out,
    [[maybe_unused]] std::span<hal::byte> p_data_in,
    [[maybe_unused]] hal::byte p_filler) override
  {
    write_record.push_back({ p_data_out.begin(), p_data_out.end() });
    return transfer_t{};
  };
};
}  // namespace hal::mock
