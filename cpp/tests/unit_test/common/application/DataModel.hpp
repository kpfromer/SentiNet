#ifndef TEST_COMMON_DATAMODEL_HPP
#define TEST_COMMON_APPLICATION_HPP

#include "TestUtils.hpp"
#include "common/application/DataModel.hpp"
#include "messages/MessageUtils.hpp"

typedef std::function<void(std::unique_ptr<MessageInterface> msg)> MsgFunc;
typedef std::function<int32_t(const std::string& topic, MsgFunc)> RegisterTopicFunc;
class DataModelTest : public GtestHarness {
 public:
  void SetUp() {
    ::GtestHarness::SetUp();
  }

  void TearDown() {
    ::GtestHarness::TearDown();
  }
};

class BootstrapDatamodel : public DataModelApplication {
 public:
  BootstrapDatamodel() {
  }

  void process_data(std::list<std::unique_ptr<MessageInterface>>&){};

  template <typename T>
  void register_topic_callback(const std::string& topic,
                               const std::function<void(const std::unique_ptr<T>& msg)> callable) {
    DataModelApplication::register_topic_callback(topic, callable);
  }

  void bind_register_msg_callback(RegisterTopicFunc callback) {
    DataModelApplication::bind_register_msg_callback(callback);
  }

  void terminate() {
    DataModelApplication::terminate();
  }
};

#endif