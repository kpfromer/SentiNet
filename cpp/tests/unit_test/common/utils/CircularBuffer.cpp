/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "TestUtils.hpp"
#include "common/utils/CircularBuffer.hpp"

template <typename T>
void ValidateCircularBufferNumeric(uint16_t size) {
  // Check the default parameters
  utils::CircularBuffer<T> buff(size);
  EXPECT_EQ(0U, buff.size());
  EXPECT_EQ(size, buff.capacity());
  EXPECT_EQ(&buff.cget_head(), &buff.get_head());
  EXPECT_EQ(&buff.cget_tail(), &buff.get_tail());
  EXPECT_EQ(&buff.cget_head(), &buff.get_tail());
  EXPECT_EQ(&buff.cget_tail(), &buff.get_head());

  // Set how many additional data objects we'll insert into the buffer
  T extra = 7U;

  // Insert data and move the head
  EXPECT_TRUE(buff.is_empty());
  EXPECT_FALSE(buff.is_full());
  for(T i = 0U; i < buff.capacity() + extra; ++i) {
    // Insert data and check that it's loaded correctly
    buff.get_head() = i;
    EXPECT_EQ(i, buff.get_head());
    EXPECT_EQ(i, buff.cget_head());
    ASSERT_TRUE(buff.move_head());
    // Check the size and full/empty
    EXPECT_FALSE(buff.is_empty());
    if(i + 1U >= buff.capacity()) {
      EXPECT_EQ(buff.capacity(), buff.size());
      EXPECT_TRUE(buff.is_full());
    } else {
      EXPECT_EQ(i + 1U, buff.size());
      EXPECT_FALSE(buff.is_full());
    }
  }

  // Clear out the data
  buff.clear();
  EXPECT_EQ(0U, buff.size());
  EXPECT_EQ(size, buff.capacity());
  EXPECT_EQ(&buff.cget_head(), &buff.get_head());
  EXPECT_EQ(&buff.cget_tail(), &buff.get_tail());
  EXPECT_EQ(&buff.cget_head(), &buff.get_tail());
  EXPECT_EQ(&buff.cget_tail(), &buff.get_head());

  // Insert data and move the head
  EXPECT_TRUE(buff.is_empty());
  EXPECT_FALSE(buff.is_full());
  for(T i = 0U; i < buff.capacity() + extra; ++i) {
    // Insert data and check that it's loaded correctly
    buff.get_head() = i;
    EXPECT_EQ(i, buff.get_head());
    EXPECT_EQ(i, buff.cget_head());
    ASSERT_TRUE(buff.move_head());
    // Check the size and full/empty
    EXPECT_FALSE(buff.is_empty());
    if(i + 1 >= buff.capacity()) {
      EXPECT_EQ(buff.capacity(), buff.size());
      EXPECT_TRUE(buff.is_full());
    } else {
      EXPECT_EQ(i + 1U, buff.size());
      EXPECT_FALSE(buff.is_full());
    }
  }

  // Read the data and move the tail
  EXPECT_FALSE(buff.is_empty());
  EXPECT_TRUE(buff.is_full());
  for(T i = 0U; i < buff.capacity(); ++i) {
    // Insert data and check that it's loaded correctly
    EXPECT_EQ(i + extra, buff.get_tail());
    EXPECT_EQ(i + extra, buff.cget_tail());
    ASSERT_TRUE(buff.move_tail());
    // Check the size and full/empty
    EXPECT_FALSE(buff.is_full());
    EXPECT_EQ(buff.capacity(), size);
    if(i + 1U >= buff.capacity()) {
      EXPECT_EQ(0U, buff.size());
      EXPECT_TRUE(buff.is_empty());
    } else {
      EXPECT_EQ(buff.capacity() - (i + 1U), buff.size());
      EXPECT_FALSE(buff.is_empty());
    }
  }

  // You can move the tail when it's empty, but the size stays the same and nothing really happens
  EXPECT_TRUE(buff.move_tail());
  EXPECT_TRUE(buff.is_empty());
  EXPECT_TRUE(buff.move_tail());
  EXPECT_TRUE(buff.is_empty());
  EXPECT_TRUE(buff.move_tail());
  EXPECT_TRUE(buff.is_empty());

  // Check that we can resize the buffer
  EXPECT_EQ(10U, buff.resize(10U));
  EXPECT_EQ(0U, buff.size());
  EXPECT_EQ(10U, buff.capacity());
  EXPECT_EQ(&buff.cget_head(), &buff.get_head());
  EXPECT_EQ(&buff.cget_tail(), &buff.get_tail());
  EXPECT_EQ(&buff.cget_head(), &buff.get_tail());
  EXPECT_EQ(&buff.cget_tail(), &buff.get_head());
}

