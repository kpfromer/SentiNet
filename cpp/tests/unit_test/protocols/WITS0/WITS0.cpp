/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "WITS0.hpp"
#include "protocols/WITS0/tools/WITS0Helper.hpp"
#include "messages/Raw/FloatRandomizer.hpp"
#include "messages/Common/EventInterface.h"

// C++ includes
#include <cmath>

static const std::string PROTOCOL_NAME = "WITS0";
static const std::string CONFIG_FILENAME = "../config/WITS0/loopback_config.yml";

/** Helper function designed to generate a vector of randomly loaded Raw::Floats using the
 *  WITS0 Record definition currently loaded.
 */
std::vector<std::unique_ptr<Raw::FloatInterface>> generate_random_analysis_values(
    const uint32_t size, const std::vector<WITS0::WITS0Field_t>& definitions) {
  // Load up some randomized objects that will get written to the WITS0 stream
  std::vector<std::unique_ptr<Raw::FloatInterface>> objs;
  for(auto i = 0U; i < size; ++i) {
    const auto& def = definitions[i];
    auto tmp = Randomizer::Raw::Float::new_instance();
    tmp->set_topic(utils::join({"kelvin://ANADARKO/PD464", def.name}, "/"));
    tmp->set_value(utils::get_random<float>(-1000, 1000));
    // Truncate the float if necessary
    if(def.type.find("int") != std::string::npos) {
      tmp->set_value(static_cast<int32_t>(tmp->get_value()));
    } else {
      // The processing stream doesn't do more than decimal places
      std::stringstream ss;
      ss << std::fixed << tmp->get_value() << std::endl;
      tmp->set_value(std::stof(ss.str()));
    }
    objs.emplace_back(std::move(tmp));
  }

  return objs;
}

/** Tests that the WITS0 processing method is able to handle normal messages and malformed
 *  messages.  Since the WITS0 stream does not include integrity checks and the stream can
 *  get into a corrupted state, make sure that the processing method is able to handle those
 *  cases without throwing errors.
 */
