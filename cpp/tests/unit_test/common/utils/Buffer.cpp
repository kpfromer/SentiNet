/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "TestUtils.hpp"
#include "common/utils/Buffer.hpp"

// C++ includes
#include <numeric>
#include <list>

template <typename T>
void ValidateBufferNumeric(uint16_t size) {
  // Preload a size vector with values
  std::vector<T> populated_buffer(size);
  std::iota(std::begin(populated_buffer), std::end(populated_buffer), 0U);

  // Check the default parameters
  utils::Buffer<T> buff(size);
  EXPECT_EQ(0U, buff.size());
  EXPECT_EQ(size, buff.capacity());
  EXPECT_FALSE(buff.is_full());
  EXPECT_TRUE(buff.is_empty());

  // Set how many additional data objects we'll insert into the buffer
  T extra = 7U;

  // Insert a bunch of data into the buffer and verify that it rejects when full.
  for(T i = 0U; i < (size + extra); ++i) {
    // Insert data
    if(i < size) {
      EXPECT_EQ(1U, buff.insert(i, 1U)) << "Iteration: " << i;
      EXPECT_EQ(i, buff[i]) << "Iteration: " << i;
      EXPECT_EQ(buff[i], buff.get()[i]) << "Iteration: " << i;
    } else {
      EXPECT_EQ(0U, buff.insert(i, 1U)) << "Iteration: " << i;
      EXPECT_EQ(static_cast<T>(size - 1U), buff[size - 1U]) << "Iteration: " << i;
      EXPECT_EQ(buff[size - 1U], buff.get()[size - 1U]) << "Iteration: " << i;
    }

    // Check the indicators
    if(i + 1 < size) {
      EXPECT_FALSE(buff.is_full());
      EXPECT_FALSE(buff.is_empty());
    } else {
      EXPECT_TRUE(buff.is_full());
      EXPECT_FALSE(buff.is_empty());
    }
  }

  // Clear all the data
  buff.clear();
  EXPECT_EQ(0U, buff.size());
  EXPECT_EQ(size, buff.capacity());
  EXPECT_FALSE(buff.is_full());
  EXPECT_TRUE(buff.is_empty());

  // Insert a bunch of data into the buffer and verify that it rejects when full.
  for(T i = 0U; i < (size + extra); ++i) {
    // Insert data
    if(i < size) {
      EXPECT_EQ(1U, buff.insert(i));
      EXPECT_EQ(i, buff[i]);
      EXPECT_EQ(buff[i], buff.get()[i]);
    } else {
      EXPECT_EQ(0U, buff.insert(i));
      EXPECT_EQ(static_cast<T>(size - 1U), buff[size - 1U]);
      EXPECT_EQ(buff[size - 1U], buff.get()[size - 1U]);
    }

    // Check the indicators
    if(i + 1 < size) {
      EXPECT_FALSE(buff.is_full());
      EXPECT_FALSE(buff.is_empty());
    } else {
      EXPECT_TRUE(buff.is_full());
      EXPECT_FALSE(buff.is_empty());
    }
  }

  // Erase nothing
  EXPECT_EQ(0U, buff.erase(0U));
  EXPECT_EQ(buff.size(), buff.capacity());
  EXPECT_TRUE(buff.is_full());
  EXPECT_FALSE(buff.is_empty());
  // Erase nothing, again
  EXPECT_EQ(0U, buff.erase(std::numeric_limits<uint16_t>::max(), std::numeric_limits<uint16_t>::max()));
  EXPECT_EQ(buff.size(), buff.capacity());
  EXPECT_TRUE(buff.is_full());
  EXPECT_FALSE(buff.is_empty());
  // Erase nothing, again, again
  EXPECT_EQ(0U, buff.erase(1U, 0U));
  EXPECT_EQ(buff.size(), buff.capacity());
  EXPECT_TRUE(buff.is_full());
  EXPECT_FALSE(buff.is_empty());

  // Erase the data in blocks
  EXPECT_EQ(buff.capacity(), buff.erase(buff.capacity()));
  EXPECT_EQ(0U, buff.size());
  EXPECT_EQ(size, buff.capacity());
  EXPECT_FALSE(buff.is_full());
  EXPECT_TRUE(buff.is_empty());

  // Insert a bunch of data into the buffer using the copy operation
  EXPECT_EQ(buff.capacity(), buff.insert(&populated_buffer[0U], populated_buffer.size()));
  for(T i = 0U; i < size; ++i) {
    EXPECT_EQ(i, buff[i]);
    EXPECT_EQ(buff[i], buff.get()[i]);
    // Modify and validate
    buff[i] = buff[i] + size;
    EXPECT_EQ(i + size, buff[i]);
    EXPECT_EQ(buff[i], buff.get()[i]);
  }
  EXPECT_TRUE(buff.is_full());
  EXPECT_FALSE(buff.is_empty());

  // Check that we can remove data in chucks
  auto remaining = size;
  for(const auto removed : {(size / 2U), size - (size / 2U)}) {
    EXPECT_EQ(removed, buff.erase(removed));
    remaining -= removed;
    EXPECT_EQ(remaining, buff.size());
    // Validate the remaining contents
    if(remaining > 0U) {
      for(T i = 0U; i < buff.size(); ++i) {
        EXPECT_EQ(i + (size - remaining) + size, buff[i]);
        EXPECT_EQ(buff[i], buff.get()[i]);
      }
    }
  }

  // Check that the buffer is now empty
  EXPECT_EQ(0U, buff.size());
  EXPECT_EQ(size, buff.capacity());
  EXPECT_FALSE(buff.is_full());
  EXPECT_TRUE(buff.is_empty());

  // Load the buffer into different containers.  This is really just a compile time example
  std::list<std::unique_ptr<utils::Buffer<T>>> l;
  std::vector<std::unique_ptr<utils::Buffer<T>>> v;
  for(auto i = 0; i < 3; ++i) {
    l.emplace_back(std::make_unique<utils::Buffer<T>>(size));
    l.push_back(std::make_unique<utils::Buffer<T>>(size));
    EXPECT_EQ(2U, l.size());
    l.clear();
    EXPECT_EQ(0U, l.size());

    v.emplace_back(std::make_unique<utils::Buffer<T>>(size));
    v.push_back(std::make_unique<utils::Buffer<T>>(size));
    EXPECT_EQ(2U, v.size());
    v.clear();
    EXPECT_EQ(0U, v.size());
  }
}

