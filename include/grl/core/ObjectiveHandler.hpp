/**
 *  @file ObjectiveHandler
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Monday Jul 01, 2019 20:15:39 MDT
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

class ObjectiveHandlerBase
{
	public:
		virtual ~ObjectiveHandlerBase() = default;
		
		virtual void* read_data(std::string hash);
		virtual void write_data(void*, t);

}

class ObjectiveHandler
{
    public:
        ObjectiveHandler ();
        virtual ~ObjectiveHandler ();
    private:
        /* private data */
};

}
}

#endif /* end of include guard OBJECTIVEHANDLER_HPP */

