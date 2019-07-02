/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "common/Utils.hpp"
#include "TestUtils.hpp"
#include "protocols/Db2Register/Protocol.hpp"
#include "common/logging/LoggingInterface.hpp"

class Db2RegisterProtocolTest : public GtestHarness {
 public:
  void SetUp() {
    ::GtestHarness::SetUp();
    EXPECT_EQ(0U, LoggingInterface::get_instance()->set_warning_cnt(0U));
    EXPECT_EQ(0U, LoggingInterface::get_instance()->set_error_cnt(0U));
  }

  void TearDown() {
    ::GtestHarness::TearDown();
  }
};

/** Test the SupervisoryFrame
 */
void BuildSupervisoryFrame() {
  // Test empty Station Id
  {
    Protocol::Db2Register::SupervisoryFrame_t supervisory_frame;

    auto warnings = LoggingInterface::get_instance()->get_warning_cnt();

    Protocol::Db2Register::build_supervisory_frame("", supervisory_frame);

    // This should log a warning
    EXPECT_EQ(LoggingInterface::get_instance()->get_warning_cnt(), warnings + 1);

    EXPECT_EQ(supervisory_frame.sync, 0xff);
    EXPECT_EQ(supervisory_frame.soh, 0x1);
    EXPECT_TRUE(
        utils::is_equal(std::string(supervisory_frame.station_id, sizeof(supervisory_frame.station_id)), "          "));
    EXPECT_EQ(supervisory_frame.control_code, 0x58);
    EXPECT_EQ(supervisory_frame.info, 0xff);

    std::vector<uint8_t> v{0xff, 0x1, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 0x58, 0xff};
    auto crc = CRC16::generate(v);
    crc = (crc ^ 0xcfffU) ^ 0x0f00U;
    crc = static_cast<uint16_t>(((crc & 0xff00U) >> 8U) | ((crc & 0xffU) << 8U));
    EXPECT_EQ(supervisory_frame.crc, crc);

    // Cast the frame to std::vector<uint8_t>
    v.emplace_back(crc & 0xFFu);
    v.emplace_back(static_cast<uint8_t>(crc >> 8u) & 0xFFu);

    EXPECT_EQ(v, static_cast<const std::vector<uint8_t>>(supervisory_frame));
  }

  // Test maximum Station Id size allowed
  {
    Protocol::Db2Register::SupervisoryFrame_t supervisory_frame;

    auto warnings = LoggingInterface::get_instance()->get_warning_cnt();

    Protocol::Db2Register::build_supervisory_frame("TOTALFLOW 1", supervisory_frame);

    // This should log a warning
    EXPECT_EQ(LoggingInterface::get_instance()->get_warning_cnt(), warnings + 1);

    EXPECT_EQ(supervisory_frame.sync, 0xff);
    EXPECT_EQ(supervisory_frame.soh, 0x1);
    EXPECT_TRUE(
        utils::is_equal(std::string(supervisory_frame.station_id, sizeof(supervisory_frame.station_id)), "TOTALFLOW "));
    EXPECT_EQ(supervisory_frame.control_code, 0x58);
    EXPECT_EQ(supervisory_frame.info, 0xff);

    std::vector<uint8_t> v{0xff, 0x1, 'T', 'O', 'T', 'A', 'L', 'F', 'L', 'O', 'W', ' ', 0x58, 0xff};
    auto crc = CRC16::generate(v);
    crc = (crc ^ 0xcfffU) ^ 0x0f00U;
    crc = static_cast<uint16_t>(((crc & 0xff00U) >> 8U) | ((crc & 0xffU) << 8U));
    EXPECT_EQ(supervisory_frame.crc, crc);

    // Cast the frame to std::vector<uint8_t>
    v.emplace_back(crc & 0xFFu);
    v.emplace_back(static_cast<uint8_t>(crc >> 8u) & 0xFFu);

    EXPECT_EQ(v, static_cast<const std::vector<uint8_t>>(supervisory_frame));
  }

  // Test valid SupervisoryFrame
  {
    Protocol::Db2Register::SupervisoryFrame_t supervisory_frame;
    Protocol::Db2Register::build_supervisory_frame("TOTALFLOW", supervisory_frame);

    EXPECT_EQ(supervisory_frame.sync, 0xff);
    EXPECT_EQ(supervisory_frame.soh, 0x1);
    EXPECT_TRUE(
        utils::is_equal(std::string(supervisory_frame.station_id, sizeof(supervisory_frame.station_id)), "TOTALFLOW "));
    EXPECT_EQ(supervisory_frame.control_code, 0x58);
    EXPECT_EQ(supervisory_frame.info, 0xff);

    std::vector<uint8_t> v{0xff, 0x1, 'T', 'O', 'T', 'A', 'L', 'F', 'L', 'O', 'W', ' ', 0x58, 0xff};
    auto crc = CRC16::generate(v);
    crc = (crc ^ 0xcfffU) ^ 0x0f00U;
    crc = static_cast<uint16_t>(((crc & 0xff00U) >> 8U) | ((crc & 0xffU) << 8U));
    EXPECT_EQ(supervisory_frame.crc, crc);

    // Cast the frame to std::vector<uint8_t>
    v.emplace_back(crc & 0xFFu);
    v.emplace_back(static_cast<uint8_t>(crc >> 8u) & 0xFFu);

    EXPECT_EQ(v, static_cast<const std::vector<uint8_t>>(supervisory_frame));
  }
}

void ParseSupervisoryFrame() {
  // Test insufficient data
  {
    Protocol::Db2Register::SupervisoryFrame_t supervisory_frame;
    EXPECT_EQ(Protocol::Db2Register::parse_supervisory_frame("", supervisory_frame),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_INSUFFICIENT_DATA);
  }

  // Test an invalid CRC
  {
    const std::vector<uint8_t> v{0xff, 0x1, 'T', 'O', 'T', 'A', 'L', 'F', 'L', 'O', 'W', ' ', 0x58, 0xff, 0x00, 0x00};
    const std::string buffer(v.begin(), v.end());

    Protocol::Db2Register::SupervisoryFrame_t supervisory_frame;
    EXPECT_EQ(Protocol::Db2Register::parse_supervisory_frame(buffer, supervisory_frame),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_CRC_MISMATCH);
  }

  // Test a valid frame with sync byte
  {
    std::vector<uint8_t> v{0xff, 0x1, 'T', 'O', 'T', 'A', 'L', 'F', 'L', 'O', 'W', ' ', 0x58, 0xff};
    auto crc = CRC16::generate(v);
    v.emplace_back((crc & 0xff00U) >> 8U);
    v.emplace_back(crc & 0xffU);
    const std::string buffer(v.begin(), v.end());

    Protocol::Db2Register::SupervisoryFrame_t supervisory_frame;
    EXPECT_EQ(Protocol::Db2Register::parse_supervisory_frame(buffer, supervisory_frame),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_NO_ERROR);
    EXPECT_EQ(supervisory_frame.sync, 0xff);
    EXPECT_EQ(supervisory_frame.soh, 0x1);
    EXPECT_TRUE(
        utils::is_equal(std::string(supervisory_frame.station_id, sizeof(supervisory_frame.station_id)), "TOTALFLOW "));
    EXPECT_EQ(supervisory_frame.control_code, 0x58);
    EXPECT_EQ(supervisory_frame.info, 0xff);
  }

  // Test a valid frame without sync byte
  {
    std::vector<uint8_t> v{0x1, 'T', 'O', 'T', 'A', 'L', 'F', 'L', 'O', 'W', ' ', 0x58, 0xff};
    auto crc = CRC16::generate(v);
    v.emplace_back((crc & 0xff00U) >> 8U);
    v.emplace_back(crc & 0xffU);
    const std::string buffer(v.begin(), v.end());

    Protocol::Db2Register::SupervisoryFrame_t supervisory_frame;
    EXPECT_EQ(Protocol::Db2Register::parse_supervisory_frame(buffer, supervisory_frame),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_NO_ERROR);
    EXPECT_EQ(supervisory_frame.sync, 0xff);
    EXPECT_EQ(supervisory_frame.soh, 0x1);
    EXPECT_TRUE(
        utils::is_equal(std::string(supervisory_frame.station_id, sizeof(supervisory_frame.station_id)), "TOTALFLOW "));
    EXPECT_EQ(supervisory_frame.control_code, 0x58);
    EXPECT_EQ(supervisory_frame.info, 0xff);
  }

  // Test a frame error by changing a random position to an random char after the CRC calculation
  {
    std::vector<uint8_t> v{0xff, 0x1, 'T', 'O', 'T', 'A', 'L', 'F', 'L', 'O', 'W', ' ', 0x58, 0xff};
    auto crc = CRC16::generate(v);
    v.emplace_back((crc & 0xff00U) >> 8U);
    v.emplace_back(crc & 0xffU);

    auto rand_pos = utils::get_random<size_t>(0, v.size() - 1);

    // Invert the bits of a random index of the vector
    v[rand_pos] = static_cast<uint8_t>(~v[rand_pos]);

    const std::string buffer(v.begin(), v.end());

    Protocol::Db2Register::SupervisoryFrame_t supervisory_frame;
    EXPECT_EQ(Protocol::Db2Register::parse_supervisory_frame(buffer, supervisory_frame),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_CRC_MISMATCH);
  }
}

TEST_F(Db2RegisterProtocolTest, SupervisoryFrame) {
  BuildSupervisoryFrame();
  ParseSupervisoryFrame();
}

/** Test the XSingleHeader frame
 */
void BuildXSingleHeaderFrame() {
  // Build with default Control Byte
  {
    Protocol::Db2Register::XSingleHeader_t x_single_header;
    uint8_t register_sets_count = 10U;
    Protocol::Db2Register::build_x_single_header_frame(register_sets_count, x_single_header);

    EXPECT_EQ(x_single_header.length, 3U);
    EXPECT_EQ(x_single_header.not_length, static_cast<uint16_t>(~3U));
    EXPECT_EQ(x_single_header.header_type, Protocol::Db2Register::HeaderType::DATA_REQUEST);
    EXPECT_EQ(x_single_header.control_byte, Protocol::Db2Register::ControlByte::READ_REQUEST);
    EXPECT_EQ(x_single_header.register_sets_count, register_sets_count);

    std::vector<uint8_t> v{0x3,
                           0x0,
                           0xfc,
                           0xff,
                           static_cast<uint8_t>(Protocol::Db2Register::HeaderType::DATA_REQUEST),
                           static_cast<uint8_t>(Protocol::Db2Register::ControlByte::READ_REQUEST),
                           register_sets_count};
    auto crc = CRC16::generate(v);
    EXPECT_EQ(x_single_header.crc, crc);

    // Cast the frame to std::vector<uint8_t>
    v.emplace_back(crc & 0xFFu);
    v.emplace_back(static_cast<uint8_t>(crc >> 8u) & 0xFFu);

    EXPECT_EQ(v, static_cast<const std::vector<uint8_t>>(x_single_header));
  }

  // Build with provided Control Byte
  {
    Protocol::Db2Register::XSingleHeader_t x_single_header;
    uint8_t register_sets_count = 10U;
    Protocol::Db2Register::build_x_single_header_frame(register_sets_count, x_single_header,
                                                       Protocol::Db2Register::ControlByte::WRITE_REQUEST);

    EXPECT_EQ(x_single_header.length, 3U);
    EXPECT_EQ(x_single_header.not_length, static_cast<uint16_t>(~3U));
    EXPECT_EQ(x_single_header.header_type, Protocol::Db2Register::HeaderType::DATA_REQUEST);
    EXPECT_EQ(x_single_header.control_byte, Protocol::Db2Register::ControlByte::WRITE_REQUEST);
    EXPECT_EQ(x_single_header.register_sets_count, register_sets_count);

    std::vector<uint8_t> v{0x3,
                           0x0,
                           0xfc,
                           0xff,
                           static_cast<uint8_t>(Protocol::Db2Register::HeaderType::DATA_REQUEST),
                           static_cast<uint8_t>(Protocol::Db2Register::ControlByte::WRITE_REQUEST),
                           register_sets_count};
    auto crc = CRC16::generate(v);
    EXPECT_EQ(x_single_header.crc, crc);

    // Cast the frame to std::vector<uint8_t>
    v.emplace_back(crc & 0xFFu);
    v.emplace_back(static_cast<uint8_t>(crc >> 8u) & 0xFFu);

    EXPECT_EQ(v, static_cast<const std::vector<uint8_t>>(x_single_header));
  }
}

