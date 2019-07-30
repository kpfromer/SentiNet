/**
 *  @file file_system
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@varnsen)
 *  @created      Monday Jul 29, 2019 16:31:26 MDT
 *  @bug No Known Bugs
 */

#ifndef FILE_SYSTEM_HPP

#define FILE_SYSTEM_HPP

namespace utils {
namespace defaults {
const char file_seperator = '/';
}
namespace fs {
static inline bool mkdir(const std::string &path) {
  std::stringstream ss;
  ss << "mkdir -p " << path;
  return system(ss.str().c_str()) >= 0;
}

static inline bool rmrf(const std::string &path) {
  std::stringstream ss;
  ss << "rm -rf " << path;
  return system(ss.str().c_str()) >= 0;
}

static inline std::string cat(const std::string &file) {
  std::ifstream input(file);
  std::string cat_string((std::istreambuf_iterator<char>(input)),
                         std::istreambuf_iterator<char>());
  input.close();
  return cat_string;
}

static inline bool touch(const std::string &file) {
  std::stringstream ss;
  ss << "touch " << file;
  return system(ss.str().c_str()) >= 0;
}

static inline bool write(const std::string &path, const std::string &content) {
  std::ofstream input(path, std::ios_base::app);
  input << content;
  input.close();
  return true;
}

static inline bool overwrite(const std::string &path,
                             const std::string &content) {
  std::ofstream input(path, std::ios_base::out);
  input << content;
  input.close();
  return true;
}

static inline std::string pwd() {
  char *wd = get_current_dir_name();
  std::string cwd(wd);
  free(wd);
  return cwd;
}

static inline std::string absolute_path(const std::string &file) {
  std::string path = file;
  if (path.size() >= 2 && path[0] == '.' && path[1] == '/')
    path = path.substr(2);
  return ::utils::strings::join_d("/", pwd(), path);
}

} // namespace fs
} // namespace utils

#endif /* end of include guard FILE_SYSTEM_HPP */
