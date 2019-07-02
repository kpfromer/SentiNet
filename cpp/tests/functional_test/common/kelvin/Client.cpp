/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "TestUtils.hpp"
#include "common/kelvin/client/KelvinHTTPClient.h"

TEST(KelvinClientTest, Initialize) {
  auto tmp = std::make_unique<Kelvin::client::KelvinHTTPClient>("", "", "");
}
