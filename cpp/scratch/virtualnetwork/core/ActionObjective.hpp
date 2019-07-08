/**
 *  @file ActionObjective
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Tuesday Jul 02, 2019 18:45:51 MDT
 *  @bug No Known Bugs
 */

#ifndef ACTIONOBJECTIVE_HPP

#define ACTIONOBJECTIVE_HPP

// C++ includes
#include <iostream>

// Project includes

namespace obps {
namespace core {

class ActionObjective {
 public:
  ActionObjective(const int size_);

  virtual ~ActionObjective();

  template <typename message>
  message read() {
    return *(reinterpret_cast<message*>(data));
  }

  template <typename message>
  void write(message* a) {
    if (sizeof(message) != buffer_size) {
      std::cout << "Invalid write data type" << std::endl;
      exit(1);
    }
    *(reinterpret_cast<message*>(data)) = *a;
  }

 private:
  void* data;
  const int buffer_size;  // assuming data doesn't change sizes
};

}  // namespace core
}  // namespace obps
#endif /* end of include guard ACTIONOBJECTIVE_HPP */
