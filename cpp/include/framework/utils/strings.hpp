
#ifndef UTILS_STRING_HPP
#define UTILS_STRING_HPP

namespace utils {
namespace strings {
static inline std::string upper(const std::string &value) {
  auto val = value;
  std::transform(val.begin(), val.end(), val.begin(), ::toupper);
  return val;
}

static inline std::string lower(const std::string &value) {
  auto val = value;
  std::transform(val.begin(), val.end(), val.begin(), ::tolower);
  return val;
}
static inline std::vector<std::string> parse(const std::string &value,
                                             const char delim) {
  std::vector<std::string> values;
  auto pos = 0U;
  auto size = value.size();
  while (pos < size) {
    std::string temp;
    while (pos < size && value[pos] != delim)
      temp += value[pos++];
    pos++;
    values.push_back(temp);
  }
  return values;
}

// Helper for join
template <typename T> static inline T join(T value) { return value; }

// Join a list of strings / ints / whatever (streamable) no deliminator
template <typename T, typename... ArgsT>
static inline std::string join(T value, ArgsT... strings) {
  std::stringstream ss;
  ss << value << join(strings...);
  return ss.str();
}

// Helper for join_d
template <typename T, typename D> static inline T join_d(D delim, T value) {
  return value;
}
// Join a list of strings / ints / whatever (streamable) with a deliminator
template <typename Delim, typename T, typename... ArgsT>
static inline std::string join_d(Delim delim, T value, ArgsT... strings) {
  std::stringstream ss;
  ss << value << delim << join_d(delim, strings...);
  return ss.str();
}

} // namespace strings
} // namespace utils
#endif
