#include <libhal-util/input_pin.hpp>

#include <deque>

#include <libhal-mock/input_pin.hpp>

#include <boost/ut.hpp>

namespace hal::mock {
void input_pin_mock_test()
{
  using namespace boost::ut;

  "hal::mock::input_pin::configure()"_test = []() {
    // Setup
    constexpr hal::input_pin::settings mock_settings_default{};
    constexpr hal::input_pin::settings mock_settings_custom{
      .resistor = pin_resistor::pull_down,
    };
    hal::mock::input_pin mock;

    // Exercise
    auto result1 = mock.configure(mock_settings_default);
    auto result2 = mock.configure(mock_settings_custom);

    // Verify
    expect(bool{ result1 });
    expect(mock_settings_default ==
           std::get<0>(mock.spy_configure.call_history().at(0)));
    expect(bool{ result2 });
    expect(mock_settings_custom ==
           std::get<0>(mock.spy_configure.call_history().at(1)));
  };
  "hal::mock::input_pin::set() + level()"_test = []() {
    // Setup
    hal::mock::input_pin mock;
    std::deque inputs{
      input_pin::level_t{ .state = true },
      input_pin::level_t{ .state = false },
      input_pin::level_t{ .state = true },
    };
    std::queue queue(inputs);

    // Exercise
    mock.set(queue);

    // Verify
    expect(that % true == mock.level().value().state);
    expect(that % false == mock.level().value().state);
    expect(that % true == mock.level().value().state);
    expect(!bool{ mock.level() });
  };
  "hal::mock::input_pin::reset()"_test = []() {
    // Setup
    constexpr hal::input_pin::settings mock_settings_default{};
    hal::mock::input_pin mock;
    (void)mock.configure(mock_settings_default);
    expect(that % 1 == mock.spy_configure.call_history().size());

    // Exercise
    mock.reset();

    // Verify
    expect(that % 0 == mock.spy_configure.call_history().size());
  };
};
}  // namespace hal::mock