void ParseXSingleHeaderFrame() {
  // Test insufficient data
  {
    Protocol::Db2Register::XSingleHeader_t x_single_header;
    EXPECT_EQ(Protocol::Db2Register::parse_x_single_header_frame("", x_single_header),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_INSUFFICIENT_DATA);
  }

  // Test an invalid CRC
  {
    const std::vector<uint8_t> v{0x3,
                                 0x0,
                                 0xfc,
                                 0xff,
                                 static_cast<uint8_t>(Protocol::Db2Register::HeaderType::DATA_REQUEST),
                                 static_cast<uint8_t>(Protocol::Db2Register::ControlByte::READ_REQUEST),
                                 0xa,
                                 0x0,
                                 0x0};
    const std::string buffer(v.begin(), v.end());

    Protocol::Db2Register::XSingleHeader_t x_single_header;
    EXPECT_EQ(Protocol::Db2Register::parse_x_single_header_frame(buffer, x_single_header),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_CRC_MISMATCH);
  }

  // Test a frame error by changing a random position to an random char after the CRC calculation
  {
    std::vector<uint8_t> v{0x3,
                           0x0,
                           0xfc,
                           0xff,
                           static_cast<uint8_t>(Protocol::Db2Register::HeaderType::DATA_REQUEST),
                           static_cast<uint8_t>(Protocol::Db2Register::ControlByte::READ_REQUEST),
                           0xa};
    auto crc = CRC16::generate(v);
    v.emplace_back(crc & 0xffU);
    v.emplace_back((crc & 0xff00U) >> 8U);

    auto rand_pos = utils::get_random<size_t>(0, v.size() - 1);

    // Invert the bits of a random index of the vector
    v[rand_pos] = static_cast<uint8_t>(~v[rand_pos]);

    const std::string buffer4(v.begin(), v.end());

    Protocol::Db2Register::XSingleHeader_t x_single_header;
    EXPECT_EQ(Protocol::Db2Register::parse_x_single_header_frame(buffer4, x_single_header),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_CRC_MISMATCH);
  }

  // Test a valid frame
  {
    std::vector<uint8_t> v{0x3,
                           0x0,
                           0xfc,
                           0xff,
                           static_cast<uint8_t>(Protocol::Db2Register::HeaderType::DATA_REQUEST),
                           static_cast<uint8_t>(Protocol::Db2Register::ControlByte::READ_REQUEST),
                           0xa};
    auto crc = CRC16::generate(v);
    v.emplace_back(crc & 0xffU);
    v.emplace_back((crc & 0xff00U) >> 8U);

    const std::string buffer4(v.begin(), v.end());

    Protocol::Db2Register::XSingleHeader_t x_single_header;
    EXPECT_EQ(Protocol::Db2Register::parse_x_single_header_frame(buffer4, x_single_header),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_NO_ERROR);
    EXPECT_EQ(x_single_header.length, 3U);
    EXPECT_EQ(x_single_header.not_length, static_cast<uint16_t>(~3U));
    EXPECT_EQ(x_single_header.header_type, Protocol::Db2Register::HeaderType::DATA_REQUEST);
    EXPECT_EQ(x_single_header.control_byte, Protocol::Db2Register::ControlByte::READ_REQUEST);
    EXPECT_EQ(x_single_header.register_sets_count, 10U);
  }
}

TEST_F(Db2RegisterProtocolTest, XSingleHeaderFrame) {
  BuildXSingleHeaderFrame();
  ParseXSingleHeaderFrame();
}

/** Test the XRegisterSet frame
 */
void BuildXRegisterSetFrame() {
  // Build a XRegisterSet frame. No data is passed to the function.
  {
    Protocol::Db2Register::XRegisterSet_t x_register_set;
    Protocol::Db2Register::RegisterAddress_t register_address{123U, 5U, 265U};
    uint16_t register_count = 10U;

    Protocol::Db2Register::build_x_register_set_frame(register_address, register_count, {}, x_register_set);

    EXPECT_EQ(x_register_set.length, 10U);
    EXPECT_EQ(x_register_set.not_length, static_cast<uint16_t>(~10U));
    EXPECT_EQ(x_register_set.data_compression_type, Protocol::Db2Register::CompressionType::UNCOMPRESSED_DATA);
    EXPECT_EQ(x_register_set.request_method, 2U);
    EXPECT_EQ(x_register_set.address.application, 123U);
    EXPECT_EQ(x_register_set.address.array, 5U);
    EXPECT_EQ(x_register_set.address.index, 265U);
    EXPECT_EQ(x_register_set.register_count, 10U);

    std::vector<uint8_t> v{
        0xa, 0x0,  0xf5, 0xff, static_cast<uint8_t>(Protocol::Db2Register::CompressionType::UNCOMPRESSED_DATA),
        0x2, 0x7b, 0x5,  0x9,  0x1,
        0xa, 0x0,  0x0,  0x0};
    auto crc = CRC16::generate(v);

    EXPECT_EQ(x_register_set.crc, crc);

    // Cast the frame to std::vector<uint8_t>
    v.emplace_back(crc & 0xFFu);
    v.emplace_back(static_cast<uint8_t>(crc >> 8u) & 0xFFu);

    EXPECT_EQ(v, static_cast<const std::vector<uint8_t>>(x_register_set));
  }

  // Build a XRegisterSet frame with data (this use case occurs when building a XRegisterData frame).
  {
    Protocol::Db2Register::XRegisterSet_t x_register_set;
    Protocol::Db2Register::RegisterAddress_t register_address{123U, 5U, 265U};
    uint16_t register_count = 10U;
    std::vector<uint8_t> data{0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa};

    Protocol::Db2Register::build_x_register_set_frame(register_address, register_count, data, x_register_set);

    EXPECT_EQ(x_register_set.length, 20U);
    EXPECT_EQ(x_register_set.not_length, static_cast<uint16_t>(~20U));
    EXPECT_EQ(x_register_set.data_compression_type, Protocol::Db2Register::CompressionType::UNCOMPRESSED_DATA);
    EXPECT_EQ(x_register_set.request_method, 2U);
    EXPECT_EQ(x_register_set.address.application, 123U);
    EXPECT_EQ(x_register_set.address.array, 5U);
    EXPECT_EQ(x_register_set.address.index, 265U);
    EXPECT_EQ(x_register_set.register_count, 10U);

    std::vector<uint8_t> v{
        0x14, 0x0,  0xeb, 0xff, static_cast<uint8_t>(Protocol::Db2Register::CompressionType::UNCOMPRESSED_DATA),
        0x2,  0x7b, 0x5,  0x9,  0x1,
        0xa,  0x0,  0x0,  0x0,  0x1,
        0x2,  0x3,  0x4,  0x5,  0x6,
        0x7,  0x8,  0x9,  0xa};
    auto crc = CRC16::generate(v);

    EXPECT_EQ(x_register_set.crc, crc);
  }
}

