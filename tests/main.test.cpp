namespace hal::mock {
extern void adc_mock_test();
extern void can_mock_test();
extern void dac_mock_test();
extern void input_pin_mock_test();
extern void interrupt_pin_mock_test();
extern void motor_mock_test();
extern void output_pin_mock_test();
extern void pwm_mock_test();
extern void servo_mock_test();
extern void spi_mock_test();
extern void steady_clock_mock_test();
extern void testing_utilities_test();
extern void timer_mock_test();
}  // namespace hal::mock

int main()
{
  hal::mock::adc_mock_test();
  hal::mock::can_mock_test();
  hal::mock::dac_mock_test();
  hal::mock::input_pin_mock_test();
  hal::mock::interrupt_pin_mock_test();
  hal::mock::motor_mock_test();
  hal::mock::output_pin_mock_test();
  hal::mock::pwm_mock_test();
  hal::mock::servo_mock_test();
  hal::mock::spi_mock_test();
  hal::mock::steady_clock_mock_test();
  hal::mock::testing_utilities_test();
  hal::mock::timer_mock_test();
}
