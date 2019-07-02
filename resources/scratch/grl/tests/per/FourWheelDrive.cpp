#include "FourWheelDrive.hp"

FourWheelDrive::FourWheelDrive(ConfigurationClient& config, unsigned int id)
: PeripheralInterface(config, id){
  linear = 0;
  angular = 0;
} 

FourWheelDrive::~FourWheelDrive(){}

void FourWheelDrive::create_outputs(ConfigurationClient& config, unsigned int id)
{
  for(int i = 0; i < 4; i++)
  {
    dc_motor * temp = new dc_motor(config, id, i);
    std::pair<std::string, std::shared_ptr<OutputInterface>> temp_pair(temp->get_name(), std::move(temp));
    outputs.insert(temp_pair);
  }
}

void FourWheelDrive::create_inputs(ConfigurationClient&, std::string){return;}

void FourWheelDrive::loop()
{
  outputs["FL"]->update(linear + angular);
  outputs["FR"]->update(linear - angular);
  outputs["BL"]->update(linear + angular);
  outputs["BR"]->update(linear - angular);
}

void FourWheelDrive::update(int linear_, int angular_)
{
   linear = linear_;
   angular = angular_;
}

void FourWheelDrive::emergency_cut_power()
{
  outputs["FL"]->emergency_cut_power();
  outputs["FR"]->emergency_cut_power();
  outputs["BL"]->emergency_cut_power();
  outputs["BR"]->emergency_cut_power();
}

