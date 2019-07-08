/**
 *  @file InterfaceTemplate
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Saturday Jul 06, 2019 12:21:47 MDT
 *  @bug No Known Bugs
 */

#ifndef INTERFACETEMPLATE_HPP

#define INTERFACETEMPLATE_HPP

//C++ includes

//Project includes



namespace networking
{
namespace client
{

class InterfaceTemplate
{
    public:
        InterfaceTemplate ();
        virtual ~InterfaceTemplate ();

		virtual bool initialize();
		virtual bool terminate();

		virtual bool connect(std::string server_address, void*);
		virtual bool disconnect(std::string server_address);

		virtual MessageInterface request(std::string& server, MessageInterface* message);
    private:
        /* private data */
};

}
}

#endif /* end of include guard INTERFACETEMPLATE_HPP */

