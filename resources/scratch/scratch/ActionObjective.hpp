/**
 *  @file ActionObjective
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Saturday Jun 29, 2019 15:03:33 MDT
 *  @bug No Known Bugs
 */

#ifndef ACTION_OBJECTIVE_HPP

#define ACTION_OBJECTIVE_HPP

// C++ includes
#include <memory>
#include <iostream>
#include <functional>

// Project includes

namespace grl {
namespace processes {

/**
 * @brief A Class to contain the templated class ActionObjective
 *
 * This class exists a) to only contain pure virtual functions
 * and b) to allow ActionProcess to store arbitrary Action Objectives
 * regardless of type
 */
class ActionObjectiveBase {
 public:
  virtual ~ActionObjectiveBase() = default;
  ACTION_ID get_aid() const;

 private:
  const ACTION_ID aid;
};
/**
 *	@brief A class to read and write to an input and ouput
 *
 *	An action objective allows Action processes to read and write to
 *	an arbitrary read and write location
 */
template <typename type>
class ActionObjective : public ActionObjectiveBase {
 public:
  ActionObjective(std::shared_ptr<type> input_address, const ACTION_ID& aid);
  ActionObjective(void* data_address);  //!< c declaration

  virtual ~ActionObjective();

 protected:
  input get_data();
  void put_data(type);

 private:
  // the data
  type information;
};
}  // namespace processes
}  // namespace grl

extern "C" {
grl::processes::ActionObjectiveBase load_action_objective(void*, void*, void*);
}

#endif /* end of include guard ACTION_OBJECTIVE_HPP */
