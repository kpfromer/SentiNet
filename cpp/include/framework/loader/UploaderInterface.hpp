/**
 * @author      : theo (theo@$theo-lenovo-yoga-Arch)
 * @file        : UploaderInterface
 * @created     : Saturday Aug 10, 2019 20:38:17 MDT
 */

#ifndef UPLOADERINTERFACE_HPP

#define UPLOADERINTERFACE_HPP

// C++ includes
#include <string>

// Project inludes



class UploaderInterface
{
    public:
        ~UploaderInterface (){};
  
        // Recievers
        virtual bool grab_file(const std::string& url, const std::string& filename) = 0;
        virtual bool ftp_get(const std::string& url, const std::string& filename) = 0;
        virtual std::string http_get(const std::string& url, const std::string& request) = 0;
        virtual std::string https_get(const std::string& url, const std::string& request) = 0;


        // Posters
        virtual bool post_file(const std::string& url, const std::string& filename) = 0;
        virtual bool ftp_post(const std::string& url, const std::string& filename) = 0;
        virtual std::string http_post(const std::string& url, const std::string& filename) = 0;
};

#endif /* end of include guard UPLOADERINTERFACE_HPP */

