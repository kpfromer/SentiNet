/** Auto-generated on { now }.  Do NOT modify by hand, changes will be overwritten next time message are generated.
 *
 *  Some Kelvin specific source code header describing the use and what license it might be released under.
 */
#ifndef TEST_COMMON_MESSAGES_HPP
#define TEST_COMMON_MESSAGES_HPP

// Kelvin includes
#include "messages/Common/AllMessageRandomizers.h"
#include "TestUtils.hpp"

// The custom GTEST harness for the Common testing
class CommonMessageTest : public GtestHarness {
 public:
  void SetUp() {
    ::GtestHarness::SetUp();
  }

  void TearDown() {
    ::GtestHarness::TearDown();
  }
};

/** Create a Common message wrapper to test out the message interface.
 */
class CommonEventMessage : public Randomizer::Common::Event {
 public:
  CommonEventMessage() {
  }
};

/** Create a Common message wrapper to test out the message interface.
 */
class CommonLocationMessage : public Randomizer::Common::Location {
 public:
  CommonLocationMessage() {
  }
};

#endif /* TEST_COMMON_MESSAGES_HPP */