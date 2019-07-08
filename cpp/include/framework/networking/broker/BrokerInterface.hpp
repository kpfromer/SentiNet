/**
 * @author    : theo (theo@$HOSTNAME)
 * @file    : BrokerInterface
 * @created   : Saturday Jul 13, 2019 15:16:49 MDT
 */

#ifndef BROKERINTERFACE_HPP

#define BROKERINTERFACE_HPP

class BrokerInterface {
 public:
  BrokerInterface(const std::string& frontend_addr,
                  const std::string& backend_addr);
  virtual ~BrokerInterface();

 private:
  const std::string frontend_addr;
  const std::string backend_addr;
  /* private data */
};

#endif /* end of include guard BROKERINTERFACE_HPP */
