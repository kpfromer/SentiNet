/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "TestUtils.hpp"
#include "common/performance/Timing.h"

// C++ includes
#include <numeric>

class PerformanceTimingTest : public GtestHarness {};

/** The templatized version of the constructor to test a bunch of different types.
 */
template <typename T>
void Constructor() {
  {
    // Default constructor
    const auto perf = std::make_unique<performance::Timing<T>>();
    EXPECT_TRUE(utils::is_equal("", perf->get_filename()));
    EXPECT_EQ(performance::TimingDefault::ENABLED, perf->get_enabled());
  }

  {
    // The filename constructor
    const auto filename = utils::join({UNITTEST_DIR, "hello_world.perf"}, "/");
    auto perf = std::make_unique<performance::Timing<T>>(filename);
    EXPECT_TRUE(utils::is_equal(filename, perf->get_filename()));
    EXPECT_EQ(performance::TimingDefault::ENABLED, perf->get_enabled());
    perf->set_enabled(!perf->get_enabled());
    EXPECT_NE(performance::TimingDefault::ENABLED, perf->get_enabled());
  }

  {
    // The filename and enabled constructor
    const auto filename = utils::join({UNITTEST_DIR, "hello_world.perf"}, "/");
    auto perf = std::make_unique<performance::Timing<T>>(filename, !performance::TimingDefault::ENABLED);
    EXPECT_TRUE(utils::is_equal(filename, perf->get_filename()));
    EXPECT_NE(performance::TimingDefault::ENABLED, perf->get_enabled());
    perf->set_enabled(!perf->get_enabled());
    EXPECT_EQ(performance::TimingDefault::ENABLED, perf->get_enabled());
  }

  {
    // The filename and enabled constructor as a parent class
    const auto filename = utils::join({UNITTEST_DIR, "hello_world.perf"}, "/");
    std::unique_ptr<performance::TimingInterface<T>> parent =
        std::make_unique<performance::Timing<T>>(filename, !performance::TimingDefault::ENABLED);
    auto perf = dynamic_cast<performance::Timing<T>*>(parent.get());
    EXPECT_TRUE(utils::is_equal(filename, perf->get_filename()));
    EXPECT_NE(performance::TimingDefault::ENABLED, parent->get_enabled());
    parent->set_enabled(!parent->get_enabled());
    EXPECT_EQ(performance::TimingDefault::ENABLED, parent->get_enabled());
  }
}

/** The templatized version to test a bunch of different types.
 */
template <typename T>
void Stdout() {
  for(const T id : {static_cast<T>(123), std::numeric_limits<T>::min(), std::numeric_limits<T>::max()}) {
    auto perf = std::make_unique<performance::Timing<decltype(id)>>("", true);
    auto parent = dynamic_cast<performance::TimingInterface<decltype(id)>*>(perf.get());
    auto output = testing::internal::GetCapturedStdout();
    LoggingInterface::get_instance()->set_logging_level(LoggingDefault::Level::DEBUG);

    // Write out performance data
    for(const auto type :
        {performance::TimingDefault::TimingType::INVALID, performance::TimingDefault::TimingType::START,
         performance::TimingDefault::TimingType::STOP}) {
      testing::internal::CaptureStdout();
      perf->add_entry(id, type);
      output = testing::internal::GetCapturedStdout();
      EXPECT_LT(0U, output.size()) << output;
      perf->set_enabled(false);

      testing::internal::CaptureStdout();
      perf->add_entry(id, type);
      output = testing::internal::GetCapturedStdout();
      EXPECT_EQ(0U, output.size()) << output;
      perf->set_enabled(true);

      // Now with a parent class pointer
      testing::internal::CaptureStdout();
      parent->add_entry(id, type);
      output = testing::internal::GetCapturedStdout();
      EXPECT_LT(0U, output.size()) << output;
      parent->set_enabled(false);

      testing::internal::CaptureStdout();
      parent->add_entry(id, type);
      output = testing::internal::GetCapturedStdout();
      EXPECT_EQ(0U, output.size()) << output;
      parent->set_enabled(true);
    }

    // Need to return the stdout state so that the parent class can handle it
    testing::internal::CaptureStdout();
  }
}

/** The templatized version to test a bunch of different types.
 */
template <typename T>
void Fileout() {
  for(const T id : {static_cast<T>(123), std::numeric_limits<T>::min(), std::numeric_limits<T>::max()}) {
    const auto filename = utils::join({UNITTEST_DIR, "timing.perf"}, "/");
    auto perf = std::make_unique<performance::Timing<decltype(id)>>(filename, true);
    auto output = testing::internal::GetCapturedStdout();
    LoggingInterface::get_instance()->set_logging_level(LoggingDefault::Level::DEBUG);

    // Write out performance data
    for(const auto type :
        {performance::TimingDefault::TimingType::INVALID, performance::TimingDefault::TimingType::START,
         performance::TimingDefault::TimingType::STOP}) {
      testing::internal::CaptureStdout();
      perf->add_entry(id, type);
      output = testing::internal::GetCapturedStdout();
      EXPECT_EQ(0U, output.size()) << output;
      perf->set_enabled(false);

      testing::internal::CaptureStdout();
      perf->add_entry(id, type);
      output = testing::internal::GetCapturedStdout();
      EXPECT_EQ(0U, output.size()) << output;
      perf->set_enabled(true);

      // Force the loop to automatically reopen() the file
      perf->set_filename(filename);
    }

    // Need to return the stdout state so that the parent class can handle it
    testing::internal::CaptureStdout();
  }
}

/** Tests that we are able to using the performance class for generating start/stop timing points
 *  that can be used for timing diagrams.
 */
TEST_F(PerformanceTimingTest, Constructor) {
  Constructor<uint8_t>();
  Constructor<int8_t>();
  Constructor<uint16_t>();
  Constructor<int16_t>();
  Constructor<uint32_t>();
  Constructor<int32_t>();
  Constructor<uint64_t>();
  Constructor<int64_t>();
  Constructor<float>();
  Constructor<double>();
}

/** Make sure that data is being written to stdout and not to file
 */
TEST_F(PerformanceTimingTest, Stdout) {
  Stdout<uint8_t>();
  Stdout<int8_t>();
  Stdout<uint16_t>();
  Stdout<int16_t>();
  Stdout<uint32_t>();
  Stdout<int32_t>();
  Stdout<uint64_t>();
  Stdout<int64_t>();
  Stdout<float>();
  Stdout<double>();
}

/** Make sure that data is being written to a file and not stdout
 */
TEST_F(PerformanceTimingTest, Fileout) {
  Fileout<uint8_t>();
  Fileout<int8_t>();
  Fileout<uint16_t>();
  Fileout<int16_t>();
  Fileout<uint32_t>();
  Fileout<int32_t>();
  Fileout<uint64_t>();
  Fileout<int64_t>();
  Fileout<float>();
  Fileout<double>();
}
