#pragma once

#include <algorithm>
#include <array>
#include <chrono>
#include <ios>
#include <span>
#include <tuple>
#include <vector>

#include <libhal/error.hpp>

namespace hal {
/**
 * @brief Helper utility for making mocks for class functions that return
 * status.
 *
 * This class stores records of a functions call history in order to be
 * recovered later for inspection in tests and simulations.
 *
 * See pwm_mock.hpp and tests/pwm_mock.test.cpp as an example of how this is
 * done in practice.
 *
 * @tparam args_t - the arguments of the class function
 */
template<typename... args_t>
class spy_handler
{
public:
  /**
   * @brief Set the record function to return an error after a specified number
   * of recordings.
   *
   * @param p_call_count_before_trigger - how many calls before an error is
   * returned.
   */
  void trigger_error_on_call(int p_call_count_before_trigger)
  {
    if (p_call_count_before_trigger < 0) {
      throw std::range_error("trigger_error_on_call() must be 0 or above");
    }
    m_error_trigger = p_call_count_before_trigger;
  }

  /**
   * @brief Record the arguments of a function being spied on.
   *
   * @param p_args - arguments to record
   * @return status - success or failure
   * error trigger has been reached.
   */
  [[nodiscard]] status record(args_t... p_args)
  {
    m_call_history.push_back(std::make_tuple(p_args...));

    if (m_error_trigger > 1) {
      m_error_trigger--;
    } else if (m_error_trigger == 1) {
      m_error_trigger--;
      return hal::new_error();
    }

    return {};
  }

  /**
   * @brief Return the call history of the save function
   *
   * @return const auto& - reference to the call history vector
   */
  const auto& call_history() const
  {
    return m_call_history;
  }

  /**
   * @brief Return argument from one of call history parameters
   *
   * @param p_call - history call from 0 to N
   * @return const auto& - reference to the call history vector
   * @throws std::out_of_range - if p_call is beyond the size of call_history
   */
  template<size_t ArgumentIndex>
  const auto& history(size_t p_call) const
  {
    return std::get<ArgumentIndex>(m_call_history.at(p_call));
  }

  /**
   * @brief Reset call recordings and turns off error trigger
   *
   */
  void reset()
  {
    m_call_history.clear();
    m_error_trigger = 0;
  }

private:
  std::vector<std::tuple<args_t...>> m_call_history{};
  int m_error_trigger = 0;
};
}  // namespace hal

template<typename Rep, typename Period>
inline std::ostream& operator<<(
  std::ostream& p_os,
  const std::chrono::duration<Rep, Period>& p_duration)
{
  return p_os << p_duration.count() << " * (" << Period::num << "/"
              << Period::den << ")s";
}

template<typename T, size_t size>
inline std::ostream& operator<<(std::ostream& p_os,
                                const std::array<T, size>& p_array)
{
  p_os << "{";
  for (const auto& element : p_array) {
    p_os << element << ", ";
  }
  return p_os << "}\n";
}

template<typename T>
inline std::ostream& operator<<(std::ostream& p_os, const std::span<T>& p_array)
{
  p_os << "{";
  for (const auto& element : p_array) {
    p_os << element << ", ";
  }
  return p_os << "}\n";
}