template <typename T>
void ValidateBufferNumericWithOffset(uint16_t size) {
  // Preload a size vector with values
  std::vector<T> populated_buffer(size);
  std::iota(std::begin(populated_buffer), std::end(populated_buffer), 0U);

  // Check the default parameters with differing offsets
  utils::Buffer<T> buff(size);

  // Run through a couple offset sizes
  for(const auto& offset : {size / 2, size / 3, size / 4}) {
    auto capacity = size - offset;

    // Check the empty buffer
    buff.set_offset(offset);
    EXPECT_EQ(0U, buff.size());
    EXPECT_EQ(capacity, buff.capacity());
    EXPECT_FALSE(buff.is_full());
    EXPECT_TRUE(buff.is_empty());

    // Insert a bunch of data into the buffer using the copy operation and check the results
    ASSERT_EQ(capacity, buff.insert(&populated_buffer[0U], populated_buffer.size()));
    for(T i = 0U; i < static_cast<T>(capacity); ++i) {
      ASSERT_EQ(i, buff[i]);
      // Modify and validate
      buff[i] = buff[i] + capacity;
      EXPECT_EQ(i + capacity, buff[i]);
      EXPECT_EQ(buff[i], buff.get()[i + offset]);
    }
    buff.clear();

    // This going to mimic performing a read() into a buffer and updating the counters one
    // element at a time.
    for(auto j = static_cast<T>(0U); j < static_cast<T>(capacity); ++j) {
      memcpy(&buff[0U], &populated_buffer[0U], j * sizeof(T));
      EXPECT_TRUE(buff.is_empty());
      buff.set_head(j);
      ASSERT_EQ(j, buff.size());
      for(T i = 0U; i < j; ++i) {
        EXPECT_EQ(i, buff[i]);
        EXPECT_EQ(buff[i], buff.get()[i + offset]);
      }
      buff.clear();
    }

    // This going to mimic performing a read() into a buffer and updating the counters.
    // memcpy() the data directly and validate the results.
    buff.clear();
    memcpy(&buff[0U], &populated_buffer[0U], buff.capacity() * sizeof(T));
    EXPECT_TRUE(buff.is_empty());
    buff.set_head(buff.capacity());
    ASSERT_EQ(capacity, buff.size());
    for(T i = 0U; i < static_cast<T>(capacity); ++i) {
      EXPECT_EQ(i, buff[i]);
      EXPECT_EQ(buff[i], buff.get()[i + offset]);
    }
    buff.clear();
  }
}

template <typename T>
class MyStruct {
 public:
  T val;
};

