/**
 *  @file Curlloader
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Sunday Jul 07, 2019 17:45:00 MDT
 *  @bug No Known Bugs
 */

#ifndef CURLLOADER_HPP

#define CURLLOADER_HPP

//C++ includes
#include <stdio.h>
#include <curl/curl.h>
#include <string>
#include <iostream>

//Project includes

namespace networking
{
namespace curl
{

typedef struct 
{
	const char* filename;
	FILE* stream;
} FtpFile;

class Curlloader
{
    public:
		Curlloader ();
        Curlloader (const std::string& destination_url);
		Curlloader (const std::string& destination_url, const std::string& output_path);
        virtual ~Curlloader ();

		void set_output_path(const std::string& output_path);
		void set_destination_url(const std::string& url);
		
		void grab(const std::string& file_name);
		
	private:
		void curl_start();
		static size_t file_write(void* buffer, size_t size, size_t nmemb, void* stream);
		void curl_terminate();
		
		std::string destination;
		FtpFile* ftpfile;
		CURL* curl;
		CURLcode res;
};

}
}
#endif /* end of include guard CURLLOADER_HPP */

