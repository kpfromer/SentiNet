#ifndef RKS_PROCESS_CONTROLLER_PROCESS_CONTROLLER_HPP
#define RKS_PROCESS_CONTROLLER_PROCESS_CONTROLLER_HPP

//Rum Vison
#include "rks/peripheral_interfaces/PeripheralInterface.hpp"


//C++


namespace rks
{
namespace process_controllers
{

class ProcessController
{

  public:
    ProcessController(ConfigurationClient* config);
    ~ProcessController();


  private:
    InteruptController* ic;
    std::












#endif // RKS_PROCESS_CONTROLLER_PROCESS_CONTROLLER_HPP
