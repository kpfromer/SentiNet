/**
 *  @file Loader
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Monday Jul 08, 2019 19:28:10 MDT
 *  @bug No Known Bugs
 */

#ifndef LOADER_HPP

#define LOADER_HPP

// C++ includes
#include <dlfcn.h>
#include <string>
#include <iostream>

// Project includes

namespace networking {
namespace loader {

/* THIS is NOT an accurrate / useful and it will need to be fixed up, but for
 * now, all it does is load the class, assume there's an entry point and assume
 * there's an exit point
 */
template <class T>
class Loader {
 public:
  Loader(const std::string& library, const std::string& destroy,
         const std::string& load);
  ~Loader();

  // NOTE we can't use smart pointers here, we need to provide all
  // destructors and const ourselves
  T* get_object();

 private:
  void* handle;

  typedef T* (*create_t)();
  typedef void (*destroy_t)();

  create_t create_m;
  destroy_t destroy_m;
  T* object;
};

}  // namespace loader
}  // namespace networking
#endif /* end of include guard LOADER_HPP */
