/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "applications/KelvinRegistryMap/KelvinRegistryMap.h"
#include "TestUtils.hpp"

/** The bare minimum required to make sure an application is able to compile and link.
 */
TEST(KelvinRegistryMapTest, CompileAndLink) {
  auto app = std::make_unique<KelvinRegistryMap::KelvinRegistryMap>();
}
