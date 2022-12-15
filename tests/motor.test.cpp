#include <boost/ut.hpp>
#include <libhal-mock/motor.hpp>

namespace hal::mock {
void motor_mock_test()
{
  using namespace boost::ut;

  // Setup
  hal::mock::motor mock;
  constexpr auto expected1 = float(0.5);
  constexpr auto expected2 = float(0.25);
  mock.spy_power.trigger_error_on_call(3);

  // Exercise + Verify
  expect(bool{ mock.power(expected1) });
  expect(expected1 == std::get<0>(mock.spy_power.call_history().at(0)));

  expect(bool{ mock.power(expected2) });
  expect(expected2 == std::get<0>(mock.spy_power.call_history().at(1)));

  expect(!mock.power(expected2));
  expect(expected2 == std::get<0>(mock.spy_power.call_history().at(2)));
};
}  // namespace hal::mock
