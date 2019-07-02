/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "TestUtils.hpp"
#include "common/utils/math/WindowedMovingAverage.hpp"

// C++ includes
#include <numeric>

template <typename T>
void ValidateWindowedMovingAverageNumeric(uint16_t size) {
  {
    // Check the default parameters
    utils::WindowedMovingAverage<T> buff(size);

    EXPECT_EQ(0U, buff.size());
    EXPECT_EQ(size, buff.capacity());
    EXPECT_EQ(0U, buff.get_average());
  }

  {
    // Check the default parameters
    utils::WindowedMovingAverage<T> buff(size);

    EXPECT_EQ(0U, buff.size());
    EXPECT_EQ(size, buff.capacity());
    EXPECT_EQ(0U, buff.get_average());

    // Do a simple moving average of a non-changing value
    const auto val = 255U;
    for(auto i = 0; i < buff.capacity() * 2; ++i) {
      // Precheck sizes
      if(i < buff.capacity()) {
        EXPECT_EQ(i, buff.size());
      } else {
        EXPECT_EQ(buff.capacity(), buff.size());
      }
      EXPECT_EQ(size, buff.capacity());

      // Insert and validate
      EXPECT_EQ(val, buff.insert(val));
      EXPECT_EQ(size, buff.capacity());
      if(i < buff.capacity()) {
        EXPECT_EQ(i + 1, buff.size());
      } else {
        EXPECT_EQ(buff.capacity(), buff.size());
      }
    }

    // Clear the buffer
    buff.clear();
    EXPECT_EQ(0U, buff.size());
    EXPECT_EQ(size, buff.capacity());
    EXPECT_EQ(0U, buff.get_average());

    // Do a moving average of a simple incrementing value
    T sum = 0U;
    for(auto i = 0; i < buff.capacity(); ++i) {
      sum += i;
      ASSERT_EQ(sum / (i + 1), buff.insert(i));
    }

    // Let's compute a continuous moving average (this will test the circular buffer rollover)
    std::vector<T> vals = {14U, 32U, 11U, 65U, 11U, 43U, 76U, 0U, 245U, 34U, 54U, 54U, 54U, 125U, 147U};
    const auto resize = 4U;
    EXPECT_NE(0U, buff.get_average());
    ASSERT_EQ(resize, buff.resize(resize));
    // Insert the first n-1 numbers
    for(auto i = 0U; i < resize - 1; ++i) {
      buff.insert(vals[i]);
    }
    // The buffer is primed, so continue with the remaining numbers
    for(auto i = resize - 1; i < vals.size() - resize; ++i) {
      const auto average = std::accumulate(vals.begin() + ((i + 1) - resize), vals.begin() + (i + 1), 0U) / resize;
      ASSERT_EQ(average, buff.insert(vals[i]));
    }
  }
}

/** Tests that the Circular Buffer class is able to work properly when primitive numeric types are used
 */
TEST(UtilTests, WindowedMovingAverageNumeric) {
  // Validate the buffer using all the primitive numerical types
  for(const auto size :
      // std::initializer_list<uint16_t>({utils::MovingAverageDefault::SIZE, 1U, 1000U, 300U, 570U, 5000U})) {
      std::initializer_list<uint16_t>({10U})) {
    // @todo [KEMB-605] Validate the other primative types (signed fixed point and floating point)
    // ValidateMovingAverageNumeric<int16_t>(size);
    // ValidateMovingAverageNumeric<int32_t>(size);
    // ValidateMovingAverageNumeric<int64_t>(size);
    ValidateWindowedMovingAverageNumeric<uint16_t>(size);
    ValidateWindowedMovingAverageNumeric<uint32_t>(size);
    ValidateWindowedMovingAverageNumeric<uint64_t>(size);
    // ValidateMovingAverageNumeric<float>(size);
    // ValidateMovingAverageNumeric<double>(size);
  }

  {
    std::map<std::string, std::unique_ptr<utils::WindowedMovingAverage<int32_t>>> averages;
    averages.emplace("hello", std::make_unique<utils::WindowedMovingAverage<int32_t>>());
    auto found = averages.find("hello");

    EXPECT_EQ(0U, found->second->size());
    EXPECT_EQ(utils::WindowedMovingAverageDefault::SIZE, found->second->capacity());
    EXPECT_EQ(static_cast<int32_t>(0U), found->second->get_average());
  }
}
