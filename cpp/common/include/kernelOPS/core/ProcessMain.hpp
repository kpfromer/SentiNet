/**
 *  @file ProcessMain
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Tuesday Jul 02, 2019 21:07:32 MDT
 *  @bug No Known Bugs
 */

#ifndef PROCESSMAIN_HPP

#define PROCESSMAIN_HPP

//C++ includes
#include <memory>

//Project includes
#include "kernelOPS/core/TriggerHandler.hpp"
#include "kernelOPS/core/ObjectiveHandler.hpp"


namespace obps
{
namespace core
{

class ProcessMain
{
    public:
		std::shared_ptr<TriggerHandler> th;
		std::shared_ptr<ObjectiveHandler> oh;
};

}
}

#endif /* end of include guard PROCESSMAIN_HPP */

