/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "TestUtils.hpp"
#include "Db2Register.hpp"

/// @todo  [KEMB-769] finish unit tests

static const std::string PROTOCOL_NAME = "Db2Register";
static const std::string CONFIG_FILENAME = "../config/Db2Register/unit_test_config.yml";

TEST_F(Db2RegisterTest, GroupRegisterSets) {
  auto app = std::make_unique<Db2RegisterApplication>();

  auto cmp = [](const std::map<uint32_t, uint16_t>& m1, const std::map<uint32_t, uint16_t>& m2) -> bool {
    return std::equal(m1.begin(), m1.end(), m2.begin());
  };

  // Non consecutive registers
  {
    std::vector<std::vector<uint32_t>> register_sets{{11, 3, 3}, {11, 1, 3}, {11, 2, 3}, {11, 0, 3}};
    auto grouped_register_sets = app->group_register_sets(register_sets);
    std::map<uint32_t, uint16_t> expected_grouped_register_sets{{REGISTER_SET_TO_UINT32(11, 0, 3), 1},
                                                                {REGISTER_SET_TO_UINT32(11, 1, 3), 1},
                                                                {REGISTER_SET_TO_UINT32(11, 2, 3), 1},
                                                                {REGISTER_SET_TO_UINT32(11, 3, 3), 1}};
    EXPECT_TRUE(cmp(grouped_register_sets, expected_grouped_register_sets));
  }

  // All consecutive registers
  {
    std::vector<std::vector<uint32_t>> register_sets = {{11, 1, 3}, {11, 1, 2}, {11, 1, 1}, {11, 1, 0}};
    auto grouped_register_sets = app->group_register_sets(register_sets);
    std::map<uint32_t, uint16_t> expected_grouped_register_sets = {{REGISTER_SET_TO_UINT32(11, 1, 0), 4}};
    EXPECT_TRUE(cmp(grouped_register_sets, expected_grouped_register_sets));
  }

  // Consecutive registers at the beginning of the ordered register sets
  {
    std::vector<std::vector<uint32_t>> register_sets = {{11, 1, 3}, {11, 2, 2}, {11, 0, 0}, {11, 0, 1}};
    auto grouped_register_sets = app->group_register_sets(register_sets);
    std::map<uint32_t, uint16_t> expected_grouped_register_sets = {{REGISTER_SET_TO_UINT32(11, 0, 0), 2},
                                                                   {REGISTER_SET_TO_UINT32(11, 1, 3), 1},
                                                                   {REGISTER_SET_TO_UINT32(11, 2, 2), 1}};
    EXPECT_TRUE(cmp(grouped_register_sets, expected_grouped_register_sets));
  }

  // Consecutive registers at the end of the ordered register sets
  {
    std::vector<std::vector<uint32_t>> register_sets = {{11, 0, 1}, {11, 2, 5}, {11, 2, 4}, {11, 1, 1}};
    auto grouped_register_sets = app->group_register_sets(register_sets);
    std::map<uint32_t, uint16_t> expected_grouped_register_sets = {{REGISTER_SET_TO_UINT32(11, 0, 1), 1},
                                                                   {REGISTER_SET_TO_UINT32(11, 1, 1), 1},
                                                                   {REGISTER_SET_TO_UINT32(11, 2, 4), 2}};
    EXPECT_TRUE(cmp(grouped_register_sets, expected_grouped_register_sets));
  }

  // Consecutive registers at middle of the ordered register sets
  {
    std::vector<std::vector<uint32_t>> register_sets = {{11, 5, 1}, {11, 6, 5}, {11, 3, 5},
                                                        {11, 3, 4}, {11, 0, 0}, {11, 2, 0}};
    auto grouped_register_sets = app->group_register_sets(register_sets);
    std::map<uint32_t, uint16_t> expected_grouped_register_sets = {{REGISTER_SET_TO_UINT32(11, 0, 0), 1},
                                                                   {REGISTER_SET_TO_UINT32(11, 2, 0), 1},
                                                                   {REGISTER_SET_TO_UINT32(11, 3, 4), 2},
                                                                   {REGISTER_SET_TO_UINT32(11, 5, 1), 1},
                                                                   {REGISTER_SET_TO_UINT32(11, 6, 5), 1}};
    EXPECT_TRUE(cmp(grouped_register_sets, expected_grouped_register_sets));
  }

  // verbose = true;
}

