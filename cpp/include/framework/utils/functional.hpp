/**
 *  @file functional
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@varnsen)
 *  @created      Monday Jul 29, 2019 18:47:53 MDT
 *  @bug No Known Bugs
 */

#ifndef FUNCTIONAL_HPP

#define FUNCTIONAL_HPP

namespace util {
namespace functional {

std::vector<T> generate(int itterations, void (*cb)(int), int &start = 1) {
  vector<int> sequence;
  for (int i = start; i < start + itterations; i++) {
    sequence.push_back((*cb)(i));
  }
  return sequence;
}
} // namespace functional
} // namespace util

#endif /* end of include guard FUNCTIONAL_HPP */
