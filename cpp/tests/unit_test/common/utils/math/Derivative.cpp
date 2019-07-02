/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "TestUtils.hpp"
#include "common/utils/math/Derivative.hpp"
#include "common/utils/Buffer.hpp"

// C++ includes
#include <numeric>

template <typename T>
void ValidateDerivativeNumeric() {
  {
    utils::Derivative<T> derivativeTest;

    // Do a simple moving average of a non-changing value
    std::vector<T> my_vec;
    my_vec.push_back(1);
    my_vec.push_back(2);
    std::vector<T> my_result_vec;
    my_result_vec.push_back(1);
    auto calculated = derivativeTest.calculate(my_vec, 1);
    for(uint64_t i = 0; i < my_result_vec.size(); ++i) {
      EXPECT_EQ(my_result_vec[i], calculated[i]);
    }

    my_result_vec.push_back(1);
    calculated = derivativeTest.calculate(3);
    for(uint64_t i = 0; i < my_result_vec.size(); ++i) {
      EXPECT_EQ(my_result_vec[i], calculated[i]);
    }
  }
}

TEST(UtilTests, ValidateDerivativeNumeric) {
  ValidateDerivativeNumeric<float>();
  ValidateDerivativeNumeric<double>();
}
