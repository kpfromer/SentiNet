/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "Descriptors.h"
#include "common/descriptor/Poller.h"

/* The poller GtestHarness class
 */
class PollerTest : public DescriptorTest {};

/** The overloaded Poller class for the unit test.
 */
class PollerDescriptor : public descriptor::Poller {
 public:
  PollerDescriptor(const std::chrono::nanoseconds& period, const std::chrono::nanoseconds& delay)
      : Poller(period, delay) {
  }
};

/** Test that we are able to create a poller and that it's able to operate at the polling frequency.
 */
#if 0
// @todo [KEMB-703] Reeable the test when there are expected performance tuning for the different machines.
TEST_F(PollerTest, Initialize) {
  // Iterate over a couple of timing periods and validate that the timer is within a threshold
  for(const auto usecs : {1000, 2000, 4000, 8000, 10000, 25000, 50000}) {
    auto poller = std::make_unique<PollerDescriptor>(std::chrono::microseconds(usecs), std::chrono::seconds(0));
    // Clear the timer and prepare it
    poller->update();
    // Run the timer for a bunch of iterations and compare
    /// @todo [KEMB-545] Make this a parameter so that it can be executed on target to get timing information
    for(auto i = 0U; i < 10; i++) {
      // Start the local timer
      auto start_time = std::chrono::system_clock::now();
      // Wait for the timer to expire
      poller->update();
      // Compute the duration
      auto duration =
          std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - start_time).count();
      // The duration should be within +/-0.5 msec of the intended timer expiration
      EXPECT_LE(usecs - 500, duration);
      EXPECT_GT(usecs + 500, duration);
    }
  }
}
#endif