void ParseXRegisterSetFrame() {
  // Test insufficient data
  {
    Protocol::Db2Register::XRegisterSet_t x_register_set;
    size_t data_size;
    EXPECT_EQ(Protocol::Db2Register::parse_x_register_set_frame("", x_register_set, &data_size),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_INSUFFICIENT_DATA);
  }

  // Test an invalid CRC.
  {
    // No data. 10 consecutive registers starting at 123.5.256. CRC = 0x0.
    const std::vector<uint8_t> v{
        0xa, 0x0,  0xf5, 0xff, static_cast<uint8_t>(Protocol::Db2Register::CompressionType::UNCOMPRESSED_DATA),
        0x2, 0x7b, 0x5,  0x9,  0x1,
        0xa, 0x0,  0x0,  0x0,  0x0,
        0x0};
    const std::string buffer(v.begin(), v.end());

    Protocol::Db2Register::XRegisterSet_t x_register_set;
    size_t data_size;
    EXPECT_EQ(Protocol::Db2Register::parse_x_register_set_frame(buffer, x_register_set, &data_size),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_CRC_MISMATCH);
  }

  // Test a frame error by changing a random position to an random char after the CRC calculation
  {
    // 10 bytes of data. 10 consecutive registers starting at 123.5.256.
    std::vector<uint8_t> v{
        0x14, 0x0,  0xeb, 0xff, static_cast<uint8_t>(Protocol::Db2Register::CompressionType::UNCOMPRESSED_DATA),
        0x2,  0x7b, 0x5,  0x9,  0x1,
        0xa,  0x0,  0x0,  0x0,  0x1,
        0x2,  0x3,  0x4,  0x5,  0x6,
        0x7,  0x8,  0x9,  0xa};
    auto crc = CRC16::generate(v);
    v.emplace_back(crc & 0xffU);
    v.emplace_back((crc & 0xff00U) >> 8U);

    auto rand_pos = utils::get_random<size_t>(0, v.size() - 1);

    // Invert the bits of a random index of the vector
    v[rand_pos] = static_cast<uint8_t>(~v[rand_pos]);

    const std::string buffer(v.begin(), v.end());

    Protocol::Db2Register::XRegisterSet_t x_register_set;
    size_t data_size;
    EXPECT_EQ(Protocol::Db2Register::parse_x_register_set_frame(buffer, x_register_set, &data_size),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_CRC_MISMATCH);
  }

  // Test a valid frame.
  {
    // 250 bytes of data. 250 consecutive registers starting at 123.5.10
    std::vector<uint8_t> v{
        0x4,  0x1,  0xfb, 0xfe, static_cast<uint8_t>(Protocol::Db2Register::CompressionType::UNCOMPRESSED_DATA),
        0x2,  0x7b, 0x5,  0xa,  0x0,
        0xfa, 0x0,  0x0,  0x0,  0x1,
        0x2,  0x3,  0x4,  0x5,  0x6,
        0x7,  0x8,  0x9,  0xa,  0xb,
        0xc,  0xd,  0xe,  0xf,  0x10,
        0x11, 0x12, 0x13, 0x14, 0x15,
        0x16, 0x17, 0x18, 0x19, 0x1a,
        0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
        0x20, 0x21, 0x22, 0x23, 0x24,
        0x25, 0x26, 0x27, 0x28, 0x29,
        0x2a, 0x2b, 0x2c, 0x2d, 0x2e,
        0x2f, 0x30, 0x31, 0x32, 0x33,
        0x34, 0x35, 0x36, 0x37, 0x38,
        0x39, 0x3a, 0x3b, 0x3c, 0x3d,
        0x3e, 0x3f, 0x40, 0x41, 0x42,
        0x43, 0x44, 0x45, 0x46, 0x47,
        0x48, 0x49, 0x4a, 0x4b, 0x4c,
        0x4d, 0x4e, 0x4f, 0x50, 0x51,
        0x52, 0x53, 0x54, 0x55, 0x56,
        0x57, 0x58, 0x59, 0x5a, 0x5b,
        0x5c, 0x5d, 0x5e, 0x5f, 0x60,
        0x61, 0x62, 0x63, 0x64, 0x65,
        0x66, 0x67, 0x68, 0x69, 0x6a,
        0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
        0x70, 0x71, 0x72, 0x73, 0x74,
        0x75, 0x76, 0x77, 0x78, 0x79,
        0x7a, 0x7b, 0x7c, 0x7d, 0x7e,
        0x7f, 0x80, 0x81, 0x82, 0x83,
        0x84, 0x85, 0x86, 0x87, 0x88,
        0x89, 0x8a, 0x8b, 0x8c, 0x8d,
        0x8e, 0x8f, 0x90, 0x91, 0x92,
        0x93, 0x94, 0x95, 0x96, 0x97,
        0x98, 0x99, 0x9a, 0x9b, 0x9c,
        0x9d, 0x9e, 0x9f, 0xa0, 0xa1,
        0xa2, 0xa3, 0xa4, 0xa5, 0xa6,
        0xa7, 0xa8, 0xa9, 0xaa, 0xab,
        0xac, 0xad, 0xae, 0xaf, 0xb0,
        0xb1, 0xb2, 0xb3, 0xb4, 0xb5,
        0xb6, 0xb7, 0xb8, 0xb9, 0xba,
        0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4,
        0xc5, 0xc6, 0xc7, 0xc8, 0xc9,
        0xca, 0xcb, 0xcc, 0xcd, 0xce,
        0xcf, 0xd0, 0xd1, 0xd2, 0xd3,
        0xd4, 0xd5, 0xd6, 0xd7, 0xd8,
        0xd9, 0xda, 0xdb, 0xdc, 0xdd,
        0xde, 0xdf, 0xe0, 0xe1, 0xe2,
        0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
        0xe8, 0xe9, 0xea, 0xeb, 0xec,
        0xed, 0xee, 0xef, 0xf0, 0xf1,
        0xf2, 0xf3, 0xf4, 0xf5, 0xf6,
        0xf7, 0xf8, 0xf9, 0xfa};
    auto crc = CRC16::generate(v);
    v.emplace_back(crc & 0xffU);
    v.emplace_back((crc & 0xff00U) >> 8U);
    const std::string buffer(v.begin(), v.end());

    Protocol::Db2Register::XRegisterSet_t x_register_set;
    size_t data_size;
    EXPECT_EQ(Protocol::Db2Register::parse_x_register_set_frame(buffer, x_register_set, &data_size),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_NO_ERROR);
    EXPECT_EQ(x_register_set.length, 260U);
    EXPECT_EQ(x_register_set.not_length, static_cast<uint16_t>(~260U));
    EXPECT_EQ(x_register_set.data_compression_type, Protocol::Db2Register::CompressionType::UNCOMPRESSED_DATA);
    EXPECT_EQ(x_register_set.request_method, 2U);
    EXPECT_EQ(x_register_set.address.application, 123U);
    EXPECT_EQ(x_register_set.address.array, 5U);
    EXPECT_EQ(x_register_set.address.index, 10U);
    EXPECT_EQ(x_register_set.register_count, 250U);
    EXPECT_EQ(data_size, 250U);
  }
}

TEST_F(Db2RegisterProtocolTest, XRegisterSetFrame) {
  BuildXRegisterSetFrame();
  ParseXRegisterSetFrame();
}

/** Test the XRegisterData frame
 */
void BuildXRegisterDataFrame() {
  Protocol::Db2Register::XRegisterData_t x_register_data;
  Protocol::Db2Register::RegisterAddress_t register_address{125, 0, 3};
  uint16_t register_count = 4U;
  std::vector<uint8_t> data{0x1, 0x2, 0x3, 0x4};

  Protocol::Db2Register::build_x_register_data_frame(register_address, register_count, x_register_data, data);

  EXPECT_EQ(x_register_data.x_register_set.length, 14U);
  EXPECT_EQ(x_register_data.x_register_set.not_length, static_cast<uint16_t>(~14U));
  EXPECT_EQ(x_register_data.x_register_set.data_compression_type,
            Protocol::Db2Register::CompressionType::UNCOMPRESSED_DATA);
  EXPECT_EQ(x_register_data.x_register_set.request_method, 2U);
  EXPECT_EQ(x_register_data.x_register_set.address, register_address);
  EXPECT_EQ(x_register_data.x_register_set.register_count, 4U);
  EXPECT_EQ(x_register_data.x_register_set.spare, 0);

  // XRegisterSet + data
  std::vector<uint8_t> v{
      0xe, 0x0,  0xf1, 0xff, static_cast<uint8_t>(Protocol::Db2Register::CompressionType::UNCOMPRESSED_DATA),
      0x2, 0x7d, 0x0,  0x3,  0x0,
      0x4, 0x0,  0x0,  0x0,  0x1,
      0x2, 0x3,  0x4};
  auto crc = CRC16::generate(v);

  EXPECT_EQ(x_register_data.x_register_set.crc, crc);
  EXPECT_EQ(x_register_data.data, data);

  // Cast the frame to std::vector<uint8_t>
  v.emplace_back(crc & 0xFFu);
  v.emplace_back(static_cast<uint8_t>(crc >> 8u) & 0xFFu);

  EXPECT_EQ(v, static_cast<const std::vector<uint8_t>>(x_register_data));
}

void ParseXRegisterDataFrame() {
  // Test insufficient data
  {
    Protocol::Db2Register::XRegisterData_t x_register_data;
    EXPECT_EQ(Protocol::Db2Register::parse_x_register_data_frame("", x_register_data),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_INSUFFICIENT_DATA);
  }

  // Test insufficient data
  {
    // XRegisterSet + data (CRC missing)
    const std::vector<uint8_t> v{
        0xe, 0x0,  0xf1, 0xff, static_cast<uint8_t>(Protocol::Db2Register::CompressionType::UNCOMPRESSED_DATA),
        0x2, 0x7d, 0x0,  0x3,  0x0,
        0x4, 0x0,  0x0,  0x0,  0x1,
        0x2, 0x3,  0x4};
    const std::string buffer(v.begin(), v.end());

    Protocol::Db2Register::XRegisterData_t x_register_data;
    EXPECT_EQ(Protocol::Db2Register::parse_x_register_data_frame(buffer, x_register_data),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_INSUFFICIENT_DATA);
  }

  // Test an invalid CRC
  {
    // XRegisterSet + data
    const std::vector<uint8_t> v{
        0xe, 0x0,  0xf1, 0xff, static_cast<uint8_t>(Protocol::Db2Register::CompressionType::UNCOMPRESSED_DATA),
        0x2, 0x7d, 0x0,  0x3,  0x0,
        0x4, 0x0,  0x0,  0x0,  0x1,
        0x2, 0x3,  0x4,  0x0,  0x0};
    const std::string buffer(v.begin(), v.end());

    Protocol::Db2Register::XRegisterData_t x_register_data;
    EXPECT_EQ(Protocol::Db2Register::parse_x_register_data_frame(buffer, x_register_data),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_CRC_MISMATCH);
  }

  // Test a valid frame
  {
    // XRegisterSet + data
    std::vector<uint8_t> v{
        0xe, 0x0,  0xf1, 0xff, static_cast<uint8_t>(Protocol::Db2Register::CompressionType::UNCOMPRESSED_DATA),
        0x2, 0x7d, 0x0,  0x3,  0x0,
        0x4, 0x0,  0x0,  0x0,  0x1,
        0x2, 0x3,  0x4};

    auto crc = CRC16::generate(v);

    v.emplace_back(crc & 0xffU);
    v.emplace_back((crc & 0xff00U) >> 8U);

    const std::string buffer(v.begin(), v.end());

    Protocol::Db2Register::XRegisterData_t x_register_data;
    EXPECT_EQ(Protocol::Db2Register::parse_x_register_data_frame(buffer, x_register_data),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_NO_ERROR);
    EXPECT_EQ(x_register_data.x_register_set.length, 14U);
    EXPECT_EQ(x_register_data.x_register_set.not_length, static_cast<uint16_t>(~14U));
    EXPECT_EQ(x_register_data.x_register_set.data_compression_type,
              Protocol::Db2Register::CompressionType::UNCOMPRESSED_DATA);
    EXPECT_EQ(x_register_data.x_register_set.request_method, 2U);
    EXPECT_EQ(x_register_data.x_register_set.address.application, 125U);
    EXPECT_EQ(x_register_data.x_register_set.address.array, 0U);
    EXPECT_EQ(x_register_data.x_register_set.address.index, 3U);
    EXPECT_EQ(x_register_data.x_register_set.register_count, 4U);
    EXPECT_EQ(x_register_data.data.size(), 4U);
    const std::vector<uint8_t> data{{0x1, 0x2, 0x3, 0x4}};
    EXPECT_EQ(x_register_data.data, data);
  }
}

TEST_F(Db2RegisterProtocolTest, XRegisterDataFrame) {
  BuildXRegisterDataFrame();
  ParseXRegisterDataFrame();
}

/** Test the DataRequestMessage
 */
