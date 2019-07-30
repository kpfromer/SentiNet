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

// C++ includes
#include <iostream>
#include <string>

// Project includes

class MessageInterface {
public:
  virtual ~MessageInterface() = default;

  friend std::ostream &operator<<(std::ostream &, const MessageInterface &);
  friend std::istream &operator>>(std::istream &, MessageInterface &);

  operator std::string() {
    std::string value;
    serialize_to_string(&value);
    return value;
  }

private:
  virtual bool serialize_to_string(std::string *output_string) const = 0;
  virtual bool parse_from_string(const std::string &value) = 0;
  virtual bool serialize_to_ostream(std::ostream *output) const = 0;
  virtual bool parse_from_istream(std::istream *input) = 0;
};

std::ostream &operator<<(std::ostream &output,
                         const MessageInterface &message) {
  message.serialize_to_ostream(&output);
  return output;
}

std::istream &operator>>(std::istream &input, MessageInterface &message) {
  message.parse_from_istream(&input);
  return input;
}

#endif // MESSAGEINTERFACE_HPP
