/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "TestUtils.hpp"
#include "common/descriptor/Serial.h"
#include "common/Utils.hpp"

// System includes
#include <pty.h>

/* The poller GtestHarness class
 */
class SerialTest : public GtestHarness {
 public:
  void SetUp() {
    ::GtestHarness::SetUp();
    open_pty();
  }

  void TearDown() {
    ::GtestHarness::TearDown();
    close_pty();
  }

 public:
  // Opens up a serial pseudo-terminals
  void open_pty() {
    EXPECT_EQ(0, openpty(&master, &slave, device_name, NULL, NULL));
  }

  // Closes the serial pseudo-terminals
  void close_pty() {
    EXPECT_EQ(0, close(master));
    EXPECT_EQ(0, close(slave));
  }

 public:
  int master;
  int slave;
  char device_name[256];
};

/** The overloaded Serial class for the unit test.
 */
class SerialDescriptor : public descriptor::Serial {
 public:
  SerialDescriptor(const std::string& device, const uint32_t& baud) : Serial(device, baud) {
  }
};

/** Test that we are able to create a serial device and that it's able to send and received.
 */
TEST_F(SerialTest, Initialize) {
  // EXPECT_THROW(std::make_unique<SerialDescriptor>("", 9600U), RuntimeException);
  // EXPECT_THROW(std::make_unique<SerialDescriptor>("/dev/null", 9600U), RuntimeException);

  // Iterate over the available baud rates
  auto baud_rates = {0U,    50U,   75U,   110U,  134U,  150U,   200U,   300U,   600U,
                     1200U, 1800U, 2400U, 4800U, 9600U, 19200U, 38400U, 57600U, 115200U};
  for(const auto& baud_rate : baud_rates) {
    auto serial = std::make_unique<SerialDescriptor>(device_name, baud_rate);

    // Iterate a bunch of times and write data over the pseudo-terminal
    for(auto i = 0; i < 100; ++i) {
      std::string buffer(100, '\0');
      std::string data = utils::get_random_string(20, buffer.size());

      EXPECT_EQ(static_cast<int32_t>(strlen(data.c_str())), write(master, data.c_str(), strlen(data.c_str())));
      EXPECT_EQ(static_cast<int32_t>(strlen(data.c_str())), read(serial->get_fd(), &buffer[0], buffer.size()));
      buffer.resize(strlen(buffer.c_str()));
      EXPECT_TRUE(utils::is_equal(data, buffer));
    }
  }
}
