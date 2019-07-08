/**
 *  @file ActionObjective
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Tuesday Jul 02, 2019 20:14:29 MDT
 *  @bug No Known Bugs
 */

#include "kernelOPS/core/ActionObjective.hpp"
#include <cstdlib>

using namespace obps::core;

ActionObjective::ActionObjective(const int size_) : buffer_size(size_) {
  data = (void*)malloc(size_);
}

ActionObjective::~ActionObjective() { free(data); }