TEST_F(Db2RegisterTest, PrebuiltDataRequest) {
  auto app = std::make_unique<Db2RegisterApplication>();
  app->set_station_id("TOTALFLOW");

  // Pre-build using an empty map
  {
    std::vector<uint8_t> prebuilt_request;
    auto built = app->prebuilt_data_request({}, prebuilt_request);
    EXPECT_EQ(built, false);
    EXPECT_EQ(prebuilt_request.capacity(), 0U);
  }

  // Pre-build using a map that should generate one XRegisterSet frame
  {
    std::vector<uint8_t> prebuilt_request;
    std::map<uint32_t, uint16_t> grouped_register_sets{{REGISTER_SET_TO_UINT32(11, 0, 0), 1}};
    auto built = app->prebuilt_data_request(grouped_register_sets, prebuilt_request);

    // SupervisoryFrame - [0xff, 0x1, 'T', 'O', 'T',  'A',  'L', 'F', 'L',  'O', 'W', ' ', 0x58, 0xff, 0xfe, 0x3a]
    // SyncFrame        - [0xd3, 0xd9, 0xcE, 0xc3]
    // XSingleHeader    - [0x3, 0x0, 0xfc, 0xff, 0x0, 0x0, 0x1, 0x92, 0xc0]
    // XRegisterSet     - [0xa, 0x0, 0xf5, 0xff, 0x0, 0x2, 0xb, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x2f, 0x3f]
    const std::vector<uint8_t> expected_prebuilt_request{
        0xff, 0x1,  'T',  'O',  'T',  'A', 'L', 'F',  'L',  'O', 'W', ' ', 0x58, 0xff, 0xfe,
        0x3a, 0xd3, 0xd9, 0xcE, 0xc3, 0x3, 0x0, 0xfc, 0xff, 0x0, 0x0, 0x1, 0x92, 0xc0, 0xa,
        0x0,  0xf5, 0xff, 0x0,  0x2,  0xb, 0x0, 0x0,  0x0,  0x1, 0x0, 0x0, 0x0,  0x2f, 0x3f};

    EXPECT_EQ(built, true);
    EXPECT_EQ(prebuilt_request.capacity(), expected_prebuilt_request.size());
    EXPECT_EQ(prebuilt_request, expected_prebuilt_request);
  }

  // Pre-build using a map that should generate more than one XRegisterSet frames
  {
    std::vector<uint8_t> prebuilt_request;
    std::map<uint32_t, uint16_t> grouped_register_sets{{REGISTER_SET_TO_UINT32(11, 0, 0), 1},
                                                       {REGISTER_SET_TO_UINT32(11, 1, 0), 2}};
    auto built = app->prebuilt_data_request(grouped_register_sets, prebuilt_request);

    // SupervisoryFrame - [0xff, 0x1, 'T', 'O', 'T',  'A',  'L', 'F', 'L',  'O', 'W', ' ', 0x58, 0xff, 0xfe, 0x3a]
    // SyncFrame        - [0xd3, 0xd9, 0xcE, 0xc3]
    // XSingleHeader    - [0x3, 0x0, 0xfc, 0xff, 0x0, 0x0, 0x2, 0xd2, 0xc1]
    // XRegisterSet     - [0xa, 0x0, 0xf5, 0xff, 0x0, 0x2, 0xb, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x2f, 0x3f]
    // XRegisterSet     - [0xa, 0x0, 0xf5, 0xff, 0x0, 0x2, 0xb, 0x1, 0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0x3f, 0xbb]
    const std::vector<uint8_t> expected_prebuilt_request{
        0xff, 0x1,  'T',  'O',  'T', 'A', 'L',  'F',  'L', 'O', 'W', ' ',  0x58, 0xff, 0xfe, 0x3a,
        0xd3, 0xd9, 0xcE, 0xc3, 0x3, 0x0, 0xfc, 0xff, 0x0, 0x0, 0x2, 0xd2, 0xc1, 0xa,  0x0,  0xf5,
        0xff, 0x0,  0x2,  0xb,  0x0, 0x0, 0x0,  0x1,  0x0, 0x0, 0x0, 0x2f, 0x3f, 0xa,  0x0,  0xf5,
        0xff, 0x0,  0x2,  0xb,  0x1, 0x0, 0x0,  0x2,  0x0, 0x0, 0x0, 0x3f, 0xbb};

    EXPECT_EQ(built, true);
    EXPECT_EQ(prebuilt_request.capacity(), expected_prebuilt_request.size());
    EXPECT_EQ(prebuilt_request, expected_prebuilt_request);
  }

  // verbose = true;
}

/** The following cases need to be tested
 * 1 - Normal message exchange - a DataTransmission message is received and parsed successfully
 * 2 - DataRequest NACK sent by the RTU
 * 3 - The host received a DataTransmission with a CRC error
 */
