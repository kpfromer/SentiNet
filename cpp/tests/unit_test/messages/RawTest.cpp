/** Auto-generated on { now }.  Do NOT modify by hand, changes will be overwritten next time message are generated.
 *
 *  Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "RawTest.hpp"

/**
 */
TEST_F(RawMessageTest, ValidateRawDouble) {
  // Create an empty() instance of the Raw message
  auto empty = std::make_unique<Raw::Double>();
  EXPECT_FALSE(empty->has_value());

  // Create a randomized instance of the Raw message
  auto msg = std::make_unique<RawDoubleMessage>();
  EXPECT_EQ(static_cast<decltype(msg->get_id())>(0xE93B1C0B7A01DAE1), msg->get_id());
  EXPECT_EQ("Raw::Double", msg->get_message_name());
  ASSERT_NE(nullptr, msg->cget_instance());
  ASSERT_EQ(msg->cget_instance(), msg->get_instance());
  // Check the topic loading
  const std::string topic = "kelvin://well00/flow_meter15";
  EXPECT_EQ("", msg->get_topic());
  msg->set_topic(topic);
  EXPECT_EQ(topic, msg->get_topic());
  // Check the time loading
  const auto time = std::chrono::nanoseconds(123456);
  msg->set_time_of_validity(time);
  EXPECT_EQ(time, msg->get_time_of_validity());
  // Check that we can serialize
  std::stringstream ser;
  EXPECT_TRUE(msg->serialize(ser));
  std::string deser;
  EXPECT_TRUE(msg->deserialize(deser));

  /// @todo [KEMB-563] For some reason I can't get the has() methods to work after the clone() until you
  //  call a set() on the data member ... this should not be necessary
  // for field in msg.get_fields()
  // EXPECT_TRUE(msg->has_{ { field.name } }());
  // endfor

  // Validate the getter methods ... we are explicitly doing this so we avoid the operator=
  auto clone_obj = msg->clone();
  auto clone = dynamic_cast<Raw::Double*>(clone_obj.get());
  EXPECT_EQ(msg->get_id(), clone->get_id());
  EXPECT_EQ(msg->get_message_name(), clone->get_message_name());
  EXPECT_EQ(msg->get_topic(), clone->get_topic());
  // for field in msg.get_fields()
  // EXPECT_TRUE(clone->has_{ { field.name } }());
  // endfor
  clone->set_value(msg->get_value() + static_cast<decltype(msg->get_value())>(1));
  EXPECT_NE(msg->get_value(), clone->get_value());

  // Get some code coverage
  // @todo [KEMB-564] Add unit testing of the randomizer instead of just getting some code coverage without validation
  auto random = Randomizer::Raw::Double::new_instance();
  std::stringstream ss;
  ss << *random << std::endl;
}

/**
 */
TEST_F(RawMessageTest, ValidateRawFloat) {
  // Create an empty() instance of the Raw message
  auto empty = std::make_unique<Raw::Float>();
  EXPECT_FALSE(empty->has_value());

  // Create a randomized instance of the Raw message
  auto msg = std::make_unique<RawFloatMessage>();
  EXPECT_EQ(static_cast<decltype(msg->get_id())>(0xF84E9267C10AA7E9), msg->get_id());
  EXPECT_EQ("Raw::Float", msg->get_message_name());
  ASSERT_NE(nullptr, msg->cget_instance());
  ASSERT_EQ(msg->cget_instance(), msg->get_instance());
  // Check the topic loading
  const std::string topic = "kelvin://well00/flow_meter15";
  EXPECT_EQ("", msg->get_topic());
  msg->set_topic(topic);
  EXPECT_EQ(topic, msg->get_topic());
  // Check the time loading
  const auto time = std::chrono::nanoseconds(123456);
  msg->set_time_of_validity(time);
  EXPECT_EQ(time, msg->get_time_of_validity());
  // Check that we can serialize
  std::stringstream ser;
  EXPECT_TRUE(msg->serialize(ser));
  std::string deser;
  EXPECT_TRUE(msg->deserialize(deser));

  /// @todo [KEMB-563] For some reason I can't get the has() methods to work after the clone() until you
  //  call a set() on the data member ... this should not be necessary
  // for field in msg.get_fields()
  // EXPECT_TRUE(msg->has_{ { field.name } }());
  // endfor

  // Validate the getter methods ... we are explicitly doing this so we avoid the operator=
  auto clone_obj = msg->clone();
  auto clone = dynamic_cast<Raw::Float*>(clone_obj.get());
  EXPECT_EQ(msg->get_id(), clone->get_id());
  EXPECT_EQ(msg->get_message_name(), clone->get_message_name());
  EXPECT_EQ(msg->get_topic(), clone->get_topic());
  // for field in msg.get_fields()
  // EXPECT_TRUE(clone->has_{ { field.name } }());
  // endfor
  clone->set_value(msg->get_value() + static_cast<decltype(msg->get_value())>(1));
  EXPECT_NE(msg->get_value(), clone->get_value());

  // Get some code coverage
  // @todo [KEMB-564] Add unit testing of the randomizer instead of just getting some code coverage without validation
  auto random = Randomizer::Raw::Float::new_instance();
  std::stringstream ss;
  ss << *random << std::endl;
}

