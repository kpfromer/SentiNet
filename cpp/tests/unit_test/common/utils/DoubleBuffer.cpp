/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "TestUtils.hpp"
#include "common/utils/DoubleBuffer.hpp"

template <typename T>
void ValidateDoubleBufferNumeric(uint16_t size) {
  // Check the default parameters
  utils::DoubleBuffer<T> buff(size);

  for(auto i = 0U; i < utils::DoubleBufferDefault::BUFFER_CNT; ++i) {
    // Check the current buffers
    ASSERT_NE(&buff.read(), &buff.write());
    ASSERT_EQ(utils::DoubleBufferDefault::BUFFER_CNT, buff.buffer_cnt());
    // Read buffer
    EXPECT_EQ(0U, buff.read().size());
    EXPECT_TRUE(buff.read().is_empty());
    EXPECT_FALSE(buff.read().is_full());
    ASSERT_EQ(size, buff.read().capacity());
    // Write buffer
    EXPECT_EQ(0U, buff.write().size());
    EXPECT_TRUE(buff.write().is_empty());
    EXPECT_FALSE(buff.write().is_full());
    ASSERT_EQ(size, buff.write().capacity());

    auto write_size = static_cast<unsigned>(buff.write().capacity() / (i + 1));

    // Insert data into the write buffer
    for(T j = 0U; static_cast<unsigned>(j) < write_size; ++j) {
      buff.write()[j] = j;
      buff.increment_write_head(1);
    }

    buff.swap();  // Buffer swap!

    // Check the buffer information
    ASSERT_EQ(0U, buff.write().size());
    ASSERT_EQ(size, buff.write().capacity());
    ASSERT_EQ(write_size, buff.read().size());

    // Check the read buffer contents
    for(T j = 0U; static_cast<unsigned>(j) < static_cast<unsigned>(buff.read().size()); ++j) {
      EXPECT_EQ(j, buff.read()[j]);
      EXPECT_EQ(buff.read()[j], buff.read().get()[j]);
    }
    // Insert into the other buffer
    T extra = 7U;
    for(T j = 0U; static_cast<unsigned>(j) < write_size; ++j) {
      buff.write()[j] = j + extra;
      buff.write().set_head(j + 1);
    }
    buff.swap();  // Buffer swap!

    // Insert into the other buffer and check to read buffer
    for(T j = 0U; static_cast<unsigned>(j) < static_cast<unsigned>(buff.read().size()); ++j) {
      EXPECT_EQ(j + extra, buff.read()[j]);
      EXPECT_EQ(buff.read()[j], buff.read().get()[j]);
    }

    // Clear all the data
    buff.clear();
  }
}

