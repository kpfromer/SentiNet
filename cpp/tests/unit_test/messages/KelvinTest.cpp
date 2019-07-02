/** Auto-generated on { now }.  Do NOT modify by hand, changes will be overwritten next time message are generated.
 *
 *  Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "KelvinTest.hpp"

/**
 */
TEST_F(KelvinMessageTest, ValidateKelvinHealthAndStatus) {
  // Create an empty() instance of the Kelvin message
  auto empty = std::make_unique<Kelvin::HealthAndStatus>();
  EXPECT_FALSE(empty->has_app_name());
  EXPECT_FALSE(empty->has_sent_msg_cnt());
  EXPECT_FALSE(empty->has_recv_msg_cnt());
  EXPECT_FALSE(empty->has_uptime());
  EXPECT_FALSE(empty->has_sent_byte_cnt());
  EXPECT_FALSE(empty->has_recv_byte_cnt());
  EXPECT_FALSE(empty->has_warning_cnt());
  EXPECT_FALSE(empty->has_error_cnt());
  EXPECT_FALSE(empty->has_critical_cnt());
  EXPECT_FALSE(empty->has_disconnect_cnt());
  EXPECT_FALSE(empty->has_reconfigure_cnt());
  EXPECT_FALSE(empty->has_custom());

  // Create a randomized instance of the Kelvin message
  auto msg = std::make_unique<KelvinHealthAndStatusMessage>();
  EXPECT_EQ(static_cast<decltype(msg->get_id())>(0x6DBF3720CC6ECEBD), msg->get_id());
  EXPECT_EQ("Kelvin::HealthAndStatus", msg->get_message_name());
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
  auto clone = dynamic_cast<Kelvin::HealthAndStatus*>(clone_obj.get());
  EXPECT_EQ(msg->get_id(), clone->get_id());
  EXPECT_EQ(msg->get_message_name(), clone->get_message_name());
  EXPECT_EQ(msg->get_topic(), clone->get_topic());
  // for field in msg.get_fields()
  // EXPECT_TRUE(clone->has_{ { field.name } }());
  // endfor
  clone->set_app_name(utils::get_random_string(10, 16));
  EXPECT_NE(msg->get_app_name(), clone->get_app_name());
  clone->set_sent_msg_cnt(msg->get_sent_msg_cnt() + static_cast<decltype(msg->get_sent_msg_cnt())>(1));
  EXPECT_NE(msg->get_sent_msg_cnt(), clone->get_sent_msg_cnt());
  clone->set_recv_msg_cnt(msg->get_recv_msg_cnt() + static_cast<decltype(msg->get_recv_msg_cnt())>(1));
  EXPECT_NE(msg->get_recv_msg_cnt(), clone->get_recv_msg_cnt());
  clone->set_uptime(msg->get_uptime() + static_cast<decltype(msg->get_uptime())>(1));
  EXPECT_NE(msg->get_uptime(), clone->get_uptime());
  clone->set_sent_byte_cnt(msg->get_sent_byte_cnt() + static_cast<decltype(msg->get_sent_byte_cnt())>(1));
  EXPECT_NE(msg->get_sent_byte_cnt(), clone->get_sent_byte_cnt());
  clone->set_recv_byte_cnt(msg->get_recv_byte_cnt() + static_cast<decltype(msg->get_recv_byte_cnt())>(1));
  EXPECT_NE(msg->get_recv_byte_cnt(), clone->get_recv_byte_cnt());
  clone->set_warning_cnt(msg->get_warning_cnt() + static_cast<decltype(msg->get_warning_cnt())>(1));
  EXPECT_NE(msg->get_warning_cnt(), clone->get_warning_cnt());
  clone->set_error_cnt(msg->get_error_cnt() + static_cast<decltype(msg->get_error_cnt())>(1));
  EXPECT_NE(msg->get_error_cnt(), clone->get_error_cnt());
  clone->set_critical_cnt(msg->get_critical_cnt() + static_cast<decltype(msg->get_critical_cnt())>(1));
  EXPECT_NE(msg->get_critical_cnt(), clone->get_critical_cnt());
  clone->set_disconnect_cnt(msg->get_disconnect_cnt() + static_cast<decltype(msg->get_disconnect_cnt())>(1));
  EXPECT_NE(msg->get_disconnect_cnt(), clone->get_disconnect_cnt());
  clone->set_reconfigure_cnt(msg->get_reconfigure_cnt() + static_cast<decltype(msg->get_reconfigure_cnt())>(1));
  EXPECT_NE(msg->get_reconfigure_cnt(), clone->get_reconfigure_cnt());
  clone->set_custom(utils::get_random_string(10, 16));
  EXPECT_NE(msg->get_custom(), clone->get_custom());

  // Get some code coverage
  // @todo [KEMB-564] Add unit testing of the randomizer instead of just getting some code coverage without validation
  auto random = Randomizer::Kelvin::HealthAndStatus::new_instance();
  std::stringstream ss;
  ss << *random << std::endl;
}

/**
 */
TEST_F(KelvinMessageTest, ValidateKelvinRequest) {
  // Create an empty() instance of the Kelvin message
  auto empty = std::make_unique<Kelvin::Request>();
  EXPECT_FALSE(empty->has_name());

  // Create a randomized instance of the Kelvin message
  auto msg = std::make_unique<KelvinRequestMessage>();
  EXPECT_EQ(static_cast<decltype(msg->get_id())>(0xA753FE850ABAB582), msg->get_id());
  EXPECT_EQ("Kelvin::Request", msg->get_message_name());
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
  auto clone = dynamic_cast<Kelvin::Request*>(clone_obj.get());
  EXPECT_EQ(msg->get_id(), clone->get_id());
  EXPECT_EQ(msg->get_message_name(), clone->get_message_name());
  EXPECT_EQ(msg->get_topic(), clone->get_topic());
  // for field in msg.get_fields()
  // EXPECT_TRUE(clone->has_{ { field.name } }());
  // endfor
  clone->set_name(utils::get_random_string(10, 16));
  EXPECT_NE(msg->get_name(), clone->get_name());

  // Get some code coverage
  // @todo [KEMB-564] Add unit testing of the randomizer instead of just getting some code coverage without validation
  auto random = Randomizer::Kelvin::Request::new_instance();
  std::stringstream ss;
  ss << *random << std::endl;
}
