/**
 *  @file Curlloader
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Sunday Jul 07, 2019 17:59:19 MDT
 *  @bug No Known Bugs
 */

#include "networking/curl/Curlloader.hpp"

using namespace networking::curl;

Curlloader::Curlloader() {
  destination = "";
  ftpfile = nullptr;
  curl = nullptr;
}

Curlloader::Curlloader(const std::string& destination_url) {
  destination = destination_url;
  ftpfile = nullptr;
  curl = nullptr;
}

Curlloader::Curlloader(const std::string& destination_url,
                       const std::string& output_path) {
  destination = destination_url;
  ftpfile = new FtpFile;
  ftpfile->filename = destination_url.c_str();
  ftpfile->stream = nullptr;
  curl = nullptr;
}

Curlloader::~Curlloader() {
  ftpfile = nullptr;
  curl = nullptr;
}

void Curlloader::set_output_path(const std::string& output_path) {
  if (!ftpfile) ftpfile = new FtpFile;
  ftpfile->filename = output_path.c_str();
  ftpfile->stream = nullptr;
}

void Curlloader::set_destination_url(const std::string& url) {
  destination = url;
}

void Curlloader::curl_start() {
  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();
}

void Curlloader::curl_terminate() {
  if (ftpfile->stream) fclose(ftpfile->stream);

  curl_global_cleanup();
}

void Curlloader::grab(const std::string& file_name) {
  if (destination.empty()) {
    std::cout << "No destination!" << std::endl;
    return;
  }
  if (file_name.empty()) {
    std::cout << "Empty string" << std::endl;
    return;
  }
  std::string full_path = destination + file_name;
  curl_start();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, full_path.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, file_write);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, ftpfile);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (CURLE_OK != res) std::cout << "Curl error: " << res << std::endl;
  }
  curl_terminate();
}

size_t Curlloader::file_write(void* buffer, size_t size, size_t nmemb,
                              void* stream) {
  FtpFile* out = (FtpFile*)stream;
  if (!out->stream) {
    out->stream = fopen(out->filename, "wb");
    if (!out->stream) return -1;
  }
  return fwrite(buffer, size, nmemb, out->stream);
}