template <typename T>
void ValidateDoubleBufferNumericWithRollover(uint16_t size) {
  // Check the default parameters
  utils::DoubleBuffer<T> buff(size, true);

  for(auto i = 0U; i < utils::DoubleBufferDefault::BUFFER_CNT; ++i) {
    for(const auto& write_elem : {size / 1U, size / 2U, size / 3U, size / 4U}) {
      // Allocate the new size
      EXPECT_EQ(write_elem, buff.resize(write_elem));
      const auto write_size = static_cast<unsigned>(buff.write().capacity());
      ASSERT_EQ(write_elem, write_size);

      // Check the current buffers
      ASSERT_NE(&buff.read(), &buff.write());
      ASSERT_EQ(utils::DoubleBufferDefault::BUFFER_CNT, buff.buffer_cnt());
      // Read buffer
      EXPECT_EQ(0U, buff.read().size());
      EXPECT_EQ(write_size, buff.read().get_offset());
      EXPECT_TRUE(buff.read().is_empty());
      EXPECT_FALSE(buff.read().is_full());
      ASSERT_EQ(write_size, buff.read().capacity());
      // Write buffer
      EXPECT_EQ(0U, buff.write().size());
      EXPECT_EQ(write_size, buff.write().get_offset());
      EXPECT_TRUE(buff.write().is_empty());
      EXPECT_FALSE(buff.write().is_full());
      ASSERT_EQ(write_size, buff.write().capacity());

      /////////////
      // Direct access buffers ... you have to do pointer maintenance

      // Insert data into the write buffer directly
      for(T j = 0U; static_cast<unsigned>(j) < write_size; ++j) {
        buff.write()[j] = j;
        buff.increment_write_head(1);
      }

      // Check the buffer information
      ASSERT_EQ(write_size, buff.write().get_offset());
      ASSERT_EQ(write_size, buff.write().size());
      ASSERT_EQ(write_size, buff.write().capacity());
      ASSERT_EQ(write_size, buff.read().get_offset());
      ASSERT_EQ(0U, buff.read().size());
      ASSERT_EQ(write_size, buff.read().capacity());
      buff.swap();  // Buffer swap!
      ASSERT_EQ(write_size, buff.write().get_offset());
      ASSERT_EQ(0U, buff.write().size());
      ASSERT_EQ(write_size, buff.write().capacity());
      ASSERT_EQ(write_size, buff.read().get_offset());
      ASSERT_EQ(write_size, buff.read().size());
      ASSERT_EQ(write_size, buff.read().capacity());

      // Insert into the other buffer
      T extra = 7U;
      for(T j = 0U; static_cast<unsigned>(j) < write_size; ++j) {
        buff.write()[j] = j + write_size + extra;
        buff.write().set_head(j + 1);
      }

      // Check the buffer information
      ASSERT_EQ(write_size, buff.write().get_offset());
      ASSERT_EQ(write_size, buff.write().size());
      ASSERT_EQ(write_size, buff.write().capacity());
      ASSERT_EQ(write_size, buff.read().get_offset());
      ASSERT_EQ(write_size, buff.read().size());
      ASSERT_EQ(write_size, buff.read().capacity());
      buff.swap();  // Buffer swap!
      ASSERT_EQ(write_size * 2U, buff.write().get_offset());
      ASSERT_EQ(0U, buff.write().size());
      ASSERT_EQ(0U, buff.write().capacity());
      ASSERT_EQ(0U, buff.read().get_offset());
      ASSERT_EQ(write_size * 2U, buff.read().size());
      ASSERT_EQ(write_size * 2U, buff.read().capacity());

      // Check the read buffer contents
      for(T j = 0U; static_cast<unsigned>(j) < static_cast<unsigned>(buff.read().size()); ++j) {
        if(static_cast<unsigned>(j) < write_size) {
          EXPECT_EQ(j, buff.read()[j]);
          EXPECT_EQ(buff.read()[j], buff.read().get()[j]);
        } else {
          EXPECT_EQ(j + extra, buff.read()[j]);
          EXPECT_EQ(buff.read()[j], buff.read().get()[j]);
        }
      }
      buff.increment_read_tail(buff.read().size());

      // Check the buffer information
      ASSERT_EQ(write_size * 2U, buff.write().get_offset());
      ASSERT_EQ(0U, buff.write().size());
      ASSERT_EQ(0U, buff.write().capacity());
      ASSERT_EQ(0U, buff.read().get_offset());
      ASSERT_EQ(0U, buff.read().size());
      ASSERT_EQ(0U, buff.read().capacity());
      buff.swap();  // Buffer swap!
      ASSERT_EQ(write_size, buff.write().get_offset());
      ASSERT_EQ(0U, buff.write().size());
      ASSERT_EQ(write_size, buff.write().capacity());
      ASSERT_EQ(write_size, buff.read().get_offset());
      ASSERT_EQ(0U, buff.read().size());
      ASSERT_EQ(write_size, buff.read().capacity());

      ////////////
      // Use the helper methods ... no pointer maintenance
      const uint16_t new_write_size = write_size / 2U;
      const uint16_t new_read_size = new_write_size / 2U;

      // Insert data into the write buffer directly
      for(T j = 0U; static_cast<unsigned>(j) < new_write_size; ++j) {
        buff.write().insert(j);
      }

      // Check the buffer information
      ASSERT_EQ(write_size, buff.write().get_offset());
      ASSERT_EQ(new_write_size, buff.write().size());
      ASSERT_EQ(write_size, buff.write().capacity());
      ASSERT_EQ(write_size, buff.read().get_offset());
      ASSERT_EQ(0U, buff.read().size());
      ASSERT_EQ(write_size, buff.read().capacity());
      buff.swap();  // Buffer swap!
      ASSERT_EQ(write_size, buff.write().get_offset());
      ASSERT_EQ(0U, buff.write().size());
      ASSERT_EQ(write_size, buff.write().capacity());
      ASSERT_EQ(write_size, buff.read().get_offset());
      ASSERT_EQ(new_write_size, buff.read().size());
      ASSERT_EQ(write_size, buff.read().capacity());

      // Check the read buffer contents
      T idx = static_cast<unsigned>(0U);
      for(; !buff.read().is_empty(); ++idx) {
        EXPECT_EQ(idx, buff.read()[0]);
        buff.increment_read_tail(1U);
      }
      ASSERT_EQ(new_write_size, idx);

      // Check that the buffers are cleared out
      buff.swap();  // Buffer swap!
      ASSERT_EQ(write_size, buff.write().get_offset());
      ASSERT_EQ(0U, buff.write().size());
      ASSERT_EQ(write_size, buff.write().capacity());
      ASSERT_EQ(write_size, buff.read().get_offset());
      ASSERT_EQ(0U, buff.read().size());
      ASSERT_EQ(write_size, buff.read().capacity());

      // Use the buffer a couple of times to show that swapping maintains the state
      for(auto swap : {1U, 2U, 3U, 4U}) {
        // Insert data into the write buffer directly
        for(T j = 0U; static_cast<unsigned>(j) < new_write_size; ++j) {
          buff.write().insert(j);
        }
        buff.swap();  // Buffer swap!

        // Check the read buffer contents and only read half.
        idx = 0U;
        for(; idx < new_read_size; ++idx) {
          EXPECT_EQ(idx, buff.read()[0]);
          buff.increment_read_tail(1U);
        }
        ASSERT_EQ(new_read_size, static_cast<unsigned>(idx));
        const auto remaining = new_write_size - new_read_size;

        // Check the buffer information before
        ASSERT_EQ(write_size, buff.write().get_offset());
        ASSERT_EQ(0U, buff.write().size());
        ASSERT_EQ(write_size, buff.write().capacity());
        ASSERT_EQ(write_size, buff.read().get_offset());
        ASSERT_EQ(remaining, buff.read().size());
        ASSERT_EQ(write_size - idx, buff.read().capacity());

        // Swap the buffer a bunch of times to make sure data gets rolled over properly
        for(; swap > 0; --swap) {
          buff.swap();  // Buffer swap!
        }

        // Check the buffer information after swapping multiple times
        ASSERT_EQ(write_size, buff.write().get_offset());
        ASSERT_EQ(0U, buff.write().size());
        ASSERT_EQ(write_size, buff.write().capacity());
        ASSERT_EQ(write_size - remaining, buff.read().get_offset());
        ASSERT_EQ(remaining, buff.read().size());
        ASSERT_EQ(write_size + remaining, buff.read().capacity());

        // Check the read buffer contents and only read half.
        for(; idx < new_write_size; ++idx) {
          ASSERT_EQ(idx, buff.read()[0]);
          buff.increment_read_tail(1U);
        }
        ASSERT_EQ(new_write_size, static_cast<unsigned>(idx));

        // Check the buffer information after flushing all the data in the buffer
        ASSERT_EQ(write_size, buff.write().get_offset());
        ASSERT_EQ(0U, buff.write().size());
        ASSERT_EQ(write_size, buff.write().capacity());
        ASSERT_EQ(write_size - remaining, buff.read().get_offset());
        ASSERT_EQ(0U, buff.read().size());
        ASSERT_EQ(write_size, buff.read().capacity());
        buff.swap();  // Buffer swap!
        ASSERT_EQ(write_size, buff.write().get_offset());
        ASSERT_EQ(0U, buff.write().size());
        ASSERT_EQ(write_size, buff.write().capacity());
        ASSERT_EQ(write_size, buff.read().get_offset());
        ASSERT_EQ(0U, buff.read().size());
        ASSERT_EQ(write_size, buff.read().capacity());
      }
    }
  }
}

