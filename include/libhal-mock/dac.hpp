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
  status driver_write(float p_value) override
  {
    return spy_write.record(p_value);
  };
};
}  // namespace hal::mock
