/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "common/crc/CRC16.hpp"
#include "CRC.hpp"

class CRC16Test : public CRCTest {};

/** Tests that the Application based class is able to initialize with command line arguments and
 *  can handle improper and proper arguments.
 */
TEST_F(CRC16Test, ROCProtocolExample) {
  // Unsigned
  {
    // Create a listing that should match the ROC protocol CRC definition
    std::list<std::pair<std::vector<unsigned char>, uint16_t>> samples = {
        {{0}, 0x0000},
        {{1}, 0xC0C1},
        // Some random streams
        {{0x51, 0x02, 0xCC, 0x4D, 0x2A, 0xFF, 0x56, 0x00, 0x09}, 0x5F7A},
        {{0xFF, 0x34, 0x11, 0x59, 0x00, 0xDF, 0xEE, 0x2A, 0xBC}, 0xEB0F},
        // From the ROC protocol specification manual (4-1, pg 311)
        {{1, 2, 1, 0, 17, 3, 'M', 'O', 'C'}, (24 << 8) + 133},                         // decimal
        {{0x01, 0x02, 0x01, 0x00, 0x11, 0x03, 0x4D, 0x4F, 0x43}, (0x18 << 8) + 0x85},  // hexidecimal
    };

    // Validate the samples
    for(const auto& sample : samples) {
      EXPECT_EQ(sample.second, CRC16::generate(&sample.first[0], sample.first.size()));
    }
  }

  // Signed
  {
    // Create a listing that should match the ROC protocol CRC definition
    std::list<std::pair<std::vector<char>, uint16_t>> samples = {
        {{0}, 0x0000},
        {{1}, 0xC0C1},
        // Some random streams
        // 'narrowing' occurs when using the signed char vesion ... explicitly force a signed char
        {{0x51, 0x02, static_cast<char>(0xCC), 0x4D, 0x2A, static_cast<char>(0xFF), 0x56, 0x00, 0x09}, 0x5F7A},
        {{static_cast<char>(0xFF), 0x34, 0x11, 0x59, 0x00, static_cast<char>(0xDF), static_cast<char>(0xEE),
          static_cast<char>(0x2A), static_cast<char>(0xBC)},
         0xEB0F},
        // From the ROC protocol specification manual (4-1, pg 311)
        {{1, 2, 1, 0, 17, 3, 'M', 'O', 'C'}, (24 << 8) + 133},                         // decimal
        {{0x01, 0x02, 0x01, 0x00, 0x11, 0x03, 0x4D, 0x4F, 0x43}, (0x18 << 8) + 0x85},  // hexidecimal
    };

    // Validate the samples
    for(const auto& sample : samples) {
      EXPECT_EQ(sample.second, CRC16::generate(&sample.first[0], sample.first.size()));
    }
  }
}
