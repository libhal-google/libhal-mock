#include <boost/ut.hpp>
#include <libhal-mock/servo.hpp>

namespace hal::mock {
void servo_mock_test()
{
  using namespace boost::ut;

  "hal::mock::servo::position()"_test = []() {
    // Setup
    hal::mock::servo mock;
    auto expected1 = float(0.5);
    auto expected2 = float(0.25);

    // Exercise
    auto result1 = mock.position(expected1);
    auto result2 = mock.position(expected2);

    // Verify
    expect(bool{ result1 });
    expect(bool{ result2 });
    expect(expected1 == std::get<0>(mock.spy_position.call_history().at(0)));
    expect(expected2 == std::get<0>(mock.spy_position.call_history().at(1)));
  };
};
}  // namespace hal::mock
