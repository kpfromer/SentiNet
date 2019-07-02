/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "TestUtils.hpp"
#include "common/descriptor/Ethernet.h"

// System includes
#include <pty.h>

/* The poller GtestHarness class
 */
class EthernetTest : public GtestHarness {
 public:
  void SetUp() {
    ::GtestHarness::SetUp();
  }

  void TearDown() {
    ::GtestHarness::TearDown();
  }
};

#if 0
/** The overloaded Serial class for the unit test.
 */
class EthernetTCPDescriptor : public descriptor::EthernetTCP {
 public:
  EthernetTCPDescriptor(const std::string& device, const uint32_t& baud) : EthernetTCP(device, baud) {
  }
};
#endif

/** Test that we are able to create a poller and that it's able to operate at the polling frequency.
 */
TEST_F(EthernetTest, HelperMethod) {
  // Check the EthenetMode helper method
  EXPECT_EQ(descriptor::EthernetMode::INVALID, descriptor::EthernetHelper::to_mode(""));
  EXPECT_EQ(descriptor::EthernetMode::INVALID, descriptor::EthernetHelper::to_mode(" "));
  EXPECT_EQ(descriptor::EthernetMode::INVALID, descriptor::EthernetHelper::to_mode("hello"));
  EXPECT_EQ(descriptor::EthernetMode::INVALID, descriptor::EthernetHelper::to_mode("11"));
  EXPECT_EQ(descriptor::EthernetMode::INVALID, descriptor::EthernetHelper::to_mode("1 "));
  EXPECT_EQ(descriptor::EthernetMode::INVALID, descriptor::EthernetHelper::to_mode(" 1"));
  EXPECT_EQ(descriptor::EthernetMode::INVALID, descriptor::EthernetHelper::to_mode("22"));
  EXPECT_EQ(descriptor::EthernetMode::INVALID, descriptor::EthernetHelper::to_mode("2 "));
  EXPECT_EQ(descriptor::EthernetMode::INVALID, descriptor::EthernetHelper::to_mode(" 2"));
  // Show that string case doesn't matter
  EXPECT_EQ(descriptor::EthernetMode::SERVER, descriptor::EthernetHelper::to_mode("SeRvEr"));
  EXPECT_EQ(descriptor::EthernetMode::SERVER, descriptor::EthernetHelper::to_mode("1"));
  EXPECT_EQ(descriptor::EthernetMode::CLIENT, descriptor::EthernetHelper::to_mode("ClIeNt"));
  EXPECT_EQ(descriptor::EthernetMode::CLIENT, descriptor::EthernetHelper::to_mode("2"));

  // Check the EthenetType helper method
  EXPECT_EQ(descriptor::EthernetType::INVALID, descriptor::EthernetHelper::to_type(""));
  EXPECT_EQ(descriptor::EthernetType::INVALID, descriptor::EthernetHelper::to_type(" "));
  EXPECT_EQ(descriptor::EthernetType::INVALID, descriptor::EthernetHelper::to_type("hello"));
  EXPECT_EQ(descriptor::EthernetType::INVALID, descriptor::EthernetHelper::to_type("11"));
  EXPECT_EQ(descriptor::EthernetType::INVALID, descriptor::EthernetHelper::to_type("1 "));
  EXPECT_EQ(descriptor::EthernetType::INVALID, descriptor::EthernetHelper::to_type(" 1"));
  EXPECT_EQ(descriptor::EthernetType::INVALID, descriptor::EthernetHelper::to_type("22"));
  EXPECT_EQ(descriptor::EthernetType::INVALID, descriptor::EthernetHelper::to_type("2 "));
  EXPECT_EQ(descriptor::EthernetType::INVALID, descriptor::EthernetHelper::to_type(" 2"));
  // Show that string case doesn't matter
  EXPECT_EQ(descriptor::EthernetType::TCP, descriptor::EthernetHelper::to_type("Tcp"));
  EXPECT_EQ(descriptor::EthernetType::TCP, descriptor::EthernetHelper::to_type("1"));
  EXPECT_EQ(descriptor::EthernetType::UDP, descriptor::EthernetHelper::to_type("uDp"));
  EXPECT_EQ(descriptor::EthernetType::UDP, descriptor::EthernetHelper::to_type("2"));
}
