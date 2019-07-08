#include <string>
class OutputInterface {
 public:
  OutputInterface() { asset_name = "name"; }

  virtual ~OutputInterface() = default;

 public:
  virtual bool initialize() = 0;
  virtual bool terminate() = 0;

 protected:
  virtual bool start_up_scan() = 0;
  virtual bool termination_scan() = 0;

  virtual bool start_up_sequence_test() = 0;
  virtual bool termination_sequence_test() = 0;

 public:
  std::string get_name() const { return asset_name; }
  virtual void get_description(bool) = 0;

 public:
  virtual void loop() = 0;
  virtual void emergency_cut_power() = 0;

 protected:
  std::string asset_name;
};
