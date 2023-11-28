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

#include <algorithm>
#include <array>
#include <chrono>
#include <ios>
#include <span>
#include <string_view>
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
   * thrown.
   */
  void trigger_error_on_call(std::uint32_t p_call_count_before_trigger)
  {
    m_error_trigger = p_call_count_before_trigger;
  }

  /**
   * @brief Record the arguments of a function being spied on.
   *
   * @param p_args - arguments to record
   * @throws std::runtime_error - when the error trigger is reached
   */
  void record(args_t... p_args)
  {
    m_call_history.push_back(std::make_tuple(p_args...));

    if (m_error_trigger > 1) {
      m_error_trigger--;
    }

    if (m_error_trigger == 0) {
      throw std::runtime_error("record error trigger reached!");
    }
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
  std::uint32_t m_error_trigger = 0;
};
}  // namespace hal

template<typename Rep, typename Period>
inline std::ostream& operator<<(
  std::ostream& p_os,
  const std::chrono::duration<Rep, Period>& p_duration)
{
  using namespace std::literals;
  p_os << p_duration.count() << " * ("sv << Period::num << "/"sv << Period::den
       << ")s"sv;
}

template<typename T, size_t size>
inline std::ostream& operator<<(std::ostream& p_os,
                                const std::array<T, size>& p_array)
{
  using namespace std::literals;
  p_os << "{"sv;
  for (const auto& element : p_array) {
    p_os << element << ", "sv;
  }
  return p_os << "}\n"sv;
}

template<typename T>
inline std::ostream& operator<<(std::ostream& p_os, const std::span<T>& p_array)
{
  using namespace std::literals;
  p_os << "{"sv;
  for (const auto& element : p_array) {
    p_os << element << ", "sv;
  }
  return p_os << "}\n"sv;
}
