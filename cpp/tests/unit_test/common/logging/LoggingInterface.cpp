/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "TestUtils.hpp"
#include "common/logging/LoggingInterface.hpp"
#include "messages/Kelvin/HealthAndStatusRandomizer.hpp"

// A vector of all the levels
static const std::vector<LoggingDefault::Level> levels = {
    LoggingDefault::Level::DEBUG, LoggingDefault::Level::INFO,     LoggingDefault::Level::WARN,
    LoggingDefault::Level::ERROR, LoggingDefault::Level::CRITICAL,
};

#define STDOUT_TEST01(level)                            \
  /* Capture the stdout */                              \
  testing::internal::CaptureStdout();                   \
  const char* str = "Hello";                            \
  const auto line_num = __LINE__;                       \
  char output[LoggingDefault::MAX_LOG_SIZE];            \
  char type = 'U';                                      \
  if(level == LoggingDefault::Level::DEBUG) {           \
    LOG_DEBUG("%s", str);                               \
    type = 'D';                                         \
  } else if(level == LoggingDefault::Level::INFO) {     \
    LOG_INFO("%s", str);                                \
    type = 'I';                                         \
  } else if(level == LoggingDefault::Level::WARN) {     \
    LOG_WARN("%s", str);                                \
    type = 'W';                                         \
  } else if(level == LoggingDefault::Level::ERROR) {    \
    LOG_ERROR("%s", str);                               \
    type = 'E';                                         \
  } else if(level == LoggingDefault::Level::CRITICAL) { \
    LOG_CRITICAL("%s", str);                            \
    type = 'C';                                         \
  }                                                     \
  snprintf(output, sizeof(output), "[%20s:%4d:%c] - %s", FILENAME, line_num, type, str);

/** Tests that the Logging base class is able to properly write to the logger based on verbosity level
 *  set in the LoggingInterface.  Since the class is singleton, so can't assume the default, we must
 *  explicitly set the level and then validate.
 */
TEST(LoggingInterfaceTests, LoggingLevelsStdOut) {
  // Grab a reference pointer to the singleton
  auto pointer = LoggingInterface::cget_instance();
  ASSERT_NE(nullptr, pointer);
  ASSERT_EQ(LoggingInterface::get_instance(), LoggingInterface::cget_instance());
  EXPECT_FALSE(LoggingInterface::get_instance()->set_logging_level("Made up level"));

  // A mapping that will be used to validate the different verbosity levels
  std::map<LoggingDefault::Level, std::vector<bool>> enum_tests = {
      // Level, { DEBUG, INFO, WARN, ERROR, CRITICAL }
      {LoggingDefault::Level::DEBUG, {true, true, true, true, true}},
      {LoggingDefault::Level::INFO, {false, true, true, true, true}},
      {LoggingDefault::Level::WARN, {false, false, true, true, true}},
      {LoggingDefault::Level::ERROR, {false, false, false, true, true}},
      {LoggingDefault::Level::CRITICAL, {false, false, false, false, true}},
      // Now to really test the interface limits
      {static_cast<LoggingDefault::Level>(LoggingDefault::Level::ERROR + 1), {false, false, false, false, false}},
      {static_cast<LoggingDefault::Level>(
           std::numeric_limits<std::underlying_type<LoggingDefault::Level>::type>::max()),
       {false, false, false, false, false}},
      // This is technically going to be same as DEBUG, but I wanted to have it
      {static_cast<LoggingDefault::Level>(
           std::numeric_limits<std::underlying_type<LoggingDefault::Level>::type>::min()),
       {true, true, true, true, true}},
  };

  // Iterate over the different logging levels
  for(const auto& test : enum_tests) {
    EXPECT_TRUE(LoggingInterface::get_instance()->set_logging_level(test.first));
    ASSERT_EQ(test.first, LoggingInterface::cget_instance()->get_logging_level()) << " - " << test.first;
    // Make sure we are always getting the same instance.
    ASSERT_EQ(pointer, LoggingInterface::get_instance()) << " - " << test.first;
    ASSERT_EQ(pointer, LoggingInterface::cget_instance()) << " - " << test.first;

    // Iterate over the boolean to see if we should expect an empty response or a verbose response
    for(auto i = 0U; i < test.second.size(); ++i) {
      STDOUT_TEST01(levels[i]);
      EXPECT_EQ((test.second[i] ? output : ""), testing::internal::GetCapturedStdout())
          << " - " << test.first << ", " << i;
    }
  }

  // A mapping that will be used to validate the different verbosity levels
  std::map<std::string, std::vector<bool>> str_tests = {
      // Level, { DEBUG, INFO, WARN, ERROR, CRITICAL }
      {"DEBUG", {true, true, true, true, true}},        {"info", {false, true, true, true, true}},
      {"WaRn", {false, false, true, true, true}},       {"errOR", {false, false, false, true, true}},
      {"CriTICAL", {false, false, false, false, true}},
  };

  // Iterate over the different logging levels
  for(const auto& test : str_tests) {
    EXPECT_TRUE(LoggingInterface::get_instance()->set_logging_level(test.first));
    // Make sure we are always getting the same instance.
    ASSERT_EQ(pointer, LoggingInterface::get_instance()) << " - " << test.first;
    ASSERT_EQ(pointer, LoggingInterface::cget_instance()) << " - " << test.first;

    // Iterate over the boolean to see if we should expect an empty response or a verbose response
    for(auto i = 0U; i < test.second.size(); ++i) {
      STDOUT_TEST01(levels[i]);
      EXPECT_EQ((test.second[i] ? output : ""), testing::internal::GetCapturedStdout())
          << " - " << test.first << ", " << i;
    }
  }
}