void BuildDataRequestMessage() {
  // Test empty register sets map
  {
    Protocol::Db2Register::DataRequestMessage_t data_request_message;

    auto warnings = LoggingInterface::get_instance()->get_warning_cnt();

    EXPECT_EQ(Protocol::Db2Register::build_data_request("TOTALFLOW", {}, data_request_message), false);

    // This should log a warning
    EXPECT_EQ(LoggingInterface::get_instance()->get_warning_cnt(), warnings + 1);
  }

  // Test a register sets map that exceeds the maximum size by 1
  {
    Protocol::Db2Register::DataRequestMessage_t data_request_message;

    const auto max_count =
        std::numeric_limits<decltype(data_request_message.x_single_header.register_sets_count)>::max();
    std::map<uint32_t, uint16_t> register_sets_map;
    for(auto key = 0U; key <= max_count; ++key) {
      register_sets_map[key] = 1;
    }

    auto errors = LoggingInterface::get_instance()->get_error_cnt();

    EXPECT_EQ(Protocol::Db2Register::build_data_request("TOTALFLOW", register_sets_map, data_request_message), false);

    // This should log an error
    EXPECT_EQ(LoggingInterface::get_instance()->get_error_cnt(), errors + 1);
  }

  // Test a valid DataRequestMessage containing one XRegisterSet frame
  {
    Protocol::Db2Register::DataRequestMessage_t data_request_message;
    const std::map<uint32_t, uint16_t> register_sets_map{{REGISTER_SET_TO_UINT32(123, 5, 7), 2}};
    EXPECT_EQ(Protocol::Db2Register::build_data_request("TOTALFLOW", register_sets_map, data_request_message), true);

    // SupervisoryFrame
    EXPECT_EQ(data_request_message.supervisory_frame.sync, 0xff);
    EXPECT_EQ(data_request_message.supervisory_frame.soh, 0x1);
    EXPECT_TRUE(utils::is_equal(std::string(data_request_message.supervisory_frame.station_id,
                                            sizeof(data_request_message.supervisory_frame.station_id)),
                                "TOTALFLOW "));
    EXPECT_EQ(data_request_message.supervisory_frame.control_code, 0x58);
    EXPECT_EQ(data_request_message.supervisory_frame.info, 0xff);
    EXPECT_EQ(data_request_message.supervisory_frame.crc, 0x3afe);

    // XSingleHeader
    EXPECT_EQ(data_request_message.x_single_header.length, 3U);
    EXPECT_EQ(data_request_message.x_single_header.not_length, static_cast<uint16_t>(~3U));
    EXPECT_EQ(data_request_message.x_single_header.header_type, Protocol::Db2Register::HeaderType::DATA_REQUEST);
    EXPECT_EQ(data_request_message.x_single_header.control_byte, Protocol::Db2Register::ControlByte::READ_REQUEST);
    EXPECT_EQ(data_request_message.x_single_header.register_sets_count, 1U);
    EXPECT_EQ(data_request_message.x_single_header.crc, 0xc092);

    // The provided map ({{REGISTER_SET_TO_UINT32(123, 5, 7), 2}}) will only generate one XRegisterSet
    EXPECT_EQ(data_request_message.x_register_sets[0].length, 10U);
    EXPECT_EQ(data_request_message.x_register_sets[0].not_length, static_cast<uint16_t>(~10U));
    EXPECT_EQ(data_request_message.x_register_sets[0].data_compression_type,
              Protocol::Db2Register::CompressionType::UNCOMPRESSED_DATA);
    EXPECT_EQ(data_request_message.x_register_sets[0].request_method, 2U);
    EXPECT_EQ(data_request_message.x_register_sets[0].address.application, 123U);
    EXPECT_EQ(data_request_message.x_register_sets[0].address.array, 5U);
    EXPECT_EQ(data_request_message.x_register_sets[0].address.index, 7U);
    EXPECT_EQ(data_request_message.x_register_sets[0].register_count, 2U);
    EXPECT_EQ(data_request_message.x_register_sets[0].spare, 0U);
    EXPECT_EQ(data_request_message.x_register_sets[0].crc, 0xe87c);

    std::vector<uint8_t> v{0xff, 0x1,  'T',  'O',  'T',  'A',  'L', 'F',  'L',  'O', 'W', ' ', 0x58, 0xff, 0xfe,
                           0x3a, 0xd3, 0xd9, 0xce, 0xc3, 0x3,  0x0, 0xfc, 0xff, 0x0, 0x0, 0x1, 0x92, 0xc0, 0xa,
                           0x0,  0xf5, 0xff, 0x0,  0x2,  0x7b, 0x5, 0x7,  0x0,  0x2, 0x0, 0x0, 0x0,  0x7c, 0xe8};
    EXPECT_EQ(v.size(), sizeof(Protocol::Db2Register::SupervisoryFrame_t) + sizeof(Protocol::Db2Register::SyncFrame_t) +
                            sizeof(Protocol::Db2Register::XSingleHeader_t) +
                            sizeof(Protocol::Db2Register::XRegisterSet_t));

    // Cast the request to std::vector<uint8_t>
    EXPECT_EQ(v, static_cast<const std::vector<uint8_t>>(data_request_message));
  }

  // Test a valid DataRequestMessage containing two XRegisterSet frames
  {
    Protocol::Db2Register::DataRequestMessage_t data_request_message;
    const std::map<uint32_t, uint16_t> register_sets_map{{REGISTER_SET_TO_UINT32(123, 5, 7), 2},
                                                         {REGISTER_SET_TO_UINT32(123, 0, 7), 10}};
    EXPECT_EQ(Protocol::Db2Register::build_data_request("TOTALFLOW", register_sets_map, data_request_message), true);

    // SupervisoryFrame
    EXPECT_EQ(data_request_message.supervisory_frame.sync, 0xff);
    EXPECT_EQ(data_request_message.supervisory_frame.soh, 0x1);
    EXPECT_TRUE(utils::is_equal(std::string(data_request_message.supervisory_frame.station_id,
                                            sizeof(data_request_message.supervisory_frame.station_id)),
                                "TOTALFLOW "));
    EXPECT_EQ(data_request_message.supervisory_frame.control_code, 0x58);
    EXPECT_EQ(data_request_message.supervisory_frame.info, 0xff);
    EXPECT_EQ(data_request_message.supervisory_frame.crc, 0x3afe);

    // XSingleHeader
    EXPECT_EQ(data_request_message.x_single_header.length, 3U);
    EXPECT_EQ(data_request_message.x_single_header.not_length, static_cast<uint16_t>(~3U));
    EXPECT_EQ(data_request_message.x_single_header.header_type, Protocol::Db2Register::HeaderType::DATA_REQUEST);
    EXPECT_EQ(data_request_message.x_single_header.control_byte, Protocol::Db2Register::ControlByte::READ_REQUEST);
    EXPECT_EQ(data_request_message.x_single_header.register_sets_count, 2U);
    EXPECT_EQ(data_request_message.x_single_header.crc, 0xc1d2);

    // First XRegisterSet
    EXPECT_EQ(data_request_message.x_register_sets[0].length, 10U);
    EXPECT_EQ(data_request_message.x_register_sets[0].not_length, static_cast<uint16_t>(~10U));
    EXPECT_EQ(data_request_message.x_register_sets[0].data_compression_type,
              Protocol::Db2Register::CompressionType::UNCOMPRESSED_DATA);
    EXPECT_EQ(data_request_message.x_register_sets[0].request_method, 2U);
    EXPECT_EQ(data_request_message.x_register_sets[0].address.application, 123U);
    EXPECT_EQ(data_request_message.x_register_sets[0].address.array, 0U);
    EXPECT_EQ(data_request_message.x_register_sets[0].address.index, 7U);
    EXPECT_EQ(data_request_message.x_register_sets[0].register_count, 10U);
    EXPECT_EQ(data_request_message.x_register_sets[0].spare, 0U);
    EXPECT_EQ(data_request_message.x_register_sets[0].crc, 0x882b);

    // Second XRegisterSet
    EXPECT_EQ(data_request_message.x_register_sets[1].length, 10U);
    EXPECT_EQ(data_request_message.x_register_sets[1].not_length, static_cast<uint16_t>(~10U));
    EXPECT_EQ(data_request_message.x_register_sets[1].data_compression_type,
              Protocol::Db2Register::CompressionType::UNCOMPRESSED_DATA);
    EXPECT_EQ(data_request_message.x_register_sets[1].request_method, 2U);
    EXPECT_EQ(data_request_message.x_register_sets[1].address.application, 123U);
    EXPECT_EQ(data_request_message.x_register_sets[1].address.array, 5U);
    EXPECT_EQ(data_request_message.x_register_sets[1].address.index, 7U);
    EXPECT_EQ(data_request_message.x_register_sets[1].register_count, 2U);
    EXPECT_EQ(data_request_message.x_register_sets[1].spare, 0U);
    EXPECT_EQ(data_request_message.x_register_sets[1].crc, 0xe87c);

    std::vector<uint8_t> v{0xff, 0x1,  'T',  'O',  'T', 'A', 'L',  'F',  'L', 'O', 'W', ' ',  0x58, 0xff, 0xfe, 0x3a,
                           0xd3, 0xd9, 0xce, 0xc3, 0x3, 0x0, 0xfc, 0xff, 0x0, 0x0, 0x2, 0xd2, 0xc1, 0xa,  0x0,  0xf5,
                           0xff, 0x0,  0x2,  0x7b, 0x0, 0x7, 0x0,  0xa,  0x0, 0x0, 0x0, 0x2b, 0x88, 0xa,  0x0,  0xf5,
                           0xff, 0x0,  0x2,  0x7b, 0x5, 0x7, 0x0,  0x2,  0x0, 0x0, 0x0, 0x7c, 0xe8};
    EXPECT_EQ(v.size(), sizeof(Protocol::Db2Register::SupervisoryFrame_t) + sizeof(Protocol::Db2Register::SyncFrame_t) +
                            sizeof(Protocol::Db2Register::XSingleHeader_t) +
                            2 * sizeof(Protocol::Db2Register::XRegisterSet_t));

    // Cast the request to std::vector<uint8_t>
    EXPECT_EQ(v, static_cast<const std::vector<uint8_t>>(data_request_message));
  }
}

TEST_F(Db2RegisterProtocolTest, DataRequestMessage) {
  BuildDataRequestMessage();
}

/** Test the DataRequestMessage
 */
