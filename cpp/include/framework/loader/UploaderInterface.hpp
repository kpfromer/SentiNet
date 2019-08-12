/**
 * @author      : theo (theo@$theo-lenovo-yoga-Arch)
 * @file        : UploaderInterface
 * @created     : Saturday Aug 10, 2019 20:38:17 MDT
 */

#ifndef UPLOADERINTERFACE_HPP

#define UPLOADERINTERFACE_HPP

// C++ includes
#include <string>

// Project inludes
#include "framework/utils/file_system.hpp"

class UploaderInterface {
public:
  UploaderInterface();
  virtual ~UploaderInterface();

  // Recievers
  virtual bool grab_file(const std::string &url,
                         const std::string &filename) = 0;
  virtual bool ftp_get(const std::string &url, const std::string &filename) = 0;
  virtual std::string http_get(const std::string &url,
                               const std::string &request) = 0;
  virtual std::string https_get(const std::string &url,
                                const std::string &request) = 0;

  // Posters
  virtual bool post_file(const std::string &url,
                         const std::string &filename) = 0;
  virtual bool ftp_post(const std::string &url,
                        const std::string &filename) = 0;
  virtual std::string http_post(const std::string &url,
                                const std::string &filename) = 0;
  // virtual bool smtp_send(const std::string& address, const std::string&
  // content) = 0; virtual bool smtp_ssl_send(const std::string& address, const
  // std::string& content) = 0;
};

#endif /* end of include guard UPLOADERINTERFACE_HPP */
