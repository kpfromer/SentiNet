/** Auto-generated on { now }.  Do NOT modify by hand, changes will be overwritten next time message are generated.
 *
 *  Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "CommonTest.hpp"

/**
 */
TEST_F(CommonMessageTest, ValidateCommonEvent) {
  // Create an empty() instance of the Common message
  auto empty = std::make_unique<Common::Event>();
  EXPECT_FALSE(empty->has_event_id());
  EXPECT_FALSE(empty->has_level());
  EXPECT_FALSE(empty->has_summary());
  EXPECT_FALSE(empty->has_source());
  EXPECT_FALSE(empty->has_component());
  EXPECT_FALSE(empty->has_type());
  EXPECT_FALSE(empty->has_custom_json());

  // Create a randomized instance of the Common message
  auto msg = std::make_unique<CommonEventMessage>();
  EXPECT_EQ(static_cast<decltype(msg->get_id())>(0x9B5733CD84E227F5), msg->get_id());
  EXPECT_EQ("Common::Event", msg->get_message_name());
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
  auto clone = dynamic_cast<Common::Event*>(clone_obj.get());
  EXPECT_EQ(msg->get_id(), clone->get_id());
  EXPECT_EQ(msg->get_message_name(), clone->get_message_name());
  EXPECT_EQ(msg->get_topic(), clone->get_topic());
  // for field in msg.get_fields()
  // EXPECT_TRUE(clone->has_{ { field.name } }());
  // endfor
  clone->set_event_id(msg->get_event_id() + static_cast<decltype(msg->get_event_id())>(1));
  EXPECT_NE(msg->get_event_id(), clone->get_event_id());
  clone->set_level(msg->get_level() + static_cast<decltype(msg->get_level())>(1));
  EXPECT_NE(msg->get_level(), clone->get_level());
  clone->set_summary(utils::get_random_string(10, 16));
  EXPECT_NE(msg->get_summary(), clone->get_summary());
  clone->set_source(utils::get_random_string(10, 16));
  EXPECT_NE(msg->get_source(), clone->get_source());
  clone->set_component(utils::get_random_string(10, 16));
  EXPECT_NE(msg->get_component(), clone->get_component());
  clone->set_type(utils::get_random_string(10, 16));
  EXPECT_NE(msg->get_type(), clone->get_type());
  clone->set_custom_json(utils::get_random_string(10, 16));
  EXPECT_NE(msg->get_custom_json(), clone->get_custom_json());

  // Get some code coverage
  // @todo [KEMB-564] Add unit testing of the randomizer instead of just getting some code coverage without validation
  auto random = Randomizer::Common::Event::new_instance();
  std::stringstream ss;
  ss << *random << std::endl;
}

/**
 */
TEST_F(CommonMessageTest, ValidateCommonLocation) {
  // Create an empty() instance of the Common message
  auto empty = std::make_unique<Common::Location>();
  EXPECT_FALSE(empty->has_latitude());
  EXPECT_FALSE(empty->has_longitude());
  EXPECT_FALSE(empty->has_altitude());

  // Create a randomized instance of the Common message
  auto msg = std::make_unique<CommonLocationMessage>();
  EXPECT_EQ(static_cast<decltype(msg->get_id())>(0xAEF8AB336DFC644A), msg->get_id());
  EXPECT_EQ("Common::Location", msg->get_message_name());
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
  auto clone = dynamic_cast<Common::Location*>(clone_obj.get());
  EXPECT_EQ(msg->get_id(), clone->get_id());
  EXPECT_EQ(msg->get_message_name(), clone->get_message_name());
  EXPECT_EQ(msg->get_topic(), clone->get_topic());
  // for field in msg.get_fields()
  // EXPECT_TRUE(clone->has_{ { field.name } }());
  // endfor
  clone->set_latitude(utils::get_random<decltype(msg->get_latitude())>(-1.57079632679, 1.57079632679));
  EXPECT_NE(msg->get_latitude(), clone->get_latitude());
  clone->set_longitude(utils::get_random<decltype(msg->get_longitude())>(3.14159265359, 3.14159265359));
  EXPECT_NE(msg->get_longitude(), clone->get_longitude());
  clone->set_altitude(msg->get_altitude() + static_cast<decltype(msg->get_altitude())>(1));
  EXPECT_NE(msg->get_altitude(), clone->get_altitude());

  // Get some code coverage
  // @todo [KEMB-564] Add unit testing of the randomizer instead of just getting some code coverage without validation
  auto random = Randomizer::Common::Location::new_instance();
  std::stringstream ss;
  ss << *random << std::endl;
}
