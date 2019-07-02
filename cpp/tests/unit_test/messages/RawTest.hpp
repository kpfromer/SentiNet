/** Auto-generated on { now }.  Do NOT modify by hand, changes will be overwritten next time message are generated.
 *
 *  Some Kelvin specific source code header describing the use and what license it might be released under.
 */
#ifndef TEST_RAW_MESSAGES_HPP
#define TEST_RAW_MESSAGES_HPP

// Kelvin includes
#include "messages/Raw/AllMessageRandomizers.h"
#include "TestUtils.hpp"

// The custom GTEST harness for the Raw testing
class RawMessageTest : public GtestHarness {
 public:
  void SetUp() {
    ::GtestHarness::SetUp();
  }

  void TearDown() {
    ::GtestHarness::TearDown();
  }
};

/** Create a Raw message wrapper to test out the message interface.
 */
class RawDoubleMessage : public Randomizer::Raw::Double {
 public:
  RawDoubleMessage() {
  }
};

/** Create a Raw message wrapper to test out the message interface.
 */
class RawFloatMessage : public Randomizer::Raw::Float {
 public:
  RawFloatMessage() {
  }
};

/** Create a Raw message wrapper to test out the message interface.
 */
class RawInt32Message : public Randomizer::Raw::Int32 {
 public:
  RawInt32Message() {
  }
};

/** Create a Raw message wrapper to test out the message interface.
 */
class RawInt64Message : public Randomizer::Raw::Int64 {
 public:
  RawInt64Message() {
  }
};

/** Create a Raw message wrapper to test out the message interface.
 */
class RawStringMessage : public Randomizer::Raw::String {
 public:
  RawStringMessage() {
  }
};

#endif /* TEST_RAW_MESSAGES_HPP */