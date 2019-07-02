/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "protocols/OPCUA/OPCUA.h"
#include "TestUtils.hpp"

/** The bare minimum required to make sure an application is able to compile and link.
 */
TEST(OPCUATest, CompileAndLink) {
  { auto app = std::make_unique<OPCUA::Server>(); }
  { auto app = std::make_unique<OPCUA::Client>(); }
}
