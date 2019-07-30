/**
 *  @file main
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@varnsen)
 *  @created      Monday Jul 29, 2019 17:05:41 MDT
 *  @bug No Known Bugs
 */

#include "utils.hpp"

int main() {
  std::cout << utils::fs::cat("dummy") << std::endl;
  LOG_INFO("Yes");
  LOG_ERROR("NOO!");
  LOG_WARN("OOPS");
  LOG_FATAL("NOPE");
  LOG_DEBUG("EH");
  return 0;
}
