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

#include <queue>

#include <libhal/steady_clock.hpp>

namespace hal::mock {
/**
 * @brief mock steady_clock implementation for use in unit tests and
 * simulations.
 *
 */
struct steady_clock : public hal::steady_clock
{
  /**
   * @brief Set the frequency to be returned from frequency()
   *
   * @param p_frequency - Frequency to return
   */
  void set_frequency(frequency_t p_frequency)
  {
    m_frequency = p_frequency;
  }

  /**
   * @brief Queues the uptimes to be returned from uptimes()
   *
   * @param p_uptime_values - Queue of uptimes
   */
  void set_uptimes(std::queue<uptime_t>& p_uptime_values)
  {
    m_uptime_values = p_uptime_values;
  }

private:
  frequency_t driver_frequency()
  {
    return m_frequency;
  }

  uptime_t driver_uptime()
  {
    if (m_uptime_values.size() == 0) {
      return m_last_uptime;
    }

    m_last_uptime = m_uptime_values.front();
    m_uptime_values.pop();
    return m_last_uptime;
  }

  frequency_t m_frequency{ .operating_frequency = 1.0_Hz };
  std::queue<uptime_t> m_uptime_values{};
  uptime_t m_last_uptime{};
};
}  // namespace hal::mock