TEST_F(Db2RegisterTest, ReadTransaction) {
  auto config = ConfigurationInterface::load_file(CONFIG_FILENAME);
  // Create an instance of the Db2Register application
  auto app = std::make_unique<Db2RegisterApplication>();
  ASSERT_TRUE(app->initialize(*config));
  app->set_descriptor(writer.get());

  // 1 - Normal message exchange
  // For now, we are only interested in testing a successful DataRequest transactions. If the DataRequest fails, it
  // won't be resent.
  {
    // Set the state of the transaction status to TransactionStatus::SENT_DATA_REQUEST by sending a DataRequest
    // SupervisoryFrame + SyncFrame + XSingleHeader + 1 * XRegisterSet (read 3 registers staring at 11.3.71)
    std::vector<uint8_t> request{0xff, 0x1,  'T',  'O',  'T',  'A', 'L', 'F',  'L',  'O', 'W', ' ', 0x58, 0xff, 0xfe,
                                 0x3a, 0xd3, 0xd9, 0xce, 0xc3, 0x3, 0x0, 0xfc, 0xff, 0x0, 0x0, 0x1, 0x92, 0xc0, 0xa,
                                 0x0,  0xf5, 0xff, 0x0,  0x2,  0xb, 0x3, 0x47, 0x0,  0x3, 0x0, 0x0, 0x0,  0x12, 0xf0};
    app->poll_data(request);
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::SENT_DATA_REQUEST);

    // Read back the DataRequest
    std::vector<uint8_t> buffer(request.size(), '\0');
    auto ret = read(send_fds[READ], &buffer[0], buffer.size());
    ASSERT_EQ(ret, static_cast<int>(request.size()));
    EXPECT_EQ(request, buffer);

    // Simulate the RTU response - HeaderType = 0, ControlByte = 2
    // SupervisoryFrame + XSingleHeader + 1 * XRegisterData (12 bytes of data from registers 11.3.71 to 11.3.73)
    const std::vector<uint8_t> response{0x1,  'T',  'O',  'T',  'A',  'L',  'F',  'L',  'O',  'W',  ' ',  0x58, 0x0,
                                        0x81, 0x7a, 0x3,  0x0,  0xfc, 0xff, 0x0,  0x2,  0x1,  0x93, 0xa0, 0x16, 0x0,
                                        0xe9, 0xff, 0x0,  0x2,  0xb,  0x3,  0x47, 0x0,  0x3,  0x0,  0x0,  0x0,  0xde,
                                        0xad, 0xfe, 0xed, 0xbe, 0xef, 0xc,  0xab, 0xde, 0xad, 0xfe, 0xed, 0xa5, 0x4d};
    ret = write(fds[WRITE], &response[0], response.size());
    ASSERT_EQ(ret, static_cast<int>(response.size()));
    EXPECT_EQ(app->get_properties().pending_read_requests.size(), 1U);
    app->rtu_response(reader.get());
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::IDLE);
    EXPECT_EQ(app->get_properties().pending_read_requests.size(), 0U);
  }

  // 2 - DataRequest NACK - HeaderType = 1, ControlByte = 2
  {
    // Set the state of the transaction status to TransactionStatus::SENT_DATA_REQUEST by sending a DataRequest
    // SupervisoryFrame + SyncFrame + XSingleHeader + 1 * XRegisterSet (read 3 registers staring at 11.3.71)
    std::vector<uint8_t> request{0xff, 0x1,  'T',  'O',  'T',  'A', 'L', 'F',  'L',  'O', 'W', ' ', 0x58, 0xff, 0xfe,
                                 0x3a, 0xd3, 0xd9, 0xce, 0xc3, 0x3, 0x0, 0xfc, 0xff, 0x0, 0x0, 0x1, 0x92, 0xc0, 0xa,
                                 0x0,  0xf5, 0xff, 0x0,  0x2,  0xb, 0x3, 0x47, 0x0,  0x3, 0x0, 0x0, 0x0,  0x12, 0xf0};
    app->poll_data(request);
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::SENT_DATA_REQUEST);

    // Read back the DataRequest
    std::vector<uint8_t> buffer(request.size(), '\0');
    auto ret = read(send_fds[READ], &buffer[0], buffer.size());
    ASSERT_EQ(ret, static_cast<int>(request.size()));
    EXPECT_EQ(request, buffer);

    // Simulate the RTU response
    // SupervisoryFrame + XSingleHeader
    const std::vector<uint8_t> response{0x1, 'T',  'O',  'T', 'A', 'L',  'F',  'L', 'O', 'W',  ' ',  0x58,
                                        0x0, 0x81, 0x7a, 0x3, 0x0, 0xfc, 0xff, 0x1, 0x2, 0x66, 0x83, 0x8a};
    ret = write(fds[WRITE], &response[0], response.size());
    ASSERT_EQ(ret, static_cast<int>(response.size()));
    EXPECT_EQ(app->get_properties().pending_read_requests.size(), 1U);

    app->rtu_response(reader.get());

    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::IDLE);
    EXPECT_EQ(app->get_properties().pending_read_requests.size(), 0U);
  }

  // 3 - Received a DataTransmission with a CRC error
  {
    // Set the state of the transaction status to TransactionStatus::SENT_DATA_REQUEST by sending a DataRequest
    // SupervisoryFrame + SyncFrame + XSingleHeader + 1 * XRegisterSet (read 3 registers staring at 11.3.71)
    std::vector<uint8_t> request{0xff, 0x1,  'T',  'O',  'T',  'A', 'L', 'F',  'L',  'O', 'W', ' ', 0x58, 0xff, 0xfe,
                                 0x3a, 0xd3, 0xd9, 0xce, 0xc3, 0x3, 0x0, 0xfc, 0xff, 0x0, 0x0, 0x1, 0x92, 0xc0, 0xa,
                                 0x0,  0xf5, 0xff, 0x0,  0x2,  0xb, 0x3, 0x47, 0x0,  0x3, 0x0, 0x0, 0x0,  0x12, 0xf0};
    app->poll_data(request);
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::SENT_DATA_REQUEST);

    // Read back the DataRequest
    std::vector<uint8_t> buffer(request.size(), '\0');
    auto ret = read(send_fds[READ], &buffer[0], buffer.size());
    ASSERT_EQ(ret, static_cast<int>(request.size()));
    EXPECT_EQ(request, buffer);

    // Simulate the RTU containing a CRC error by changing the StationID.
    // SupervisoryFrame + XSingleHeader + 1 * XRegisterData (12 bytes of data from registers 11.3.71 to 11.3.73)
    const std::vector<uint8_t> response{0x1,  'T',  'O',  'T',  'A',  'L',  'F',  'L',  'O',  'Z',  ' ',  0x58, 0x0,
                                        0x81, 0x7a, 0x3,  0x0,  0xfc, 0xff, 0x0,  0x2,  0x1,  0x93, 0xa0, 0x16, 0x0,
                                        0xe9, 0xff, 0x0,  0x2,  0xb,  0x3,  0x47, 0x0,  0x3,  0x0,  0x0,  0x0,  0xde,
                                        0xad, 0xfe, 0xed, 0xbe, 0xef, 0xc,  0xab, 0xde, 0xad, 0xfe, 0xed, 0xa5, 0x4d};
    ret = write(fds[WRITE], &response[0], response.size());
    ASSERT_EQ(ret, static_cast<int>(response.size()));
    EXPECT_EQ(app->get_properties().pending_read_requests.size(), 1U);
    app->rtu_response(reader.get());
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::IDLE);
    EXPECT_EQ(app->get_properties().pending_read_requests.size(), 0U);
  }

  /// Other cases

  // rtu_response callback called even if a request wasn't sent (very unlikely to happen)
  {
    ASSERT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::IDLE);
    ASSERT_EQ(app->get_properties().pending_read_requests.size(), 0U);

    // Simulate the RTU response
    // SupervisoryFrame + XSingleHeader + 1 * XRegisterData (12 bytes of data from registers 11.3.71 to 11.3.73)
    const std::vector<uint8_t> response{0x1,  'T',  'O',  'T',  'A',  'L',  'F',  'L',  'O',  'W',  ' ',  0x58, 0x0,
                                        0x81, 0x7a, 0x3,  0x0,  0xfc, 0xff, 0x0,  0x2,  0x1,  0x93, 0xa0, 0x16, 0x0,
                                        0xe9, 0xff, 0x0,  0x2,  0xb,  0x3,  0x47, 0x0,  0x3,  0x0,  0x0,  0x0,  0xde,
                                        0xad, 0xfe, 0xed, 0xbe, 0xef, 0xc,  0xab, 0xde, 0xad, 0xfe, 0xed, 0xa5, 0x4d};
    auto ret = write(fds[WRITE], &response[0], response.size());
    ASSERT_EQ(ret, static_cast<int>(response.size()));

    app->rtu_response(reader.get());
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::IDLE);
  }

  // verbose = true;
}

/** The following cases need to be tested
 * 1 - Normal message exchange - no Confirmation message errors or CRC mismathces
 * 2 - Confirmation NACK sent by the RTU
 * 3 - Confirmation message with CRC mismatch
 * 4 - DataTransmission NACK
 * 5 - Acknowledgement CRC mismatch
 * 6 - Test maximum number of retries when a WriteRequest or a DataTransmission NACK is received
 * 7 - Test unhandled WriteRequest Confirmation Header Type and Control Byte
 *   a - HeaderType = 0 and ControlByte = 0
 *   b - HeaderType = 0 and ControlByte = 2
 * 8 - Test unhandled DataTransmission Acknowledgement Header Type and Control Byte
 *   a - HeaderType = 0 and ControlByte = 1
 *   b - HeaderType = 0 and ControlByte = 2
 */
