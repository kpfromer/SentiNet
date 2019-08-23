/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : ServerInterface
 * @created     : Saturday Aug 10, 2019 20:45:40 MDT
 */

#ifndef SERVERINTERFACE_HPP

#define SERVERINTERFACE_HPP

// Local Includes
#include "framework/utils/utils.hpp"

class ServerInterface {
public:
  ServerInterface(const std::string &b_path);
  virtual ~ServerInterface() = default;

  virtual void serve() = 0;

protected:
  std::string base_path;
};

#endif /* end of include guard SERVERINTERFACE_HPP */
