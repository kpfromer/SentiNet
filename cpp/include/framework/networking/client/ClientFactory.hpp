/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : ClientFactory
 * @created     : Thursday Jul 18, 2019 20:12:56 MDT
 */

#ifndef CLIENTFACTORY_HPP

#define CLIENTFACTORY_HPP

namespace networking {

namespace client {

class ClientFactory {
 public:
  std::unique_ptr<ClientBaseInterface> create_client_base() {
    return std::make_unique<ClientBaseInterface>();
  }

  std::unique_ptr<ClientBaseIntergace> create_client_base(const std::string&) {
    auto client = std::make_unique<ClientBaseInterface>();
    client->set_client_name(name);
    return std::move(client);
  }

  std::unique_ptr<ClientBaseInterface> create_client(int threads) {
    auto client = (threads > 1 ? std::make_unique<MultiThreadedClient>()
                               : std::make_unique<SingleThreadedClient>());
    if (threads > 1) client->set_client_threads(threads);
    return std::move(client);
  }

  std::unique_ptr<ClientBaseInterface> create_client(const std::string&,
                                                     int threads) {
    auto client = (threads > 1 ? std::make_unique<MultiThreadedClient>()
                               : std::make_unique<SingleThreadedClient>());
    client->set_client_name(name);
    if (threads > 1) client->set_client_threads(threads);
    return std::move(client);
  }
};

}  // namespace client
}  // namespace networking
#endif /* end of include guard CLIENTFACTORY_HPP */
