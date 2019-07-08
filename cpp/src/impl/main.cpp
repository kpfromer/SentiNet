/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : main
 * @created     : Wednesday Jul 24, 2019 11:24:28 MDT
 */

#include "DebugMessage.hpp"
#include "networking/publisher/PublisherInterface.hpp"

int main() {
  DebugMessage* a = new DebugMessage();
  a->set_name("Henry");
  a->set_id(3);
  a->set_extra("Extra Yall");

  int i = a->get_id();

  DebugMessage* b = new DebugMessage();

  std::string a_str = (std::string)*a;
  b->parse_from_string(a_str);

  std::cout << i << std::endl;

  std::cout << *a << "\n\n";
  std::cout << a->print() << std::endl;
  delete a;
  std::cout << b->print() << std::endl;
  return 0;
}
