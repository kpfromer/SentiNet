/**
 *  @file MessageInterface
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Saturday Jul 06, 2019 11:22:33 MDT
 *  @bug No Known Bugs
 */

#ifndef MESSAGEINTERFACE_HPP

#define MESSAGEINTERFACE_HPP

//C++ includes

//Project includes


class MessageInterface
{
    public:
        MessageInterface(std::string value)
		{
			this->message = value;
		}

		virtual ~MessageInterface (); 


		/*
		 * The main methods
		 */
		virtual bool SerializeToString(std::string* output_string) = 0;
		virtual bool ParseFromString(const std::string& value) = 0;
		virtual bool SerializeToOstream(std::ostream* output) = 0;
		virtual bool ParseFromIstream(std::istream* input) = 0;

		operator std::string()
		{
			std::string output;
			SerializeToString(&output);
			return output;
		}

		operator std::string*()
		{
			std::string* output = new std::string;
			SerializeToString(output);
			return output;
		}
    private:
		std::string message;
};


std::ostream& operator<<(std::ostream &output, const MessageInterface& message)
{
	message->SerializeToOstream(output);
	return output;
}


#endif /* end of include guard MESSAGEINTERFACE_HPP */

