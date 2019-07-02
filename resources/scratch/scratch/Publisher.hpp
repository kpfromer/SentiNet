/**
 *  @file Publisher
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Sunday Jun 30, 2019 15:10:50 MDT
 *  @bug No Known Bugs
 */

#ifndef PUBLISHER_HPP

#define PUBLISHER_HPP

//C++ includes

//Project includes


namespace grl
{
namespace processes
{

class PublisherBase
{
	public:
		virtual ~PublisherBase() = default;
		std::string get_name() const;

	private:
		const std::string name;
}

template <typename message>
class Publisher : public PublisherBase
{
    public:
        Publisher ();
        virtual ~Publisher ();

	public:
		void publish(message, ActionProcessBase*);
		void publish(message, const std::string&);

    private:
		std::shared_ptr<ObjectiveHandler> handler;
};

}	//namespace processes
}	//namespace grl

#endif /* end of include guard PUBLISHER_HPP */