/** Tests that the Logging base class is able to manage the internal counters for message types.
 *  The looping is tests all the macro versions of the logging API and also validates the results
 *  using all the available logging levels to make sure logging level doesn't affect the counting
 *  of the numbers.
 */
TEST(LoggingInterfaceTests, LoggingEmitCounters) {
  // Suppress the stdout data
  testing::internal::CaptureStdout();

  const auto obj = Randomizer::Kelvin::HealthAndStatus::new_instance();
  auto logger = LoggingInterface::get_instance();
  auto counts = {0U, 10U, 5231U, 23456725U};

  // Check that the counters work for the different logging levels and different starting values.
  for(const auto& count : counts) {
    const auto loop_max = 100U;

    // Start the loggers off at the correct number
    EXPECT_EQ(count, logger->set_warning_cnt(count));
    EXPECT_EQ(count, logger->get_warning_cnt());
    EXPECT_EQ(count, logger->set_error_cnt(count));
    EXPECT_EQ(count, logger->get_error_cnt());
    EXPECT_EQ(count, logger->set_critical_cnt(count));
    EXPECT_EQ(count, logger->get_critical_cnt());

    // DEBUG, INFO - Check that count isn't affect by the different logging levels
    for(const auto& level : levels) {
      EXPECT_TRUE(logger->set_logging_level(level)) << " - " << level;
      for(auto i = 0U; i < loop_max; ++i) {
        switch(i % 3) {
          case 0:
            LOG_DEBUG("%s\n", utils::get_random_string().c_str());
            LOG_INFO("%s\n", utils::get_random_string().c_str());
            break;
          case 1:
            LOG_DEBUG_NOHEADER("%s\n", utils::get_random_string().c_str());
            LOG_INFO_NOHEADER("%s\n", utils::get_random_string().c_str());
            break;
          case 2:
            LOG_DEBUG_DATAMODEL(obj);
            LOG_INFO_DATAMODEL(obj);
            break;
        }
      }
    }
    EXPECT_EQ(count, logger->get_warning_cnt());
    EXPECT_EQ(count, logger->get_error_cnt());
    EXPECT_EQ(count, logger->get_critical_cnt());

    // WARN - Check that count isn't affect by the different logging levels
    for(const auto& level : levels) {
      EXPECT_TRUE(logger->set_logging_level(level)) << " - " << level;
      for(auto i = 0U; i < loop_max; ++i) {
        switch(i % 3) {
          case 0:
            LOG_WARN("%s\n", utils::get_random_string().c_str());
            break;
          case 1:
            LOG_WARN_NOHEADER("%s\n", utils::get_random_string().c_str());
            break;
          case 2:
            LOG_WARN_DATAMODEL(obj);
            break;
        }
      }
    }
    EXPECT_EQ(count + (levels.size() * loop_max), logger->get_warning_cnt());
    EXPECT_EQ(count, logger->get_error_cnt());
    EXPECT_EQ(count, logger->get_critical_cnt());

    // ERROR - Check that count isn't affect by the different logging levels
    for(const auto& level : levels) {
      EXPECT_TRUE(logger->set_logging_level(level)) << " - " << level;
      for(auto i = 0U; i < loop_max; ++i) {
        switch(i % 3) {
          case 0:
            LOG_ERROR("%s\n", utils::get_random_string().c_str());
            break;
          case 1:
            LOG_ERROR_NOHEADER("%s\n", utils::get_random_string().c_str());
            break;
          case 2:
            LOG_ERROR_DATAMODEL(obj);
            break;
        }
      }
    }
    EXPECT_EQ(count + (levels.size() * loop_max), logger->get_warning_cnt());
    EXPECT_EQ(count + (levels.size() * loop_max), logger->get_error_cnt());
    EXPECT_EQ(count, logger->get_critical_cnt());

    // CRITICAL - Check that count isn't affect by the different logging levels
    for(const auto& level : levels) {
      EXPECT_TRUE(logger->set_logging_level(level)) << " - " << level;
      for(auto i = 0U; i < loop_max; ++i) {
        switch(i % 3) {
          case 0:
            LOG_CRITICAL("%s\n", utils::get_random_string().c_str());
            break;
          case 1:
            LOG_CRITICAL_NOHEADER("%s\n", utils::get_random_string().c_str());
            break;
          case 2:
            LOG_CRITICAL_DATAMODEL(obj);
            break;
        }
      }
    }
    EXPECT_EQ(count + (levels.size() * loop_max), logger->get_warning_cnt());
    EXPECT_EQ(count + (levels.size() * loop_max), logger->get_error_cnt());
    EXPECT_EQ(count + (levels.size() * loop_max), logger->get_critical_cnt());
  }

  // Test for the roll-over condition
  const auto max_cnt = std::numeric_limits<uint64_t>::max();
  EXPECT_EQ(max_cnt, logger->set_warning_cnt(max_cnt));
  EXPECT_EQ(max_cnt, logger->get_warning_cnt());
  EXPECT_EQ(max_cnt, logger->set_error_cnt(max_cnt));
  EXPECT_EQ(max_cnt, logger->get_error_cnt());
  EXPECT_EQ(max_cnt, logger->set_critical_cnt(max_cnt));
  EXPECT_EQ(max_cnt, logger->get_critical_cnt());

  LOG_WARN("%s\n", "Test");
  EXPECT_EQ(0U, logger->get_warning_cnt());
  EXPECT_EQ(max_cnt, logger->get_error_cnt());
  EXPECT_EQ(max_cnt, logger->get_critical_cnt());

  LOG_ERROR("%s\n", "Test");
  EXPECT_EQ(0U, logger->get_warning_cnt());
  EXPECT_EQ(0U, logger->get_error_cnt());
  EXPECT_EQ(max_cnt, logger->get_critical_cnt());

  LOG_CRITICAL("%s\n", "Test");
  EXPECT_EQ(0U, logger->get_warning_cnt());
  EXPECT_EQ(0U, logger->get_error_cnt());
  EXPECT_EQ(0U, logger->get_critical_cnt());

  // Clear the stdout flag for the next tests
  testing::internal::GetCapturedStdout();
}
