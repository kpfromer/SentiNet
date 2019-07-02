/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */
#ifndef TEST_WIST0_HPP
#define TEST_WIST0_HPP

// Kelvin includes
#include "TestUtils.hpp"
#include "protocols/DelimiterParser/DelimiterParser.h"
#include "common/descriptor/Fd.hpp"

// The custom GTEST harness for the DelimiterParser testing
class DelimiterParserTest : public GtestHarness {
 public:
  void SetUp() {
    ::GtestHarness::SetUp();
    EXPECT_EQ(0U, LoggingInterface::get_instance()->set_warning_cnt(0U));
    EXPECT_EQ(0U, LoggingInterface::get_instance()->set_error_cnt(0U));

    /// The Fd class needs to be moved into the unit test and/or we need to create a Pipe descriptor.
    reader = std::make_unique<descriptor::Fd>(fds[READ]);
  }

  void TearDown() {
    ::GtestHarness::TearDown();
  }

  std::unique_ptr<DescriptorInterface> reader;
};

/** Create a DelimiterParser application that is able to expose all of the protected data members.
 *  This makes testing much easier and helps with scoping of the application under test.
 */
class DelimiterParserApplication : public DelimiterParser::DelimiterParser {
 public:
  DelimiterParserApplication() {
    // @todo [KEMB-627] Saving all the unique pointers from the register() methods will eventually need to be done
    // Bind on the registration method so that we can capture the std::unique pointer and keep it in scope
    DelimiterParser::bind<ApplicationInterface::BoundDescriptorFunc>(
        [this](std::unique_ptr<DescriptorInterface> descriptor,
               std::function<void(DescriptorInterface * descriptor)>) -> int32_t {
          callbacks.emplace_back(std::move(descriptor));
          return -1;
        });
  }

  bool initialize(const ConfigurationInterface& config) {
    auto ret = true;

    // @todo [KEMB-556] This needs to be exposed so that unit testing can setup the application in the same manner
    // This should probably call configure_dso()

    // Register the data mode objects with the base class
    if(ConfigurationInterface::has_key(config, "data_model:definition")) {
      // Load the registery map
      const auto filename = config["data_model"]["definition"].as<std::string>();
      auto cfg = *ConfigurationInterface::load_file(filename);
      auto all_topics =
          ConfigurationInterface::build_topics(cfg["registry"].as<std::map<std::string, ConfigurationInterface>>());

      // Check if the listing of topics needs to be filtered based on the handler application name
      if(!config["data_model"]["load_all"] || config["data_model"]["load_all"].as<bool>() == false) {
        for(auto it = all_topics.cbegin(); it != all_topics.cend();) {
          const auto& def = it->second;
          // Check if this object applies to the current application
          if(def["handler"] && def["handler"]["app"] &&
             utils::is_equal(this->get_name(), def["handler"]["app"].as<std::string>())) {
            ++it;
          } else {
            it = all_topics.erase(it);
          }
        }
      }

      // Run through the topic listing and filter the registration based on explicit topics and the wildcard
      if(ConfigurationInterface::has_key(config, "data_model:filter:topics")) {
        std::set<std::string> filtered_topics;

        // @todo [KEMB-725] Convert the wildcard into a regex that can be used for filtering.
        for(const auto& topic : config["data_model"]["filter"]["topics"].as<std::list<std::string>>()) {
          // If it's the wildcard, we want to include all the topics.  Do not remove/disable any of them
          if(utils::is_equal("*", topic)) {
            filtered_topics.clear();
            for(const auto& pair : all_topics) {
              filtered_topics.emplace(pair.first);
            }
            break;
          }

          // Check for a wildcard in the topic filter name
          // @todo [KEMB-725] This should be using the regex library in C++
          if(topic.find("*") != std::string::npos) {
            const auto split = utils::split(topic, "*");
            if(split.size() > 3) {
              LOG_ERROR("Can only have up to 2 wildcards in the topic filter: %s\n", topic.c_str());
              ret = false;
              continue;
            }

            // Find the matching topics that should be included in the filter list
            for(auto i = 0U; i < split.size(); ++i) {
              const auto& val = split[i];
              if(!val.empty()) {
                for(const auto& pair : all_topics) {
                  auto found = pair.first.find(val);
                  if(found == std::string::npos) {
                    // It doesn't exist in the current topic name, so just continue to the next one
                    continue;
                  } else if(i == 0 && found == 0) {
                    // Must match the beginning
                    filtered_topics.emplace(pair.first);
                  } else if((i + 1) == split.size() && found == pair.first.size() - val.size()) {
                    // Must match the end
                    filtered_topics.emplace(pair.first);
                  } else {
                    // Can match anywhere in the string
                    filtered_topics.emplace(pair.first);
                  }
                }
              }
            }
          } else {
            filtered_topics.emplace(topic);
          }
          LOG_WARN("Filtering topics based on: %s\n", topic.c_str());
        }

        // Remove all the topics that are not matched in the filter list
        if(filtered_topics.empty()) {
          all_topics.clear();
        } else {
          for(auto it = all_topics.cbegin(); it != all_topics.cend();) {
            bool matched = false;
            for(const auto& v : filtered_topics) {
              if(utils::is_equal(it->first, v)) {
                matched = true;
              }
            }
            if(matched) {
              ++it;
            } else {
              it = all_topics.erase(it);
            }
          }
        }
      }

      // Set the filtered listing of topics as the loaded registry
      this->set_registry_map(all_topics);
    } else {
      LOG_INFO("%s\n",
               "No registry supplied.  No automatic registration of data model objects or generation to default topic "
               "names");
    }

    return ret && DelimiterParser::DelimiterParser::initialize(config);
  }

  ApplicationProperties& get_properties() {
    return properties;
  }

  void set_name(const std::string& name) {
    DelimiterParser::DelimiterParser::set_name(name);
  }

  void process_data(DescriptorInterface* descriptor) {
    DelimiterParser::DelimiterParser::process_data(descriptor);
  }

  void flush_buffer() {
    DelimiterParser::flush_buffer();
  }

  void bind_publish_callback(PublishFunc callback) {
    DelimiterParser::DelimiterParser::bind_publish_callback(callback);
  }

 private:
  std::list<std::unique_ptr<DescriptorInterface>> callbacks;
};

#endif /* TEST_WIST0_HPP */
