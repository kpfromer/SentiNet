/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "TestUtils.hpp"
#include "common/descriptor/Utils.hpp"

// System includes
#include <pty.h>

/* The descriptor utilities GtestHarness class
 */
class DescriptorUtilsTest : public GtestHarness {
 public:
  void SetUp() {
    ::GtestHarness::SetUp();
  }

  void TearDown() {
    ::GtestHarness::TearDown();
  }
};

/** Test that we are able to create a descriptor devices from a configuration file.
 */
TEST_F(DescriptorUtilsTest, FromConfiguration) {
  // Invalid configurations
  {
    auto logger = LoggingInterface::get_instance();
    auto count = 0U;
    ConfigurationInterface cfg;

    EXPECT_EQ(count, logger->set_error_cnt(count));
    EXPECT_EQ(count, logger->get_error_cnt());

    EXPECT_THROW(descriptor::utils::from_configuration(cfg), std::exception);
    count = logger->get_error_cnt();

    cfg["hello"] = "still not very useful";
    EXPECT_THROW(descriptor::utils::from_configuration(cfg), std::exception);
    count = logger->get_error_cnt();

    cfg["type"] = "This isn't much of a descriptor";
    EXPECT_THROW(descriptor::utils::from_configuration(cfg), std::exception);
    count = logger->get_error_cnt();
  }

  // Serial configuration
  {
    ConfigurationInterface cfg;
    cfg["type"] = "SERial";

    // Some edge cases
    EXPECT_THROW(descriptor::utils::from_configuration(cfg), std::exception);
    cfg["device"] = "/dev/null";
    EXPECT_THROW(descriptor::utils::from_configuration(cfg), std::exception);
    cfg["baud"] = 38400U;
    EXPECT_THROW(descriptor::utils::from_configuration(cfg), std::exception);

    // Check that a Serial descriptor is returned .. we don't care that this is a file and not a tty for this test
    cfg["device"] = UNITTEST_DIR;
    auto ptr = descriptor::utils::from_configuration(cfg);
    ASSERT_NE(nullptr, ptr);
    EXPECT_NE(nullptr, dynamic_cast<descriptor::Serial*>(ptr.get()));
    EXPECT_EQ(nullptr, dynamic_cast<descriptor::Ethernet*>(ptr.get()));
  }

  // Poller configuration
  {
    ConfigurationInterface cfg;
    cfg["type"] = "polLER";

    // Some edge cases
    EXPECT_THROW(descriptor::utils::from_configuration(cfg), std::exception);
    cfg["period"] = "/dev/null";
    EXPECT_THROW(descriptor::utils::from_configuration(cfg), std::exception);

    // Check that a Poller descriptor is returned
    cfg["period"] = 0.01;
    auto ptr = descriptor::utils::from_configuration(cfg);
    ASSERT_NE(nullptr, ptr);
    EXPECT_NE(nullptr, dynamic_cast<descriptor::Poller*>(ptr.get()));
    EXPECT_EQ(nullptr, dynamic_cast<descriptor::Serial*>(ptr.get()));
  }

  // Ethernet configuration
  {
    ConfigurationInterface cfg;
    cfg["type"] = "EtHeRnEt";

    // Some edge cases
    EXPECT_THROW(descriptor::utils::from_configuration(cfg), std::exception);
    cfg["mode"] = "madeup";
    EXPECT_THROW(descriptor::utils::from_configuration(cfg), std::exception);
    cfg["mode"] = "server";
    cfg["protocol"] = "madeup";
    EXPECT_THROW(descriptor::utils::from_configuration(cfg), std::exception);
    cfg["mode"] = "server";
    cfg["protocol"] = "tcp";
    cfg["ip"] = "madeup";
    EXPECT_THROW(descriptor::utils::from_configuration(cfg), std::exception);
    cfg["mode"] = "server";
    cfg["protocol"] = "tcp";
    cfg["ip"] = "127.0.0.1";
    cfg["port"] = "madeup";
    EXPECT_THROW(descriptor::utils::from_configuration(cfg), std::exception);

    // Check that a Ethernet descriptor is returned
    cfg["port"] = 8877U;
    auto ptr = descriptor::utils::from_configuration(cfg);
    ASSERT_NE(nullptr, ptr);
    EXPECT_NE(nullptr, dynamic_cast<descriptor::Ethernet*>(ptr.get()));
    EXPECT_EQ(nullptr, dynamic_cast<descriptor::Poller*>(ptr.get()));
  }

  // File configuration
  {
    ConfigurationInterface cfg;
    cfg["type"] = "FiLe";

    // Some edge cases
    EXPECT_THROW(descriptor::utils::from_configuration(cfg), std::exception);
    cfg["filename"] = "may the force be with you";
    EXPECT_THROW(descriptor::utils::from_configuration(cfg), std::exception);

    // Check that a File descriptor is returned
    cfg["filename"] = "run_unit_tests";
    auto ptr = descriptor::utils::from_configuration(cfg);
    ASSERT_NE(nullptr, ptr);
    EXPECT_NE(nullptr, dynamic_cast<descriptor::File*>(ptr.get()));
    EXPECT_EQ(nullptr, dynamic_cast<descriptor::Serial*>(ptr.get()));
  }
}
