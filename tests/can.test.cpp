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

#include <libhal-util/can.hpp>

#include <libhal-mock/can.hpp>

#include <boost/ut.hpp>

namespace hal::mock {
void can_mock_test()
{
  using namespace boost::ut;
  "hal::can::configure tests"_test = []() {
    // Setup
    hal::mock::can mock;
    constexpr hal::can::settings expected1{};
    constexpr hal::can::settings expected2{
      .baud_rate = 1.0_Hz,
    };
    mock.spy_configure.trigger_error_on_call(3);

    // Exercise + Verify
    mock.configure(expected1);
    expect(expected1 == std::get<0>(mock.spy_configure.call_history().at(0)));

    mock.configure(expected2);
    expect(expected2 == std::get<0>(mock.spy_configure.call_history().at(1)));

    throws([&]() { mock.configure(expected2); });
    expect(expected2 == std::get<0>(mock.spy_configure.call_history().at(2)));
  };

  "hal::can::send tests"_test = []() {
    // Setup
    hal::mock::can mock;
    constexpr hal::can::message_t expected1{ .id = 1,
                                             .payload = { 'a' },
                                             .length = 1 };
    constexpr hal::can::message_t expected2{ .id = 2,
                                             .payload = { 'b' },
                                             .length = 1 };
    mock.spy_send.trigger_error_on_call(3);

    // Exercise + Verify
    mock.send(expected1);
    expect(that % expected1.payload ==
           std::get<0>(mock.spy_send.call_history().at(0)).payload);
    expect(that % expected1.id ==
           std::get<0>(mock.spy_send.call_history().at(0)).id);

    mock.send(expected2);
    expect(that % expected2.payload ==
           std::get<0>(mock.spy_send.call_history().at(1)).payload);
    expect(that % expected2.id ==
           std::get<0>(mock.spy_send.call_history().at(1)).id);

    throws([&]() { mock.send(expected2); });
    expect(that % expected2.payload ==
           std::get<0>(mock.spy_send.call_history().at(2)).payload);
    expect(that % expected2.id ==
           std::get<0>(mock.spy_send.call_history().at(2)).id);
  };

  "hal::can::on_receive tests"_test = []() {
    // Setup
    hal::mock::can mock;
    int counter = 0;
    constexpr hal::can::message_t expected_message{ .id = 1,
                                                    .payload = { 'a' },
                                                    .length = 1 };
    const hal::callback<hal::can::handler> expected1 =
      [&counter](const hal::can::message_t) { counter++; };
    const hal::callback<hal::can::handler> expected2 =
      [&counter](hal::can::message_t) { counter--; };
    mock.spy_on_receive.trigger_error_on_call(3);

    // Exercise + Verify
    mock.on_receive(expected1);
    auto handler1 = std::get<0>(mock.spy_on_receive.call_history().at(0));
    handler1(expected_message);
    expect(that % 1 == counter);

    mock.on_receive(expected2);
    auto handler2 = std::get<0>(mock.spy_on_receive.call_history().at(1));
    handler2(expected_message);
    expect(that % 0 == counter);

    mock.on_receive(expected2);
    auto handler3 = std::get<0>(mock.spy_on_receive.call_history().at(2));
    handler3(expected_message);
    expect(that % -1 == counter);
  };
};
}  // namespace hal::mock
