#pragma once

#include "testing.hpp"
#include <libhal/alias.hpp>
#include <libhal/can.hpp>

namespace hal::mock {
/**
 * @brief Mock can implementation for use in unit tests and simulations
 *
 */
struct can : public hal::can
{
  /**
   * @brief Reset spy information for functions
   *
   */
  void reset()
  {
    spy_configure.reset();
    spy_send.reset();
    spy_on_receive.reset();
  }

  /// Spy handler for hal::can::configure()
  spy_handler<settings> spy_configure;
  /// Spy handler for hal::can::send()
  spy_handler<message_t> spy_send;
  /// Spy handler for hal::can::on_receive()
  spy_handler<std::function<handler>> spy_on_receive;

private:
  status driver_configure(const settings& p_settings) override
  {
    return spy_configure.record(p_settings);
  };
  status driver_send(const message_t& p_message) override
  {
    return spy_send.record(p_message);
  };
  status driver_on_receive(hal::function_ref<handler> p_handler) override
  {
    return spy_on_receive.record(p_handler);
  };
};
}  // namespace hal::mock

/**
 * @brief print can::message_t type using ostreams
 *
 * Meant for unit testing, testing and simulation purposes
 * C++ streams, in general, should not be used for any embedded project that
 * will ever have to be used on an MCU due to its memory cost.
 *
 * @tparam CharT - character type
 * @tparam Traits - ostream traits type
 * @param p_ostream - the ostream
 * @param p_message - object to convert to a string
 * @return std::basic_ostream<CharT, Traits>& - reference to the ostream
 */
template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(
  std::basic_ostream<CharT, Traits>& p_ostream,
  const hal::can::message_t& p_message)
{
  p_ostream << "{ id: " << std::hex << "0x" << p_message.id;
  p_ostream << ", length: " << std::dec << unsigned{ p_message.length };
  p_ostream << ", is_remote_request: " << p_message.is_remote_request;
  p_ostream << ", payload = [";
  for (const auto& element : p_message.payload) {
    p_ostream << std::hex << "0x" << unsigned{ element } << ", ";
  }
  p_ostream << "] }";
  return p_ostream;
}
