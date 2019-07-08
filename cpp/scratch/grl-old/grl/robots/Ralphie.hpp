

typedef struct {
  int angular;
  int linear;
  bool mining;
  bool dumping;
  bool turn_off;
  bool turn_on;
} objective_type;

class Ralphie : RobotInterface {
 public:
  Ralphie(configurationClient&) : RobotInterface(ConfigurationClient&);

  ~Ralphie() {}

 protected:
  void create_peripherals(ConfigurationClient&);

  void loop();
  void update(objective_type);

  void emergency_cut_power();

 private:
  objective_type objective;
  bool running;
}
