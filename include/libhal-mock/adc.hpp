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

#include <libhal/adc.hpp>
#include <libhal/error.hpp>

namespace hal::mock {
/**
 * @brief Mock adc implementation for use in unit tests and simulations.
 */
struct adc : public hal::adc
{
  /**
   * @brief Queues the floats to be returned for read()
   *
   * @param p_adc_values - queue of floats
   */
  void set(std::queue<read_t>& p_adc_values)
  {
    m_adc_values = p_adc_values;
  }

private:
  /**
   * @return read_t - value popped off of the adc queue
   * @throws std::out_of_range - error when the adc queue is empty
   */
  read_t driver_read() override
  {
    if (m_adc_values.empty()) {
      throw std::out_of_range("floats queue is empty!");
    }
    auto m_current_value = m_adc_values.front();
    m_adc_values.pop();
    return m_current_value;
  }

  std::queue<read_t> m_adc_values{};
};
}  // namespace hal::mock
