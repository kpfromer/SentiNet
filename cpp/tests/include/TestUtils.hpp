/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */
#ifndef TEST_COMMON_TESTUTILS_HPP
#define TEST_COMMON_TESTUTILS_HPP

// Kelvin includes
#include "common/Utils.hpp"
#include "common/configuration/ConfigurationInterface.hpp"
#include "common/filesystem/FileSystem.hpp"

// system includes
#include "gtest/gtest.h"

// The indexes for the pipes that are created
const int READ = 0;
const int WRITE = READ + 1;

static const std::string UNITTEST_DIR = "./unittest_scratch";
static const std::string RESULTS_DIR = "./results";
static const auto now = utils::time::replay::now().count();

class GtestHarness : public ::testing::Test {
 public:
  GtestHarness() : fds{-1, -1} {
  }

  // Code that's called at the beginning of each test
  void SetUp() {
    // Use the Google seed as the C++ seed (allows for reproducing results)
    srand(::testing::UnitTest::GetInstance()->random_seed());

    verbose = false;
    allow_stdout(verbose);

    EXPECT_EQ(0, pipe(fds));
    EXPECT_NE(-1, fds[READ]);
    EXPECT_NE(-1, fds[WRITE]);

    // Make sure we remove and re-create the scratch directory
    EXPECT_LE(0U, FileSystem::remove_all(UNITTEST_DIR));
    EXPECT_TRUE(FileSystem::create_directories(UNITTEST_DIR));
  }

  // Code that's called at the end of each test
  void TearDown() {
    for(auto i : {READ, WRITE}) {
      if(fds[i] >= 0) {
        EXPECT_EQ(0, close(fds[i]));
      }
      i = -1;
    }

    if(!use_stdout) {
      const auto output = testing::internal::GetCapturedStdout();
      // If the test failed, print the output.  Otherwise, just suppress the output
      if(::testing::Test::HasFailure() || verbose) {
        std::cout << output << std::endl;
      }
    }

    if(FileSystem::exists(UNITTEST_DIR)) {
      EXPECT_LT(0U, FileSystem::remove_all(UNITTEST_DIR));
    }
  }

  // Let's an application set whether to be using stdout instead of background buffering.
  void allow_stdout(const bool value) {
    use_stdout = value;
    if(use_stdout) {
      // Stop redirect stdout
      testing::internal::GetCapturedStdout();
    } else {
      // Redirect stdout
      testing::internal::CaptureStdout();
    }
  }

  // Check that we are able to read from the current file descriptor without blocking
  bool check_read_fd(const int fd) {
    fd_set master;
    FD_ZERO(&master);
    FD_SET(fd, &master);
    struct timeval tv = {0, 0};
    return select(FD_SETSIZE, &master, NULL, NULL, &tv) > 0;
  }

  static std::string get_results_dir() {
    return utils::join({RESULTS_DIR, std::to_string(now)}, "/");
  }

 public:
  int fds[2];
  bool verbose;
  bool use_stdout;
};

static const std::string DATA_MODEL_PERFORMANCE_SUMMARY =
    utils::join({GtestHarness::get_results_dir(), "data_model_performance_summary.json"}, "/");
static const std::string DATA_MODEL_PERFORMANCE_DETAILED =
    utils::join({GtestHarness::get_results_dir(), "data_model_performance_detailed.json"}, "/");

#endif /* TEST_COMMON_TESTUTILS_HPP */
