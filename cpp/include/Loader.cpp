/**
 *  @file Loader
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Monday Jul 08, 2019 19:49:49 MDT
 *  @bug No Known Bugs
 */

#include "networking/loader/Loader.hpp"
#include <iostream>
#include <cstdlib.h>

namespace networking {
namespace loader {

template <class T>
Loader<T>::Loader(const std::string &library, const std::string &destroy_symbol,
                  const std::string &load_symbol) {
  handle = dlopen(library.c_str(), RTLD_LAZY);
  if (!handle) {
    std::cerr << "Cannot load library: " << dlerror() << std::endl;
    exit(1);
  }
  // static cast instead of dynamic because we know this'll be a void ptr
  create_m = (create_t)(dlsym(handle, load_symbol.c_str()));
  const char *dlsym_error = dlerror();
  if (dlsym_error) {
    std::cerr << "Cannot load symbol create: " << dlsym_error << std::endl;
    exit(1);
  }

  destroy_m = (destroy_t)(dlsym(handle, destroy_symbol.c_str()));
  dlsym_error = dlerror();
  if (dlsym_error) {
    std::cerr << "Cannot load symbol destroy: " << dlsym_error << std::endl;
    exit(1);
  }

  object = (*create_m)();
}

template <class T> Loader<T>::~Loader() {
  if (object)
    (*destroy_m)(object);
  if (handle)
    dlclose(handle);
}

template <class T> T *Loader<T>::get_object() { return object; }

} // namespace loader
} // namespace networking
