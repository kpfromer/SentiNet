/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */
#ifndef TEST_PROTOCOL_DB2REGISTER_HPP
#define TEST_PROTOCOL_DB2REGISTER_HPP

// Kelvin includes
#include "TestUtils.hpp"
#include "protocols/Db2Register/Db2Register.h"
#include "protocols/Db2Register/Protocol.hpp"
#include "common/descriptor/Fd.hpp"

// The custom GTEST harness for the application base class implementaton testing
class Db2RegisterTest : public GtestHarness {
 public:
  Db2RegisterTest() : send_fds{-1, -1} {
  }
  void SetUp() {
    ::GtestHarness::SetUp();
    EXPECT_EQ(0U, LoggingInterface::get_instance()->set_warning_cnt(0U));
    EXPECT_EQ(0U, LoggingInterface::get_instance()->set_error_cnt(0U));

    EXPECT_EQ(0, pipe(send_fds));
    EXPECT_NE(-1, send_fds[READ]);
    EXPECT_NE(-1, send_fds[WRITE]);

    reader = std::make_unique<descriptor::Fd>(fds[READ]);
    writer = std::make_unique<descriptor::Fd>(send_fds[WRITE]);
  }

  void TearDown() {
    ::GtestHarness::TearDown();

    for(auto i : {READ, WRITE}) {
      if(send_fds[i] >= 0) {
        EXPECT_EQ(0, close(send_fds[i]));
      }
      i = -1;
    }
  }

 public:
  std::unique_ptr<DescriptorInterface> reader;
  std::unique_ptr<DescriptorInterface> writer;
  int send_fds[2];
};

/** Create a Db2Register application that is able to expose all of the protected data members.
 *  This makes testing much easier and helps with scoping of the application under test.
 */
class Db2RegisterApplication : public Db2Register::Db2Register {
 public:
  Db2RegisterApplication() {
    Db2Register::bind<ApplicationInterface::BoundDescriptorFunc>(
        [this](std::unique_ptr<DescriptorInterface> descriptor,
               std::function<void(DescriptorInterface * descriptor)>) -> int32_t {
          callbacks.emplace_back(std::move(descriptor));
          return -1;
        });
  }

  bool initialize(const ConfigurationInterface& config) {
    return Db2Register::Db2Register::initialize(config);
  }

  void set_transaction_status(const Db2Register::TransactionStatus& transaction_status) {
    Db2Register::Db2Register::properties.transaction_status = transaction_status;
  }

  ApplicationProperties_t& get_properties() {
    return properties;
  }

  void set_descriptor(DescriptorInterface* descriptor) {
    properties.descriptor = descriptor;
  }

  const std::map<uint32_t, uint16_t> group_register_sets(std::vector<std::vector<uint32_t>>& register_sets) const {
    return Db2Register::group_register_sets(register_sets);
  }

  bool prebuilt_data_request(const std::map<uint32_t, uint16_t>& map, std::vector<uint8_t>& prebuilt_request) {
    return Db2Register::prebuilt_data_request(map, prebuilt_request);
  }

  void set_station_id(const std::string& station_id) {
    Db2Register::station_id = station_id;
  }

  void rtu_response(DescriptorInterface* descriptor) {
    Db2Register::rtu_response(descriptor);
  }

  bool send_request(const std::vector<uint8_t>& request) {
    return Db2Register::send_request(request);
  }

  void poll_data(const std::vector<uint8_t>& request) {
    Db2Register::poll_data(request);
  }

  void process_data(std::list<std::unique_ptr<MessageInterface>>& msgs) {
    Db2Register::process_data(msgs);
  }

 private:
  std::list<std::unique_ptr<DescriptorInterface>> callbacks;
};

#endif /* TEST_PROTOCOL_DB2REGISTER_HPP */
