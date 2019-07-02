/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */
#ifndef TEST_COMMON_CRC_HPP
#define TEST_COMMON_CRC_HPP

// Kelvin includes
#include "TestUtils.hpp"

// The custom GTEST harness for the application base class implementaton testing
class CRCTest : public GtestHarness {
 public:
  void SetUp() {
    ::GtestHarness::SetUp();
  }

  void TearDown() {
    ::GtestHarness::TearDown();
  }
};

#endif /* TEST_COMMON_CRC_HPP */
