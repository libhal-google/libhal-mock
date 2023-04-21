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
#include <libhal/dac.hpp>

namespace hal::mock {
/**
 * @brief Mock dac implementation for use in unit tests and simulations with a
 * spy function for write()
 *
 */
struct dac : public hal::dac
{
  /**
   * @brief Reset spy information for write()
   *
   */
  void reset()
  {
    spy_write.reset();
  }

  /// Spy handler for hal::dac::write()
  spy_handler<float> spy_write;

private:
  result<write_t> driver_write(float p_value) override
  {
    HAL_CHECK(spy_write.record(p_value));
    return write_t{};
  };
};
}  // namespace hal::mock
