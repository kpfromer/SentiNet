/**
 *  @file ObjectiveHandler
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Sunday Jun 30, 2019 14:52:10 MDT
 *  @bug No Known Bugs
 */

#ifndef OBJECTIVEHANDLER_HPP

#define OBJECTIVEHANDLER_HPP

//C++ includes

//Project includes


namespace grl
{
namespace processes
{

class ObjectiveHandler
{
    public:
        ObjectiveHandler ();
        virtual ~ObjectiveHandler ();
		
		std::unique_ptr<ActionObjectiveBase> get_action_objective(const std::string&);

    private:
		std::vector<std::unique_ptr<ActionObjectiveBase>> action_objectives;
		
};

} 	//namespace processes
}	//namespcae grl

#endif /* end of include guard OBJECTIVEHANDLER_HPP */

