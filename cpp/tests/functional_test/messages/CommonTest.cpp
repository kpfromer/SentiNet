/** Auto-generated on { now }.  Do NOT modify by hand, changes will be overwritten next time message are generated.
 *
 *  Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "messages/Common/AllMessageRandomizers.h"
#include "TestUtils.hpp"

// C++ includes
#include <fstream>

// Global externs
extern ConfigurationInterface performance_summary;
extern ConfigurationInterface performance_detailed;

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


/**
 */
TEST_F(CommonMessageTest, PerformanceCommonEvent) {
  uint64_t init = 0U;
  {
      auto start = utils::time::replay::now().count();
      auto random = Randomizer::Common::Event::new_instance(1, true);
      auto stop = utils::time::replay::now().count();
      init = stop - start;
  }

  const auto iterations = 10000U;
  std::vector<uint64_t> results;
  results.reserve(iterations);
  ASSERT_LE(iterations, results.capacity());
  ASSERT_EQ(0U, results.size());

  for(const auto& loop : { iterations } ) {
    for(auto i = 0U; i < loop; ++i) {
      auto start = utils::time::replay::now().count();
      auto random = std::make_unique<Randomizer::Common::Event>();
      auto stop = utils::time::replay::now().count();
      results.emplace_back(stop - start);
    }
  }

  {
    // Build and load the summary performance data
    YAML::Node node;
    node["name"] = Common::Event::static_get_msg_name();
    node["avg"] = std::accumulate(results.begin(), results.end(), 0U) / results.size();
    node["min"] = *std::min_element(results.begin(), results.end());
    node["max"] = *std::max_element(results.begin(), results.end());
    performance_summary["results"].push_back(node);
  }

  {
    // Build and load the detailed performance data.
    YAML::Node node;
    node["name"] = Common::Event::static_get_msg_name();
    node["values"] = results;
    node["init"] = init;
    performance_detailed["results"].push_back(node);
  }
}

/**
 */
TEST_F(CommonMessageTest, PerformanceCommonLocation) {
  uint64_t init = 0U;
  {
      auto start = utils::time::replay::now().count();
      auto random = Randomizer::Common::Location::new_instance(1, true);
      auto stop = utils::time::replay::now().count();
      init = stop - start;
  }

  const auto iterations = 10000U;
  std::vector<uint64_t> results;
  results.reserve(iterations);
  ASSERT_LE(iterations, results.capacity());
  ASSERT_EQ(0U, results.size());

  for(const auto& loop : { iterations } ) {
    for(auto i = 0U; i < loop; ++i) {
      auto start = utils::time::replay::now().count();
      auto random = std::make_unique<Randomizer::Common::Location>();
      auto stop = utils::time::replay::now().count();
      results.emplace_back(stop - start);
    }
  }

  {
    // Build and load the summary performance data
    YAML::Node node;
    node["name"] = Common::Location::static_get_msg_name();
    node["avg"] = std::accumulate(results.begin(), results.end(), 0U) / results.size();
    node["min"] = *std::min_element(results.begin(), results.end());
    node["max"] = *std::max_element(results.begin(), results.end());
    performance_summary["results"].push_back(node);
  }

  {
    // Build and load the detailed performance data.
    YAML::Node node;
    node["name"] = Common::Location::static_get_msg_name();
    node["values"] = results;
    node["init"] = init;
    performance_detailed["results"].push_back(node);
  }
}