template <typename T>
void ValidateBufferUniquePtr(uint16_t size) {
  // Check the default parameters
  utils::Buffer<std::unique_ptr<MyStruct<T>>> buff(size);
  EXPECT_EQ(0U, buff.size());
  EXPECT_EQ(size, buff.capacity());

  buff[0U];

  // Set how many additional data objects we'll insert into the buffer
  // T extra = 7U;
}

/** Tests that the Buffer class is able to work properly when primitive numeric types are used
 */
TEST(UtilTests, BufferNumeric) {
  // Check the size of 0 edge case
  {
      // @todo [KEMB-628]
  }

  // Check the size of 1 edge case
  {
    // @todo [KEMB-628]
  }

  // Validate the buffer using all the primitive numerical types
  for(const auto size : std::initializer_list<uint16_t>({utils::BufferDefault::SIZE, 1U, 1013U, 305U, 570U})) {
    ValidateBufferNumeric<int16_t>(size);
    ValidateBufferNumeric<int32_t>(size);
    ValidateBufferNumeric<int64_t>(size);
    ValidateBufferNumeric<uint16_t>(size);
    ValidateBufferNumeric<uint32_t>(size);
    ValidateBufferNumeric<uint64_t>(size);
    ValidateBufferNumeric<float>(size);
    ValidateBufferNumeric<double>(size);
  }
  // Validate the buffer using all the primitive numerical types
  // @todo [KEMB-628] Due to the unit test setup, we can't use large number with these primitive type.
  for(const auto size : std::initializer_list<uint16_t>({1U, 60U, 31U, 57U, 41U})) {
    ValidateBufferNumeric<unsigned char>(size);
    ValidateBufferNumeric<char>(size);
    ValidateBufferNumeric<int8_t>(size);
    ValidateBufferNumeric<uint8_t>(size);
  }
}

/** Tests that the Buffer class is able to work properly when primitive numeric types are used
 *  and the offset being set and changed
 */
TEST(UtilTests, BufferNumericWithOffset) {
  // Check the size of 0 edge case
  {
      // @todo [KEMB-628]
  }

  // Check the size of 1 edge case
  {
    // @todo [KEMB-628]
  }

  // Validate the buffer using all the primitive numerical types
  for(const auto size : std::initializer_list<uint16_t>({utils::BufferDefault::SIZE, 113U, 305U, 570U})) {
    ValidateBufferNumericWithOffset<int16_t>(size);
    ValidateBufferNumericWithOffset<int32_t>(size);
    ValidateBufferNumericWithOffset<int64_t>(size);
    ValidateBufferNumericWithOffset<uint16_t>(size);
    ValidateBufferNumericWithOffset<uint32_t>(size);
    ValidateBufferNumericWithOffset<uint64_t>(size);
    ValidateBufferNumericWithOffset<float>(size);
    ValidateBufferNumericWithOffset<double>(size);
  }
  // Validate the buffer using all the primitive numerical types
  // @todo [KEMB-628] Due to the unit test setup, we can't use large number with these primitive type.
  for(const auto size : std::initializer_list<uint16_t>({65U, 31U, 57U, 40U})) {
    ValidateBufferNumericWithOffset<unsigned char>(size);
    ValidateBufferNumericWithOffset<char>(size);
    ValidateBufferNumericWithOffset<int8_t>(size);
    ValidateBufferNumericWithOffset<uint8_t>(size);
  }
}

#if 0
/** Tests that the Circular Buffer class is able to work properly when primitive numeric types are used
 */
TEST(UtilTests, BufferUniquePtr) {
  // Validate the buffer using all the primitive numerical types
  for(const auto size : std::initializer_list<uint16_t>({utils::BufferDefault::SIZE, 1U, 100U, 30U, 57U})) {
    ValidateBufferUniquePtr<char>(size);
    ValidateBufferUniquePtr<unsigned char>(size);
    ValidateBufferUniquePtr<int8_t>(size);
    ValidateBufferUniquePtr<int16_t>(size);
    ValidateBufferUniquePtr<int32_t>(size);
    ValidateBufferUniquePtr<int64_t>(size);
    ValidateBufferUniquePtr<uint8_t>(size);
    ValidateBufferUniquePtr<uint16_t>(size);
    ValidateBufferUniquePtr<uint32_t>(size);
    ValidateBufferUniquePtr<uint64_t>(size);
    ValidateBufferUniquePtr<float>(size);
    ValidateBufferUniquePtr<double>(size);
  }
}
#endif
