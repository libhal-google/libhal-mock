#include <cstdio>

#include <libhal-mock/pwm.hpp>

int main()
{
  using namespace hal::literals;

  int status = 0;
  hal::mock::pwm mock;

  hal::attempt_all(
    [&mock]() -> hal::status {
      HAL_CHECK(mock.frequency(10.0_kHz));

      HAL_CHECK(mock.duty_cycle(0.25));
      std::printf("Spy history #1: %f\n",
                  std::get<0>(mock.spy_duty_cycle.call_history().at(0)));
      HAL_CHECK(mock.duty_cycle(0.50));
      std::printf("Spy history #2: %f\n",
                  std::get<0>(mock.spy_duty_cycle.call_history().at(1)));
      HAL_CHECK(mock.duty_cycle(-0.25));
      std::printf("Spy history #3: %f\n",
                  std::get<0>(mock.spy_duty_cycle.call_history().at(2)));
      HAL_CHECK(mock.duty_cycle(-1.0));
      std::printf("Spy history #4: %f\n",
                  std::get<0>(mock.spy_duty_cycle.call_history().at(3)));

      return hal::success();
    },
    [&status]() {
      std::printf("Caught error successfully!\n");
      status = -1;
    });
  return status;
}