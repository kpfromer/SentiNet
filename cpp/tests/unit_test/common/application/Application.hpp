/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */
#ifndef TEST_COMMON_APPLICATION_HPP
#define TEST_COMMON_APPLICATION_HPP

// Kelvin includes
#include "TestUtils.hpp"
#include "common/application/impl/Application.h"

// The custom GTEST harness for the application base class implementaton testing
class ApplicationTest : public GtestHarness {
 public:
  void SetUp() {
    ::GtestHarness::SetUp();
  }

  void TearDown() {
    ::GtestHarness::TearDown();
  }
};

/** Create a WITS0 application that is able to expose all of the protected data members.
 *  This makes testing much easier and helps with scoping of the application under test.
 */
class BootstrapApplication : public Application {
 public:
  BootstrapApplication() {
  }

  void handle_publish(std::unique_ptr<MessageInterface> msg, const std::string& topic) {
    Application::handle_publish(std::move(msg), topic);
  }

  Application::ApplicationProperties& get_properties() {
    return app;
  }

  Application::SoftwareBus& get_software_bus() {
    return sb;
  }

  Application::Telemetry& get_telemetry() {
    return telemetry;
  }

  bool load_dso(const std::string& dso_name) {
    return Application::load_dso(dso_name);
  }

  bool handle_register_data_model_object(const uint64_t& hash,
                                         std::function<std::unique_ptr<MessageInterface>()> new_instance_callback) {
    return Application::handle_register_data_model_object(hash, new_instance_callback);
  }

  bool handle_unregister_data_model_object(const uint64_t& hash) {
    return Application::handle_unregister_data_model_object(hash);
  }

  bool handle_unregister_data_model_objects() {
    return Application::handle_unregister_data_model_objects();
  }
};

#endif /* TEST_COMMON_APPLICATION_HPP */
