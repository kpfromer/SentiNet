/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "applications/KelvinShark/KelvinShark.h"
#include "TestUtils.hpp"

/** The bare minimum required to make sure an application is able to compile and link.
 */
TEST(KelvinSharkTest, CompileAndLink) {
  auto app = std::make_unique<KelvinShark::KelvinShark>();
}
