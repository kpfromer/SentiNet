#include "TestUtils.hpp"
#include <fstream>

// Some global variables to speed up the functional testing
ConfigurationInterface performance_summary;
ConfigurationInterface performance_detailed;

// Global setup/teardown ... done at the very beginning and very end
class GlobalEnvironment : public ::testing::Environment {
 public:
  GlobalEnvironment() {
  }

  // Override this to define how to set up the environment.
  void SetUp() override {
    EXPECT_TRUE(FileSystem::create_directories(GtestHarness::get_results_dir()));
    std::stringstream ss;
    // WTF ... how is test_start_time changing ... ugh
    // ss << "ln -s " << test_start_time << " " << utils::join({RESULTS_DIR, "latest"}, "/");
    auto split = utils::split(GtestHarness::get_results_dir(), "/");
    ss << "ln -sfn " << split[split.size() - 1] << " " << utils::join({RESULTS_DIR, "latest"}, "/");
    EXPECT_TRUE(system(ss.str().c_str()) >= 0);

    // Write out the header to the performance file
    // @todo [KEMB-545] Should this be YAML?
    std::fstream out;
    out.open(DATA_MODEL_PERFORMANCE_SUMMARY, std::fstream::out | std::fstream::app);
    out << "Object Name,Iterations,Minimum (nsec),Average (nsec),Maximum (nsec)" << std::endl;
  }

  // Override this to define how to tear down the environment.
  void TearDown() override {
    // A mapping of the YAML data to output file
    std::map<std::string, std::reference_wrapper<ConfigurationInterface>> outputs = {
        std::make_pair(DATA_MODEL_PERFORMANCE_SUMMARY, std::ref(performance_summary)),
        std::make_pair(DATA_MODEL_PERFORMANCE_DETAILED, std::ref(performance_detailed)),
    };

    for(const auto& output : outputs) {
      // Write the detailed information to disk
      YAML::Emitter emitter;
      emitter << YAML::DoubleQuoted << YAML::Flow << output.second;

      std::fstream out;
      out.open(output.first, std::fstream::out);
      out << emitter.c_str();
      out.close();
    }
  }
};

volatile auto const env = ::testing::AddGlobalTestEnvironment(new GlobalEnvironment);

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  int ret = RUN_ALL_TESTS();
  return ret;
}
