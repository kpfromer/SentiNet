#include "DataModel.hpp"
#include "TestUtils.hpp"

void rawint64_testable(const std::unique_ptr<Raw::Int64>& msg) {
  LOG_DEBUG_DATAMODEL(msg);
};

TEST_F(DataModelTest, RegisterTopicCallback) {
  auto app = std::make_unique<BootstrapDatamodel>();
  const std::string raw_int64_topic = "RAW::INT64";
  EXPECT_NO_THROW(
      app->register_topic_callback<Raw::Int64>(raw_int64_topic, std::bind(&rawint64_testable, std::placeholders::_1)));
  app->terminate();
}

TEST_F(DataModelTest, CallbackArgs) {
  auto app = std::make_unique<BootstrapDatamodel>();
  const std::string raw_int64_topic = "RAW::INT64";
  auto test_good_type = std::make_unique<Raw::Int64>();
  auto test_bad_type = std::make_unique<Raw::Float>();

  MsgFunc func;
  std::string tested_topic;
  app->bind_register_msg_callback(
      [&func, &tested_topic](const std::string& topic, MsgFunc callback) mutable -> int32_t {
        tested_topic = topic;
        func = callback;
        return 1;
      });

  EXPECT_NO_THROW(
      app->register_topic_callback<Raw::Int64>(raw_int64_topic, std::bind(&rawint64_testable, std::placeholders::_1)));

  EXPECT_EQ(raw_int64_topic, tested_topic);
  EXPECT_NO_THROW(func(std::move(test_good_type)));
  EXPECT_ANY_THROW(func(std::move(test_bad_type)));
  // validate
  app->terminate();
}