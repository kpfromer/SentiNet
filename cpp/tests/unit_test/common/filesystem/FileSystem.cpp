/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "TestUtils.hpp"
#include "common/filesystem/FileSystem.hpp"

// C++ includes
#include <fstream>

/* The poller GtestHarness class
 */
class FileSystemTest : public GtestHarness {
 public:
  void SetUp() {
    ::GtestHarness::SetUp();
  }

  void TearDown() {
    ::GtestHarness::TearDown();
  }
};

/** Test that we are able to check the existance of files and there types
 */
TEST_F(FileSystemTest, CreateExistsRemove) {
  // Sanity check on garbage input
  EXPECT_FALSE(FileSystem::exists(""));
  EXPECT_FALSE(FileSystem::exists("the_empire_did_nothing_wrong"));
  EXPECT_FALSE(FileSystem::is_directory(""));
  EXPECT_FALSE(FileSystem::is_directory("/star/wars"));
  EXPECT_FALSE(FileSystem::is_directory("/star/wars/"));
  EXPECT_FALSE(FileSystem::is_regular_file(""));
  EXPECT_FALSE(FileSystem::is_regular_file("/rebel/scum.plans"));
  EXPECT_EQ(-1, FileSystem::file_size(""));
  EXPECT_EQ(-1, FileSystem::file_size("/imperial/lifestar.plans"));

  // Check directories
  const std::vector<std::string> dirs = {
      utils::join({UNITTEST_DIR, "dir1"}, "/"), utils::join({UNITTEST_DIR, "dir2"}, "/"),
      utils::join({UNITTEST_DIR, "dir3"}, "/"), utils::join({UNITTEST_DIR, "dir4"}, "/")};

  // Do some looping
  for(const auto& dir : dirs) {
    // Create some directories
    EXPECT_FALSE(FileSystem::exists(dir));
    EXPECT_TRUE(FileSystem::create_directories(dir));
    EXPECT_FALSE(FileSystem::is_regular_file(dir));
    EXPECT_TRUE(FileSystem::is_directory(dir));

    // Creating an existing directory should be fine
    EXPECT_TRUE(FileSystem::create_directories(dir));

    const std::vector<std::string> files = {"file1.txt", "file2.log", "file3", "file4.json"};
    const std::string data = "We are going to put some data into that file.";

    for(auto i = 0U; i < files.size(); ++i) {
      EXPECT_EQ(i, FileSystem::directory_iterator(dir).size());
      // Create some files
      const std::string filename = utils::join({dir, files[i]}, "/");
      EXPECT_FALSE(FileSystem::exists(filename));
      std::fstream out(filename, std::ios::out);
      ASSERT_TRUE(out.is_open());
      out << data;
      out.close();
      EXPECT_TRUE(FileSystem::exists(filename));
      EXPECT_FALSE(FileSystem::is_directory(filename));
      EXPECT_TRUE(FileSystem::is_regular_file(filename));
      EXPECT_EQ(static_cast<signed>(data.size()), FileSystem::file_size(filename));
      EXPECT_TRUE(utils::is_equal(data, FileSystem::load_file(filename)))
          << data << " != " << FileSystem::load_file(filename);

      EXPECT_EQ(i + 1, FileSystem::directory_iterator(dir).size());
    }

    // Validate that all the returned files are a part of the listing
    for(const auto& f : FileSystem::directory_iterator(dir)) {
      EXPECT_TRUE(std::find(files.begin(), files.end(), f) != files.end());
    }
  }

  // Validate that all the returned directories are a part of the listing
  for(const auto& d : FileSystem::directory_iterator(UNITTEST_DIR)) {
    EXPECT_TRUE(std::find(dirs.begin(), dirs.end(), utils::join({UNITTEST_DIR, d}, "/")) != dirs.end());
  }
  // Check that we can get the correct number of items for alternate queriy sizes
  for(auto i = 0U; i <= dirs.size(); ++i) {
    EXPECT_EQ(i, FileSystem::directory_iterator(UNITTEST_DIR, i).size());
  }
  EXPECT_EQ(dirs.size(), FileSystem::directory_iterator(UNITTEST_DIR).size());
  EXPECT_EQ(dirs.size(), FileSystem::directory_iterator(UNITTEST_DIR, dirs.size() + 1).size());

  // Remove all the directories
  for(const auto& dir : dirs) {
    EXPECT_TRUE(FileSystem::remove_all(dir));
    // Removing an already missing directory is fine
    EXPECT_TRUE(FileSystem::remove_all(dir));
  }
}
