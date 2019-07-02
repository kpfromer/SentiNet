/** Auto-generated on { now }.  Do NOT modify by hand, changes will be overwritten next time message are generated.
 *
 *  Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "messages/Raw/AllMessageRandomizers.h"
#include "TestUtils.hpp"

// C++ includes
#include <fstream>

// Global externs
extern ConfigurationInterface performance_summary;
extern ConfigurationInterface performance_detailed;

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


/**
 */
TEST_F(RawMessageTest, PerformanceRawDouble) {
  uint64_t init = 0U;
  {
      auto start = utils::time::replay::now().count();
      auto random = Randomizer::Raw::Double::new_instance(1, true);
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
      auto random = std::make_unique<Randomizer::Raw::Double>();
      auto stop = utils::time::replay::now().count();
      results.emplace_back(stop - start);
    }
  }

  {
    // Build and load the summary performance data
    YAML::Node node;
    node["name"] = Raw::Double::static_get_msg_name();
    node["avg"] = std::accumulate(results.begin(), results.end(), 0U) / results.size();
    node["min"] = *std::min_element(results.begin(), results.end());
    node["max"] = *std::max_element(results.begin(), results.end());
    performance_summary["results"].push_back(node);
  }

  {
    // Build and load the detailed performance data.
    YAML::Node node;
    node["name"] = Raw::Double::static_get_msg_name();
    node["values"] = results;
    node["init"] = init;
    performance_detailed["results"].push_back(node);
  }
}

/**
 */
TEST_F(RawMessageTest, PerformanceRawFloat) {
  uint64_t init = 0U;
  {
      auto start = utils::time::replay::now().count();
      auto random = Randomizer::Raw::Float::new_instance(1, true);
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
      auto random = std::make_unique<Randomizer::Raw::Float>();
      auto stop = utils::time::replay::now().count();
      results.emplace_back(stop - start);
    }
  }

  {
    // Build and load the summary performance data
    YAML::Node node;
    node["name"] = Raw::Float::static_get_msg_name();
    node["avg"] = std::accumulate(results.begin(), results.end(), 0U) / results.size();
    node["min"] = *std::min_element(results.begin(), results.end());
    node["max"] = *std::max_element(results.begin(), results.end());
    performance_summary["results"].push_back(node);
  }

  {
    // Build and load the detailed performance data.
    YAML::Node node;
    node["name"] = Raw::Float::static_get_msg_name();
    node["values"] = results;
    node["init"] = init;
    performance_detailed["results"].push_back(node);
  }
}

/**
 */
TEST_F(RawMessageTest, PerformanceRawInt32) {
  uint64_t init = 0U;
  {
      auto start = utils::time::replay::now().count();
      auto random = Randomizer::Raw::Int32::new_instance(1, true);
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
      auto random = std::make_unique<Randomizer::Raw::Int32>();
      auto stop = utils::time::replay::now().count();
      results.emplace_back(stop - start);
    }
  }

  {
    // Build and load the summary performance data
    YAML::Node node;
    node["name"] = Raw::Int32::static_get_msg_name();
    node["avg"] = std::accumulate(results.begin(), results.end(), 0U) / results.size();
    node["min"] = *std::min_element(results.begin(), results.end());
    node["max"] = *std::max_element(results.begin(), results.end());
    performance_summary["results"].push_back(node);
  }

  {
    // Build and load the detailed performance data.
    YAML::Node node;
    node["name"] = Raw::Int32::static_get_msg_name();
    node["values"] = results;
    node["init"] = init;
    performance_detailed["results"].push_back(node);
  }
}

/**
 */
TEST_F(RawMessageTest, PerformanceRawInt64) {
  uint64_t init = 0U;
  {
      auto start = utils::time::replay::now().count();
      auto random = Randomizer::Raw::Int64::new_instance(1, true);
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
      auto random = std::make_unique<Randomizer::Raw::Int64>();
      auto stop = utils::time::replay::now().count();
      results.emplace_back(stop - start);
    }
  }

  {
    // Build and load the summary performance data
    YAML::Node node;
    node["name"] = Raw::Int64::static_get_msg_name();
    node["avg"] = std::accumulate(results.begin(), results.end(), 0U) / results.size();
    node["min"] = *std::min_element(results.begin(), results.end());
    node["max"] = *std::max_element(results.begin(), results.end());
    performance_summary["results"].push_back(node);
  }

  {
    // Build and load the detailed performance data.
    YAML::Node node;
    node["name"] = Raw::Int64::static_get_msg_name();
    node["values"] = results;
    node["init"] = init;
    performance_detailed["results"].push_back(node);
  }
}

/**
 */
TEST_F(RawMessageTest, PerformanceRawString) {
  uint64_t init = 0U;
  {
      auto start = utils::time::replay::now().count();
      auto random = Randomizer::Raw::String::new_instance(1, true);
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
      auto random = std::make_unique<Randomizer::Raw::String>();
      auto stop = utils::time::replay::now().count();
      results.emplace_back(stop - start);
    }
  }

  {
    // Build and load the summary performance data
    YAML::Node node;
    node["name"] = Raw::String::static_get_msg_name();
    node["avg"] = std::accumulate(results.begin(), results.end(), 0U) / results.size();
    node["min"] = *std::min_element(results.begin(), results.end());
    node["max"] = *std::max_element(results.begin(), results.end());
    performance_summary["results"].push_back(node);
  }

  {
    // Build and load the detailed performance data.
    YAML::Node node;
    node["name"] = Raw::String::static_get_msg_name();
    node["values"] = results;
    node["init"] = init;
    performance_detailed["results"].push_back(node);
  }
}
