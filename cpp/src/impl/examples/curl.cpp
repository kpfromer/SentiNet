/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : curl
 * @created     : Sunday Aug 11, 2019 13:59:49 MDT
 */

#include "networking/curl/CurlUploader.hpp"
#include <memory>

int main(int argc, char** argv) {
  auto a = std::make_unique<CurlUploader>();

  a->ftp_post("ftp://ftp.dlptest.com/", "./temp"); /// error checking's for dweeps
  return 0;

}
