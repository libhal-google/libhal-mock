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
  result<schedule_t> driver_schedule(hal::callback<void(void)> p_callback,
                                     std::chrono::nanoseconds p_delay) override
  {
    m_is_running = true;
    HAL_CHECK(spy_schedule.record(p_callback, p_delay));
    return schedule_t{};
  }

  result<is_running_t> driver_is_running() override
  {
    auto result = spy_is_running.record(true);
    if (!result) {
      return result.error();
    }
    return is_running_t{ .is_running = m_is_running };
  }

  result<cancel_t> driver_cancel() override
  {
    m_is_running = false;

    HAL_CHECK(spy_cancel.record(true));

    return cancel_t{};
  }

  bool m_is_running = false;
};
}  // namespace hal::mock
