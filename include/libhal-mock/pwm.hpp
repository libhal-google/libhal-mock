#pragma once

#include "testing.hpp"
#include <libhal/pwm.hpp>

namespace hal::mock {
/**
 * @brief Mock pwm implementation for use in unit tests and simulations with spy
 * functions for frequency() and duty_cycle().
 *
 */
struct pwm : public hal::pwm
{
  /**
   * @brief Reset spy information for both frequency() and duty_cycle()
   *
   */
  void reset()
  {
    spy_frequency.reset();
    spy_duty_cycle.reset();
  }

  /// Spy handler for hal::pwm::frequency()
  spy_handler<hertz> spy_frequency;
  /// Spy handler for hal::pwm::duty_cycle()
  spy_handler<float> spy_duty_cycle;

private:
  result<frequency_t> driver_frequency(hertz p_settings) override
  {
    HAL_CHECK(spy_frequency.record(p_settings));
    return frequency_t{};
  }

  result<duty_cycle_t> driver_duty_cycle(float p_duty_cycle) override
  {
    HAL_CHECK(spy_duty_cycle.record(p_duty_cycle));

    return duty_cycle_t{};
  }
};
}  // namespace hal::mock
