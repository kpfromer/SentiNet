/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : ClientInterface
 * @created     : Thursday Aug 15, 2019 18:49:35 MDT
 */

#ifndef CLIENTINTERFACE_HPP

#define CLIENTINTERFACE_HPP

// C++ Includes

// Local Includes
#include "framework/utils/utils.hpp"

class ClientInterface
{
  public:
        ClientInterface ();
        virtual ~ClientInterface ();

    protected:
        /**
         * API Guide, these are the functions we will impliment
         */
        virtual bool upload_file(const std::string& url, const std::string& local_file_path) = 0;
        virtual std::string request_file(const std::string& url) = 0;

    private:
        virtual bool trash_file(const std::string& file) {
          auto it = std::find(files.begin(), files.end(), 16);
          if(it == files.end()) {
            LOG_WARN("File Doesn't exist");
            return true;
          }
          files.erase(it);
          return ::utils::fs::rmrf(file);
        }

        virtual std::string request_file(const std::string& url_base, const std::string& file_endpoint){
          if(!url_base.size() || !file_endpoint.size()){
            LOG_ERROR("Invalid file endpoint");
            return "";
          }
          return request_file(::utils::fs::file_append(url_base, file_endpoint));
        }


    public:
        virtual bool upload(const std::string& url_base, const std::string& file_path) {
          return upload_file(url_base, file_path);
        }

        virtual std::string request(const std::string& url_base, const std::string& endpoint) {
          std::string name = request_file(url_base, endpoint);
          if(name == "")
            return "";
          files.push_back(endpoint);
          return name;
        }

    private:
        std::list<std::string> files;


};

#endif /* end of include guard CLIENTINTERFACE_HPP */