void BuildDataTransmission() {
  // Test register definition not found
  {
    Protocol::Db2Register::RegisterAddress_t register_address{125, 0, 1};
    Protocol::Db2Register::RegisterDefinition_t register_definition{
        register_address, "float", 4U, "gas_flow", false, {}, "", "", ""};
    std::map<uint32_t, Protocol::Db2Register::RegisterDefinition_t> registers_definitions{
        {static_cast<uint32_t>(register_address), register_definition}};
    Protocol::Db2Register::DataTransmissionMessage_t data_transmission;

    EXPECT_EQ(Protocol::Db2Register::build_data_transmission("TOTALFLOW", {123, 0, 1}, data_transmission, {},
                                                             registers_definitions),
              false);
  }

  // Test invalid data size
  {
    Protocol::Db2Register::RegisterAddress_t register_address{125, 0, 1};
    Protocol::Db2Register::RegisterDefinition_t register_definition{
        register_address, "float", 4U, "gas_flow", false, {}, "", "", ""};
    std::map<uint32_t, Protocol::Db2Register::RegisterDefinition_t> registers_definitions{
        {static_cast<uint32_t>(register_address), register_definition}};
    Protocol::Db2Register::DataTransmissionMessage_t data_transmission;
    std::vector<uint8_t> data;

    EXPECT_EQ(Protocol::Db2Register::build_data_transmission("TOTALFLOW", register_address, data_transmission, data,
                                                             registers_definitions),
              false);
  }

  // Test valid DataTransmission
  {
    Protocol::Db2Register::RegisterAddress_t register_address{125, 0, 1};
    Protocol::Db2Register::RegisterDefinition_t register_definition{
        register_address, "float", 4U, "gas_flow", false, {}, "", "", ""};
    std::map<uint32_t, Protocol::Db2Register::RegisterDefinition_t> registers_definitions{
        {static_cast<uint32_t>(register_address), register_definition}};
    Protocol::Db2Register::DataTransmissionMessage_t data_transmission;
    std::vector<uint8_t> data{0x00, 0x3c, 0x1c, 0x46};

    ASSERT_EQ(Protocol::Db2Register::build_data_transmission("TOTALFLOW", register_address, data_transmission, data,
                                                             registers_definitions),
              true);
    EXPECT_EQ(data_transmission.message_type, Protocol::Db2Register::DataTransmissionMessageType::HOST_TO_DEVICE);

    // Validate SupervisoryFrame
    EXPECT_EQ(data_transmission.supervisory_frame.sync, 0xff);
    EXPECT_EQ(data_transmission.supervisory_frame.soh, 0x1);
    EXPECT_TRUE(utils::is_equal(std::string(data_transmission.supervisory_frame.station_id,
                                            sizeof(data_transmission.supervisory_frame.station_id)),
                                "TOTALFLOW "));
    EXPECT_EQ(data_transmission.supervisory_frame.control_code, 0x58);
    EXPECT_EQ(data_transmission.supervisory_frame.info, 0xff);
    EXPECT_EQ(data_transmission.supervisory_frame.crc, 0x3afe);

    // Validate XRegisterData frame
    ASSERT_EQ(data_transmission.x_register_data.size(), 1U);
    EXPECT_EQ(data_transmission.x_register_data[0].x_register_set.length, 14U);
    EXPECT_EQ(data_transmission.x_register_data[0].x_register_set.not_length, static_cast<uint16_t>(~14U));
    EXPECT_EQ(data_transmission.x_register_data[0].x_register_set.data_compression_type,
              Protocol::Db2Register::CompressionType::UNCOMPRESSED_DATA);
    EXPECT_EQ(data_transmission.x_register_data[0].x_register_set.request_method, 2U);
    EXPECT_EQ(data_transmission.x_register_data[0].x_register_set.address.application, 125U);
    EXPECT_EQ(data_transmission.x_register_data[0].x_register_set.address.array, 0U);
    EXPECT_EQ(data_transmission.x_register_data[0].x_register_set.address.index, 1U);
    EXPECT_EQ(data_transmission.x_register_data[0].x_register_set.register_count, 1U);
    EXPECT_EQ(data_transmission.x_register_data[0].x_register_set.spare, 0U);
    EXPECT_EQ(data_transmission.x_register_data[0].x_register_set.crc, 0xa62a);
    EXPECT_EQ(data_transmission.x_register_data[0].x_register_set.address.index, 1U);

    ASSERT_EQ(data_transmission.x_register_data[0].data.size(), data.size());
    EXPECT_EQ(data_transmission.x_register_data[0].data, data);

    // Cast the request to std::vector<uint8_t>
    std::vector<uint8_t> v{0xff, 0x1,  'T',  'O',  'T', 'A', 'L',  'F',  'L',  'O',  'W',  ' ',
                           0x58, 0xff, 0xfe, 0x3a, 0xe, 0x0, 0xf1, 0xff, 0x0,  0x2,  0x7d, 0x0,
                           0x1,  0x0,  0x1,  0x0,  0x0, 0x0, 0x00, 0x3c, 0x1c, 0x46, 0x2a, 0xa6};
    EXPECT_EQ(v.size(), sizeof(Protocol::Db2Register::SupervisoryFrame_t) +
                            sizeof(Protocol::Db2Register::XRegisterSet_t) + data.size());

    EXPECT_EQ(v, static_cast<const std::vector<uint8_t>>(data_transmission));
  }
}

