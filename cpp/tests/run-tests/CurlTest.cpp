/**
 *  @file CurlTest
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Sunday Jul 07, 2019 18:37:54 MDT
 *  @bug No Known Bugs
 */

#include "networking/curl/Curlloader.hpp"
using namespace networking::curl;

int main(int argc, char* argv[]) {
  std::string value = (argc > 1 ? argv[1] : "./hello");
  Curlloader* a = new Curlloader("ftp://127.0.0.1/");
  a->set_output_path(value);
  a->grab("hello");
  return 0;
}
