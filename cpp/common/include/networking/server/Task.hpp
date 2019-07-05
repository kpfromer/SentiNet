/**
 *  @file Task
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Thursday Jul 04, 2019 20:35:49 MDT
 *  @bug No Known Bugs
 */

#ifndef TASK_HPP

#define TASK_HPP

//C++ includes

//Project includes
#include "networking/server/ServerInterface.hpp"

namespace networking
{
namespace server
{

class Task
{
    public:
        Task (ServerInterface* server);
        virtual ~Task ();
    private:
        /* private data */
};

}
}

#endif /* end of include guard TASK_HPP */

