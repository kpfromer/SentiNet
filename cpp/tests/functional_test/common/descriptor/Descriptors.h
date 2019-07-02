/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */
#ifndef TEST_COMMON_DESCRIPTOR_H
#define TEST_COMMON_DESCRIPTOR_H

// Kelvin includes
#include "TestUtils.hpp"

// The custom GTEST harness for the application base class implementation testing
class DescriptorTest : public GtestHarness {
 public:
  void SetUp() {
    ::GtestHarness::SetUp();
  }

  void TearDown() {
    ::GtestHarness::TearDown();
  }
};

#endif /* TEST_COMMON_DESCRIPTOR_H */
