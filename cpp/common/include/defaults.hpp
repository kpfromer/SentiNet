
#include <string>
#include <iostream>

namespace utils
{
namespace defaults
{
namespace networking
{
  const std::string default_bind("tcp://*");
  const std::string default_localhost("tcp://localhost");
}

namespace callbacks
{
	auto default_callback = [](void* context, int size) -> std::string
	{
		std::cout<<"WARNING using default  callback function, you haven't  initialized your callback"<<std::endl;
		char* value = (char*)malloc(size);
		memcpy(value, context, size);
		value[size] = '\0';
		std::string val = std::string(value);
		val += " response";
		free(value);
		return val;
	};
}
}
}