void ParseDataTransmission() {
  // Test empty data
  {
    Protocol::Db2Register::DataTransmissionMessage_t data_transmission;
    uint8_t error_code;

    EXPECT_EQ(Protocol::Db2Register::parse_data_transmission("", data_transmission, &error_code),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_INSUFFICIENT_DATA);
  }

  // Test insufficient data
  {
    Protocol::Db2Register::DataTransmissionMessage_t data_transmission;
    uint8_t error_code;

    const std::vector<uint8_t> v{0xff, 0x1, 'T', 'O', 'T', 'A', 'L', 'F', 'L', 'O', 'W', ' ', 0x58, 0xff};
    const std::string buffer(v.begin(), v.end());

    EXPECT_EQ(Protocol::Db2Register::parse_data_transmission(buffer, data_transmission, &error_code),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_INSUFFICIENT_DATA);
  }

  // Test a device NAK message (header_type = 1 and control_byte = 2)
  {
    Protocol::Db2Register::DataTransmissionMessage_t data_transmission;
    uint8_t error_code = 0U;

    const std::vector<uint8_t> v{0xff, 0x1,  'T',  'O', 'T', 'A',  'L',  'F', 'L', 'O',  'W',  ' ', 0x58,
                                 0xff, 0x3e, 0xc5, 0x3, 0x0, 0xfc, 0xff, 0x1, 0x2, 0x66, 0x83, 0x8a};
    const std::string buffer(v.begin(), v.end());

    auto errors = LoggingInterface::get_instance()->get_error_cnt();

    EXPECT_EQ(Protocol::Db2Register::parse_data_transmission(buffer, data_transmission, &error_code),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_NACK);
    EXPECT_EQ(error_code, 102U);

    // This should log an error
    EXPECT_EQ(LoggingInterface::get_instance()->get_error_cnt(), errors + 1);
  }

  // Test unknown error (header_type = 1 and control_byte = 2)
  {
    Protocol::Db2Register::DataTransmissionMessage_t data_transmission;
    uint8_t error_code = 0U;

    const std::vector<uint8_t> v{0xff, 0x1,  'T',  'O', 'T', 'A',  'L',  'F', 'L', 'O', 'W',  ' ', 0x58,
                                 0xff, 0x3e, 0xc5, 0x3, 0x0, 0xfc, 0xff, 0x2, 0x2, 0x2, 0x72, 0x61};
    const std::string buffer(v.begin(), v.end());

    auto errors = LoggingInterface::get_instance()->get_error_cnt();

    EXPECT_EQ(Protocol::Db2Register::parse_data_transmission(buffer, data_transmission, &error_code),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_UNKNOWN_ERROR);
    EXPECT_EQ(error_code, 2U);

    // This should log an error
    EXPECT_EQ(LoggingInterface::get_instance()->get_error_cnt(), errors + 1);
  }

  // Test unknown error (header_type = <DONT CARE> and control_byte = 0)
  {
    Protocol::Db2Register::DataTransmissionMessage_t data_transmission;
    uint8_t error_code = 0U;

    const std::vector<uint8_t> v{0xff, 0x1,  'T',  'O', 'T', 'A',  'L',  'F', 'L', 'O', 'W',  ' ', 0x58,
                                 0xff, 0x3e, 0xc5, 0x3, 0x0, 0xfc, 0xff, 0x0, 0x0, 0x2, 0xd2, 0xc1};
    const std::string buffer(v.begin(), v.end());

    auto errors = LoggingInterface::get_instance()->get_error_cnt();

    EXPECT_EQ(Protocol::Db2Register::parse_data_transmission(buffer, data_transmission, &error_code),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_UNKNOWN_ERROR);
    EXPECT_EQ(error_code, 2U);

    // This should log an error
    EXPECT_EQ(LoggingInterface::get_instance()->get_error_cnt(), errors + 1);
  }

  // Test unknown error (header_type = <DONT CARE> and control_byte = 1)
  {
    Protocol::Db2Register::DataTransmissionMessage_t data_transmission;
    uint8_t error_code = 0U;

    const std::vector<uint8_t> v{0xff, 0x1,  'T',  'O', 'T', 'A',  'L',  'F', 'L', 'O', 'W',  ' ', 0x58,
                                 0xff, 0x3e, 0xc5, 0x3, 0x0, 0xfc, 0xff, 0x0, 0x1, 0x2, 0xd3, 0x51};
    const std::string buffer(v.begin(), v.end());

    auto errors = LoggingInterface::get_instance()->get_error_cnt();

    EXPECT_EQ(Protocol::Db2Register::parse_data_transmission(buffer, data_transmission, &error_code),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_UNKNOWN_ERROR);
    EXPECT_EQ(error_code, 2U);

    // This should log an error
    EXPECT_EQ(LoggingInterface::get_instance()->get_error_cnt(), errors + 1);
  }

  // Test a device NAK message which has a random byte changed
  {
    Protocol::Db2Register::DataTransmissionMessage_t data_transmission;
    uint8_t error_code = 0U;

    std::vector<uint8_t> v{0xff, 0x1,  'T',  'O', 'T', 'A',  'L',  'F', 'L', 'O', 'W',  ' ', 0x58,
                           0xff, 0x3e, 0xc5, 0x3, 0x0, 0xfc, 0xff, 0x1, 0x2, 0x2, 0x82, 0x61};

    auto rand_pos = utils::get_random<size_t>(0, v.size() - 1);

    // Invert the bits of a random index of the vector
    v[rand_pos] = static_cast<uint8_t>(~v[rand_pos]);

    const std::string buffer(v.begin(), v.end());

    auto errors = LoggingInterface::get_instance()->get_error_cnt();

    EXPECT_EQ(Protocol::Db2Register::parse_data_transmission(buffer, data_transmission, &error_code),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_CRC_MISMATCH);

    // This should log an error
    EXPECT_EQ(LoggingInterface::get_instance()->get_error_cnt(), errors + 1);
  }

  // Test a valid DataTransmission containing one XRegisterData which has 8 bytes of data from register 125.7.5 to
  // 125.7.12. The SupervisoryFrame doesn't have the Synchronization character.
  {
    Protocol::Db2Register::DataTransmissionMessage_t data_transmission;
    uint8_t error_code = 0U;

    const std::vector<uint8_t> v{0x1,  'T',  'O',  'T',  'A',  'L',  'F',  'L',  'O', 'W',  ' ',  0x58,
                                 0x0,  0x81, 0x7a, 0x3,  0x0,  0xfc, 0xff, 0x0,  0x2, 0x1,  0x93, 0xa0,
                                 0x12, 0x0,  0xed, 0xff, 0x0,  0x2,  0x7d, 0x7,  0x5, 0x0,  0x8,  0x0,
                                 0x0,  0x0,  0xde, 0xad, 0xfe, 0xed, 0xbe, 0xef, 0xc, 0xab, 0xd6, 0xc6};
    const std::string buffer(v.begin(), v.end());

    auto errors = LoggingInterface::get_instance()->get_error_cnt();
    auto warnings = LoggingInterface::get_instance()->get_warning_cnt();

    EXPECT_EQ(Protocol::Db2Register::parse_data_transmission(buffer, data_transmission, &error_code),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_NO_ERROR);
    EXPECT_EQ(error_code, 0U);

    // Validate the SupervisoryFrame
    EXPECT_EQ(data_transmission.supervisory_frame.sync, 0xff);
    EXPECT_EQ(data_transmission.supervisory_frame.soh, 0x1);
    EXPECT_TRUE(utils::is_equal(std::string(data_transmission.supervisory_frame.station_id,
                                            sizeof(data_transmission.supervisory_frame.station_id)),
                                "TOTALFLOW "));
    EXPECT_EQ(data_transmission.supervisory_frame.control_code, 0x58);
    EXPECT_EQ(data_transmission.supervisory_frame.info, 0x0);
    EXPECT_EQ(data_transmission.supervisory_frame.crc, 0x7a81);

    // Validate the XSingleHeader frame
    EXPECT_EQ(data_transmission.x_single_header.length, 3U);
    EXPECT_EQ(data_transmission.x_single_header.not_length, static_cast<uint16_t>(~3U));
    EXPECT_EQ(data_transmission.x_single_header.header_type, Protocol::Db2Register::HeaderType::DATA_REQUEST);
    EXPECT_EQ(data_transmission.x_single_header.control_byte, Protocol::Db2Register::ControlByte::REQUEST_ACK);
    EXPECT_EQ(data_transmission.x_single_header.register_sets_count, 1U);
    EXPECT_EQ(data_transmission.x_single_header.crc, 0xa093);

    // Validate the XRegisterData frame
    ASSERT_EQ(data_transmission.x_register_data.size(), 1U);
    EXPECT_EQ(data_transmission.x_register_data[0].x_register_set.length, 18U);
    EXPECT_EQ(data_transmission.x_register_data[0].x_register_set.length, static_cast<uint16_t>(18U));
    EXPECT_EQ(data_transmission.x_register_data[0].x_register_set.data_compression_type,
              Protocol::Db2Register::CompressionType::UNCOMPRESSED_DATA);
    EXPECT_EQ(data_transmission.x_register_data[0].x_register_set.request_method, 2U);
    EXPECT_EQ(data_transmission.x_register_data[0].x_register_set.address.application, 125U);
    EXPECT_EQ(data_transmission.x_register_data[0].x_register_set.address.array, 7U);
    EXPECT_EQ(data_transmission.x_register_data[0].x_register_set.address.index, 5U);
    EXPECT_EQ(data_transmission.x_register_data[0].x_register_set.register_count, 8U);
    EXPECT_EQ(data_transmission.x_register_data[0].x_register_set.spare, 0U);
    EXPECT_EQ(data_transmission.x_register_data[0].x_register_set.crc, 0xc6d6);
    const std::vector<uint8_t> data{0xde, 0xad, 0xfe, 0xed, 0xbe, 0xef, 0xc, 0xab};
    ASSERT_EQ(data_transmission.x_register_data[0].data.size(), 8U);
    ASSERT_EQ(data_transmission.x_register_data[0].data, data);

    // This shouldn't log any errors
    EXPECT_EQ(LoggingInterface::get_instance()->get_error_cnt(), errors);
    // This shouldn't log any warnings
    EXPECT_EQ(LoggingInterface::get_instance()->get_warning_cnt(), warnings);
  }

  // Test if the code can handle a DataTransmission containing more than one XRegisterData
  // Test a valid DataTransmission containing two XRegisterData frames having 8 bytes of data from register 125.7.5 to
  // 125.7.12 and 16 bytes of data from register 126.0.1 to 126.0.16. The SupervisoryFrame doesn't have the
  // Synchronization character.
  {
    Protocol::Db2Register::DataTransmissionMessage_t data_transmission;
    uint8_t error_code = 0U;

    // {0x12, 0x0, 0xed, 0xff, 0x0, 0x2, 0x7d, 0x7, 0x5, 0x0, 0x8, 0x0, 0x0, 0x0, 0xde, 0xad, 0xfe, 0xed, 0xbe, 0xef,
    // 0xc, 0xab, 0xd6, 0xc6}
    const std::vector<uint8_t> v{0xff, 0x1,  'T',  'O',  'T',  'A',  'L',  'F',  'L',  'O',  'W',  ' ',  0x58, 0x0,
                                 0x7e, 0x85, 0x3,  0x0,  0xfc, 0xff, 0x0,  0x2,  0x2,  0xd3, 0xa1, 0x12, 0x0,  0xed,
                                 0xff, 0x0,  0x2,  0x7d, 0x7,  0x5,  0x0,  0x8,  0x0,  0x0,  0x0,  0xde, 0xad, 0xfe,
                                 0xed, 0xbe, 0xef, 0xc,  0xab, 0xd6, 0xc6, 0x1a, 0x0,  0xe5, 0xff, 0x0,  0x2,  0x7e,
                                 0x0,  0x1,  0x0,  0x10, 0x0,  0x0,  0x0,  0xde, 0xad, 0xfe, 0xed, 0xbe, 0xef, 0xc,
                                 0xab, 0xde, 0xad, 0xfe, 0xed, 0xbe, 0xef, 0xc,  0xab, 0xce, 0x4b};
    const std::string buffer(v.begin(), v.end());

    auto errors = LoggingInterface::get_instance()->get_error_cnt();
    auto warnings = LoggingInterface::get_instance()->get_warning_cnt();

    EXPECT_EQ(Protocol::Db2Register::parse_data_transmission(buffer, data_transmission, &error_code),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_NO_ERROR);
    EXPECT_EQ(error_code, 0U);

    // Validate the SupervisoryFrame
    EXPECT_EQ(data_transmission.supervisory_frame.sync, 0xff);
    EXPECT_EQ(data_transmission.supervisory_frame.soh, 0x1);
    EXPECT_TRUE(utils::is_equal(std::string(data_transmission.supervisory_frame.station_id,
                                            sizeof(data_transmission.supervisory_frame.station_id)),
                                "TOTALFLOW "));
    EXPECT_EQ(data_transmission.supervisory_frame.control_code, 0x58);
    EXPECT_EQ(data_transmission.supervisory_frame.info, 0x0);
    EXPECT_EQ(data_transmission.supervisory_frame.crc, 0x857e);

    // Validate the XSingleHeader frame
    EXPECT_EQ(data_transmission.x_single_header.length, 3U);
    EXPECT_EQ(data_transmission.x_single_header.not_length, static_cast<uint16_t>(~3U));
    EXPECT_EQ(data_transmission.x_single_header.header_type, Protocol::Db2Register::HeaderType::DATA_REQUEST);
    EXPECT_EQ(data_transmission.x_single_header.control_byte, Protocol::Db2Register::ControlByte::REQUEST_ACK);
    EXPECT_EQ(data_transmission.x_single_header.register_sets_count, 2U);
    EXPECT_EQ(data_transmission.x_single_header.crc, 0xa1d3);

    // Validate the XRegisterData frame
    ASSERT_EQ(data_transmission.x_register_data.size(), 2U);
    {
      EXPECT_EQ(data_transmission.x_register_data[0].x_register_set.length, 18U);
      EXPECT_EQ(data_transmission.x_register_data[0].x_register_set.not_length, static_cast<uint16_t>(~18U));
      EXPECT_EQ(data_transmission.x_register_data[0].x_register_set.data_compression_type,
                Protocol::Db2Register::CompressionType::UNCOMPRESSED_DATA);
      EXPECT_EQ(data_transmission.x_register_data[0].x_register_set.request_method, 2U);
      EXPECT_EQ(data_transmission.x_register_data[0].x_register_set.address.application, 125U);
      EXPECT_EQ(data_transmission.x_register_data[0].x_register_set.address.array, 7U);
      EXPECT_EQ(data_transmission.x_register_data[0].x_register_set.address.index, 5U);
      EXPECT_EQ(data_transmission.x_register_data[0].x_register_set.register_count, 8U);
      EXPECT_EQ(data_transmission.x_register_data[0].x_register_set.spare, 0U);
      EXPECT_EQ(data_transmission.x_register_data[0].x_register_set.crc, 0xc6d6);
      const std::vector<uint8_t> data{0xde, 0xad, 0xfe, 0xed, 0xbe, 0xef, 0xc, 0xab};
      ASSERT_EQ(data_transmission.x_register_data[0].data.size(), 8U);
      ASSERT_EQ(data_transmission.x_register_data[0].data, data);
    }
    {
      EXPECT_EQ(data_transmission.x_register_data[1].x_register_set.length, 26U);
      EXPECT_EQ(data_transmission.x_register_data[1].x_register_set.not_length, static_cast<uint16_t>(~26U));
      EXPECT_EQ(data_transmission.x_register_data[1].x_register_set.data_compression_type,
                Protocol::Db2Register::CompressionType::UNCOMPRESSED_DATA);
      EXPECT_EQ(data_transmission.x_register_data[1].x_register_set.request_method, 2U);
      EXPECT_EQ(data_transmission.x_register_data[1].x_register_set.address.application, 126U);
      EXPECT_EQ(data_transmission.x_register_data[1].x_register_set.address.array, 0U);
      EXPECT_EQ(data_transmission.x_register_data[1].x_register_set.address.index, 1U);
      EXPECT_EQ(data_transmission.x_register_data[1].x_register_set.register_count, 16U);
      EXPECT_EQ(data_transmission.x_register_data[1].x_register_set.spare, 0U);
      EXPECT_EQ(data_transmission.x_register_data[1].x_register_set.crc, 0x4bce);
      const std::vector<uint8_t> data{0xde, 0xad, 0xfe, 0xed, 0xbe, 0xef, 0xc, 0xab,
                                      0xde, 0xad, 0xfe, 0xed, 0xbe, 0xef, 0xc, 0xab};
      ASSERT_EQ(data_transmission.x_register_data[1].data.size(), 16U);
      ASSERT_EQ(data_transmission.x_register_data[1].data, data);
    }
    // This shouldn't log any errors
    EXPECT_EQ(LoggingInterface::get_instance()->get_error_cnt(), errors);
    // This shouldn't log any warnings
    EXPECT_EQ(LoggingInterface::get_instance()->get_warning_cnt(), warnings);
  }
}

TEST_F(Db2RegisterProtocolTest, DataTransmission) {
  BuildDataTransmission();
  ParseDataTransmission();
}

/** Test the WriteRequestMessage
 */
void BuildWriteRequest() {
  Protocol::Db2Register::WriteRequestMessage_t write_request_message;
  EXPECT_EQ(Protocol::Db2Register::build_write_request("TOTALFLOW", 255, write_request_message), true);

  // Validate SupervisoryFrame
  EXPECT_EQ(write_request_message.supervisory_frame.sync, 0xff);
  EXPECT_EQ(write_request_message.supervisory_frame.soh, 0x1);
  EXPECT_TRUE(utils::is_equal(std::string(write_request_message.supervisory_frame.station_id,
                                          sizeof(write_request_message.supervisory_frame.station_id)),
                              "TOTALFLOW "));
  EXPECT_EQ(write_request_message.supervisory_frame.control_code, 0x58);
  EXPECT_EQ(write_request_message.supervisory_frame.info, 0xff);
  EXPECT_EQ(write_request_message.supervisory_frame.crc, 0x3afe);

  // Validate XSingleHeader frame
  EXPECT_EQ(write_request_message.x_single_header.length, 3U);
  EXPECT_EQ(write_request_message.x_single_header.not_length, static_cast<uint16_t>(~3U));
  EXPECT_EQ(write_request_message.x_single_header.header_type, Protocol::Db2Register::HeaderType::DATA_REQUEST);
  EXPECT_EQ(write_request_message.x_single_header.control_byte, Protocol::Db2Register::ControlByte::WRITE_REQUEST);
  EXPECT_EQ(write_request_message.x_single_header.register_sets_count, 255U);
  EXPECT_EQ(write_request_message.x_single_header.crc, 0xd012);

  // Cast the request to std::vector<uint8_t>
  std::vector<uint8_t> v{0xff, 0x1,  'T',  'O',  'T',  'A', 'L', 'F',  'L',  'O', 'W', ' ',  0x58, 0xff, 0xfe,
                         0x3a, 0xd3, 0xd9, 0xce, 0xc3, 0x3, 0x0, 0xfc, 0xff, 0x0, 0x1, 0xff, 0x12, 0xd0};
  ASSERT_EQ(v.size(), sizeof(Protocol::Db2Register::SupervisoryFrame_t) + sizeof(Protocol::Db2Register::SyncFrame_t) +
                          sizeof(Protocol::Db2Register::XSingleHeader_t));
  EXPECT_EQ(v, static_cast<const std::vector<uint8_t>>(write_request_message));
}