/** Tests that the Double Buffer class is able to work properly when primitive numeric types are used
 */
TEST(UtilTests, DoubleBuffer) {
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
    ValidateDoubleBufferNumeric<int16_t>(size);
    ValidateDoubleBufferNumeric<int32_t>(size);
    ValidateDoubleBufferNumeric<int64_t>(size);
    ValidateDoubleBufferNumeric<uint16_t>(size);
    ValidateDoubleBufferNumeric<uint32_t>(size);
    ValidateDoubleBufferNumeric<uint64_t>(size);
    ValidateDoubleBufferNumeric<float>(size);
    ValidateDoubleBufferNumeric<double>(size);
  }
  // Validate the buffer using all the primitive numerical types
  // @todo [KEMB-628] Due to the unit test setup, we can't use large number with these primitive type.
  for(const auto size : std::initializer_list<uint16_t>({1U, 60U, 31U, 57U, 41U})) {
    ValidateDoubleBufferNumeric<unsigned char>(size);
    ValidateDoubleBufferNumeric<char>(size);
    ValidateDoubleBufferNumeric<int8_t>(size);
    ValidateDoubleBufferNumeric<uint8_t>(size);
  }
}

/** Tests that the Double Buffer class is able to work properly when primitive numeric types are used with
 *  rollover support
 */
TEST(UtilTests, DoubleBufferWithRollover) {
  // Check the size of 0 edge case
  {
      // @todo [KEMB-628]
  }

  // Check the size of 1 edge case
  {
    // @todo [KEMB-628]
  }

  // Validate the buffer using all the primitive numerical types
  for(const auto size : std::initializer_list<uint16_t>({utils::BufferDefault::SIZE, 1013U, 305U, 570U})) {
    ValidateDoubleBufferNumericWithRollover<int16_t>(size);
    ValidateDoubleBufferNumericWithRollover<int32_t>(size);
    ValidateDoubleBufferNumericWithRollover<int64_t>(size);
    ValidateDoubleBufferNumericWithRollover<uint16_t>(size);
    ValidateDoubleBufferNumericWithRollover<uint32_t>(size);
    ValidateDoubleBufferNumericWithRollover<uint64_t>(size);
    ValidateDoubleBufferNumericWithRollover<float>(size);
    ValidateDoubleBufferNumericWithRollover<double>(size);
  }
  // Validate the buffer using all the primitive numerical types
  // @todo [KEMB-628] Due to the unit test setup, we can't use large number with these primitive type.
  for(const auto size : std::initializer_list<uint16_t>({60U, 31U, 57U, 41U})) {
    ValidateDoubleBufferNumericWithRollover<unsigned char>(size);
    ValidateDoubleBufferNumericWithRollover<char>(size);
    ValidateDoubleBufferNumericWithRollover<int8_t>(size);
    ValidateDoubleBufferNumericWithRollover<uint8_t>(size);
  }
}