TEST_F(Db2RegisterTest, WriteTransaction) {
  auto config = ConfigurationInterface::load_file(CONFIG_FILENAME);
  // Create an instance of the Db2Register application
  auto app = std::make_unique<Db2RegisterApplication>();
  ASSERT_TRUE(app->initialize(*config));
  app->set_descriptor(writer.get());

  // 1 - Test a successful Write Transaction
  {
    // Call the app process_data method. It has an hardcoded WriteRequest.
    std::list<std::unique_ptr<MessageInterface>> msgs;
    app->process_data(msgs);
    EXPECT_EQ(app->get_properties().pending_write_requests.size(), 2U);
    EXPECT_EQ(app->get_properties().pending_read_requests.size(), 0U);
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::SENT_WRITE_REQUEST);

    // Simulate a Confirmation message sent by the RTU
    const std::vector<uint8_t> confirmation{0x1, 'T',  'O',  'T', 'A', 'L',  'F',  'L', 'O', 'W', ' ', 0x58,
                                            0x0, 0x81, 0x7a, 0x3, 0x0, 0xfc, 0xff, 0x1, 0x0, 0x0, 0x2, 0xc0};
    auto ret = write(fds[WRITE], &confirmation[0], confirmation.size());
    ASSERT_EQ(ret, static_cast<int>(confirmation.size()));

    app->rtu_response(reader.get());
    EXPECT_EQ(app->get_properties().transaction_retries, 0U);
    EXPECT_EQ(app->get_properties().transaction_status,
              Db2RegisterApplication::TransactionStatus::SENT_DATA_TRANSMISSION);

    // Simulate an Acknowledgement message sent by the RTU
    const std::vector<uint8_t> ack{0x1, 'T',  'O',  'T', 'A', 'L',  'F',  'L', 'O', 'W', ' ', 0x58,
                                   0x0, 0x81, 0x7a, 0x3, 0x0, 0xfc, 0xff, 0x1, 0x1, 0x0, 0x3, 0x50};
    ret = write(fds[WRITE], &ack[0], ack.size());
    ASSERT_EQ(ret, static_cast<int>(ack.size()));

    app->rtu_response(reader.get());
    EXPECT_EQ(app->get_properties().transaction_retries, 0U);
    EXPECT_EQ(app->get_properties().pending_write_requests.size(), 0U);
    EXPECT_EQ(app->get_properties().pending_read_requests.size(), 0U);
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::IDLE);
  }

  // 2 - Test resending a Write Transaction due to a Confirmation NACK sent by the RTU
  {
    // Call the app process_data method. It has an hardcoded WriteRequest.
    std::list<std::unique_ptr<MessageInterface>> msgs;
    app->process_data(msgs);
    EXPECT_EQ(app->get_properties().pending_write_requests.size(), 2U);
    EXPECT_EQ(app->get_properties().pending_read_requests.size(), 0U);
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::SENT_WRITE_REQUEST);

    // Simulate a Confirmation message NACK sent by the RTU
    const std::vector<uint8_t> nack{0x1, 'T',  'O',  'T', 'A', 'L',  'F',  'L', 'O', 'W', ' ', 0x58,
                                    0x0, 0x81, 0x7a, 0x3, 0x0, 0xfc, 0xff, 0x1, 0x2, 0x0, 0x3, 0xa0};
    auto ret = write(fds[WRITE], &nack[0], nack.size());
    ASSERT_EQ(ret, static_cast<int>(nack.size()));

    app->rtu_response(reader.get());
    EXPECT_EQ(app->get_properties().transaction_retries, 1U);
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::SENT_WRITE_REQUEST);

    // Simulate a Confirmation message sent by the RTU
    const std::vector<uint8_t> confirmation{0x1, 'T',  'O',  'T', 'A', 'L',  'F',  'L', 'O', 'W', ' ', 0x58,
                                            0x0, 0x81, 0x7a, 0x3, 0x0, 0xfc, 0xff, 0x1, 0x0, 0x0, 0x2, 0xc0};
    ret = write(fds[WRITE], &confirmation[0], confirmation.size());
    ASSERT_EQ(ret, static_cast<int>(confirmation.size()));

    app->rtu_response(reader.get());
    EXPECT_EQ(app->get_properties().transaction_retries, 0U);
    EXPECT_EQ(app->get_properties().transaction_status,
              Db2RegisterApplication::TransactionStatus::SENT_DATA_TRANSMISSION);

    // Simulate an Acknowledgement message sent by the RTU
    const std::vector<uint8_t> ack{0x1, 'T',  'O',  'T', 'A', 'L',  'F',  'L', 'O', 'W', ' ', 0x58,
                                   0x0, 0x81, 0x7a, 0x3, 0x0, 0xfc, 0xff, 0x1, 0x1, 0x0, 0x3, 0x50};
    ret = write(fds[WRITE], &ack[0], ack.size());
    ASSERT_EQ(ret, static_cast<int>(ack.size()));

    app->rtu_response(reader.get());
    EXPECT_EQ(app->get_properties().transaction_retries, 0U);
    EXPECT_EQ(app->get_properties().pending_write_requests.size(), 0U);
    EXPECT_EQ(app->get_properties().pending_read_requests.size(), 0U);
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::IDLE);
  }

  // 3 - Resending a Write Transaction due to a Confirmation message CRC mismatch
  {
    // Call the app process_data method. It has an hardcoded WriteRequest.
    std::list<std::unique_ptr<MessageInterface>> msgs;
    app->process_data(msgs);
    EXPECT_EQ(app->get_properties().pending_write_requests.size(), 2U);
    EXPECT_EQ(app->get_properties().pending_read_requests.size(), 0U);
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::SENT_WRITE_REQUEST);

    // Simulate a Confirmation message that will generate a CRC mismatch
    const std::vector<uint8_t> confirmation_crc_error{0x1, 'T',  'O',  'T', 'A', 'L',  'F',  'L', 'O', 'Z', ' ', 0x58,
                                                      0x0, 0x81, 0x7a, 0x3, 0x0, 0xfc, 0xff, 0x1, 0x0, 0x0, 0x2, 0xc0};
    auto ret = write(fds[WRITE], &confirmation_crc_error[0], confirmation_crc_error.size());
    ASSERT_EQ(ret, static_cast<int>(confirmation_crc_error.size()));

    app->rtu_response(reader.get());
    EXPECT_EQ(app->get_properties().transaction_retries, 1U);
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::SENT_WRITE_REQUEST);

    // Simulate a Confirmation message sent by the RTU
    const std::vector<uint8_t> confirmation{0x1, 'T',  'O',  'T', 'A', 'L',  'F',  'L', 'O', 'W', ' ', 0x58,
                                            0x0, 0x81, 0x7a, 0x3, 0x0, 0xfc, 0xff, 0x1, 0x0, 0x0, 0x2, 0xc0};
    ret = write(fds[WRITE], &confirmation[0], confirmation.size());
    ASSERT_EQ(ret, static_cast<int>(confirmation.size()));

    app->rtu_response(reader.get());
    EXPECT_EQ(app->get_properties().transaction_retries, 0U);
    EXPECT_EQ(app->get_properties().transaction_status,
              Db2RegisterApplication::TransactionStatus::SENT_DATA_TRANSMISSION);

    // Simulate an Acknowledgement message sent by the RTU
    const std::vector<uint8_t> ack{0x1, 'T',  'O',  'T', 'A', 'L',  'F',  'L', 'O', 'W', ' ', 0x58,
                                   0x0, 0x81, 0x7a, 0x3, 0x0, 0xfc, 0xff, 0x1, 0x1, 0x0, 0x3, 0x50};
    ret = write(fds[WRITE], &ack[0], ack.size());
    ASSERT_EQ(ret, static_cast<int>(ack.size()));

    app->rtu_response(reader.get());
    EXPECT_EQ(app->get_properties().transaction_retries, 0U);
    EXPECT_EQ(app->get_properties().pending_write_requests.size(), 0U);
    EXPECT_EQ(app->get_properties().pending_read_requests.size(), 0U);
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::IDLE);
  }

  // 4 - Resending a Write Transaction due to a DataTransmission NACK
  {
    // Call the app process_data method. It has an hardcoded WriteRequest.
    std::list<std::unique_ptr<MessageInterface>> msgs;
    app->process_data(msgs);
    EXPECT_EQ(app->get_properties().pending_write_requests.size(), 2U);
    EXPECT_EQ(app->get_properties().pending_read_requests.size(), 0U);
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::SENT_WRITE_REQUEST);

    // Simulate a Confirmation message sent by the RTU
    const std::vector<uint8_t> confirmation{0x1, 'T',  'O',  'T', 'A', 'L',  'F',  'L', 'O', 'W', ' ', 0x58,
                                            0x0, 0x81, 0x7a, 0x3, 0x0, 0xfc, 0xff, 0x1, 0x0, 0x0, 0x2, 0xc0};
    auto ret = write(fds[WRITE], &confirmation[0], confirmation.size());
    ASSERT_EQ(ret, static_cast<int>(confirmation.size()));

    app->rtu_response(reader.get());
    EXPECT_EQ(app->get_properties().transaction_retries, 0U);
    EXPECT_EQ(app->get_properties().transaction_status,
              Db2RegisterApplication::TransactionStatus::SENT_DATA_TRANSMISSION);

    // Simulate DataTransmission NACK
    const std::vector<uint8_t> nack{0x1, 'T',  'O',  'T', 'A', 'L',  'F',  'L', 'O', 'W', ' ', 0x58,
                                    0x0, 0x81, 0x7a, 0x3, 0x0, 0xfc, 0xff, 0x1, 0x2, 0x0, 0x3, 0xa0};
    ret = write(fds[WRITE], &nack[0], nack.size());
    ASSERT_EQ(ret, static_cast<int>(nack.size()));

    app->rtu_response(reader.get());
    EXPECT_EQ(app->get_properties().transaction_retries, 1U);
    EXPECT_EQ(app->get_properties().pending_write_requests.size(), 2U);
    EXPECT_EQ(app->get_properties().pending_read_requests.size(), 0U);
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::SENT_WRITE_REQUEST);

    // Simulate a Confirmation message sent by the RTU
    ret = write(fds[WRITE], &confirmation[0], confirmation.size());
    ASSERT_EQ(ret, static_cast<int>(confirmation.size()));

    app->rtu_response(reader.get());
    EXPECT_EQ(app->get_properties().transaction_retries, 0U);
    EXPECT_EQ(app->get_properties().transaction_status,
              Db2RegisterApplication::TransactionStatus::SENT_DATA_TRANSMISSION);

    // Simulate an Acknowledgement message sent by the RTU
    const std::vector<uint8_t> ack{0x1, 'T',  'O',  'T', 'A', 'L',  'F',  'L', 'O', 'W', ' ', 0x58,
                                   0x0, 0x81, 0x7a, 0x3, 0x0, 0xfc, 0xff, 0x1, 0x1, 0x0, 0x3, 0x50};
    ret = write(fds[WRITE], &ack[0], ack.size());
    ASSERT_EQ(ret, static_cast<int>(ack.size()));

    app->rtu_response(reader.get());
    EXPECT_EQ(app->get_properties().transaction_retries, 0U);
    EXPECT_EQ(app->get_properties().pending_write_requests.size(), 0U);
    EXPECT_EQ(app->get_properties().pending_read_requests.size(), 0U);
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::IDLE);
  }

  // 5 - DataTransmission Acknowledgement CRC mismatch
  {
    // Call the app process_data method. It has an hardcoded WriteRequest.
    std::list<std::unique_ptr<MessageInterface>> msgs;
    app->process_data(msgs);
    EXPECT_EQ(app->get_properties().pending_write_requests.size(), 2U);
    EXPECT_EQ(app->get_properties().pending_read_requests.size(), 0U);
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::SENT_WRITE_REQUEST);

    // Simulate a Confirmation message sent by the RTU
    const std::vector<uint8_t> confirmation{0x1, 'T',  'O',  'T', 'A', 'L',  'F',  'L', 'O', 'W', ' ', 0x58,
                                            0x0, 0x81, 0x7a, 0x3, 0x0, 0xfc, 0xff, 0x1, 0x0, 0x0, 0x2, 0xc0};
    auto ret = write(fds[WRITE], &confirmation[0], confirmation.size());
    ASSERT_EQ(ret, static_cast<int>(confirmation.size()));

    app->rtu_response(reader.get());
    EXPECT_EQ(app->get_properties().transaction_retries, 0U);
    EXPECT_EQ(app->get_properties().transaction_status,
              Db2RegisterApplication::TransactionStatus::SENT_DATA_TRANSMISSION);

    // Simulate an Acknowledgement with CRC a mismatch
    const std::vector<uint8_t> ack_crc{0x1, 'T',  'O',  'T', 'A', 'L',  'F',  'L', 'O', 'Z', ' ', 0x58,
                                       0x0, 0x81, 0x7a, 0x3, 0x0, 0xfc, 0xff, 0x1, 0x1, 0x0, 0x3, 0x50};
    ret = write(fds[WRITE], &ack_crc[0], ack_crc.size());
    ASSERT_EQ(ret, static_cast<int>(ack_crc.size()));

    app->rtu_response(reader.get());
    EXPECT_EQ(app->get_properties().transaction_retries, 1U);
    EXPECT_EQ(app->get_properties().pending_write_requests.size(), 2U);
    EXPECT_EQ(app->get_properties().pending_read_requests.size(), 0U);
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::SENT_WRITE_REQUEST);

    // Simulate a Confirmation message sent by the RTU
    ret = write(fds[WRITE], &confirmation[0], confirmation.size());
    ASSERT_EQ(ret, static_cast<int>(confirmation.size()));

    app->rtu_response(reader.get());
    EXPECT_EQ(app->get_properties().transaction_retries, 0U);
    EXPECT_EQ(app->get_properties().transaction_status,
              Db2RegisterApplication::TransactionStatus::SENT_DATA_TRANSMISSION);

    // Simulate an Acknowledgement message sent by the RTU
    const std::vector<uint8_t> ack{0x1, 'T',  'O',  'T', 'A', 'L',  'F',  'L', 'O', 'W', ' ', 0x58,
                                   0x0, 0x81, 0x7a, 0x3, 0x0, 0xfc, 0xff, 0x1, 0x1, 0x0, 0x3, 0x50};
    ret = write(fds[WRITE], &ack[0], ack.size());
    ASSERT_EQ(ret, static_cast<int>(ack.size()));

    app->rtu_response(reader.get());
    EXPECT_EQ(app->get_properties().transaction_retries, 0U);
    EXPECT_EQ(app->get_properties().pending_write_requests.size(), 0U);
    EXPECT_EQ(app->get_properties().pending_read_requests.size(), 0U);
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::IDLE);
  }

  // 6 - Test resending a Write Transaction due to 3 NACKs sent by the RTU (the transaction should be canceled after 3
  // Confirmation NACK messages)
  {
    // Call the app process_data method. It has an hardcoded WriteRequest.
    EXPECT_EQ(app->get_properties().transaction_retries, 0U);

    std::list<std::unique_ptr<MessageInterface>> msgs;
    app->process_data(msgs);
    EXPECT_EQ(app->get_properties().pending_write_requests.size(), 2U);
    EXPECT_EQ(app->get_properties().pending_read_requests.size(), 0U);
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::SENT_WRITE_REQUEST);

    // Simulate a Confirmation NACK message sent by the RTU
    const std::vector<uint8_t> nack{0x1, 'T',  'O',  'T', 'A', 'L',  'F',  'L', 'O', 'W', ' ', 0x58,
                                    0x0, 0x81, 0x7a, 0x3, 0x0, 0xfc, 0xff, 0x1, 0x2, 0x0, 0x3, 0xa0};

    for(auto i = 0U; i < 3; i++) {
      auto ret = write(fds[WRITE], &nack[0], nack.size());
      ASSERT_EQ(ret, static_cast<int>(nack.size()));

      app->rtu_response(reader.get());
    }

    EXPECT_EQ(app->get_properties().transaction_retries, 0U);
    EXPECT_EQ(app->get_properties().pending_write_requests.size(), 0U);
    EXPECT_EQ(app->get_properties().pending_read_requests.size(), 0U);
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::IDLE);
  }

  // 7a - Test unhandled WriteRequest Confirmation HeaderType = 0 and ControlByte = 0
  {
    // Call the app process_data method. It has an hardcoded WriteRequest.
    std::list<std::unique_ptr<MessageInterface>> msgs;
    app->process_data(msgs);
    EXPECT_EQ(app->get_properties().pending_write_requests.size(), 2U);
    EXPECT_EQ(app->get_properties().pending_read_requests.size(), 0U);
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::SENT_WRITE_REQUEST);

    // Simulate unhandled Confirmation message sent by the RTU
    const std::vector<uint8_t> confirmation{0x1, 'T',  'O',  'T', 'A', 'L',  'F',  'L', 'O', 'W', ' ',  0x58,
                                            0x0, 0x81, 0x7a, 0x3, 0x0, 0xfc, 0xff, 0x0, 0x0, 0x0, 0x53, 0x0};
    auto ret = write(fds[WRITE], &confirmation[0], confirmation.size());
    ASSERT_EQ(ret, static_cast<int>(confirmation.size()));

    app->rtu_response(reader.get());
    EXPECT_EQ(app->get_properties().transaction_retries, 0U);
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::IDLE);
  }

  // 7b - Test unhandled WriteRequest Confirmation HeaderType = 0 and ControlByte = 2
  {
    // Call the app process_data method. It has an hardcoded WriteRequest.
    std::list<std::unique_ptr<MessageInterface>> msgs;
    app->process_data(msgs);
    EXPECT_EQ(app->get_properties().pending_write_requests.size(), 2U);
    EXPECT_EQ(app->get_properties().pending_read_requests.size(), 0U);
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::SENT_WRITE_REQUEST);

    // Simulate unhandled Confirmation message sent by the RTU
    const std::vector<uint8_t> confirmation{0x1, 'T',  'O',  'T', 'A', 'L',  'F',  'L', 'O', 'W', ' ',  0x58,
                                            0x0, 0x81, 0x7a, 0x3, 0x0, 0xfc, 0xff, 0x0, 0x2, 0x0, 0x52, 0x60};
    auto ret = write(fds[WRITE], &confirmation[0], confirmation.size());
    ASSERT_EQ(ret, static_cast<int>(confirmation.size()));

    app->rtu_response(reader.get());
    EXPECT_EQ(app->get_properties().transaction_retries, 0U);
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::IDLE);
  }

  // 8a - Test unhandled DataTransmission Confirmation HeaderType = 0 and ControlByte = 0
  {
    // Call the app process_data method. It has an hardcoded WriteRequest.
    std::list<std::unique_ptr<MessageInterface>> msgs;
    app->process_data(msgs);
    EXPECT_EQ(app->get_properties().pending_write_requests.size(), 2U);
    EXPECT_EQ(app->get_properties().pending_read_requests.size(), 0U);
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::SENT_WRITE_REQUEST);

    // Simulate a Confirmation message sent by the RTU
    const std::vector<uint8_t> confirmation{0x1, 'T',  'O',  'T', 'A', 'L',  'F',  'L', 'O', 'W', ' ', 0x58,
                                            0x0, 0x81, 0x7a, 0x3, 0x0, 0xfc, 0xff, 0x1, 0x0, 0x0, 0x2, 0xc0};
    auto ret = write(fds[WRITE], &confirmation[0], confirmation.size());
    ASSERT_EQ(ret, static_cast<int>(confirmation.size()));

    app->rtu_response(reader.get());
    EXPECT_EQ(app->get_properties().transaction_retries, 0U);
    EXPECT_EQ(app->get_properties().transaction_status,
              Db2RegisterApplication::TransactionStatus::SENT_DATA_TRANSMISSION);

    // Simulate an unhandled DataTransmission Acknowledgement
    const std::vector<uint8_t> ack{0x1, 'T',  'O',  'T', 'A', 'L',  'F',  'L', 'O', 'W', ' ',  0x58,
                                   0x0, 0x81, 0x7a, 0x3, 0x0, 0xfc, 0xff, 0x0, 0x0, 0x0, 0x53, 0x0};
    ret = write(fds[WRITE], &ack[0], ack.size());
    ASSERT_EQ(ret, static_cast<int>(ack.size()));

    app->rtu_response(reader.get());
    EXPECT_EQ(app->get_properties().transaction_retries, 0U);
    EXPECT_EQ(app->get_properties().pending_write_requests.size(), 0U);
    EXPECT_EQ(app->get_properties().pending_read_requests.size(), 0U);
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::IDLE);
  }

  // 8b - Test unhandled DataTransmission Confirmation HeaderType = 0 and ControlByte = 2
  {
    // Call the app process_data method. It has an hardcoded WriteRequest.
    std::list<std::unique_ptr<MessageInterface>> msgs;
    app->process_data(msgs);
    EXPECT_EQ(app->get_properties().pending_write_requests.size(), 2U);
    EXPECT_EQ(app->get_properties().pending_read_requests.size(), 0U);
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::SENT_WRITE_REQUEST);

    // Simulate a Confirmation message sent by the RTU
    const std::vector<uint8_t> confirmation{0x1, 'T',  'O',  'T', 'A', 'L',  'F',  'L', 'O', 'W', ' ', 0x58,
                                            0x0, 0x81, 0x7a, 0x3, 0x0, 0xfc, 0xff, 0x1, 0x0, 0x0, 0x2, 0xc0};
    auto ret = write(fds[WRITE], &confirmation[0], confirmation.size());
    ASSERT_EQ(ret, static_cast<int>(confirmation.size()));

    app->rtu_response(reader.get());
    EXPECT_EQ(app->get_properties().transaction_retries, 0U);
    EXPECT_EQ(app->get_properties().transaction_status,
              Db2RegisterApplication::TransactionStatus::SENT_DATA_TRANSMISSION);

    // Simulate an unhandled DataTransmission Acknowledgement
    const std::vector<uint8_t> ack{0x1, 'T',  'O',  'T', 'A', 'L',  'F',  'L', 'O', 'W', ' ',  0x58,
                                   0x0, 0x81, 0x7a, 0x3, 0x0, 0xfc, 0xff, 0x0, 0x2, 0x0, 0x52, 0x60};
    ret = write(fds[WRITE], &ack[0], ack.size());
    ASSERT_EQ(ret, static_cast<int>(ack.size()));

    app->rtu_response(reader.get());
    EXPECT_EQ(app->get_properties().transaction_retries, 0U);
    EXPECT_EQ(app->get_properties().pending_write_requests.size(), 0U);
    EXPECT_EQ(app->get_properties().pending_read_requests.size(), 0U);
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::IDLE);
  }

  /// Other cases

  // Test the insertion of multiple WriteRequests
  {
    // Call the app process_data method. It has an hardcoded WriteRequest.
    std::list<std::unique_ptr<MessageInterface>> msgs;
    // Simultate multiple WriteRequest calls
    app->process_data(msgs);
    app->process_data(msgs);
    app->process_data(msgs);
    app->process_data(msgs);

    EXPECT_EQ(app->get_properties().pending_write_requests.size(), 8U);
    EXPECT_EQ(app->get_properties().pending_read_requests.size(), 0U);
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::SENT_WRITE_REQUEST);

    // Simulate the responses to the WriteRequest messages
    const std::vector<uint8_t> confirmation{0x1, 'T',  'O',  'T', 'A', 'L',  'F',  'L', 'O', 'W', ' ', 0x58,
                                            0x0, 0x81, 0x7a, 0x3, 0x0, 0xfc, 0xff, 0x1, 0x0, 0x0, 0x2, 0xc0};
    const std::vector<uint8_t> ack{0x1, 'T',  'O',  'T', 'A', 'L',  'F',  'L', 'O', 'W', ' ', 0x58,
                                   0x0, 0x81, 0x7a, 0x3, 0x0, 0xfc, 0xff, 0x1, 0x1, 0x0, 0x3, 0x50};

    auto ret = 0;
    auto pending_write_requests_count = 8U;
    for(auto i = 0U; i < 4; ++i) {
      // Simulate a Confirmation message sent by the RTU
      ret = write(fds[WRITE], &confirmation[0], confirmation.size());
      ASSERT_EQ(ret, static_cast<int>(confirmation.size()));

      app->rtu_response(reader.get());
      EXPECT_EQ(app->get_properties().transaction_retries, 0U);
      EXPECT_EQ(app->get_properties().transaction_status,
                Db2RegisterApplication::TransactionStatus::SENT_DATA_TRANSMISSION);

      // Simulate an Acknowledgement message sent by the RTU
      ret = write(fds[WRITE], &ack[0], ack.size());
      ASSERT_EQ(ret, static_cast<int>(ack.size()));

      pending_write_requests_count -= 2;

      app->rtu_response(reader.get());
      EXPECT_EQ(app->get_properties().transaction_retries, 0U);
      EXPECT_EQ(app->get_properties().pending_write_requests.size(), pending_write_requests_count);
      EXPECT_EQ(app->get_properties().pending_read_requests.size(), 0U);

      if(i < 3) {
        EXPECT_EQ(app->get_properties().transaction_status,
                  Db2RegisterApplication::TransactionStatus::SENT_WRITE_REQUEST);
      } else {
        EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::IDLE);
      }
    }
  }

  // Test the insertion of multiple WriteRequest and DataRequest messages. The WriteRequest messages should have
  // priority over the DataRequest messages
  {
    // Call the app process_data method. It has an hardcoded WriteRequest.
    std::list<std::unique_ptr<MessageInterface>> msgs;
    // Simultate multiple WriteRequest calls
    app->process_data(msgs);
    app->process_data(msgs);
    app->process_data(msgs);

    // Simulate a DataRequest. This request should be inserted into the pending read requests queue and delay till all
    // the WriteRequest messages are handled
    std::vector<uint8_t> data_request{0xff, 0x1,  'T',  'O',  'T',  'A',  'L',  'F',  'L',  'O', 'W',  ' ',
                                      0x58, 0xff, 0xfe, 0x3a, 0xd3, 0xd9, 0xce, 0xc3, 0x3,  0x0, 0xfc, 0xff,
                                      0x0,  0x0,  0x1,  0x92, 0xc0, 0xa,  0x0,  0xf5, 0xff, 0x0, 0x2,  0xb,
                                      0x3,  0x47, 0x0,  0x3,  0x0,  0x0,  0x0,  0x12, 0xf0};
    app->poll_data(data_request);
    // The current state shouldn't be affected
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::SENT_WRITE_REQUEST);
    EXPECT_EQ(app->get_properties().pending_read_requests.size(), 1U);
    EXPECT_EQ(app->get_properties().pending_write_requests.size(), 6U);

    // Simulate the responses to the WriteRequest messages
    const std::vector<uint8_t> confirmation{0x1, 'T',  'O',  'T', 'A', 'L',  'F',  'L', 'O', 'W', ' ', 0x58,
                                            0x0, 0x81, 0x7a, 0x3, 0x0, 0xfc, 0xff, 0x1, 0x0, 0x0, 0x2, 0xc0};
    const std::vector<uint8_t> ack{0x1, 'T',  'O',  'T', 'A', 'L',  'F',  'L', 'O', 'W', ' ', 0x58,
                                   0x0, 0x81, 0x7a, 0x3, 0x0, 0xfc, 0xff, 0x1, 0x1, 0x0, 0x3, 0x50};

    auto ret = 0;
    auto pending_write_requests_count = 6U;
    for(auto i = 0U; i < 3; ++i) {
      // Simulate a Confirmation message sent by the RTU
      ret = write(fds[WRITE], &confirmation[0], confirmation.size());
      ASSERT_EQ(ret, static_cast<int>(confirmation.size()));

      app->rtu_response(reader.get());
      EXPECT_EQ(app->get_properties().transaction_retries, 0U);
      EXPECT_EQ(app->get_properties().transaction_status,
                Db2RegisterApplication::TransactionStatus::SENT_DATA_TRANSMISSION);

      // Simulate an Acknowledgement message sent by the RTU
      ret = write(fds[WRITE], &ack[0], ack.size());
      ASSERT_EQ(ret, static_cast<int>(ack.size()));

      pending_write_requests_count -= 2;

      app->rtu_response(reader.get());
      EXPECT_EQ(app->get_properties().transaction_retries, 0U);
      EXPECT_EQ(app->get_properties().pending_write_requests.size(), pending_write_requests_count);
      EXPECT_EQ(app->get_properties().pending_read_requests.size(), 1U);

      if(i < 2) {
        EXPECT_EQ(app->get_properties().transaction_status,
                  Db2RegisterApplication::TransactionStatus::SENT_WRITE_REQUEST);
      } else {
        // All the WriteRequests were handled. The DataRequest should be the last one.
        EXPECT_EQ(app->get_properties().transaction_status,
                  Db2RegisterApplication::TransactionStatus::SENT_DATA_REQUEST);
      }
    }

    // Simulate the RTU response
    // SupervisoryFrame + XSingleHeader + 1 * XRegisterData (12 bytes of data from registers 11.3.71 to 11.3.73)
    const std::vector<uint8_t> data_transmission{
        0x1,  'T', 'O',  'T',  'A',  'L',  'F',  'L',  'O',  'W',  ' ',  0x58, 0x0,  0x81, 0x7a, 0x3, 0x0, 0xfc,
        0xff, 0x0, 0x2,  0x1,  0x93, 0xa0, 0x16, 0x0,  0xe9, 0xff, 0x0,  0x2,  0xb,  0x3,  0x47, 0x0, 0x3, 0x0,
        0x0,  0x0, 0xde, 0xad, 0xfe, 0xed, 0xbe, 0xef, 0xc,  0xab, 0xde, 0xad, 0xfe, 0xed, 0xa5, 0x4d};
    ret = write(fds[WRITE], &data_transmission[0], data_transmission.size());
    ASSERT_EQ(ret, static_cast<int>(data_transmission.size()));
    EXPECT_EQ(app->get_properties().pending_read_requests.size(), 1U);
    app->rtu_response(reader.get());
    EXPECT_EQ(app->get_properties().transaction_status, Db2RegisterApplication::TransactionStatus::IDLE);
  }

  // verbose = true;
}
