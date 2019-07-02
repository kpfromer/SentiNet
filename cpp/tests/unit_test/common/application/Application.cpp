/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "Application.hpp"
#include "common/messages/Utils.hpp"
#include "common/filesystem/FileSystem.hpp"
#include "common/software_bus/zmq/ZMQ.h"
#include "messages/MessageUtils.hpp"

/** Tests that the Application based class is able to initialize with command line arguments and
 *  can handle improper and proper arguments.
 */
TEST_F(ApplicationTest, Initialize) {
  // Test that if garbage is sent into the initialize function, it throws an error.
  {
    auto app = std::make_unique<BootstrapApplication>();

    const int argc = 4;
    const char* argv[] = {"hello", "world", "with", "parameters"};

    // Bounds checking on the parameter inputs
    EXPECT_THROW(app->initialize(-1, nullptr), InvalidArgument);
    EXPECT_THROW(app->initialize(std::numeric_limits<int>::min(), nullptr);, InvalidArgument);
    EXPECT_THROW(app->initialize(argc, nullptr);, InvalidArgument);
    EXPECT_THROW(app->initialize(0, argv), InvalidArgument);
    // This doesn't contain --cfg, so the application will exit
    EXPECT_EXIT(app->initialize(argc, argv), ::testing::ExitedWithCode(1), "");
  }

  // Test that a valid --cfg must be supplied.
  {
    auto app = std::make_unique<BootstrapApplication>();

    const char* argv[] = {"--cfg", "a/made/up/file"};

    // Verify that we must have correct, valid parameters
    EXPECT_EXIT(app->initialize(1, argv), ::testing::ExitedWithCode(1), "");
    EXPECT_EXIT(app->initialize(2, argv), ::testing::ExitedWithCode(1), "");
  }
}

/** Tests that the Application based class is able to handle loading DSOs from disk
 *  and validating the contents of the shared object.
 */
TEST_F(ApplicationTest, DynamicSharedObject) {
  // Load some invalid DSO names and validate error handling
  {
    auto app = std::make_unique<BootstrapApplication>();
    EXPECT_THROW(app->load_dso(""), InvalidArgument);
    EXPECT_THROW(app->load_dso("some_made_up_dso"), InvalidArgument);
    EXPECT_THROW(app->load_dso("run_unit_test"), InvalidArgument);
  }

  // Load all of the compiled applications and protocols and do some sanity checking
  {
    for(const auto& path : {"../app", "../protocol"}) {
      const auto entries = FileSystem::directory_iterator(path);
      ASSERT_LT(0U, entries.size());
      for(const auto& entry : entries) {
        /** @todo [KEMB-543] This is considered a HACK and is used to ignore applications that might have outstanding
         *  ticket that needs to be addressed before they can included in the unit framework.
         */
        bool ignore = false;
        for(const auto& name : {"KelvinUploader", "metrics"}) {
          if(entry.find(name) != std::string::npos) {
            ignore = true;
          }
        }
        if(ignore) {
          continue;
        }

        // Create a bootstrapped application
        auto app = std::make_unique<BootstrapApplication>();
        auto& properties = app->get_properties();

        // Load the shared object and make sure we can inflate the application
        const auto split = utils::split(entry, "/");
        const auto cfg_file = "../config/" + utils::replace(split[split.size() - 1], ".so", "") + "/jenkins_config.yml";
        ASSERT_TRUE(FileSystem::is_regular_file(cfg_file))
            << "Create a configuration file (" << cfg_file << ") for use in validation.";

        // Load an example configuration and make sure nothing fails
        properties.config = ConfigurationInterface::load_file(cfg_file);
        ASSERT_TRUE(app->load_dso(entry)) << cfg_file << std::endl << *properties.config;
        app->terminate();
      }
    }
  }

  verbose = true;
}

/** Tests that the Application based class is able to handle publishing to multple topics that
 *  have been registered through a Kelvin control message.
 */
