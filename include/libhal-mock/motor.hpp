#pragma once

#include "testing.hpp"
#include <libhal/motor.hpp>

namespace hal::mock {
/**
 * @brief Mock motor implementation for use in unit tests and simulations with a
 * spy function for power()
 *
 */
struct motor : public hal::motor
{
  /**
   * @brief Reset spy information for power()
   *
   */
  void reset()
  {
    spy_power.reset();
  }

  /// Spy handler for hal::motor::write()
  spy_handler<float> spy_power;

private:
  status driver_power(float p_power) override
  {
    return spy_power.record(p_power);
  };
};
}  // namespace hal::mock
