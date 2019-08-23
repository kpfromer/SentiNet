/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : CurlClient
 * @created     : Thursday Aug 15, 2019 19:40:29 MDT
 */

#ifndef CURLCLIENT_HPP

#define CURLCLIENT_HPP

// Local Includes
#include "framework/networking/ClientInterface.hpp"

class CurlClient : public ClientInterface {
  CurlClient();
  virtual ~CurlClient();

protected:
  virtual bool upload_file(const std::string &url,
                           const std::string &local_file) override;
  virtual std::string request_file(const std::string &file) override;

private:
  /* private data */
};

#endif /* end of include guard CURLCLIENT_HPP */
