/**
 *  @file file_system
 *  @brief A file system library for sentinet
 *
 *  This will help functionalities like parsing files, reading
 *  basic headers from retrieved endpoints and other things useful
 *  for having a more reliable file checker
 *
 *  @author       theo (theo@varnsen)
 *  @created      Monday Jul 29, 2019 16:31:26 MDT
 */

#ifndef FILE_SYSTEM_HPP

#define FILE_SYSTEM_HPP

#include "strings.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>

namespace utils {
namespace defaults {
const char file_seperator = '/';
}
namespace fs {
/**
 * @brief Similar to the bash mkdir command, creates a directory (can be
 * embedded)
 *
 * @param path The directory to build (can be embedded, i.e.
 * mkdir("./val/thing/");
 *
 * @return System status
 */
static inline bool mkdir(const std::string &path) {
  std::stringstream ss;
  ss << "mkdir -p " << path;
  return system(ss.str().c_str()) >= 0;
}

/**
 * @brief Removes everything, use sparingly, this is rm -rf equivalent
 *
 * @param path The path to remove
 *
 * @return System status
 */
static inline bool rmrf(const std::string &path) {
  std::stringstream ss;
  ss << "rm -rf " << path;
  return system(ss.str().c_str()) >= 0;
}

/**
 * @brief "Opens" a file and reads it. This returns a string that is the file
 * content
 *
 * @param file The file to open
 *
 * @return The contents of the file
 */
static inline std::string cat(const std::string &file) {
  std::ifstream input(file);
  std::string cat_string((std::istreambuf_iterator<char>(input)),
                         std::istreambuf_iterator<char>());
  input.close();
  return cat_string;
}

/**
 * @brief Although c++ fstream creates a file automatically, it is nice to have
 * this. Creates a blank file
 *
 * @param file The file to create
 *
 * @return System status
 */
static inline bool touch(const std::string &file) {
  std::stringstream ss;
  ss << "touch " << file;
  return system(ss.str().c_str()) >= 0;
}

/**
 * @brief Writes content to a file
 *
 * @param path The file to write to
 * @param content The content to write
 *
 * @return Status
 */
static inline bool write(const std::string &path, const std::string &content) {
  std::ofstream input(path, std::ios_base::app);
  input << content;
  input.close();
  return true;
}

/**
 * @brief Overwrites the content of a file
 *
 * @param path The file to write to
 * @param content The content to write
 *
 * @return Status
 */
static inline bool overwrite(const std::string &path,
                             const std::string &content) {
  std::ofstream input(path, std::ios_base::out);
  input << content;
  input.close();
  return true;
}
/*
static inline bool can_exec(const std::string& file)
{
    struct stat  st;
    if (stat(file.c_str(), &st) < 0)
        return false;
    if ((st.st_mode & S_IEXEC) != 0)
        return true;
    return false;
}

static inline bool execute_file(const std::string& file) {
  int pid = -1;
  switch ((pid = fork()))
  {
    case -1:
      perror ("fork");
      break;
    case 0:
      execv (file.c_str(), NULL);
      exit(EXIT_FAILURE);
      break;
    default:
      puts ("This is a message from the parent");
      break;
  }
  if(!can_exec(file)){
    LOG_ERROR("Not a valid executable file");
    return false;
  }
  .
}
*/

/**
 * @brief Appends a file with a slash
 *
 * @param base The base name
 * @param basename The name of the file
 *
 * @return The string appended
 */
static inline std::string file_append(const std::string &base,
                                      const std::string &basename) {
  std::string mutable_basename;
  std::string mutable_base;
  if (basename[0] == '/')
    mutable_basename = basename.substr(1);
  else
    mutable_basename = basename;
  if (base[base.size() - 1] == '/')
    mutable_base = base.substr(0, base.size() - 1);
  else
    mutable_base = base;
  return ::utils::strings::join_d('/', mutable_base, mutable_basename);
}

/**
 * @brief Gets the current working directory
 *
 * @return The current working directory as a string
 */
static inline std::string pwd() {
  char *wd = get_current_dir_name();
  std::string cwd(wd);
  free(wd);
  return cwd;
}

/**
 * @brief Gets the absolute path given relative path
 *
 * example:
 * absolute_path("./this_dir") might return
 * "/home/theo/destroyer/of/all/evil/this_dir
 *
 * @param file The file to get the path of
 *
 * @return The absolute path
 */
static inline std::string absolute_path(const std::string &file) {
  std::string path = file;
  if (path.size() >= 2 && path[0] == '.' && path[1] == '/')
    path = path.substr(2);
  return ::utils::strings::join_d("/", pwd(), path);
}

} // namespace fs
} // namespace utils

#endif /* end of include guard FILE_SYSTEM_HPP */