template <typename T>
class MyStruct {
 public:
  T val;
};

template <typename T>
void ValidateCircularBufferUniquePtr(uint16_t size) {
  // Check the default parameters
  utils::CircularBuffer<std::unique_ptr<MyStruct<T>>> buff(size);
  EXPECT_EQ(0U, buff.size());
  EXPECT_EQ(size, buff.capacity());
  EXPECT_EQ(&buff.cget_head(), &buff.get_head());
  EXPECT_EQ(&buff.cget_tail(), &buff.get_tail());
  EXPECT_EQ(&buff.cget_head(), &buff.get_tail());
  EXPECT_EQ(&buff.cget_tail(), &buff.get_head());

  // Set how many additional data objects we'll insert into the buffer
  T extra = 7U;

  // Insert data and move the head
  EXPECT_TRUE(buff.is_empty());
  EXPECT_FALSE(buff.is_full());
  for(T i = 0U; i < buff.capacity() + extra; ++i) {
    // Insert data and check that it's loaded correctly
    buff.get_head() = std::make_unique<MyStruct<T>>();
    buff.get_head()->val = i;
    EXPECT_EQ(i, buff.get_head()->val);
    EXPECT_EQ(i, buff.cget_head()->val);
    ASSERT_TRUE(buff.move_head());
    // Check the size and full/empty
    EXPECT_FALSE(buff.is_empty());
    if(i + 1U >= buff.capacity()) {
      EXPECT_EQ(buff.capacity(), buff.size());
      EXPECT_TRUE(buff.is_full());
    } else {
      EXPECT_EQ(i + 1U, buff.size());
      EXPECT_FALSE(buff.is_full());
    }
  }

  // Clear out the data
  buff.clear();
  EXPECT_EQ(0U, buff.size());
  EXPECT_EQ(size, buff.capacity());
  EXPECT_EQ(&buff.cget_head(), &buff.get_head());
  EXPECT_EQ(&buff.cget_tail(), &buff.get_tail());
  EXPECT_EQ(&buff.cget_head(), &buff.get_tail());
  EXPECT_EQ(&buff.cget_tail(), &buff.get_head());

  // Insert data and move the head
  EXPECT_TRUE(buff.is_empty());
  EXPECT_FALSE(buff.is_full());
  for(T i = 0U; i < buff.capacity() + extra; ++i) {
    // Insert data and check that it's loaded correctly
    buff.get_head() = std::make_unique<MyStruct<T>>();
    buff.get_head()->val = i;
    EXPECT_EQ(i, buff.get_head()->val);
    EXPECT_EQ(i, buff.cget_head()->val);
    ASSERT_TRUE(buff.move_head());
    // Check the size and full/empty
    EXPECT_FALSE(buff.is_empty());
    if(i + 1U >= buff.capacity()) {
      EXPECT_EQ(buff.capacity(), buff.size());
      EXPECT_TRUE(buff.is_full());
    } else {
      EXPECT_EQ(i + 1U, buff.size());
      EXPECT_FALSE(buff.is_full());
    }
  }

  // Read the data and move the tail
  EXPECT_FALSE(buff.is_empty());
  EXPECT_TRUE(buff.is_full());
  for(T i = 0U; i < buff.capacity(); ++i) {
    // Insert data and check that it's loaded correctly
    EXPECT_EQ(i + extra, buff.get_tail()->val);
    EXPECT_EQ(i + extra, buff.cget_tail()->val);
    ASSERT_TRUE(buff.move_tail());
    // Check the size and full/empty
    EXPECT_FALSE(buff.is_full());
    EXPECT_EQ(buff.capacity(), size);
    if(i + 1U >= buff.capacity()) {
      EXPECT_EQ(0U, buff.size());
      EXPECT_TRUE(buff.is_empty());
    } else {
      EXPECT_EQ(buff.capacity() - (i + 1U), buff.size());
      EXPECT_FALSE(buff.is_empty());
    }
  }

  // You can move the tail when it's empty, but the size stays the same and nothing really happens
  EXPECT_TRUE(buff.move_tail());
  EXPECT_TRUE(buff.is_empty());
  EXPECT_TRUE(buff.move_tail());
  EXPECT_TRUE(buff.is_empty());
  EXPECT_TRUE(buff.move_tail());
  EXPECT_TRUE(buff.is_empty());

  // Check that we can resize the buffer
  EXPECT_EQ(10U, buff.resize(10U));
  EXPECT_EQ(0U, buff.size());
  EXPECT_EQ(10U, buff.capacity());
  EXPECT_EQ(&buff.cget_head(), &buff.get_head());
  EXPECT_EQ(&buff.cget_tail(), &buff.get_tail());
  EXPECT_EQ(&buff.cget_head(), &buff.get_tail());
  EXPECT_EQ(&buff.cget_tail(), &buff.get_head());
}

