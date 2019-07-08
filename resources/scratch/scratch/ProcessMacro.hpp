/**
 *  @file ProcessMacro
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Saturday Jun 29, 2019 15:35:32 MDT
 *  @bug No Known Bugs
 */

#ifndef PROCESSMACRO_HPP

#define PROCESSMACRO_HPP

// C++ includes

// Project includes

namespace grl {
namespace processes {

/**
 * @brief
 */
class ProcessMacro {
 public:
  ProcessMacro();
  virtual ~ProcessMacro();

  // adders
 public:
  /**
   * @brief
   *
   * @param priority
   * @param std::string
   *
   * @return
   */
  virtual bool add_action_process(PROCESS_PRIORITY priority,
                                  const std::string&);
  /**
   * @brief
   *
   * @param pid
   *
   * @return
   */
  virtual bool delete_action_process(PROCESS_ID pid);
  /**
   * @brief
   *
   * @param parent
   * @param child
   *
   * @return
   */
  virtual bool add_action_objective(PROCESS_ID parent, PROCESS_ID child);
  /**
   * @brief
   *
   * @param parent
   * @param aid
   *
   * @return
   */
  virtual bool remove_action_objective(PROCESS_ID parent, ACTION_ID aid);

  // routers
 public:
  /**
   * @brief
   *
   * @param parent
   * @param child
   * @param parent2
   * @param child2
   *
   * @return
   */
  virtual bool route_action_objective(PROCESS_ID parent, PROCESS_ID child,
                                      PROCESS_ID parent2, PROCESS_ID child2);

 private:
  verbosity verbosity_level;
  std::vector<ActionObjectiveBase> action_objectives;
  std::vector<ActionProcessBase> action_processes;
  /* private data */
};

}  // namespace processes
}  // namespace grl

#endif /* end of include guard PROCESSMACRO_HPP */
