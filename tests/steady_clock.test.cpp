#include <libhal-mock/steady_clock.hpp>

#include <deque>

#include <boost/ut.hpp>

namespace hal::mock {
void steady_clock_mock_test()
{
  using namespace boost::ut;

  // Setup
  auto expected1 = steady_clock::uptime_t{ .ticks = 10 };
  auto expected2 = steady_clock::uptime_t{ .ticks = 50 };
  auto expected3 = steady_clock::uptime_t{ .ticks = 100 };
  auto expected_frequency =
    steady_clock::frequency_t{ .operating_frequency = 1.0_Hz };
  hal::mock::steady_clock mock;
  std::deque uptimes{ expected1, expected2, expected3 };
  std::queue queue(uptimes);

  // Exercise
  mock.set_frequency(expected_frequency);
  mock.set_uptimes(queue);
  auto result0 = mock.frequency().value();
  auto result1 = mock.uptime().value();
  auto result2 = mock.uptime().value();
  auto result3 = mock.uptime().value();

  // Verify
  expect(that % expected_frequency.operating_frequency ==
         result0.operating_frequency);
  expect(that % expected1.ticks == result1.ticks);
  expect(that % expected2.ticks == result2.ticks);
  expect(that % expected3.ticks == result3.ticks);
  expect(!bool{ mock.uptime() });
};
}  // namespace hal::mock
