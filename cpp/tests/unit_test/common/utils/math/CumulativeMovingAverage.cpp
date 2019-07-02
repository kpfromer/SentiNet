/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "TestUtils.hpp"
#include "common/utils/math/CumulativeMovingAverage.hpp"

// C++ includes
#include <numeric>

template <typename T>
void ValidateCumulativeMovingAverageNumeric() {
  {
    utils::CumulativeMovingAverage<T> averager;

    // Do a simple moving average of a non-changing value
    const auto val = 255U;
    for(auto i = 0; i < 100; ++i) {
      EXPECT_EQ(val, averager.insert(val));
      EXPECT_EQ(T(i + 1), averager.size());
    }
    averager.clear();

    // Do a moving average of a simple incrementing value
    T sum = 0U;
    for(auto i = 1; i < 101; ++i) {
      sum += i;
      EXPECT_EQ(sum / i, averager.insert(i)) << i << " " << sum;
    }
    averager.clear();

    averager.zero_increment(10);
    EXPECT_EQ(T(0), averager.get_average());
  }
}

TEST(UtilTests, CumulativeMovingAverageNumeric) {
  ValidateCumulativeMovingAverageNumeric<float>();
  ValidateCumulativeMovingAverageNumeric<double>();
}