/**
 */
TEST_F(RawMessageTest, ValidateRawInt32) {
  // Create an empty() instance of the Raw message
  auto empty = std::make_unique<Raw::Int32>();
  EXPECT_FALSE(empty->has_value());

  // Create a randomized instance of the Raw message
  auto msg = std::make_unique<RawInt32Message>();
  EXPECT_EQ(static_cast<decltype(msg->get_id())>(0x0CF262C2D130331F), msg->get_id());
  EXPECT_EQ("Raw::Int32", msg->get_message_name());
  ASSERT_NE(nullptr, msg->cget_instance());
  ASSERT_EQ(msg->cget_instance(), msg->get_instance());
  // Check the topic loading
  const std::string topic = "kelvin://well00/flow_meter15";
  EXPECT_EQ("", msg->get_topic());
  msg->set_topic(topic);
  EXPECT_EQ(topic, msg->get_topic());
  // Check the time loading
  const auto time = std::chrono::nanoseconds(123456);
  msg->set_time_of_validity(time);
  EXPECT_EQ(time, msg->get_time_of_validity());
  // Check that we can serialize
  std::stringstream ser;
  EXPECT_TRUE(msg->serialize(ser));
  std::string deser;
  EXPECT_TRUE(msg->deserialize(deser));

  /// @todo [KEMB-563] For some reason I can't get the has() methods to work after the clone() until you
  //  call a set() on the data member ... this should not be necessary
  // for field in msg.get_fields()
  // EXPECT_TRUE(msg->has_{ { field.name } }());
  // endfor

  // Validate the getter methods ... we are explicitly doing this so we avoid the operator=
  auto clone_obj = msg->clone();
  auto clone = dynamic_cast<Raw::Int32*>(clone_obj.get());
  EXPECT_EQ(msg->get_id(), clone->get_id());
  EXPECT_EQ(msg->get_message_name(), clone->get_message_name());
  EXPECT_EQ(msg->get_topic(), clone->get_topic());
  // for field in msg.get_fields()
  // EXPECT_TRUE(clone->has_{ { field.name } }());
  // endfor
  clone->set_value(msg->get_value() + static_cast<decltype(msg->get_value())>(1));
  EXPECT_NE(msg->get_value(), clone->get_value());

  // Get some code coverage
  // @todo [KEMB-564] Add unit testing of the randomizer instead of just getting some code coverage without validation
  auto random = Randomizer::Raw::Int32::new_instance();
  std::stringstream ss;
  ss << *random << std::endl;
}

/**
 */
