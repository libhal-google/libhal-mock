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
  result<frequency_t> driver_frequency()
  {
    return m_frequency;
  }

  result<uptime_t> driver_uptime()
  {
    if (m_uptime_values.size() == 0) {
      return hal::new_error(std::out_of_range("uptimes queue is empty!"));
    }
    auto m_current_value = m_uptime_values.front();
    m_uptime_values.pop();
    return m_current_value;
  }

  frequency_t m_frequency{ .operating_frequency = 1.0_Hz };
  std::queue<uptime_t> m_uptime_values{};
};
}  // namespace hal::mock
