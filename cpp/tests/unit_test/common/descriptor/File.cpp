/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "TestUtils.hpp"
#include "common/descriptor/File.h"

/* The poller GtestHarness class
 */
class FileTest : public GtestHarness {
 public:
  void SetUp() {
    ::GtestHarness::SetUp();
  }

  void TearDown() {
    ::GtestHarness::TearDown();
  }
};

/** The overloaded Serial class for the unit test.
 */
class FileDescriptor : public descriptor::File {
 public:
  FileDescriptor(const std::string& filename, const std::list<descriptor::FileOptions> options)
      : File(filename, options) {
  }
};

/** Test that we are able to create a file descriptor object.
 */
TEST_F(FileTest, Initialize) {
  const auto path = utils::join({UNITTEST_DIR, "a", "couple", "of", "subdirectories"}, "/");
  const auto filename = utils::join({path, "test.txt"}, "/");

  // Verify that northing is created in the default case if the Create option is not provided.
  {
    EXPECT_FALSE(FileSystem::exists(path));
    EXPECT_THROW(std::make_unique<descriptor::File>(filename), RuntimeException);
    EXPECT_FALSE(FileSystem::exists(path));
    EXPECT_FALSE(FileSystem::exists(filename));
    EXPECT_LE(0U, FileSystem::remove_all(UNITTEST_DIR));
  }

  // Verify that we can create the directory structure, but the file is NOT opened .. need 'Create'
  {
    EXPECT_FALSE(FileSystem::exists(path));
    const auto descriptor = std::make_unique<descriptor::File>(
        filename, std::initializer_list<descriptor::FileOptions>({descriptor::FileOptions::Create}));
    EXPECT_TRUE(FileSystem::exists(path));
    EXPECT_TRUE(FileSystem::exists(filename));
    EXPECT_LE(0U, FileSystem::remove_all(UNITTEST_DIR));
  }

  // Verify that we can create the directory structure and the file.  This file will NOT be opened
  // as read/write, so nothing should happen when those operations are called.
  {
    // Create everything
    EXPECT_FALSE(FileSystem::exists(path));
    const auto descriptor = std::make_unique<descriptor::File>(
        filename, std::initializer_list<descriptor::FileOptions>({descriptor::FileOptions::Create}));
    EXPECT_TRUE(FileSystem::exists(path));
    EXPECT_TRUE(FileSystem::exists(filename));
    // Verify the file doesn't change sizes and operations fail as expected.
    EXPECT_EQ(0U, FileSystem::file_size(filename));
    EXPECT_EQ(-1, write(descriptor->get_fd(), " ", 1));
    char buff[20];
    EXPECT_EQ(0, read(descriptor->get_fd(), buff, sizeof(buff)));
    EXPECT_EQ(0U, FileSystem::file_size(filename));
    // Clean up
    EXPECT_LE(0U, FileSystem::remove_all(UNITTEST_DIR));
  }

  // Verify that we can create the directory structure and the file.  This file will be opened
  // as read/write, so we can write() and read() from it.
  {
    // Create everything
    EXPECT_FALSE(FileSystem::exists(path));
    const auto descriptor = std::make_unique<descriptor::File>(
        filename, std::initializer_list<descriptor::FileOptions>(
                      {descriptor::FileOptions::Create, descriptor::FileOptions::ReadWrite}));
    EXPECT_TRUE(FileSystem::exists(path));
    EXPECT_TRUE(FileSystem::exists(filename));
    // Verify the file doesn't change sizes and operations fail as expected.
    EXPECT_EQ(0U, FileSystem::file_size(filename));
    const auto str = std::string("Hello World");
    EXPECT_EQ(static_cast<signed>(str.size()), write(descriptor->get_fd(), str.c_str(), str.size()));
    ASSERT_EQ(0, lseek(descriptor->get_fd(), 0, 0));
    char buff[20];
    EXPECT_EQ(static_cast<signed>(str.size()), read(descriptor->get_fd(), buff, sizeof(buff)));
    EXPECT_EQ(static_cast<signed>(str.size()), FileSystem::file_size(filename));
    // Clean up
    EXPECT_LE(0U, FileSystem::remove_all(UNITTEST_DIR));
  }
}