TEST_F(WITS0Test, ProcessData) {
  // We're going to use an existing configuration file to load some records
  // @todo [KEMB-556] This really needs to be a unit test specific configuration file
  auto config = ConfigurationInterface::load_file(CONFIG_FILENAME);
  // Create an instance of the WITS0 application
  auto app = std::make_unique<WITS0Application>();
  app->set_name(PROTOCOL_NAME);
  EXPECT_TRUE(app->initialize(*config));
  auto& properties = app->get_properties();
  const auto bad_descriptor = std::make_unique<descriptor::Fd>(-1);

  // Take the loaded map and convert it to a vector (uses an existing function nuder tools/)
  auto definitions = std::vector<WITS0::WITS0Field_t>();
  // @todo [KEMB-556] This really needs to be a unit test specific configuration file
  for(const auto& mapping : properties.field_mapping) {
    definitions.emplace_back(mapping.second);
  }
  ASSERT_LT(2U, definitions.size()) << "Need more definitions in order to randomize the objects.";

  {
    /** Verify that an empty message does not produce a publish() callback.  The message is empty
     *  so it should just continue processing the stream.
     */
    auto count = 0U;
    const auto before = count;
    const auto str = std::string("&&\r\n!!\r\n");

    // Attach the callback to validate the processing message
    app->bind_publish_callback([&str, &count](std::unique_ptr<MessageInterface>, const std::string&) { count++; });

    // Write an invalid message into the stream before the good one ... it'll be rejected
    write(fds[WRITE], str.c_str(), str.size() - static_cast<uint32_t>(str.size() * 0.5));
    // Write the ASCII data to the file descriptor and call the process method
    write(fds[WRITE], str.c_str(), str.size());
    app->process_data(reader.get());

    // Make sure that we actually processed a message.
    ASSERT_EQ(count, before);
  }

  {
    /** Verify that we can encode and decode WITS0 data from a randomly generated listing of
     *  Raw::Floats based on the TLP configurgation loaded.
     */

    // Create a list of sizes for the WITS0 messages to be
    /// @todo [KEMB-556] They should really create a randomized listing of indexes
    std::list<uint64_t> sizes = {1U, 2U, 3U, definitions.size()};

    // Loop over a couple of iterations of creating random object to make messages of different sizes.
    for(const uint32_t size : sizes) {
      // Load up some randomized objects that will get written to the WITS0 stream
      auto objs = generate_random_analysis_values(size, definitions);

      // Create an ASCII representation
      const auto ascii = WITS0::to_WITS0(objs, definitions);

      // Attach the callback to validate the processing message
      auto count = 0U;
      app->bind_publish_callback([&objs, &count, &ascii](std::unique_ptr<MessageInterface> msg, const std::string&) {
        // Verify that there is a matching object in the listing of objects
        if(const auto av = dynamic_cast<const Raw::FloatInterface*>(msg.get())) {
          auto found =
              std::find_if(objs.begin(), objs.end(), [&av](const std::unique_ptr<Raw::FloatInterface>& obj) -> bool {
                return utils::is_equal(av->get_topic(), obj->get_topic()) &&
                       (fabs(av->get_value() - obj->get_value()) < std::numeric_limits<float>::epsilon());
              });
          EXPECT_TRUE(found != objs.end()) << std::fixed << av->get_topic() << ": " << av->get_value() << std::endl
                                           << ascii << std::endl;
          count++;
        }
      });

      // Write an invalid message into the stream before the good one ... it'll be rejected
      const auto i = ascii.size() / 2;
      write(fds[WRITE], &ascii.c_str()[i], ascii.size() - i);
      write(fds[WRITE], ascii.c_str(), i);
      // Write the ASCII data to the file descriptor and call the process method
      write(fds[WRITE], ascii.c_str(), ascii.size());
      app->process_data(reader.get());

      // Give it an invalid file descriptor make sure it doesn't affect the final message tally.
      // This implies that nothing extra was published and the buffer was not adversely altered.
      app->process_data(bad_descriptor.get());

      // Write the ASCII data again, but this pipe will be clean, so this is a nominal execution
      write(fds[WRITE], ascii.c_str(), ascii.size());
      app->process_data(reader.get());

      // Make sure that we actually processed a message.
      ASSERT_EQ(count, objs.size() * 2U);
    }
  }

  {
    // Verify that we can process a message that is split into multiple fragments
    auto objs = generate_random_analysis_values(definitions.size(), definitions);
    const auto ascii = to_WITS0(objs, definitions);
    const auto i = ascii.size() / 2;

    // Write part of the message and verify that the callback does NOT get called
    app->bind_publish_callback([](std::unique_ptr<MessageInterface>, const std::string&) { FAIL(); });
    write(fds[WRITE], &ascii.c_str()[0], i);
    app->process_data(reader.get());

    // Write the rest of the message and verify that the callback gets called
    auto count = 0U;
    app->bind_publish_callback([&objs, &count, &ascii](std::unique_ptr<MessageInterface> msg, const std::string&) {
      // Verify that there is a matching object in the listing of objects
      if(const auto av = dynamic_cast<const Raw::FloatInterface*>(msg.get())) {
        auto found =
            std::find_if(objs.begin(), objs.end(), [&av](const std::unique_ptr<Raw::FloatInterface>& obj) -> bool {
              return utils::is_equal(av->get_topic(), obj->get_topic()) &&
                     (fabs(av->get_value() - obj->get_value()) < std::numeric_limits<float>::epsilon());
            });
        EXPECT_TRUE(found != objs.end()) << std::fixed << av->get_topic() << ": " << av->get_value() << std::endl
                                         << ascii << std::endl;
        count++;
      }
    });
    write(fds[WRITE], &ascii.c_str()[i], ascii.size() - i);
    app->process_data(reader.get());

    // Make sure that we actually processed a message.
    ASSERT_EQ(count, objs.size());
  }

  {
    // Verify that the two message in the buffer get processed correctly and in order.
    auto objs1 = generate_random_analysis_values(definitions.size(), definitions);
    auto objs2 = generate_random_analysis_values(definitions.size(), definitions);

    const auto ascii1 = to_WITS0(objs1, definitions);
    const auto ascii2 = to_WITS0(objs2, definitions);
    auto count = 0U;

    // Attach the callback to validate the processing message
    app->bind_publish_callback([&ascii1, &objs1, &ascii2, &objs2, &count](std::unique_ptr<MessageInterface> msg,
                                                                          const std::string&) {
      // Verify that there is a matching object in the listing of objects
      if(const auto av = dynamic_cast<const Raw::FloatInterface*>(msg.get())) {
        if(count++ < objs1.size()) {
          auto found =
              std::find_if(objs1.begin(), objs1.end(), [&av](const std::unique_ptr<Raw::FloatInterface>& obj) -> bool {
                return utils::is_equal(av->get_topic(), obj->get_topic()) &&
                       (fabs(av->get_value() - obj->get_value()) < std::numeric_limits<float>::epsilon());
              });
          EXPECT_TRUE(found != objs1.end()) << std::fixed << av->get_topic() << ": " << av->get_value() << std::endl
                                            << ascii1 << std::endl;
        } else {
          auto found =
              std::find_if(objs2.begin(), objs2.end(), [&av](const std::unique_ptr<Raw::FloatInterface>& obj) -> bool {
                return utils::is_equal(av->get_topic(), obj->get_topic()) &&
                       (fabs(av->get_value() - obj->get_value()) < std::numeric_limits<float>::epsilon());
              });
          EXPECT_TRUE(found != objs2.end()) << std::fixed << av->get_topic() << ": " << av->get_value() << std::endl
                                            << ascii2 << std::endl;
        }
      }
    });

    // Write the messages to the buffer
    write(fds[WRITE], ascii1.c_str(), ascii1.size());
    write(fds[WRITE], ascii2.c_str(), ascii2.size());
    app->process_data(reader.get());
  }

  {
    // Verify that we can process a message that is some garbage in the middle
    auto objs = generate_random_analysis_values(definitions.size(), definitions);
    const auto ascii = to_WITS0(objs, definitions);
    const auto i = ascii.size() / 2;
    const std::string garbage = "asdadas\n\rasd9212 12 3112 13!@#13201sdasd;lds";

    // Write part of the message and verify that the callback does NOT get called
    app->bind_publish_callback([](std::unique_ptr<MessageInterface>, const std::string&) { FAIL(); });
    write(fds[WRITE], &ascii.c_str()[0], i);
    write(fds[WRITE], garbage.c_str(), garbage.size());
    app->process_data(reader.get());

    // Write the rest of the message and verify that the callback gets called
    auto count = 0U;
    app->bind_publish_callback([&count](std::unique_ptr<MessageInterface>, const std::string&) { count++; });
    write(fds[WRITE], &ascii.c_str()[i], ascii.size() - i);
    app->process_data(reader.get());

    // Make sure that we actually processed at least one message from the buffer
    ASSERT_LT(0U, count);
  }

  {
    // Verify that we can process a message that is sent one byte at a time
    auto objs = generate_random_analysis_values(definitions.size(), definitions);
    const auto ascii = to_WITS0(objs, definitions);

    // Write each byte, one at time, until to the ending message delimiters
    for(auto i = 0U; i < ascii.size() - 4; ++i) {
      // Write the next byte and verify that the callback does NOT get called
      app->bind_publish_callback([](std::unique_ptr<MessageInterface>, const std::string&) { FAIL(); });
      write(fds[WRITE], &ascii.c_str()[i], 1);
      app->process_data(reader.get());
    }

    // Write the last byte and verify that the callback gets called
    auto count = 0U;
    app->bind_publish_callback([&objs, &count, &ascii](std::unique_ptr<MessageInterface> msg, const std::string&) {
      // Verify that there is a matching object in the listing of objects
      if(const auto av = dynamic_cast<const Raw::FloatInterface*>(msg.get())) {
        auto found =
            std::find_if(objs.begin(), objs.end(), [&av](const std::unique_ptr<Raw::FloatInterface>& obj) -> bool {
              return utils::is_equal(av->get_topic(), obj->get_topic()) &&
                     (fabs(av->get_value() - obj->get_value()) < std::numeric_limits<float>::epsilon());
            });
        EXPECT_TRUE(found != objs.end()) << std::fixed << av->get_topic() << ": " << av->get_value() << std::endl
                                         << ascii << std::endl;
        count++;
      }
    });
    write(fds[WRITE], &ascii.c_str()[ascii.size() - 4], 3);
    app->process_data(reader.get());

    // Make sure that we actually processed a message.
    ASSERT_EQ(count, objs.size());
  }

  {
    // Verify that the two message in the buffer get processed correctly and in order.
    auto objs1 = generate_random_analysis_values(definitions.size(), definitions);
    auto objs2 = generate_random_analysis_values(definitions.size(), definitions);

    const auto ascii1 = to_WITS0(objs1, definitions);
    const auto ascii2 = to_WITS0(objs2, definitions);
    auto count = 0U;

    // Attach the callback to validate the processing message
    app->bind_publish_callback([&ascii1, &objs1, &ascii2, &objs2, &count](std::unique_ptr<MessageInterface> msg,
                                                                          const std::string&) {
      // Verify that there is a matching object in the listing of objects
      if(const auto av = dynamic_cast<const Raw::FloatInterface*>(msg.get())) {
        if(count++ < objs1.size()) {
          auto found =
              std::find_if(objs1.begin(), objs1.end(), [&av](const std::unique_ptr<Raw::FloatInterface>& obj) -> bool {
                return utils::is_equal(av->get_topic(), obj->get_topic()) &&
                       (fabs(av->get_value() - obj->get_value()) < std::numeric_limits<float>::epsilon());
              });
          EXPECT_TRUE(found != objs1.end()) << std::fixed << av->get_topic() << ": " << av->get_value() << std::endl
                                            << ascii1 << std::endl;
        } else {
          auto found =
              std::find_if(objs2.begin(), objs2.end(), [&av](const std::unique_ptr<Raw::FloatInterface>& obj) -> bool {
                return utils::is_equal(av->get_topic(), obj->get_topic()) &&
                       (fabs(av->get_value() - obj->get_value()) < std::numeric_limits<float>::epsilon());
              });
          EXPECT_TRUE(found != objs2.end()) << std::fixed << av->get_topic() << ": " << av->get_value() << std::endl
                                            << ascii2 << std::endl;
        }
      }
    });

    // Write the messages to the buffer
    write(fds[WRITE], ascii1.c_str(), ascii1.size());
    write(fds[WRITE], ascii2.c_str(), ascii2.size());
    app->process_data(reader.get());
  }

  {
    // Verify that we can process a message that is some garbage in the middle
    auto objs = generate_random_analysis_values(definitions.size(), definitions);
    const auto ascii = to_WITS0(objs, definitions);
    const auto i = ascii.size() / 2;
    const std::string garbage = "asdadas\n\rasd9212 12 3112 13!@#13201sdasd;lds";

    // Write part of the message and verify that the callback does NOT get called
    app->bind_publish_callback([](std::unique_ptr<MessageInterface>, const std::string&) { FAIL(); });
    write(fds[WRITE], &ascii.c_str()[0], i);
    write(fds[WRITE], garbage.c_str(), garbage.size());
    app->process_data(reader.get());

    // Write the rest of the message and verify that the callback gets called
    auto count = 0U;
    app->bind_publish_callback([&count](std::unique_ptr<MessageInterface>, const std::string&) { count++; });
    write(fds[WRITE], &ascii.c_str()[i], ascii.size() - i);
    app->process_data(reader.get());

    // Make sure that we actually processed at least one message from the buffer
    ASSERT_LT(0U, count);
  }
}