TEST_F(RawMessageTest, ValidateRawInt64) {
  // Create an empty() instance of the Raw message
  auto empty = std::make_unique<Raw::Int64>();
  EXPECT_FALSE(empty->has_value());

  // Create a randomized instance of the Raw message
  auto msg = std::make_unique<RawInt64Message>();
  EXPECT_EQ(static_cast<decltype(msg->get_id())>(0x0E365698250A153C), msg->get_id());
  EXPECT_EQ("Raw::Int64", msg->get_message_name());
  ASSERT_NE(nullptr, msg->cget_instance());
  ASSERT_EQ(msg->cget_instance(), msg->get_instance());
  // Check the topic loading
  const std::string topic = "kelvin://well00/flow_meter15";
  EXPECT_EQ("", msg->get_topic());
  msg->set_topic(topic);
  EXPECT_EQ(topic, msg->get_topic());
  // Check the time loading
  const auto time = std::chrono::nanoseconds(123456);
  msg->set_time_of_validity(time);
  EXPECT_EQ(time, msg->get_time_of_validity());
  // Check that we can serialize
  std::stringstream ser;
  EXPECT_TRUE(msg->serialize(ser));
  std::string deser;
  EXPECT_TRUE(msg->deserialize(deser));

  /// @todo [KEMB-563] For some reason I can't get the has() methods to work after the clone() until you
  //  call a set() on the data member ... this should not be necessary
  // for field in msg.get_fields()
  // EXPECT_TRUE(msg->has_{ { field.name } }());
  // endfor

  // Validate the getter methods ... we are explicitly doing this so we avoid the operator=
  auto clone_obj = msg->clone();
  auto clone = dynamic_cast<Raw::Int64*>(clone_obj.get());
  EXPECT_EQ(msg->get_id(), clone->get_id());
  EXPECT_EQ(msg->get_message_name(), clone->get_message_name());
  EXPECT_EQ(msg->get_topic(), clone->get_topic());
  // for field in msg.get_fields()
  // EXPECT_TRUE(clone->has_{ { field.name } }());
  // endfor
  clone->set_value(msg->get_value() + static_cast<decltype(msg->get_value())>(1));
  EXPECT_NE(msg->get_value(), clone->get_value());

  // Get some code coverage
  // @todo [KEMB-564] Add unit testing of the randomizer instead of just getting some code coverage without validation
  auto random = Randomizer::Raw::Int64::new_instance();
  std::stringstream ss;
  ss << *random << std::endl;
}

/**
 */
TEST_F(RawMessageTest, ValidateRawString) {
  // Create an empty() instance of the Raw message
  auto empty = std::make_unique<Raw::String>();
  EXPECT_FALSE(empty->has_value());

  // Create a randomized instance of the Raw message
  auto msg = std::make_unique<RawStringMessage>();
  EXPECT_EQ(static_cast<decltype(msg->get_id())>(0x237300F37CE77107), msg->get_id());
  EXPECT_EQ("Raw::String", msg->get_message_name());
  ASSERT_NE(nullptr, msg->cget_instance());
  ASSERT_EQ(msg->cget_instance(), msg->get_instance());
  // Check the topic loading
  const std::string topic = "kelvin://well00/flow_meter15";
  EXPECT_EQ("", msg->get_topic());
  msg->set_topic(topic);
  EXPECT_EQ(topic, msg->get_topic());
  // Check the time loading
  const auto time = std::chrono::nanoseconds(123456);
  msg->set_time_of_validity(time);
  EXPECT_EQ(time, msg->get_time_of_validity());
  // Check that we can serialize
  std::stringstream ser;
  EXPECT_TRUE(msg->serialize(ser));
  std::string deser;
  EXPECT_TRUE(msg->deserialize(deser));

  /// @todo [KEMB-563] For some reason I can't get the has() methods to work after the clone() until you
  //  call a set() on the data member ... this should not be necessary
  // for field in msg.get_fields()
  // EXPECT_TRUE(msg->has_{ { field.name } }());
  // endfor

  // Validate the getter methods ... we are explicitly doing this so we avoid the operator=
  auto clone_obj = msg->clone();
  auto clone = dynamic_cast<Raw::String*>(clone_obj.get());
  EXPECT_EQ(msg->get_id(), clone->get_id());
  EXPECT_EQ(msg->get_message_name(), clone->get_message_name());
  EXPECT_EQ(msg->get_topic(), clone->get_topic());
  // for field in msg.get_fields()
  // EXPECT_TRUE(clone->has_{ { field.name } }());
  // endfor
  clone->set_value(utils::get_random_string(10, 16));
  EXPECT_NE(msg->get_value(), clone->get_value());

  // Get some code coverage
  // @todo [KEMB-564] Add unit testing of the randomizer instead of just getting some code coverage without validation
  auto random = Randomizer::Raw::String::new_instance();
  std::stringstream ss;
  ss << *random << std::endl;
}
