Ralphie::Ralphie(ConfigurationClient& config) : RobotInterface(config) {
  objective.angular = 0;
  objective.linear = 0;
  objective.mining = 0;
  objective.dumping = 0;
  running = false;
}

void Ralphie::create_peripherals(ConfigurationClient& config) {
  FourWheelDrive* temp = new FourWheelDrive(config, 0);
  std::pair<std::string, std::shared_ptr<PeripheralInterface>> temp_pair(
      temp->get_name(), std::move(temp));
  peripherals.insert(temp_pair);
}

void Ralphie::loop() {
  if (running) {
    peripherals["drivetrain"]->update(objective.linear, objective.angular);
  } else {
    peripherals["drivetrain"]->passive();
  }
}

void Ralphie::update(objective_type temp) { objective = temp; }

void Ralphie::emergency_cut_power() {
  peripherals["drivetrain"]->emergency_cut_power();
}
