/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "TestUtils.hpp"
#include "applications/BridgeShadowConversion/BridgeShadowConversion.h"

/** The bare minimum required to make sure an application is able to compile and link.
 */
TEST(BridgeShadowConversionTest, CompileAndLink) {
  auto app = std::make_unique<BridgeShadowConversion::BridgeShadowConversion>();
}
