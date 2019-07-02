/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "TestUtils.hpp"
#include "applications/KelvinUploader/KelvinUploader.h"

/** The bare minimum required to make sure an application is able to compile and link.
 */
TEST(KelvinUploaderTest, CompileAndLink) {
  auto app = std::make_unique<KelvinUploader::KelvinUploader>();
}
