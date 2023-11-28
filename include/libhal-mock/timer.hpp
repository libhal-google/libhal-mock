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
#include <libhal/functional.hpp>
#include <libhal/timer.hpp>

namespace hal::mock {
/**
 * @brief Mock timer implementation for use in unit tests and simulations with
 * spy functions for schedule(), clear(), and is_running()
 *
 */
struct timer : public hal::timer
{
  /**
   * @brief Reset spy information for schedule(), clear(), and is_running()
   *
   */
  void reset()
  {
    spy_schedule.reset();
    spy_cancel.reset();
    spy_is_running.reset();
  }

  /// Spy handler for hal::timer::schedule()
  spy_handler<std::function<void(void)>, std::chrono::nanoseconds> spy_schedule;
  /// Spy handler for hal::timer::is_running()
  spy_handler<bool> spy_is_running;
  /// Spy handler for hal::timer::clear()
  spy_handler<bool> spy_cancel;

private:
  schedule_t driver_schedule(hal::callback<void(void)> p_callback,
                             std::chrono::nanoseconds p_delay) override
  {
    m_is_running = true;
    spy_schedule.record(p_callback, p_delay);
    return schedule_t{};
  }

  is_running_t driver_is_running() override
  {
    spy_is_running.record(true);
    return is_running_t{ .is_running = m_is_running };
  }

  cancel_t driver_cancel() override
  {
    m_is_running = false;
    spy_cancel.record(true);
    return cancel_t{};
  }

  bool m_is_running = false;
};
}  // namespace hal::mock
