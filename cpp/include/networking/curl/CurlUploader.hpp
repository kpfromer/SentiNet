/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : CurlUploader
 * @created     : Saturday Aug 10, 2019 20:57:45 MDT
 */

#ifndef CURLUPLOADER_HPP

#define CURLUPLOADER_HPP

// C++ includes
#include <curl/curl.h>

// Local Includes
#include "framework/loader/UploaderInterface.hpp"

class CurlUploader {
public:
<<<<<<< HEAD
  CurlUploader(){}
  ~CurlUploader(){}

  bool grab_file(const std::string &url, const std::string &filename) ;
  bool ftp_get(const std::string &url, const std::string &filename) ;
  std::string http_get(const std::string &url, const std::string &request) ;
  std::string https_get(const std::string &url, const std::string &request) ;

  bool post_file(const std::string &url, const std::string &filename) ;
  bool ftp_post(const std::string &url, const std::string &filename) ;
  std::string http_post(const std::string &url, const std::string &filename) ;
=======
  CurlUploader() {}
  virtual ~CurlUploader() = default;

  bool grab_file(const std::string &url, const std::string &filename);
  bool ftp_get(const std::string &url, const std::string &filename);
  std::string http_get(const std::string &url, const std::string &request);
  std::string https_get(const std::string &url, const std::string &request);

  bool post_file(const std::string &url, const std::string &filename);
  bool ftp_post(const std::string &url, const std::string &filename);
  std::string http_post(const std::string &url, const std::string &filename);
  bool smtp_send(const std::string &address, const std::string &content);
  bool smtp_ssl_send(const std::string &address, const std::string &content);
>>>>>>> 9a05635c30a16fba78975c5381be4c1c1df4b666
};

#endif /* end of include guard CURLUPLOADER_HPP */