/** Tests that the Circular Buffer class is able to work properly when primitive numeric types are used
 */
TEST(UtilTests, CircularBufferNumeric) {
  // Validate the buffer using all the primitive numerical types
  for(const auto size :
      std::initializer_list<uint16_t>({utils::CircularBufferDefault::SIZE, 1U, 1000U, 300U, 570U, 5000U})) {
    ValidateCircularBufferNumeric<int16_t>(size);
    ValidateCircularBufferNumeric<int32_t>(size);
    ValidateCircularBufferNumeric<int64_t>(size);
    ValidateCircularBufferNumeric<uint16_t>(size);
    ValidateCircularBufferNumeric<uint32_t>(size);
    ValidateCircularBufferNumeric<uint64_t>(size);
    ValidateCircularBufferNumeric<float>(size);
    ValidateCircularBufferNumeric<double>(size);
  }
  // Validate the buffer using all the primitive numerical types
  // @todo [KEMB-628] Due to the unit test setup, we can't use large number with these primitive type.
  for(const auto size : std::initializer_list<uint16_t>({1U, 60U, 30U, 57U, 40U})) {
    ValidateCircularBufferNumeric<unsigned char>(size);
    ValidateCircularBufferNumeric<char>(size);
    ValidateCircularBufferNumeric<int8_t>(size);
    ValidateCircularBufferNumeric<uint8_t>(size);
  }
}

/** Tests that the Circular Buffer class is able to work properly when primitive numeric types are used
 */
TEST(UtilTests, CircularBufferUniquePtr) {
  // Validate the buffer using all the primitive numerical types
  for(const auto size : std::initializer_list<uint16_t>({utils::CircularBufferDefault::SIZE, 1U, 100U, 30U, 57U})) {
    ValidateCircularBufferUniquePtr<char>(size);
    ValidateCircularBufferUniquePtr<unsigned char>(size);
    ValidateCircularBufferUniquePtr<int8_t>(size);
    ValidateCircularBufferUniquePtr<int16_t>(size);
    ValidateCircularBufferUniquePtr<int32_t>(size);
    ValidateCircularBufferUniquePtr<int64_t>(size);
    ValidateCircularBufferUniquePtr<uint8_t>(size);
    ValidateCircularBufferUniquePtr<uint16_t>(size);
    ValidateCircularBufferUniquePtr<uint32_t>(size);
    ValidateCircularBufferUniquePtr<uint64_t>(size);
    ValidateCircularBufferUniquePtr<float>(size);
    ValidateCircularBufferUniquePtr<double>(size);
  }
}
