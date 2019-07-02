/** Auto-generated on { now }.  Do NOT modify by hand, changes will be overwritten next time message are generated.
 *
 *  Some Kelvin specific source code header describing the use and what license it might be released under.
 */
#ifndef TEST_KELVIN_MESSAGES_HPP
#define TEST_KELVIN_MESSAGES_HPP

// Kelvin includes
#include "messages/Kelvin/AllMessageRandomizers.h"
#include "TestUtils.hpp"

// The custom GTEST harness for the Kelvin testing
class KelvinMessageTest : public GtestHarness {
 public:
  void SetUp() {
    ::GtestHarness::SetUp();
  }

  void TearDown() {
    ::GtestHarness::TearDown();
  }
};

/** Create a Kelvin message wrapper to test out the message interface.
 */
class KelvinHealthAndStatusMessage : public Randomizer::Kelvin::HealthAndStatus {
 public:
  KelvinHealthAndStatusMessage() {
  }
};

/** Create a Kelvin message wrapper to test out the message interface.
 */
class KelvinRequestMessage : public Randomizer::Kelvin::Request {
 public:
  KelvinRequestMessage() {
  }
};

#endif /* TEST_KELVIN_MESSAGES_HPP */