TEST_F(Db2RegisterProtocolTest, WriteRequest) {
  BuildWriteRequest();
}

/** Test the ConfirmationMessage
 */
void ParseConfirmationMessage() {
  // Test empty buffer
  {
    Protocol::Db2Register::ConfirmationMessage_t confirmation_message;
    uint8_t error_code;

    auto warnings = LoggingInterface::get_instance()->get_warning_cnt();

    EXPECT_EQ(Protocol::Db2Register::parse_confirmation_message("", confirmation_message, &error_code,
                                                                Protocol::Db2Register::confirmation_validation()),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_INSUFFICIENT_DATA);

    // This should log a warning
    EXPECT_EQ(LoggingInterface::get_instance()->get_warning_cnt(), warnings + 1);
  }

  // Test insufficient data (XSingleHeader)
  {
    Protocol::Db2Register::ConfirmationMessage_t confirmation_message;
    uint8_t error_code;
    std::vector<uint8_t> v{0xff, 0x1, 'T', 'O', 'T', 'A', 'L', 'F', 'L', 'O', 'W', ' ', 0x58, 0x0, 0x7e, 0x85};
    const std::string buffer(v.begin(), v.end());
    auto errors = LoggingInterface::get_instance()->get_error_cnt();

    EXPECT_EQ(Protocol::Db2Register::parse_confirmation_message(buffer, confirmation_message, &error_code,
                                                                Protocol::Db2Register::confirmation_validation()),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_INSUFFICIENT_DATA);

    // This should log an error
    EXPECT_EQ(LoggingInterface::get_instance()->get_warning_cnt(), errors + 1);
  }

  // Test WriteRequest error confirmation (Header Type = 0, Control Byte = 0)
  {
    Protocol::Db2Register::ConfirmationMessage_t confirmation_message;
    uint8_t error_code;
    std::vector<uint8_t> v{0xff, 0x1,  'T',  'O', 'T', 'A',  'L',  'F', 'L', 'O', 'W',  ' ', 0x58,
                           0x0,  0x7e, 0x85, 0x3, 0x0, 0xfc, 0xff, 0x0, 0x0, 0x0, 0x53, 0x0};
    const std::string buffer(v.begin(), v.end());
    auto errors = LoggingInterface::get_instance()->get_error_cnt();

    EXPECT_EQ(Protocol::Db2Register::parse_confirmation_message(buffer, confirmation_message, &error_code,
                                                                Protocol::Db2Register::confirmation_validation()),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_UNKNOWN_ERROR);

    // This should log an error
    EXPECT_EQ(LoggingInterface::get_instance()->get_error_cnt(), errors + 1);

    // Validate SupervisoryFrame
    EXPECT_EQ(confirmation_message.supervisory_frame.sync, 0xff);
    EXPECT_EQ(confirmation_message.supervisory_frame.soh, 0x1);
    EXPECT_TRUE(utils::is_equal(std::string(confirmation_message.supervisory_frame.station_id,
                                            sizeof(confirmation_message.supervisory_frame.station_id)),
                                "TOTALFLOW "));
    EXPECT_EQ(confirmation_message.supervisory_frame.control_code, 0x58);
    EXPECT_EQ(confirmation_message.supervisory_frame.info, 0x0);
    EXPECT_EQ(confirmation_message.supervisory_frame.crc, 0x857e);

    // Validate XSingleHeader frame
    EXPECT_EQ(confirmation_message.x_single_header.length, 3U);
    EXPECT_EQ(confirmation_message.x_single_header.not_length, static_cast<uint16_t>(~3U));
    EXPECT_EQ(confirmation_message.x_single_header.header_type, Protocol::Db2Register::HeaderType::DATA_REQUEST);
    EXPECT_EQ(confirmation_message.x_single_header.control_byte, Protocol::Db2Register::ControlByte::READ_REQUEST);
    EXPECT_EQ(confirmation_message.x_single_header.register_sets_count, 0U);
    EXPECT_EQ(confirmation_message.x_single_header.crc, 0x53);
  }

  // Test WriteRequest error confirmation (Header Type = 1, Control Byte = 1)
  {
    Protocol::Db2Register::ConfirmationMessage_t confirmation_message;
    uint8_t error_code;
    std::vector<uint8_t> v{0xff, 0x1,  'T',  'O', 'T', 'A',  'L',  'F', 'L', 'O', 'W', ' ', 0x58,
                           0x0,  0x7e, 0x85, 0x3, 0x0, 0xfc, 0xff, 0x1, 0x1, 0x0, 0x3, 0x50};
    const std::string buffer(v.begin(), v.end());
    auto errors = LoggingInterface::get_instance()->get_error_cnt();

    EXPECT_EQ(Protocol::Db2Register::parse_confirmation_message(buffer, confirmation_message, &error_code,
                                                                Protocol::Db2Register::confirmation_validation()),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_UNKNOWN_ERROR);

    // This should log an error
    EXPECT_EQ(LoggingInterface::get_instance()->get_error_cnt(), errors + 1);
    EXPECT_EQ(error_code, 0x0);

    // Validate SupervisoryFrame
    EXPECT_EQ(confirmation_message.supervisory_frame.sync, 0xff);
    EXPECT_EQ(confirmation_message.supervisory_frame.soh, 0x1);
    EXPECT_TRUE(utils::is_equal(std::string(confirmation_message.supervisory_frame.station_id,
                                            sizeof(confirmation_message.supervisory_frame.station_id)),
                                "TOTALFLOW "));
    EXPECT_EQ(confirmation_message.supervisory_frame.control_code, 0x58);
    EXPECT_EQ(confirmation_message.supervisory_frame.info, 0x0);
    EXPECT_EQ(confirmation_message.supervisory_frame.crc, 0x857e);

    // Validate XSingleHeader frame
    EXPECT_EQ(confirmation_message.x_single_header.length, 3U);
    EXPECT_EQ(confirmation_message.x_single_header.not_length, static_cast<uint16_t>(~3U));
    EXPECT_EQ(confirmation_message.x_single_header.header_type, Protocol::Db2Register::HeaderType::DEVICE_ACK);
    EXPECT_EQ(confirmation_message.x_single_header.control_byte, Protocol::Db2Register::ControlByte::WRITE_REQUEST);
    EXPECT_EQ(confirmation_message.x_single_header.register_sets_count, 0U);
    EXPECT_EQ(confirmation_message.x_single_header.crc, 0x5003);
  }

  // Test WriteRequest NACK (Header Type = 1, Control Byte = 2)
  {
    Protocol::Db2Register::ConfirmationMessage_t confirmation_message;
    uint8_t error_code;
    std::vector<uint8_t> v{0xff, 0x1,  'T',  'O', 'T', 'A',  'L',  'F', 'L', 'O', 'W', ' ', 0x58,
                           0x0,  0x7e, 0x85, 0x3, 0x0, 0xfc, 0xff, 0x1, 0x2, 0x0, 0x3, 0xa0};
    const std::string buffer(v.begin(), v.end());
    auto errors = LoggingInterface::get_instance()->get_error_cnt();

    EXPECT_EQ(Protocol::Db2Register::parse_confirmation_message(buffer, confirmation_message, &error_code,
                                                                Protocol::Db2Register::confirmation_validation()),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_NACK);

    // This should log an error
    EXPECT_EQ(LoggingInterface::get_instance()->get_error_cnt(), errors + 1);
    EXPECT_EQ(error_code, 0x0);

    // Validate SupervisoryFrame
    EXPECT_EQ(confirmation_message.supervisory_frame.sync, 0xff);
    EXPECT_EQ(confirmation_message.supervisory_frame.soh, 0x1);
    EXPECT_TRUE(utils::is_equal(std::string(confirmation_message.supervisory_frame.station_id,
                                            sizeof(confirmation_message.supervisory_frame.station_id)),
                                "TOTALFLOW "));
    EXPECT_EQ(confirmation_message.supervisory_frame.control_code, 0x58);
    EXPECT_EQ(confirmation_message.supervisory_frame.info, 0x0);
    EXPECT_EQ(confirmation_message.supervisory_frame.crc, 0x857e);

    // Validate XSingleHeader frame
    EXPECT_EQ(confirmation_message.x_single_header.length, 3U);
    EXPECT_EQ(confirmation_message.x_single_header.not_length, static_cast<uint16_t>(~3U));
    EXPECT_EQ(confirmation_message.x_single_header.header_type, Protocol::Db2Register::HeaderType::DEVICE_ACK);
    EXPECT_EQ(confirmation_message.x_single_header.control_byte, Protocol::Db2Register::ControlByte::REQUEST_NACK);
    EXPECT_EQ(confirmation_message.x_single_header.register_sets_count, 0U);
    EXPECT_EQ(confirmation_message.x_single_header.crc, 0xa003);
  }

  // Test WriteRequest ACK (Header Type = 1, Control Byte = 0)
  {
    Protocol::Db2Register::ConfirmationMessage_t confirmation_message;
    uint8_t error_code;
    std::vector<uint8_t> v{0xff, 0x1,  'T',  'O', 'T', 'A',  'L',  'F', 'L', 'O', 'W', ' ', 0x58,
                           0x0,  0x7e, 0x85, 0x3, 0x0, 0xfc, 0xff, 0x1, 0x0, 0x0, 0x2, 0xc0};
    const std::string buffer(v.begin(), v.end());
    auto errors = LoggingInterface::get_instance()->get_error_cnt();

    EXPECT_EQ(Protocol::Db2Register::parse_confirmation_message(buffer, confirmation_message, &error_code,
                                                                Protocol::Db2Register::confirmation_validation()),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_NO_ERROR);

    // This shouldn't log any errors
    EXPECT_EQ(LoggingInterface::get_instance()->get_error_cnt(), errors);

    // Validate SupervisoryFrame
    EXPECT_EQ(confirmation_message.supervisory_frame.sync, 0xff);
    EXPECT_EQ(confirmation_message.supervisory_frame.soh, 0x1);
    EXPECT_TRUE(utils::is_equal(std::string(confirmation_message.supervisory_frame.station_id,
                                            sizeof(confirmation_message.supervisory_frame.station_id)),
                                "TOTALFLOW "));
    EXPECT_EQ(confirmation_message.supervisory_frame.control_code, 0x58);
    EXPECT_EQ(confirmation_message.supervisory_frame.info, 0x0);
    EXPECT_EQ(confirmation_message.supervisory_frame.crc, 0x857e);

    // Validate XSingleHeader frame
    EXPECT_EQ(confirmation_message.x_single_header.length, 3U);
    EXPECT_EQ(confirmation_message.x_single_header.not_length, static_cast<uint16_t>(~3U));
    EXPECT_EQ(confirmation_message.x_single_header.header_type, Protocol::Db2Register::HeaderType::DEVICE_ACK);
    EXPECT_EQ(confirmation_message.x_single_header.control_byte, Protocol::Db2Register::ControlByte::READ_REQUEST);
    EXPECT_EQ(confirmation_message.x_single_header.register_sets_count, 0U);
    EXPECT_EQ(confirmation_message.x_single_header.crc, 0xc002);
  }
}

TEST_F(Db2RegisterProtocolTest, ConfirmationMessage) {
  ParseConfirmationMessage();
}

