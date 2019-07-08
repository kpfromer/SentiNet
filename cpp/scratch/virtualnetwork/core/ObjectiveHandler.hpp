/**
 *  @file ObjectiveHandler
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Tuesday Jul 02, 2019 18:45:17 MDT
 *  @bug No Known Bugs
 */

#ifndef OBJECTIVEHANDLER_HPP

#define OBJECTIVEHANDLER_HPP

// C++ includes
#include <memory>
#include <unordered_map>

// Project includes
#include "kernelOPS/core/ActionObjective.hpp"

namespace obps {
namespace core {

class ObjectiveHandler {
 public:
  ObjectiveHandler();

  ~ObjectiveHandler();
  std::shared_ptr<ActionObjective> get_objective(std::string);
  void add_objective(ActionObjective*, std::string);

 private:
  std::unordered_map<std::string, std::shared_ptr<ActionObjective>> objectives;

  /* private data */
};

}  // namespace core
}  // namespace obps

#endif /* end of include guard OBJECTIVEHANDLER_HPP */
