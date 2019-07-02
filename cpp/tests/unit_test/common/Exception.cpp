/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "TestUtils.hpp"
#include "common/Exceptions.hpp"

/** Tests that the Exception base class is able to properly throw exceptions.
 */
TEST(ExceptionTests, Throw) {
  // Check the throw type
  EXPECT_THROW(throw ExceptionInterface(), ExceptionInterface);
  EXPECT_THROW(throw RuntimeException("hello: %d\n", 1234), RuntimeException);
  EXPECT_THROW(THROW_RUNTIME_EXCEPTION("Testing out %s", "with parameters"), RuntimeException);
  EXPECT_THROW(throw InvalidArgument("hello: %d\n", 1234), InvalidArgument);
  EXPECT_THROW(THROW_INVALID_ARGUMENT("Testing out %s", "with parameters"), InvalidArgument);

  // Check the string error
  std::string error = "This is an example exception throw";
  try {
    throw RuntimeException(error.c_str());
  } catch(const RuntimeException& e) {
    EXPECT_EQ(error, e.what());
  }

  // Check the max error string size (no buffer overflow)
  std::string too_big = std::string(ExceptionDefault::MAX_ERROR_MESSAGE_SIZE + 1, 'T');
  try {
    throw RuntimeException(too_big.c_str());
  } catch(const RuntimeException& e) {
    EXPECT_EQ(too_big.size(), std::string(e.what()).size());
  }

  too_big = std::string(too_big.size() + 1, 'S');
  try {
    throw InvalidArgument(too_big.c_str());
  } catch(const InvalidArgument& e) {
    EXPECT_EQ(too_big.size(), std::string(e.what()).size());
  }
}