TEST_F(ApplicationTest, AutomaticTopicCreationAndPublish) {
  /** Test that we are able to add messages definition IDs to the publishers
   *  topic registration mapping, and that messages are duplicated when necessary.
   */
  auto app = std::make_unique<BootstrapApplication>();
  auto& sb = app->get_software_bus();
  auto& telemetry = app->get_telemetry();
  auto byte_cnt = 0U;

  // Create a couple of pairings
  /// @todo [KEMB-453] Convert this to the subscriber mechanism when it's available
  sb.topics.emplace(Raw::Float::static_get_msg_id(), std::list<std::string>({"hello", "world"}));
  sb.topics.emplace(Kelvin::HealthAndStatus::static_get_msg_id(), std::list<std::string>({"hello"}));

  // Make sure there is no software bus by default
  ASSERT_TRUE(sb.bus == nullptr);
  EXPECT_EQ(0U, LoggingInterface::get_instance()->set_error_cnt(0U));

  // There is no bus, so nothing should get published, but the error counter should increment
  {
    auto msg = Raw::Float::new_instance();
    const auto raw = message::utils::to_bytes(msg.get());
    app->handle_publish(std::move(msg), "new_topic");
    EXPECT_EQ(0U, telemetry.sent_msg_cnt);
    EXPECT_EQ(0U, telemetry.sent_byte_cnt);
    EXPECT_EQ(1U, LoggingInterface::cget_instance()->get_error_cnt());
  }

  // Load a software bus so that it can 'publish'
  sb.bus = std::make_unique<software_bus::zmq::ZMQ>("tcp://127.0.0.1:4444", "tcp://127.0.0.1:4445");
  ASSERT_FALSE(sb.bus == nullptr);

  // We are expecting a count of 3, this topic, plus 2 custom topics
  {
    auto msg = Raw::Float::new_instance();
    const auto raw = message::utils::to_bytes(msg.get());
    app->handle_publish(std::move(msg), "new_topic");
    EXPECT_EQ(3U, telemetry.sent_msg_cnt);
    EXPECT_EQ((byte_cnt = 3U * raw.size()), telemetry.sent_byte_cnt);
    EXPECT_EQ(1U, LoggingInterface::cget_instance()->get_error_cnt());
  }

  // We are expecting a count of 5, this topic, plus 1 custom topic (it will match one already)
  {
    auto msg = Raw::Float::new_instance();
    const auto raw = message::utils::to_bytes(msg.get());
    app->handle_publish(std::move(msg), "hello");
    EXPECT_EQ(5U, telemetry.sent_msg_cnt);
    EXPECT_EQ((byte_cnt += 2U * raw.size()), telemetry.sent_byte_cnt);
    EXPECT_EQ(1U, LoggingInterface::cget_instance()->get_error_cnt());
  }

  // We are expecting a count of 7, this topic, plus 1 custom topic
  {
    auto msg = Kelvin::HealthAndStatus::new_instance();
    const auto raw = message::utils::to_bytes(msg.get());
    app->handle_publish(std::move(msg), "new_topic");
    EXPECT_EQ(7U, telemetry.sent_msg_cnt);
    EXPECT_EQ((byte_cnt += 2U * raw.size()), telemetry.sent_byte_cnt);
    EXPECT_EQ(1U, LoggingInterface::cget_instance()->get_error_cnt());
  }

  // We are expecting a count of 7, this topic, plus 1 custom topic
  {
    auto msg = Common::Location::new_instance();
    const auto raw = message::utils::to_bytes(msg.get());
    app->handle_publish(std::move(msg), "hello");
    EXPECT_EQ(8U, telemetry.sent_msg_cnt);
    EXPECT_EQ((byte_cnt += 1U * raw.size()), telemetry.sent_byte_cnt);
    EXPECT_EQ(1U, LoggingInterface::cget_instance()->get_error_cnt());
  }
}

/** Tests that the Application based class is able to subscribe and unsubscribe to messages
 *  via the interface.
 */