/** Test the DataTransmissionAckMessage
 */
void ParseDataTransmissionAckMessage() {
  // Test empty buffer
  {
    Protocol::Db2Register::ConfirmationMessage_t confirmation_message;
    uint8_t error_code;

    auto warnings = LoggingInterface::get_instance()->get_warning_cnt();

    EXPECT_EQ(Protocol::Db2Register::parse_confirmation_message(
                  "", confirmation_message, &error_code, Protocol::Db2Register::data_transmission_ack_validation()),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_INSUFFICIENT_DATA);

    // This should log a warning
    EXPECT_EQ(LoggingInterface::get_instance()->get_warning_cnt(), warnings + 1);
  }

  // Test insufficient data (XSingleHeader)
  {
    Protocol::Db2Register::ConfirmationMessage_t confirmation_message;
    uint8_t error_code;
    std::vector<uint8_t> v{0xff, 0x1, 'T', 'O', 'T', 'A', 'L', 'F', 'L', 'O', 'W', ' ', 0x58, 0x0, 0x7e, 0x85};
    const std::string buffer(v.begin(), v.end());
    auto errors = LoggingInterface::get_instance()->get_error_cnt();

    EXPECT_EQ(Protocol::Db2Register::parse_confirmation_message(
                  buffer, confirmation_message, &error_code, Protocol::Db2Register::data_transmission_ack_validation()),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_INSUFFICIENT_DATA);

    // This should log an error
    EXPECT_EQ(LoggingInterface::get_instance()->get_warning_cnt(), errors + 1);
  }

  // Test DataTransmissionAck error (Header Type = 0, Control Byte = 0)
  {
    Protocol::Db2Register::ConfirmationMessage_t confirmation_message;
    uint8_t error_code;
    std::vector<uint8_t> v{0xff, 0x1,  'T',  'O', 'T', 'A',  'L',  'F', 'L', 'O', 'W',  ' ', 0x58,
                           0x0,  0x7e, 0x85, 0x3, 0x0, 0xfc, 0xff, 0x0, 0x0, 0x0, 0x53, 0x0};
    const std::string buffer(v.begin(), v.end());
    auto errors = LoggingInterface::get_instance()->get_error_cnt();

    EXPECT_EQ(Protocol::Db2Register::parse_confirmation_message(
                  buffer, confirmation_message, &error_code, Protocol::Db2Register::data_transmission_ack_validation()),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_UNKNOWN_ERROR);

    // This should log an error
    EXPECT_EQ(LoggingInterface::get_instance()->get_error_cnt(), errors + 1);

    // Validate SupervisoryFrame
    EXPECT_EQ(confirmation_message.supervisory_frame.sync, 0xff);
    EXPECT_EQ(confirmation_message.supervisory_frame.soh, 0x1);
    EXPECT_TRUE(utils::is_equal(std::string(confirmation_message.supervisory_frame.station_id,
                                            sizeof(confirmation_message.supervisory_frame.station_id)),
                                "TOTALFLOW "));
    EXPECT_EQ(confirmation_message.supervisory_frame.control_code, 0x58);
    EXPECT_EQ(confirmation_message.supervisory_frame.info, 0x0);
    EXPECT_EQ(confirmation_message.supervisory_frame.crc, 0x857e);

    // Validate XSingleHeader frame
    EXPECT_EQ(confirmation_message.x_single_header.length, 3U);
    EXPECT_EQ(confirmation_message.x_single_header.not_length, static_cast<uint16_t>(~3U));
    EXPECT_EQ(confirmation_message.x_single_header.header_type, Protocol::Db2Register::HeaderType::DATA_REQUEST);
    EXPECT_EQ(confirmation_message.x_single_header.control_byte, Protocol::Db2Register::ControlByte::READ_REQUEST);
    EXPECT_EQ(confirmation_message.x_single_header.register_sets_count, 0U);
    EXPECT_EQ(confirmation_message.x_single_header.crc, 0x53);
  }

  // Test DataTransmissionAck error (Header Type = 1, Control Byte = 0)
  {
    Protocol::Db2Register::ConfirmationMessage_t confirmation_message;
    uint8_t error_code;
    std::vector<uint8_t> v{0xff, 0x1,  'T',  'O', 'T', 'A',  'L',  'F', 'L', 'O', 'W', ' ', 0x58,
                           0x0,  0x7e, 0x85, 0x3, 0x0, 0xfc, 0xff, 0x1, 0x0, 0x0, 0x2, 0xc0};
    const std::string buffer(v.begin(), v.end());
    auto errors = LoggingInterface::get_instance()->get_error_cnt();

    EXPECT_EQ(Protocol::Db2Register::parse_confirmation_message(
                  buffer, confirmation_message, &error_code, Protocol::Db2Register::data_transmission_ack_validation()),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_UNKNOWN_ERROR);

    // This should log an error
    EXPECT_EQ(LoggingInterface::get_instance()->get_error_cnt(), errors + 1);
    EXPECT_EQ(error_code, 0x0);

    // Validate SupervisoryFrame
    EXPECT_EQ(confirmation_message.supervisory_frame.sync, 0xff);
    EXPECT_EQ(confirmation_message.supervisory_frame.soh, 0x1);
    EXPECT_TRUE(utils::is_equal(std::string(confirmation_message.supervisory_frame.station_id,
                                            sizeof(confirmation_message.supervisory_frame.station_id)),
                                "TOTALFLOW "));
    EXPECT_EQ(confirmation_message.supervisory_frame.control_code, 0x58);
    EXPECT_EQ(confirmation_message.supervisory_frame.info, 0x0);
    EXPECT_EQ(confirmation_message.supervisory_frame.crc, 0x857e);

    // Validate XSingleHeader frame
    EXPECT_EQ(confirmation_message.x_single_header.length, 3U);
    EXPECT_EQ(confirmation_message.x_single_header.not_length, static_cast<uint16_t>(~3U));
    EXPECT_EQ(confirmation_message.x_single_header.header_type, Protocol::Db2Register::HeaderType::DEVICE_ACK);
    EXPECT_EQ(confirmation_message.x_single_header.control_byte, Protocol::Db2Register::ControlByte::READ_REQUEST);
    EXPECT_EQ(confirmation_message.x_single_header.register_sets_count, 0U);
    EXPECT_EQ(confirmation_message.x_single_header.crc, 0xc002);
  }

  // Test DataTransmissionAck NACK (Header Type = 1, Control Byte = 2)
  {
    Protocol::Db2Register::ConfirmationMessage_t confirmation_message;
    uint8_t error_code;
    std::vector<uint8_t> v{0xff, 0x1,  'T',  'O', 'T', 'A',  'L',  'F', 'L', 'O', 'W', ' ', 0x58,
                           0x0,  0x7e, 0x85, 0x3, 0x0, 0xfc, 0xff, 0x1, 0x2, 0x0, 0x3, 0xa0};
    const std::string buffer(v.begin(), v.end());
    auto errors = LoggingInterface::get_instance()->get_error_cnt();

    EXPECT_EQ(Protocol::Db2Register::parse_confirmation_message(
                  buffer, confirmation_message, &error_code, Protocol::Db2Register::data_transmission_ack_validation()),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_NACK);

    // This should log an error
    EXPECT_EQ(LoggingInterface::get_instance()->get_error_cnt(), errors + 1);
    EXPECT_EQ(error_code, 0x0);

    // Validate SupervisoryFrame
    EXPECT_EQ(confirmation_message.supervisory_frame.sync, 0xff);
    EXPECT_EQ(confirmation_message.supervisory_frame.soh, 0x1);
    EXPECT_TRUE(utils::is_equal(std::string(confirmation_message.supervisory_frame.station_id,
                                            sizeof(confirmation_message.supervisory_frame.station_id)),
                                "TOTALFLOW "));
    EXPECT_EQ(confirmation_message.supervisory_frame.control_code, 0x58);
    EXPECT_EQ(confirmation_message.supervisory_frame.info, 0x0);
    EXPECT_EQ(confirmation_message.supervisory_frame.crc, 0x857e);

    // Validate XSingleHeader frame
    EXPECT_EQ(confirmation_message.x_single_header.length, 3U);
    EXPECT_EQ(confirmation_message.x_single_header.not_length, static_cast<uint16_t>(~3U));
    EXPECT_EQ(confirmation_message.x_single_header.header_type, Protocol::Db2Register::HeaderType::DEVICE_ACK);
    EXPECT_EQ(confirmation_message.x_single_header.control_byte, Protocol::Db2Register::ControlByte::REQUEST_NACK);
    EXPECT_EQ(confirmation_message.x_single_header.register_sets_count, 0U);
    EXPECT_EQ(confirmation_message.x_single_header.crc, 0xa003);
  }

  // Test DataTransmissionAck ACK (Header Type = 1, Control Byte = 1)
  {
    Protocol::Db2Register::ConfirmationMessage_t confirmation_message;
    uint8_t error_code;
    std::vector<uint8_t> v{0xff, 0x1,  'T',  'O', 'T', 'A',  'L',  'F', 'L', 'O', 'W', ' ', 0x58,
                           0x0,  0x7e, 0x85, 0x3, 0x0, 0xfc, 0xff, 0x1, 0x1, 0x0, 0x3, 0x50};
    const std::string buffer(v.begin(), v.end());
    auto errors = LoggingInterface::get_instance()->get_error_cnt();

    EXPECT_EQ(Protocol::Db2Register::parse_confirmation_message(
                  buffer, confirmation_message, &error_code, Protocol::Db2Register::data_transmission_ack_validation()),
              Protocol::Db2Register::TransactionResponse::TRANSACTION_NO_ERROR);

    // This shouldn't log any errors
    EXPECT_EQ(LoggingInterface::get_instance()->get_error_cnt(), errors);

    // Validate SupervisoryFrame
    EXPECT_EQ(confirmation_message.supervisory_frame.sync, 0xff);
    EXPECT_EQ(confirmation_message.supervisory_frame.soh, 0x1);
    EXPECT_TRUE(utils::is_equal(std::string(confirmation_message.supervisory_frame.station_id,
                                            sizeof(confirmation_message.supervisory_frame.station_id)),
                                "TOTALFLOW "));
    EXPECT_EQ(confirmation_message.supervisory_frame.control_code, 0x58);
    EXPECT_EQ(confirmation_message.supervisory_frame.info, 0x0);
    EXPECT_EQ(confirmation_message.supervisory_frame.crc, 0x857e);

    // Validate XSingleHeader frame
    EXPECT_EQ(confirmation_message.x_single_header.length, 3U);
    EXPECT_EQ(confirmation_message.x_single_header.not_length, static_cast<uint16_t>(~3U));
    EXPECT_EQ(confirmation_message.x_single_header.header_type, Protocol::Db2Register::HeaderType::DEVICE_ACK);
    EXPECT_EQ(confirmation_message.x_single_header.control_byte, Protocol::Db2Register::ControlByte::WRITE_REQUEST);
    EXPECT_EQ(confirmation_message.x_single_header.register_sets_count, 0U);
    EXPECT_EQ(confirmation_message.x_single_header.crc, 0x5003);
  }
}

TEST_F(Db2RegisterProtocolTest, DataTransmissionAckMessage) {
  ParseDataTransmissionAckMessage();
}