/** Tests that the WITS0 processing method is able to handle normal messages and malformed
 *  messages.  Since the WITS0 stream does not include integrity checks and the stream can
 *  get into a corrupted state, make sure that the processing method is able to handle those
 *  cases without throwing errors.
 */
TEST_F(WITS0Test, BufferFull) {
  // We're going to use an existing configuration file to load some records
  // @todo [KEMB-556] This really needs to be a unit test specific configuration file
  auto config = ConfigurationInterface::load_file(CONFIG_FILENAME);
  // Create an instance of the WITS0 application
  auto app = std::make_unique<WITS0Application>();
  app->set_name(PROTOCOL_NAME);
  EXPECT_TRUE(app->initialize(*config));
  auto& properties = app->get_properties();
  const auto bad_descriptor = std::make_unique<descriptor::Fd>(-1);

  // Take the loaded map and convert it to a vector (uses an existing function nuder tools/)
  auto definitions = std::vector<WITS0::WITS0Field_t>();
  // @todo [KEMB-556] This really needs to be a unit test specific configuration file
  for(const auto& mapping : properties.field_mapping) {
    definitions.emplace_back(mapping.second);
  }
  ASSERT_LT(2U, definitions.size()) << "Need more definitions in order to randomize the objects.";

  {
    /** Verify that an empty message does not produce a publish() callback.  The message is empty
     *  so it should just continue processing the stream.
     */
    auto count = 0U;
    const auto before = count;
    const auto str = "&&" + std::string(WITS0::Default::BUFFER_SIZE, '4');

    // Attach the callback to validate the processing message
    app->bind_publish_callback([&count](std::unique_ptr<MessageInterface> msg, const std::string&) {
      if(dynamic_cast<Common::EventInterface*>(msg.get()) == nullptr) {
        count++;
      }
    });

    // Write the ASCII data to the file descriptor and call the process method
    write(fds[WRITE], str.c_str(), str.size());
    app->process_data(reader.get());
    app->process_data(reader.get());

    // Make sure that we actually processed a message.
    ASSERT_EQ(count, before);
  }
}

/** Test the handshake callback and validate the data written to the file descriptor matches the configuration
 */
TEST_F(WITS0Test, Handshake) {
  // We're going to use an existing configuration file to load some records
  auto config = ConfigurationInterface::load_file(CONFIG_FILENAME);
  auto app = std::make_unique<WITS0Application>();
  app->set_name(PROTOCOL_NAME);
  EXPECT_TRUE(app->initialize(*config));

  // @todo [KEMB-556] The std::unique_ptr is going out of scope because the pointer is going to a No-op register()
  EXPECT_FALSE(app->send_handshake());
  // @todo [KEMB-556] Need to read() from the write() file descriptor and validate the contents.
}