TEST_F(ApplicationTest, SubscribeUnsubscribe) {
  auto app = std::make_unique<BootstrapApplication>();
  auto& sb = app->get_software_bus();

  // Attempt the call the functions with invalid parameters
  EXPECT_FALSE(app->handle_register_data_model_object(15U, nullptr));
  EXPECT_FALSE(app->handle_register_data_model_object(std::numeric_limits<uint64_t>::min(), nullptr));
  EXPECT_FALSE(app->handle_register_data_model_object(std::numeric_limits<uint64_t>::max(), nullptr));

  // Subscribe to some invalid names
  {
    auto names = {"Kelvin::MadeUpName", "Common::AnotherType", "OilAndGas::YetAgain", "RebelScum", ""};
    // Iterate over the invalid names and make sure they are unable to register
    for(const auto& name : names) {
      EXPECT_THROW(DataModel::Utils::get_registration(name), InvalidArgument);
      EXPECT_EQ(0U, sb.lookup.size());
    }
  }

  // Subscribe to some valid names
  {
    auto names = {"Kelvin::HealthAndStatus", "Raw::Float", "Common::Location"};
    // Iterate over the valid names and make sure they are able to register
    auto size = 0U;
    for(const auto& name : names) {
      auto pair = DataModel::Utils::get_registration(name);
      EXPECT_TRUE(app->handle_register_data_model_object(pair.first, pair.second));
      EXPECT_NE(sb.lookup.find(pair.first), sb.lookup.end());
      EXPECT_EQ(++size, sb.lookup.size());

      // Duplicates should be fine, but there is currently a limitation in std::function and std::bind
      // so that can't be supported at the moment.
      auto new_pair = DataModel::Utils::get_registration(name);
      EXPECT_FALSE(app->handle_register_data_model_object(new_pair.first, new_pair.second));
      EXPECT_NE(sb.lookup.find(new_pair.first), sb.lookup.end());
      EXPECT_EQ(size, sb.lookup.size());
    }

    // Iterate over the valid names and unregister them
    for(const auto& name : names) {
      auto pair = DataModel::Utils::get_registration(name);
      EXPECT_TRUE(app->handle_unregister_data_model_object(pair.first));
      EXPECT_EQ(sb.lookup.find(pair.first), sb.lookup.end());
      EXPECT_EQ(--size, sb.lookup.size());

      // Duplicate unsubscribes should be fine
      EXPECT_TRUE(app->handle_unregister_data_model_object(pair.first));
      EXPECT_EQ(sb.lookup.find(pair.first), sb.lookup.end());
      EXPECT_EQ(size, sb.lookup.size());
    }

    ASSERT_EQ(0U, size);
  }

  // Subscribe to some valid names and then unsubscribe all
  {
    auto names = {"Kelvin::HealthAndStatus", "Raw::Float", "Common::Location"};
    // Iterate over the valid names and make sure they are able to register
    auto size = 0U;
    for(const auto& name : names) {
      auto pair = DataModel::Utils::get_registration(name);
      EXPECT_TRUE(app->handle_register_data_model_object(pair.first, pair.second));
      EXPECT_NE(sb.lookup.find(pair.first), sb.lookup.end());
      EXPECT_EQ(++size, sb.lookup.size());

      // Duplicates should be fine, but there is currently a limitation in std::function and std::bind
      // so that can't be supported at the moment.
      auto new_pair = DataModel::Utils::get_registration(name);
      EXPECT_FALSE(app->handle_register_data_model_object(new_pair.first, new_pair.second));
      EXPECT_NE(sb.lookup.find(new_pair.first), sb.lookup.end());
      EXPECT_EQ(size, sb.lookup.size());
    }

    // Remove all the subscriptions
    app->handle_unregister_data_model_objects();
    ASSERT_EQ(0U, sb.lookup.size());
  }
}

/** Tests that the Application is able to process messages being serialized over the wire
 *  and deserialized internally.
 */
TEST_F(ApplicationTest, SoftwareBusFromBytes) {
  /** Placate the compiler for the time being.
   */
  std::map<uint64_t, std::function<std::unique_ptr<MessageInterface>()>> lookup;
  EXPECT_TRUE(message::utils::from_bytes("", lookup).empty());
}
