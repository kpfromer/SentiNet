#ifndef RANDOM_HPP
#define RANDOM_HPP

namespace utils {
namespace random {
static inline unsigned char random_char() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 255);
  return static_cast<unsigned char>(dis(gen));
}
static inline std::string generate_hex(const unsigned int len) {
  std::stringstream ss;
  for (unsigned int i = 0; i < len; i++) {
    auto rc = random_char();
    std::stringstream hexstream;
    hexstream << std::hex << int(rc);
    auto hex = hexstream.str();
    ss << (hex.length() < 2 ? '0' + hex : hex);
  }
  return ss.str();
}
} // namespace random
} // namespace utils

#endif /* end of include guard RANDOM_HPP */
