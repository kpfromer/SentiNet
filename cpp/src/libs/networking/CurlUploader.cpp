/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : CurlUploader
 * @created     : Saturday Aug 10, 2019 20:55:14 MDT
 */

// C++ Includes
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <iostream>

// Local Includes
#include "networking/curl/CurlUploader.hpp"
#include "framework/utils/strings.hpp"



bool CurlUploader::grab_file(const std::string &url,
                             const std::string &filename) {
  return ftp_get(url, filename);
}

struct FtpFile {
  const char *filename;
  FILE *stream;
};

static size_t curl_fwrite(void *buffer, size_t size, size_t nmemb,
                          void *stream) {
  FtpFile *out = (struct FtpFile *)stream;
  if (!out->stream) {
    /* open file for writing */
    out->stream = fopen(out->filename, "wb");
    if (!out->stream)
      return -1; /* failure, can't open file to write */
  }
  return fwrite(buffer, size, nmemb, out->stream);
}

bool CurlUploader::ftp_get(const std::string &url,
                           const std::string &filename) {
  CURL *curl;
  CURLcode ret;
  FtpFile temp = {filename.c_str(), NULL};
  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_fwrite);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &temp);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    ret = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
  }
  bool val = CURLE_OK == ret;
  if (temp.stream)
    fclose(temp.stream);
  curl_global_cleanup();
  return val;
}

std::string CurlUploader::http_get(const std::string &url,
                                   const std::string &request) {
  CURL *curl;
  CURLcode ret;
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_fwrite);
    ret = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
  } else {
    return "";
  }
  return "";
}

std::string CurlUploader::https_get(const std::string &url,
                                    const std::string &request) {
  CURL *curl;
  // CURLcode ret;
  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
#ifdef SKIP_PEER_VERIFICATION
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif
#ifdef SKIP_HOSTNAME_VERIFICATION
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif
    // ret = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  // bool result = ret == CURLE_OK;
  return "";
}

static size_t read_callback(void *ptr, size_t size, size_t nmemb,
                            FILE *stream) {
  curl_off_t nread;
  /* in real-world cases, this would probably get this data differently
     as this fread() stuff is exactly what the library already would do
     by default internally */
  size_t retcode = fread(ptr, size, nmemb, stream);

  nread = (curl_off_t)retcode;

  fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T " bytes from file\n",
          nread);
  return retcode;
}

bool CurlUploader::post_file(const std::string &url,
                             const std::string &filename) {
  std::string file_end = utils::strings::parse(filename, '/').back();
<<<<<<< HEAD:cpp/src/libs/networking/CurlUploader.cpp
    CURL *curl;
    CURLcode ret;
    FILE *hd_src;
    struct stat file_info;
    curl_off_t fsize;
    struct curl_slist *headerlist = NULL;
    static const std::string buf_1_str = ("RNFR " + file_end).c_str();
    static const std::string buf_2_str = ("RNTO " + file_end).c_str();
    if(stat(filename.c_str(), &file_info)) {
      return false;
    }
    fsize = (curl_off_t)file_info.st_size;
    hd_src = fopen(filename.c_str(), "rb");
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(curl) {
      headerlist = curl_slist_append(headerlist, buf_1_str.c_str());
      headerlist = curl_slist_append(headerlist, buf_2_str.c_str());

      curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
      curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
      curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);
      curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);
      curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)fsize);
      ret = curl_easy_perform(curl);
      if(ret == CURLE_OK)
        std::cout<<"YAY"<<std::endl;
    } else {
      return false;
    }
    curl_slist_free_all(headerlist);
    curl_easy_cleanup(curl);
    fclose(hd_src);
    curl_global_cleanup();
    return ret == CURLE_OK;
}
bool CurlUploader::ftp_post(const std::string &url, const std::string &filename) {
  return post_file(url, filename);
=======
  CURL *curl;
  CURLcode ret;
  FILE *hd_src;
  struct stat file_info;
  curl_off_t fsize;
  struct curl_slist *headerlist = NULL;
  static const std::string buf_1_str = ("RNFR " + file_end).c_str();
  static const std::string buf_2_str = ("RNTO " + file_end).c_str();
  if (stat(filename.c_str(), &file_info)) {
    return false;
  }
  fsize = (curl_off_t)file_info.st_size;
  hd_src = fopen(filename.c_str(), "rb");
  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  if (curl) {
    headerlist = curl_slist_append(headerlist, buf_1_str.c_str());
    headerlist = curl_slist_append(headerlist, buf_2_str.c_str());

    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);
    curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);
    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)fsize);
    ret = curl_easy_perform(curl);
  } else {
    return false;
  }
  curl_slist_free_all(headerlist);
  curl_easy_cleanup(curl);
  fclose(hd_src);
  curl_global_cleanup();
  return ret == CURLE_OK;
}
bool CurlUploader::ftp_post(const std::string &url,
                            const std::string &filename) {
  post_file(url, filename);
>>>>>>> 9a05635c30a16fba78975c5381be4c1c1df4b666:cpp/src/libs/uploader/CurlUploader.cpp
}

std::string CurlUploader::http_post(const std::string &url,
                                    const std::string &post_fields) {
  CURL *curl;
  // CURLcode ret;

  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields.c_str());
    // ret = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
  } else {
    return "";
  }
  return "";
}
<<<<<<< HEAD:cpp/src/libs/networking/CurlUploader.cpp
=======
/*
bool smtp_send(const std::string &from_address, const std::string& to_addr,
const std::string& cc_addr, const std::string &content) { time_t now = time(0);
  char* dt = ctime(&now);


  const char *payload_text[] = { "
    "Date: ", to_addr.c_str() };

}
bool smtp_ssl_send(const std::string &address, const std::string &content);
*/
>>>>>>> 9a05635c30a16fba78975c5381be4c1c1df4b666:cpp/src/libs/uploader/CurlUploader.cpp
